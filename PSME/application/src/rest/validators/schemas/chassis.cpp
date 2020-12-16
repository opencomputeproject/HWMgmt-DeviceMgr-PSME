/*!
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
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * */

#include "psme/rest/validators/schemas/chassis.hpp"
#include "psme/rest/constants/constants.hpp"

using namespace psme::rest;
using namespace psme::rest::validators::schema;

const jsonrpc::ProcedureValidator& ChassisPatchSchema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "chassis_patch",
        jsonrpc::PARAMS_BY_NAME,
        constants::Common::ASSET_TAG, VALID_OPTIONAL(VALID_JSON_STRING),
        nullptr
    };
    return procedure;
}

const jsonrpc::ProcedureValidator &ChassisThPatchSchema::FanThresholdSchema::get_procedure()
{
    static jsonrpc::ProcedureValidator procedure{
        "chassis_fan_threshold_patch",
        jsonrpc::PARAMS_BY_NAME,
        constants::ChassisThreshold::LOWERTHRESHOLDNONCRITICAL, VALID_JSON_REAL,
        constants::ChassisThreshold::UPPERTHRESHOLDNONCRITICAL, VALID_JSON_REAL,
        constants::ChassisThreshold::MEMBER_ID, VALID_JSON_STRING,
        nullptr};
    return procedure;
}

const jsonrpc::ProcedureValidator& ChassisThPatchSchema::ThermalThresholdSchema::get_procedure()
{
    static jsonrpc::ProcedureValidator procedure{
        "chassis_thermal_threshold_patch",
        jsonrpc::PARAMS_BY_NAME,
        constants::ChassisThreshold::LOWERTHRESHOLDNONCRITICAL, VALID_JSON_REAL,
        constants::ChassisThreshold::UPPERTHRESHOLDNONCRITICAL, VALID_JSON_REAL,
        constants::ChassisThreshold::MEMBER_ID, VALID_JSON_STRING,
        nullptr};
    return procedure;
}

const jsonrpc::ProcedureValidator& ChassisThPatchSchema::get_procedure()
{
    static jsonrpc::ProcedureValidator procedure{
        "chassis_threshold_patch",
        jsonrpc::PARAMS_BY_NAME,
        constants::ChassisThreshold::TEMPERATURES, VALID_OPTIONAL(VALID_ATTRIBUTE(ThermalThresholdSchema)),
        nullptr
    };
    return procedure;
}

