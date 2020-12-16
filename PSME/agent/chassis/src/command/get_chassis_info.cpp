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
 * @file command/get_chassis_info.cpp
 * @brief GetChassisInfo acc-chassis implementation
 * */

#include "agent-framework/module/chassis_components.hpp"
#include "agent-framework/module/common_components.hpp"
#include "agent-framework/command-ref/registry.hpp"
#include "agent-framework/command-ref/chassis_commands.hpp"
#include "agent-framework/command-ref/compute_commands.hpp"
#include "agent-framework/module/compute_components.hpp"

using namespace agent_framework::command_ref;
using namespace agent_framework::module;

REGISTER_COMMAND(GetChassisInfo,
    [] (const GetChassisInfo::Request& req, GetChassisInfo::Response& rsp) {
        log_debug(GET_LOGGER("rpc"), "GetChassisInfo with parameters: chassis "
            << req.get_uuid());
        rsp = CommonComponents::get_instance()->get_chassis_manager().get_entry(req.get_uuid());
    }
);

using agent_framework::module::ChassisComponents;

REGISTER_COMMAND(GetFanInfo,
    [] (const GetFanInfo::Request& req, GetFanInfo::Response& rsp) {
         log_debug(GET_LOGGER("rpc"), "GetFanInfo with parameters: Fan "
            << req.get_fan());   
        rsp = ChassisComponents::get_instance()->get_fan_manager().get_entry(req.get_fan());
    }
);

REGISTER_COMMAND(GetThermalZoneInfo,
    [] (const GetThermalZoneInfo::Request& req, GetThermalZoneInfo::Response& rsp) {
         log_debug(GET_LOGGER("rpc"), "GetThermalZoneInfo with parameters: ThermalZone "
            << req.get_thermal_zone());   
        rsp = ChassisComponents::get_instance()->get_thermal_zone_manager().get_entry(req.get_thermal_zone());
    }
);

REGISTER_COMMAND(GetPowerZoneInfo,
    [] (const GetPowerZoneInfo::Request& req, GetPowerZoneInfo::Response& rsp) {
         log_debug(GET_LOGGER("rpc"), "GetPowerZoneInfo with parameters: PowerZone "
            << req.get_power_zone());   
        rsp = ChassisComponents::get_instance()->get_power_zone_manager().get_entry(req.get_power_zone());
    }
);

REGISTER_COMMAND(GetPsuInfo,
    [] (const GetPsuInfo::Request& req, GetPsuInfo::Response& rsp) {
         log_debug(GET_LOGGER("rpc"), "GetPsuInfo with parameters: PSU "
            << req.get_psu());   
        rsp = ChassisComponents::get_instance()->get_psu_manager().get_entry(req.get_psu());
    }
);

REGISTER_COMMAND(GetSystemInfo,
    [] (const GetSystemInfo::Request& req, GetSystemInfo::Response& rsp) {
        log_debug(GET_LOGGER("agent"), "Getting system info.");
        rsp = CommonComponents::get_instance()->get_system_manager().get_entry(req.get_uuid());
    }
);

#ifdef CSUBSYSTEM
REGISTER_COMMAND(GetProcessorInfo,
    [] (const GetProcessorInfo::Request& req, GetProcessorInfo::Response& rsp) {
        log_debug(GET_LOGGER("agent"), "Getting processor info.");
        rsp = ComputeComponents::get_instance()->get_processor_manager().get_entry(req.get_uuid());
    }
);

REGISTER_COMMAND(GetMemoryInfo,
    [] (const GetMemoryInfo::Request& req, GetMemoryInfo::Response& rsp) {
        log_debug(GET_LOGGER("agent"), "Getting memory info.");
        rsp = ComputeComponents::get_instance()->get_memory_manager().get_entry(req.get_uuid());
    }
);

REGISTER_COMMAND(GetStorageSubsystemInfo,
    [] (const GetStorageSubsystemInfo::Request& req, GetStorageSubsystemInfo::Response& rsp) {
        log_debug(GET_LOGGER("agent"), "Getting storage subsystem info.");
        rsp = CommonComponents::get_instance()->get_storage_subsystem_manager().get_entry(req.get_uuid());
    }
);

REGISTER_COMMAND(GetDriveInfo,
    [] (const GetDriveInfo::Request& req, GetDriveInfo::Response& rsp) {
        log_debug(GET_LOGGER("agent"), "Getting drive info.");
        rsp = CommonComponents::get_instance()->get_drive_manager().get_entry(req.get_uuid());
    }
);

#endif


