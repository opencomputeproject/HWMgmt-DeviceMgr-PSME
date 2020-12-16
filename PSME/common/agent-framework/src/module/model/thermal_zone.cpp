/*!
 * @section LICENSE
 *
 * @copyright
 * Copyright (c) 2015-2017 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @section DESCRIPTION
 *
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * */

#include "agent-framework/module/model/thermal_zone.hpp"
#include "agent-framework/module/constants/chassis.hpp"

using namespace agent_framework::model;
using namespace agent_framework::model::utils;

const enums::Component ThermalZone::component = enums::Component::ThermalZone;
const enums::CollectionName ThermalZone::collection_name = enums::CollectionName::ThermalZones;

ThermalZone::ThermalZone(const std::string& parent_uuid, enums::Component parent_type) :
    Resource{parent_uuid, parent_type} {}

ThermalZone::~ThermalZone() {}

Json::Value ThermalZone::to_json() const {
    Json::Value result;
    result[literals::ThermalZone::STATUS] = get_status().to_json();
    result[literals::ThermalZone::TEMPERATURE] = get_temperature();
    result[literals::ThermalZone::COLLECTIONS] = get_collections().to_json();
    result[literals::ThermalZone::OEM] = get_oem().to_json();

    result[literals::ThermalZone::CHASSIS] = get_chassis();    
    result[literals::ThermalZone::TZ_ID]  = get_tz_id();    
    result[literals::ThermalZone::STATUS_HEALTH] = get_status_health();
    result[literals::ThermalZone::STATUS_STATE] = get_status_state();	
    result[literals::ThermalZone::WARNING_TEMP_UPPER] = get_warning_upper_temp();
    result[literals::ThermalZone::WARNING_TEMP_LOWER] = get_warning_lower_temp();
    result[literals::ThermalZone::ERROR_TEMP] = get_error_temp();
    result[literals::ThermalZone::SHUTDOWN_TEMP] = get_shutdown_temp();	
    result[literals::ThermalZone::THERMAL_TYPE] = get_thermal_type();	

    return result;
}

ThermalZone ThermalZone::from_json(const Json::Value& json) {
    ThermalZone zone;

    zone.set_status(attribute::Status::from_json(json[literals::ThermalZone::STATUS]));
    zone.set_temperature(json[literals::ThermalZone::TEMPERATURE]);
    zone.set_collections(Collections::from_json(json[literals::ThermalZone::COLLECTIONS]));
    zone.set_oem(attribute::Oem::from_json(json[literals::ThermalZone::OEM]));
    zone.set_resource_hash(json);

    zone.set_chassis(json[literals::ThermalZone::CHASSIS]);   
    zone.set_tz_id(json[literals::ThermalZone::TZ_ID]);    
    zone.set_status_health(json[literals::ThermalZone::STATUS_HEALTH]);       
    zone.set_status_state(json[literals::ThermalZone::STATUS_STATE]);   
    zone.set_warning_upper_temp(json[literals::ThermalZone::WARNING_TEMP_UPPER].asDouble());       
    zone.set_warning_lower_temp(json[literals::ThermalZone::WARNING_TEMP_LOWER].asDouble());       
    zone.set_error_temp(json[literals::ThermalZone::ERROR_TEMP].asDouble());       
    zone.set_shutdown_temp(json[literals::ThermalZone::SHUTDOWN_TEMP].asDouble()); 
    zone.set_thermal_type(json[literals::ThermalZone::THERMAL_TYPE]);          

    return zone;
}
