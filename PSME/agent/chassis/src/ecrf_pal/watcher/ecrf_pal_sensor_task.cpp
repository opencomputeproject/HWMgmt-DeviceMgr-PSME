/*
* Edgecore DeviceManager
* Copyright 2020-2021 Edgecore Networks, Inc.
 *
* Licensed to the Apache Software Foundation (ASF) under one
* or more contributor license agreements.  See the NOTICE file
* distributed with this work for additional information
* regarding copyright ownership.  The ASF licenses this file
* to you under the Apache License, Version 2.0 (the
* "License"); you may not use this file except in compliance
* with the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
* KIND, either express or implied.  See the License for the
* specific language governing permissions and limitations
* under the License.
*/
#include "agent-framework/module/network_components.hpp"
#include "agent-framework/module/chassis_components.hpp"
#include "agent-framework/module/common_components.hpp"
#include "agent-framework/command-ref/network_commands.hpp"
#include "agent-framework/eventing/event_data.hpp"
#include "agent-framework/eventing/events_queue.hpp"
#include <ecrf_pal/watcher/ecrf_pal_sensor_task.hpp>
#include <thread>
#include <ecnet_helper/ecnet_helper.hpp>
#include <eclog_helper/eclog_helper.hpp>
#include <sys/sysinfo.h>
#include <atomic>
#include <mutex>
#include <condition_variable>
using namespace ecnet_helper;
using namespace eclog_helper;
using namespace agent_framework::model::attribute;

#include "ecrf_pal_helper/api/ecrf_pal_helper.hpp"
using namespace ecrf_pal_helper;

using namespace std;
using namespace agent_framework::model;
using namespace agent_framework::module;
using namespace agent::chassis;
using namespace agent::chassis::ecrf_pal;
using namespace agent::chassis::ecrf_pal::watcher;
using namespace agent_framework::command_ref;
using namespace agent_framework::model::enums;
using namespace agent_framework::model::attribute;

using agent_framework::module::ChassisComponents;
using agent_framework::module::CommonComponents;
using agent_framework::module::NetworkComponents;
ecRF_PalSensorTask::~ecRF_PalSensorTask() {}
auto &RFD = ADbg::get_instance();
/*! Drawer ecrf_pal sensor processing*/
class RF_PalInfo
{
public:
    /*!
     * Executes Drawer sensor processing
     */
    void execute()
    {
        get_ecrf_pal_info();  // get device info. from driver layer every 3 seconds//
    }
    void get_ecrf_pal_port_info();
    void get_ecrf_pal_port_oom_info();
    void get_ecrf_pal_port_static_info();
    void get_sys_resource_info();

    int oom_period = 15; //seconds  //For update oom information 
    int port_period = 3; //seconds  //For update port present/not present information
    int sys_resource_period = 15; //seconds  //For update cpu/mem/storage usage 
private:
    void get_ecrf_pal_info();
};

void ecRF_PalSensorTask::execute()
{
    try
    {
        RF_PalInfo ps{};
        ps.execute();

        if (!m_port_detect_thread)
        {
            std::thread mThread_port(&RF_PalInfo::get_ecrf_pal_port_info, &ps);
            mThread_port.detach();
            m_port_detect_thread = true;
        }
    }
    catch (const std::exception &e)
    {
        log_debug(LOGUSR, "RF_PalInfo - exception : " << e.what());
    }
}

void RF_PalInfo::get_ecrf_pal_info()
{
    try
    {
        auto &secrf_pal = Switch::Switch::get_instance();

        /*Get/Set  FAN info.*/
        secrf_pal.get_fan_info();
        unsigned int fan_num = secrf_pal.get_fan_num();

        unsigned int fanid = 1;

        for (fanid = 1; fanid <= fan_num; fanid++)
        {
            auto &fan_manager = agent_framework::module::ChassisComponents::get_instance()->get_fan_manager();
            auto fan_uuids = fan_manager.get_keys();

            for (const auto &fan_uuid : fan_uuids)
            {
                auto fan_ = fan_manager.get_entry_reference(fan_uuid); //Get Fan object by fan_uuid//

                if (fan_->get_fan_id() == fanid)
                {
                    int current_rpm = secrf_pal.get_fan_info_by_(fanid, Switch::Fan_Content::RPM);
                    fan_->set_current_speed(current_rpm);

                    int current_fan_type = secrf_pal.get_fan_info_by_(fanid, Switch::Fan_Content::Type);
                    fan_->set_fan_type(current_fan_type);

                    std::string current_health = secrf_pal.get_fan_info_by_(fanid, "Status_Health");
                    fan_->set_status_health(current_health);

                    std::string current_state = secrf_pal.get_fan_info_by_(fanid, "Status_State");
                    fan_->set_status_state(current_state);
                }
            }
        }

        secrf_pal.update_fan_present_event();

        /*Get/Set  PSU info.*/
        secrf_pal.get_psu_info();

        unsigned int psu_num = secrf_pal.get_psu_num();

        unsigned int psuid = 1;

        auto &psu_manager = agent_framework::module::ChassisComponents::get_instance()->get_psu_manager();

        auto psu_uuids = psu_manager.get_keys();

        for (psuid = 1; psuid <= psu_num; psuid++)
        {
            for (const auto &psu_uuid : psu_uuids)
            {
                auto psu_ = psu_manager.get_entry_reference(psu_uuid); //Get Psu object by psu_uuid//

                if (psu_->get_psu_id() == psuid)
                {
                    int p_in = secrf_pal.get_psu_info_by_(psuid, Switch::Psu_Content::Pin);
                    psu_->set_power_input(p_in);

                    int p_out = secrf_pal.get_psu_info_by_(psuid, Switch::Psu_Content::Pout);
                    psu_->set_power_output(p_out);

                    std::string current_health = secrf_pal.get_psu_info_by_(psuid, "Status_Health");
                    psu_->set_status_health(current_health);

                    std::string current_state = secrf_pal.get_psu_info_by_(psuid, "Status_State");
                    psu_->set_status_state(current_state);

                    int c_in = secrf_pal.get_psu_info_by_(psuid, Switch::Psu_Content::Iin);
                    psu_->set_current_input(c_in);

                    int c_out = secrf_pal.get_psu_info_by_(psuid, Switch::Psu_Content::Iout);
                    psu_->set_current_output(c_out);

                    int v_in = secrf_pal.get_psu_info_by_(psuid, Switch::Psu_Content::Vin);
                    psu_->set_voltage_input(v_in);

                    int v_out = secrf_pal.get_psu_info_by_(psuid, Switch::Psu_Content::Vout);
                    psu_->set_voltage_output(v_out);

                    int psu_type = secrf_pal.get_psu_info_by_(psuid, Switch::Psu_Content::Psu_type);
                    psu_->set_psu_type(psu_type);

                    std::string sn = secrf_pal.get_psu_info_by_(psuid, "SN");
                    psu_->set_psu_sn(sn);

                    std::string model = secrf_pal.get_psu_info_by_(psuid, "Model");
                    psu_->set_psu_module(model);
                }
            }
        }

        secrf_pal.update_psu_present_event();

        /*Get/Set  Thernal info.*/
        secrf_pal.get_thermal_info();

        unsigned int thermal_num = secrf_pal.get_thermal_num();
        unsigned int thermalid = 1;

        auto &tz_manager = agent_framework::module::ChassisComponents::get_instance()->get_thermal_zone_manager();

        auto tz_uuids = tz_manager.get_keys();

        for (thermalid = 1; thermalid <= thermal_num; thermalid++)
        {
            for (const auto &tz_uuid : tz_uuids)
            {
                auto tz_ = tz_manager.get_entry_reference(tz_uuid); //Get Psu object by psu_uuid//
                if (tz_->get_tz_id() == thermalid)
                {
                    double current_temp = secrf_pal.get_thermal_info_by_(thermalid, Switch::Thermal_Content::Current_Temperature);
                    tz_->set_temperature(current_temp);

                    int current_thermal_type = (int)secrf_pal.get_thermal_info_by_(thermalid, Switch::Thermal_Content::Thermal_Type);
                    tz_->set_thermal_type(current_thermal_type);

                    std::string current_health = secrf_pal.get_thermal_info_by_(thermalid, "Status_Health");
                    tz_->set_status_health(current_health);

                    std::string current_state = secrf_pal.get_thermal_info_by_(thermalid, "Status_State");
                    tz_->set_status_state(current_state);

                    double warning_upper_tmp = secrf_pal.get_thermal_info_by_(thermalid, Switch::Thermal_Content::Warning_uppper);
                    tz_->set_warning_upper_temp(warning_upper_tmp);

                    double warning_lower_tmp = secrf_pal.get_thermal_info_by_(thermalid, Switch::Thermal_Content::Warning_lower);
                    tz_->set_warning_lower_temp(warning_lower_tmp);

                    double error_tmp = secrf_pal.get_thermal_info_by_(thermalid, Switch::Thermal_Content::Error);
                    tz_->set_error_temp(error_tmp);

                    double shutdown_tmp = secrf_pal.get_thermal_info_by_(thermalid, Switch::Thermal_Content::Shutdown);
                    tz_->set_shutdown_temp(shutdown_tmp);
                }
            }
        }
        secrf_pal.update_thermal_present_event();
    }
    catch (const std::exception &e)
    {
        log_debug(LOGUSR, "get_ecrf_pal_info - exception : " << e.what());
    }
}

[[noreturn]] void RF_PalInfo::get_sys_resource_info()
{
    auto &secrf_pal = Switch::Switch::get_instance();
    while (true)
    {
        sleep(sys_resource_period);
    }
}

[[noreturn]] void RF_PalInfo::get_ecrf_pal_port_info() 
{
    get_ecrf_pal_port_static_info();

    auto &secrf_pal = Switch::Switch::get_instance();

    while (true)
    {
        try
        {
            /*Get/Set  Port info.*/
            secrf_pal.get_port_present_info();
            secrf_pal.update_port_present_event();
            /*Send all events */
            std::vector<EventMsgContent_T> tmp_e_a = secrf_pal.get_Event_Port_Resouce_Add();

            for (unsigned int i = 0; i < tmp_e_a.size(); i++)
            {
                EventMsgContent_T t_es = tmp_e_a[i];
                agent_framework::eventing::EventData edat;
                edat.set_notification(::agent_framework::eventing::Notification::ResourceAdded);
                edat.set_event_content(t_es.message_content);
                edat.set_event_origin_of_condition(t_es.origin_of_condition);
                edat.set_event_message_id(t_es.message_id);
                agent_framework::eventing::EventsQueue::get_instance()->push_back(edat);
            }

            std::vector<EventMsgContent_T> tmp_e_r = secrf_pal.get_Event_Port_Resouce_Remove();

            for (unsigned int i = 0; i < tmp_e_r.size(); i++)
            {
                EventMsgContent_T  t_er = tmp_e_r[i];
                agent_framework::eventing::EventData edat;
                edat.set_notification(::agent_framework::eventing::Notification::ResourceRemoved);
                edat.set_event_content(t_er.message_content);
                edat.set_event_origin_of_condition(t_er.origin_of_condition);
                edat.set_event_message_id(t_er.message_id);
                agent_framework::eventing::EventsQueue::get_instance()->push_back(edat);
            }

            std::vector<EventMsgContent_T> tmp_e_al = secrf_pal.get_Event_Port_Resouce_Alert();

            for (unsigned int i = 0; i < tmp_e_al.size(); i++)
            {
                EventMsgContent_T t_ea = tmp_e_al[i];
                agent_framework::eventing::EventData edat;
                edat.set_notification(::agent_framework::eventing::Notification::Alert);
                edat.set_event_content(t_ea.message_content);
                edat.set_event_origin_of_condition(t_ea.origin_of_condition);
                edat.set_event_message_id(t_ea.message_id);
                agent_framework::eventing::EventsQueue::get_instance()->push_back(edat);
            }
            secrf_pal.clean_Event_Port_Resource_Event(); //Reset event //
        }
        catch (const std::exception &e)
        {
            log_debug(LOGUSR, "get_ecrf_pal_port_info - exception : " << e.what());
        }
        sleep(port_period);
    }
}

[[noreturn]] void RF_PalInfo::get_ecrf_pal_port_oom_info()
{
    auto &secrf_pal = Switch::Switch::get_instance();

    while (true)
    {
         sleep(oom_period);
    }
}

void RF_PalInfo::get_ecrf_pal_port_static_info()
{
    auto &secrf_pal = Switch::Switch::get_instance();
    try
    {
        /*Get/Set Port info.*/
        unsigned int port_num = secrf_pal.get_port_num();
        unsigned int portid = 1;

        auto network_components = agent_framework::module::NetworkComponents::get_instance();
        auto &port_manager = network_components->get_instance()->get_port_manager();
        auto port_uuids = port_manager.get_keys();

        for (portid = 1; portid <= port_num; portid++)
        {
            for (const auto &port_uuid : port_uuids)
            {
                auto port_ = port_manager.get_entry_reference(port_uuid); //Get Port object by psu_uuid//

                if (port_->get_port_id() == portid)
                {
                    if (secrf_pal.get_port_info_by_(portid, Switch::Port_Content::Port_Type) == ecrf_pal_helper::Port_Info::Port_Type::PON_Port)
                    {
                        port_->set_port_type(PortType::Downstream);
                        port_->set_port_identifier("PON port");
                    }
                    else if (secrf_pal.get_port_info_by_(portid, Switch::Port_Content::Port_Type) == ecrf_pal_helper::Port_Info::Port_Type::SFP_Port)
                    {
                        port_->set_port_type(PortType::MeshPort);
                        port_->set_port_identifier("SFP port");
                    }
                    else if (secrf_pal.get_port_info_by_(portid, Switch::Port_Content::Port_Type) == ecrf_pal_helper::Port_Info::Port_Type::QSFP_Port)
                    {
                        port_->set_port_type(PortType::MeshPort);
                        port_->set_port_identifier("QSFP port");
                    }
                    else
                    {
                        port_->set_port_type(PortType::MeshPort);
                        port_->set_port_identifier("RJ45 port");
                    }
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        log_debug(LOGUSR, "get_ecrf_pal_port_static_info - exception : " << e.what());
    }
}
