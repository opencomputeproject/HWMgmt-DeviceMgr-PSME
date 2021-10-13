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
#include "agent-framework/module/chassis_components.hpp"
#include "psme/rest/endpoints/chassis/psu_collection.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/utils.hpp"
#include "agent-framework/module/constants/chassis.hpp"

#include "ecrf_pal_helper/api/ecrf_pal_helper.hpp"
using namespace ecrf_pal_helper;

using namespace psme::rest;
using namespace psme::rest::endpoint;
using namespace psme::rest::constants;
using agent_framework::module::ChassisComponents;

namespace
{
json::Value make_prototype()
{
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#Power.Power";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ID] = "Power";
    r[Common::ODATA_TYPE] = "#Power.v1_1_0.Power";
    r[Common::NAME] = "Power Collection";
    r[Common::DESCRIPTION] = "Collection of Power";

    return r;
}
} // namespace

PsuCollection::PsuCollection(const std::string &path) : EndpointBase(path) {}

PsuCollection::~PsuCollection() {}

void PsuCollection::get(const server::Request &req, server::Response &res)
{
    try
    {
        int system_power_control_id = 1;
        auto json = ::make_prototype();

        json[Common::ODATA_ID] = PathBuilder(req).build();
        json[Common::ID] = "Power";
        //For PSU
        auto chassis_ids = ChassisComponents::get_instance()->get_thermal_zone_manager().get_ids();
        auto chassis = psme::rest::model::Find<agent_framework::model::Chassis>(req.params[PathParam::CHASSIS_ID]).get();

        auto &psu_manager = agent_framework::module::ChassisComponents::get_instance()->get_psu_manager();
        auto psu_uuids = psu_manager.get_keys();

        for (const auto &psu_uuid : psu_uuids)
        {
            auto psu_ = psu_manager.get_entry(psu_uuid); //Get Fan object by fan_uuid//

            json::Value jsonctrl;
            {
                jsonctrl[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL).append(Common::CHASSIS).append(chassis.get_id()).append("Power").build();

                jsonctrl[Common::MEMBER_ID] = std::to_string(psu_.get_psu_id());
                jsonctrl[Common::NAME] = std::string("System Power Control ") + std::to_string(system_power_control_id);

                jsonctrl[PowerZone::POWER_CONSUMED] = (psu_.get_power_output() * 0.001);
                jsonctrl[Common::STATUS][Common::HEALTH] = psu_.get_status_health();
                jsonctrl[Common::STATUS][Common::STATE] = psu_.get_status_state();

                jsonctrl[PowerZone::POWER_REQUESTED_WATTS] = json::Value::Type::NIL;
                jsonctrl[PowerZone::POWER_AVAILABLE_WATTS] = json::Value::Type::NIL;
                jsonctrl[PowerZone::POWER_CAPACITY_WATTS] = 0;
                jsonctrl[PowerZone::POWER_ALLOCATED_WATTS] = json::Value::Type::NIL;

                json::Value jsonctrl_Metrics;

                jsonctrl_Metrics[PowerZone::POWER_METRICS][PowerZone::INTER_VAL_IN_MIN] = json::Value::Type::NIL;
                jsonctrl_Metrics[PowerZone::POWER_METRICS][PowerZone::MIN_CONSUMED_WATTS] = json::Value::Type::NIL;
                jsonctrl_Metrics[PowerZone::POWER_METRICS][PowerZone::MAX_CONSUMED_WATTS] = json::Value::Type::NIL;
                jsonctrl_Metrics[PowerZone::POWER_METRICS][PowerZone::AVG_CONSUMED_WATTS] = json::Value::Type::NIL;
                jsonctrl.push_back(std::move(jsonctrl_Metrics));

                jsonctrl[PowerZone::POWER_LIMIT][PowerZone::LIMIT_IN_WATTS] = json::Value::Type::NIL;
                jsonctrl[PowerZone::POWER_LIMIT][PowerZone::LIMIT_EXCEPTION] = json::Value::Type::NIL;
                jsonctrl[PowerZone::POWER_LIMIT][PowerZone::CORRECTION_IN_MS] = json::Value::Type::NIL;

                json::Value lrejsontmp;
                lrejsontmp[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL).append(Common::CHASSIS).append(chassis.get_id()).build();
                jsonctrl[constants::ThermalZoneCollection::RELATED_ITEM].push_back(std::move(lrejsontmp));
                jsonctrl[Common::OEM] = json::Value::Type::OBJECT;
                json[PowerZone::POWER_CONTROL].push_back(std::move(jsonctrl));
            }

            json::Value jsonVoltages;
            {
                int id = psu_.get_psu_id(); 

                jsonVoltages[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL).append(Common::CHASSIS).append(chassis.get_id()).append("Power").build();
                jsonVoltages[Common::MEMBER_ID] = std::to_string(psu_.get_psu_id());
                jsonVoltages[Common::NAME] = std::string("Power supply ") + std::to_string(system_power_control_id) + std::string(" Voltage");
                jsonVoltages[ThermalZone::SENSOR_NUMBER] = id; 
                jsonVoltages[PowerZone::READING_VOLTS] = (psu_.get_voltage_output() * 0.001); 
                jsonVoltages[ChassisThreshold::UPPERTHRESHOLDNONCRITICAL] = json::Value::Type::NIL;
                jsonVoltages[ChassisThreshold::LOWERTHRESHOLDNONCRITICAL] = json::Value::Type::NIL;
                jsonVoltages[ChassisThreshold::UPPERTHRESHOLDCRITICAL] = json::Value::Type::NIL;
                jsonVoltages[ChassisThreshold::UPPERTHRESHOLDFATAL] = json::Value::Type::NIL;
                jsonVoltages[ThermalZone::PHYSICAL_CONTEXT] = "VoltageRegulator";
                jsonVoltages[Common::STATUS][Common::STATE] =  psu_.get_status_state();
                jsonVoltages[Common::STATUS][Common::HEALTH] =  psu_.get_status_health();

                json::Value lrejsontmp;
                lrejsontmp[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL).append(Common::CHASSIS).append(chassis.get_id()).build();
                jsonVoltages[constants::ThermalZoneCollection::RELATED_ITEM].push_back(std::move(lrejsontmp));
            }

            json[Root::PVOLTAGE].push_back(std::move(jsonVoltages));

            json::Value json_PowerSupplies;
            {
                json_PowerSupplies[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL).append(Common::CHASSIS).append(chassis.get_id()).append("Power").build();

                json_PowerSupplies[Common::MEMBER_ID] = std::to_string(psu_.get_psu_id());
                json_PowerSupplies[Common::NAME] = std::string("Power Supplies Unit ") + std::to_string(system_power_control_id);
                json_PowerSupplies[Common::OEM] = json::Value::Type::OBJECT;

                switch (psu_.get_psu_type())
                {
                case 0:
                    json_PowerSupplies[PowerZone::POWER_SUPPLY_TYPE] = "AC";
                    break;
                case 1:
                    json_PowerSupplies[PowerZone::POWER_SUPPLY_TYPE] = "DC";
                    break;
                case 2:
                    json_PowerSupplies[PowerZone::POWER_SUPPLY_TYPE] = "DC";
                    break;
                default:
                    json_PowerSupplies[PowerZone::POWER_SUPPLY_TYPE] = "Unknown";
                }

                json_PowerSupplies[PowerZone::LINE_INPUT_VOLTAGE_TYPE] = json::Value::Type::NIL;
                json_PowerSupplies[PowerZone::LINE_INPUT_VOLTAGE] = json::Value::Type::NIL;
                json_PowerSupplies[PowerZone::POWER_CAPACITY_WATTS] = json::Value::Type::NIL;
                json_PowerSupplies[PowerZone::LAST_POWER_OUTPUT] = (psu_.get_power_output() * 0.001);
                json_PowerSupplies[Common::MODEL] = psu_.get_psu_module();
                json_PowerSupplies[Common::MANUFACTURER] = json::Value::Type::NIL;
                json_PowerSupplies[Common::FIRMWARE_VERSION] = json::Value::Type::NIL;
                json_PowerSupplies[Common::SERIAL] = psu_.get_psu_sn();
                json_PowerSupplies[Common::PART_NUMBER] = json::Value::Type::NIL;
                json_PowerSupplies[Common::SPARE_PART_NUMBER] = json::Value::Type::NIL;
                json_PowerSupplies[Common::STATUS][Common::STATE] = psu_.get_status_state();
                json_PowerSupplies[Common::STATUS][Common::HEALTH] = psu_.get_status_health();

                json::Value json_input_range;

                json_input_range[PowerZone::LINE_INPUT_TYPE] = json::Value::Type::NIL;
                json_input_range[PowerZone::MINIMUM_VOLTAGE] = json::Value::Type::NIL;
                json_input_range[PowerZone::MAXIMUM_VOLTAGE] = json::Value::Type::NIL;
                json_input_range[PowerZone::OUTPUT_WATTAGE] = json::Value::Type::NIL;

                json::Value lrejsontmp;
                lrejsontmp[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL).append(Common::CHASSIS).append(chassis.get_id()).build();
                json_PowerSupplies[constants::ThermalZoneCollection::RELATED_ITEM].push_back(std::move(lrejsontmp));
            }

            json[PowerZone::POWER_SUPPLIES].push_back(std::move(json_PowerSupplies));
            system_power_control_id++;
        }
        json[Common::OEM] = json::Value::Type::OBJECT;
        set_response(res, json);
    }
    catch (const std::exception &e)
    {
        log_debug(LOGUSR, "PsuCollection get - exception : " << e.what());
    }
}
