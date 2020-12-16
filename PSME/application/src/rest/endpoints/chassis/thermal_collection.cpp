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
#include "psme/rest/endpoints/chassis/thermal_collection.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/utils.hpp"
#include "psme/rest/endpoints/chassis/thermal_zone.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/chassis.hpp"

#include "ecrf_pal_helper/api/ecrf_pal_helper.hpp"
#include "ecrf_pal_helper/api/ecrf_pal_helper_api.hpp"
#include "ecrf_pal_helper/api/ecrf_pal_com.hpp"
#include <eclog_helper/eclog_helper.hpp>
using namespace eclog_helper;

using namespace ecrf_pal_helper;
using namespace ecrf_pal_com_s;
using namespace ecrf_pal_com_api;

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::endpoint;
using agent_framework::module::ChassisComponents;
using namespace psme::rest::validators;

#define PSU_NUM 2
unsigned int UPPER_SYS_THRESHOLD_NON_CRITICAL = 0;
unsigned int UPPER_SYS_THRESHOLD_CRITICAL = 0;
unsigned int UPPER_SYS_THRESHOLD_FATAL = 0;

unsigned int UPPER_CPU_THRESHOLD_NON_CRITICAL = 0;
unsigned int UPPER_CPU_THRESHOLD_CRITICAL = 0;
unsigned int UPPER_CPU_THRESHOLD_FATAL = 0;

#define ZERO 0
auto &ADF = ADbg::get_instance();
namespace
{
json::Value make_prototype()
{
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#Thermal.Thermal";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#Thermal.v1_1_0.Thermal";
    r[Common::NAME] = "Thermal";
    r[Common::ID] = "Thermal";
    r[Common::DESCRIPTION] = "Collection of Thermal Sensors";
    r[constants::Switch::REDUNDANCY] = json::Value::Type::ARRAY;
    return r;
}
} // namespace

ThermalCollection::ThermalCollection(const std::string &path) : EndpointBase(path) {}

ThermalCollection::~ThermalCollection() {}

void ThermalCollection::get(const server::Request &req, server::Response &res)
{
    auto json = ::make_prototype();

    json[Common::ODATA_ID] = PathBuilder(req).build();

    auto &tz_manager = agent_framework::module::ChassisComponents::get_instance()->get_thermal_zone_manager();
    auto tz_uuids = tz_manager.get_keys();
    auto chassis = psme::rest::model::Find<agent_framework::model::Chassis>(req.params[PathParam::CHASSIS_ID]).get();

    //Set thermal related info. //
    rfpal_thresholds threshold_info = {0};

    for (const auto &tz_uuid : tz_uuids)
    {
        auto tz_ = tz_manager.get_entry(tz_uuid); //Get TZ object by fan_uuid//
        int thermal_id = tz_.get_tz_id();
        json::Value jsontmp;

        jsontmp[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL).append(Common::CHASSIS).append(chassis.get_id()).append("Thermal").build();
        jsontmp[Common::MEMBER_ID] = std::to_string(thermal_id);

        int thermal_type = tz_.get_thermal_type();

        if (thermal_type == ecrf_pal_helper::Thermal_Info::PSU_Sensor)
        {
            jsontmp[Common::NAME] = "PSU Thermal Sensor Temperature";
            jsontmp[constants::ThermalZoneCollection::PHYSICAL_CONTEXT] = "PowerSupply";
        }
        else if (thermal_type == ecrf_pal_helper::Thermal_Info::CPU_Sensor)
        {
            jsontmp[Common::NAME] = "System CPU Thermal Sensor Temperature";
            jsontmp[constants::ThermalZoneCollection::PHYSICAL_CONTEXT] = "CPU";
        }
        else if (thermal_type == ecrf_pal_helper::Thermal_Info::SYSTEM_Sensor)
        {
            jsontmp[Common::NAME] = "Chassis Thermal Sensor Temperature";
            jsontmp[constants::ThermalZoneCollection::PHYSICAL_CONTEXT] = "SystemBoard";
        }
        else
        {
            jsontmp[Common::NAME] = "unknow";
            jsontmp[constants::ThermalZoneCollection::PHYSICAL_CONTEXT] = "unknow";
        }

        jsontmp[constants::ThermalZoneCollection::SENSOR_NUMBER] = thermal_id;
        jsontmp[Common::STATUS][Common::HEALTH_ROLLUP] = json::Value::Type::NIL;
        jsontmp[constants::ThermalZoneCollection::READING_CELSIUS] = (tz_.get_temperature() * 0.001);

        //Get real time data from pal//
        if (rfpal_thermal_thresholds_get(thermal_id, &threshold_info) == ECRF_PAL_STATUS_OK)
        {
            jsontmp[ChassisThreshold::LOWERTHRESHOLDNONCRITICAL] = (threshold_info.warning_lower * 0.001);
            jsontmp[ChassisThreshold::UPPERTHRESHOLDNONCRITICAL] = (threshold_info.warning_upper * 0.001);
        }
        else
        {
            jsontmp[ChassisThreshold::LOWERTHRESHOLDNONCRITICAL] = (tz_.get_warning_lower_temp() * 0.001);
            jsontmp[ChassisThreshold::UPPERTHRESHOLDNONCRITICAL] = (tz_.get_warning_upper_temp() * 0.001);
        }
        jsontmp[ChassisThreshold::UPPERTHRESHOLDCRITICAL] = (tz_.get_error_temp() * 0.001);
        jsontmp[ChassisThreshold::UPPERTHRESHOLDFATAL] = (tz_.get_shutdown_temp() * 0.001);

        jsontmp[Common::STATUS][Common::HEALTH_ROLLUP] = tz_.get_status_health();
        jsontmp[Common::STATUS][Common::STATE] = tz_.get_status_state();

        json::Value lrejsontmp;
        lrejsontmp[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL).append(Common::CHASSIS).append(chassis.get_id()).build();
        jsontmp[constants::ThermalZoneCollection::RELATED_ITEM].push_back(std::move(lrejsontmp));
        json[constants::Root::TEMPS].push_back(std::move(jsontmp));
    }

    //Set FAN related info. //
    auto &fan_manager = agent_framework::module::ChassisComponents::get_instance()->get_fan_manager();
    auto fan_uuids = fan_manager.get_keys();

    for (const auto &fan_uuid : fan_uuids)
    {
        auto fan_ = fan_manager.get_entry(fan_uuid); //Get Fan object by fan_uuid//
        int fan_id = (int)fan_.get_id();
        signed int RPM = fan_.get_current_speed();
        json::Value jsontmp;

        jsontmp[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL).append(Common::CHASSIS).append(chassis.get_id()).append("Thermal").build();

        jsontmp[Common::MEMBER_ID] = std::to_string(fan_id);

        int fan_type = fan_.get_fan_type();

        if (fan_type == ecrf_pal_helper::Fan_Info::PSU_Fan)
        {
            jsontmp[Common::NAME] = "PSU Fan";
            jsontmp[constants::ThermalZoneCollection::PHYSICAL_CONTEXT] = "PowerSupply";
        }
        else if (fan_type == ecrf_pal_helper::Fan_Info::SYSTEM_Fan)
        {
            jsontmp[Common::NAME] = "System Fan";
            jsontmp[constants::ThermalZoneCollection::PHYSICAL_CONTEXT] = "Back";
        }
        else
        {
            jsontmp[Common::NAME] = "unknow";
            jsontmp[constants::ThermalZoneCollection::PHYSICAL_CONTEXT] = "unknow";
        }

        if (RPM >= 0)
            jsontmp[constants::ThermalZoneCollection::READING] = RPM;
        else
            jsontmp[constants::ThermalZoneCollection::READING] = json::Value::Type::NIL;

        jsontmp[Common::STATUS][Common::HEALTH_ROLLUP] = json::Value::Type::NIL;

        std::string str_Health = fan_.get_status_health();
        std::string str_State = fan_.get_status_state();

        jsontmp[Common::STATUS][Common::HEALTH_ROLLUP] = str_Health.c_str();
        jsontmp[Common::STATUS][Common::STATE] = str_State.c_str();

        jsontmp[constants::ThermalZoneCollection::READING_UNITS] = "RPM";
        jsontmp[ChassisThreshold::LOWERTHRESHOLDNONCRITICAL] = json::Value::Type::NIL;
        jsontmp[ChassisThreshold::UPPERTHRESHOLDNONCRITICAL] = json::Value::Type::NIL;
        jsontmp[ChassisThreshold::UPPERTHRESHOLDCRITICAL] = json::Value::Type::NIL;
        jsontmp[ChassisThreshold::UPPERTHRESHOLDFATAL] = json::Value::Type::NIL;

        json::Value lrejsontmp;
        lrejsontmp[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL).append(Common::CHASSIS).append(chassis.get_id()).build();
        jsontmp[constants::ThermalZoneCollection::RELATED_ITEM].push_back(std::move(lrejsontmp));
        json[constants::Root::FANS].push_back(std::move(jsontmp));
    }
    set_response(res, json);
}

void ThermalCollection::patch(const server::Request &request, server::Response &response)
{
    try
    {
        rfpal_thresholds threshold_info = {0};

        auto json = JsonValidator::validate_request_body<schema::ChassisThPatchSchema>(request);
        unsigned m_id = 0;
        double lower_value = 0;
        double upper_value = 0;

        if (json.is_member(ChassisThreshold::TEMPERATURES) && json[ChassisThreshold::TEMPERATURES].is_member(Common::MEMBER_ID))
            m_id = std::atoi(json[ChassisThreshold::TEMPERATURES][Common::MEMBER_ID].as_string().c_str());

        if (json.is_member(ChassisThreshold::TEMPERATURES) && json[ChassisThreshold::TEMPERATURES].is_member(ChassisThreshold::LOWERTHRESHOLDNONCRITICAL))
            lower_value =(double) json[ChassisThreshold::TEMPERATURES][ChassisThreshold::LOWERTHRESHOLDNONCRITICAL].as_double();

        if (json.is_member(ChassisThreshold::TEMPERATURES) && json[ChassisThreshold::TEMPERATURES].is_member(ChassisThreshold::UPPERTHRESHOLDNONCRITICAL))
            upper_value =(double)  json[ChassisThreshold::TEMPERATURES][ChassisThreshold::UPPERTHRESHOLDNONCRITICAL].as_double();

        if (lower_value >= upper_value)
        {
            ADF.acc_printf(LEVEL_WARN, "lower_value >= upper_value");
            response.set_status(server::status_4XX::BAD_REQUEST);
            return;
        }

        if (lower_value <= 0 ||  upper_value <= 0)
        {
            ADF.acc_printf(LEVEL_WARN, "setting value < zero");
            response.set_status(server::status_4XX::BAD_REQUEST);
            return;
        }

        if (upper_value > 150)
        {
            ADF.acc_printf(LEVEL_WARN, "setting value > 150");
            response.set_status(server::status_4XX::BAD_REQUEST);
            return;
        }

        threshold_info.warning_lower = lower_value;
        threshold_info.warning_upper = upper_value;

        if (rfpal_thermal_thresholds_set(m_id, threshold_info) == ECRF_PAL_STATUS_OK)
        {
            response.set_status(server::status_2XX::OK);
        }
        else
        {
            ADF.acc_printf(LEVEL_WARN, "rfpat_thermal_thresholds_set error");
            response.set_status(server::status_4XX::BAD_REQUEST);
    }
    }
    catch (const std::exception &ex)
    {
		log_warning(GET_LOGGER("rest"), "thermal or fan threshold patch error!!");
        response.set_status(server::status_4XX::BAD_REQUEST);
    }
}