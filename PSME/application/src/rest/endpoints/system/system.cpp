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

#include "agent-framework/module/requests/common.hpp"
#include "agent-framework/module/responses/common.hpp"
#include "agent-framework/module/constants/compute.hpp"

#include "ecrf_pal_helper/api/ecrf_pal_com.hpp"
#include "ecrf_pal_helper/api/ecrf_pal_helper.hpp"
#include "ecrf_pal_helper/api/ecrf_pal_helper_api.hpp"
#include "psme/rest/utils/status_helpers.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/system.hpp"
#include "psme/rest/endpoints/system/system.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"
#include "psme/rest/utils/ec_common_utils.hpp"
#include "agent-framework/service_uuid.hpp"
#include "ecsys_helper/ecsys_helper.hpp"
/* Do not remove numeric header - needed by ARM compilation */
#include <numeric>
#include <sys/sysinfo.h>

using namespace ecrf_pal_helper;
using namespace ecrf_pal_com_s;
using namespace ecrf_pal_com_api;
using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::endpoint::utils;
using namespace psme::rest::validators;
using namespace ecsys_helper;
using agent_framework::generic::ServiceUuid;
using BootOverrideTarget = agent_framework::model::enums::BootOverrideTarget;
using BootOverrideSupported = agent_framework::model::System::BootOverrideSupported;

auto &SD = ADbg::get_instance();
namespace
{

json::Value make_prototype()
{
    json::Value response(json::Value::Type::OBJECT);

    response[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#ComputerSystem.ComputerSystem";
    response[Common::ODATA_ID] = json::Value::Type::NIL;
    response[Common::ODATA_TYPE] = "#ComputerSystem.v1_3_0.ComputerSystem";
    response[Common::ID] = json::Value::Type::NIL;
    response[Common::NAME] = "Computer System";
    response[Common::DESCRIPTION] = "Computer System description";
    response[System::SYSTEM_TYPE] = json::Value::Type::NIL;
    response[Common::ASSET_TAG] = json::Value::Type::NIL;
    response[Common::MANUFACTURER] = json::Value::Type::NIL;
    response[Common::MODEL] = json::Value::Type::NIL;
    response[System::SKU] = json::Value::Type::NIL;
    response[Common::SERIAL] = json::Value::Type::NIL;
    response[Common::PART_NUMBER] = json::Value::Type::NIL;
    response[Common::UUID] = json::Value::Type::NIL;
    response[System::HOST_NAME] = json::Value::Type::NIL;
    response[Common::STATUS][Common::STATE] = json::Value::Type::NIL;
    response[Common::STATUS][Common::HEALTH] = json::Value::Type::NIL;
    response[Common::STATUS][Common::HEALTH_ROLLUP] = json::Value::Type::NIL;
    response[System::INDICATOR_LED] = json::Value::Type::NIL;
    response[System::POWER_STATE] = json::Value::Type::NIL;
    response[System::BIOS_VERSION] = json::Value::Type::NIL;

    json::Value boot;
    boot[System::BOOT_SOURCE_OVERRIDE_ENABLED] = json::Value::Type::NIL;
    boot[System::BOOT_SOURCE_OVERRIDE_TARGET] = json::Value::Type::NIL;

    response[System::BOOT] = std::move(boot);

    json::Value processors;
    processors[System::COUNT] = json::Value::Type::NIL;
    processors[Common::MODEL] = json::Value::Type::NIL;
    processors[Common::STATUS][Common::STATE] = json::Value::Type::NIL;
    processors[Common::STATUS][Common::HEALTH] = json::Value::Type::NIL;
    processors[Common::STATUS][Common::HEALTH_ROLLUP] = json::Value::Type::NIL;
    response[System::PROCESSOR_SUMMARY] = std::move(processors);

    json::Value memory;
    memory[System::TOTAL_SYSTEM_MEMORY_GIB] = json::Value::Type::NIL;
    memory[Common::STATUS][Common::STATE] = json::Value::Type::NIL;
    memory[Common::STATUS][Common::HEALTH] = json::Value::Type::NIL;
    memory[Common::STATUS][Common::HEALTH_ROLLUP] = json::Value::Type::NIL;
    response[System::MEMORY_SUMMARY] = std::move(memory);

    response[System::PROCESSORS] = json::Value::Type::NIL;
#ifndef COCP		
    response[System::ETHERNET_INTERFACES] = json::Value::Type::NIL;
#endif 

#ifdef CSYSTEM_NETWORK		
    response[System::NETWORK_INTERFACES] = json::Value::Type::NIL;
#endif
    response[System::STORAGE] = json::Value::Type::NIL;
    response[System::MEMORY] = json::Value::Type::NIL;
    response[Chassis::PCIE_DEVICES] = json::Value::Type::ARRAY;
    response[PcieDevice::PCIE_FUNCTIONS] = json::Value::Type::ARRAY;

    response[Common::LINKS][Common::ODATA_TYPE] = "#ComputerSystem.v1_2_0.Links";
    response[Common::LINKS][Common::CHASSIS] = json::Value::Type::ARRAY;
    response[Common::LINKS][System::ENDPOINTS] = json::Value::Type::ARRAY;
    response[Common::LINKS][Common::MANAGED_BY] = json::Value::Type::ARRAY;
    response[Common::LINKS][Common::OEM] = json::Value::Type::OBJECT;

    json::Value allowable;
//  allowable.push_back(System::ON);
    //allowable.push_back(System::FORCE_OFF);
    allowable.push_back(System::GRACEFUL_SHUTDOWN);
    allowable.push_back(System::GRACEFUL_RESTART);
    //allowable.push_back(System::FORCE_RESTART);

    json::Value actions;
    std::string hash_computer_system_reset(constants::Common::HASH);
    hash_computer_system_reset.append(constants::System::COMPUTER_SYSTEM_RESET);
    actions[hash_computer_system_reset][System::TARGET] = json::Value::Type::NIL;
    actions[hash_computer_system_reset][System::ALLOWABLE_RESET_TYPES] = std::move(allowable);
    response[Common::ACTIONS] = std::move(actions);

    json::Value rs;

    rs[Common::ODATA_TYPE] = "#Intel.Oem.ComputerSystem";
    rs[System::PCI_DEVICES] = json::Value::Type::ARRAY;
    rs[System::PCIE_CONNECTION_ID] = json::Value::Type::ARRAY;
    rs[System::PROCESSOR_SOCKETS] = json::Value::Type::NIL;
    rs[System::MEMORY_SOCKETS] = json::Value::Type::NIL;
    rs[System::DISCOVERY_STATE] = json::Value::Type::NIL;
    response[Common::OEM][Common::RACKSCALE] = std::move(rs);

    return response;
}

void add_processors_summary(const agent_framework::model::System &system, json::Value &response)
{
    const agent_framework::model::enums::State ENABLED = agent_framework::model::enums::State::Enabled;

    auto procs = agent_framework::module::get_manager<agent_framework::model::Processor>()
        .get_keys(system.get_uuid());

    response[constants::System::PROCESSOR_SUMMARY][Common::MODEL] = json::Value::Type::NIL;
    auto processors_count = static_cast<std::uint32_t>(procs.size());
    if (processors_count > 0)
    {
        const auto& proc = agent_framework::module::get_manager<agent_framework::model::Processor>()
            .get_entry(procs.front());
        response[constants::System::PROCESSOR_SUMMARY][Common::MODEL] = proc.get_model_name();
    }

    response[constants::System::PROCESSOR_SUMMARY][Common::STATUS][Common::STATE] = ENABLED.to_string();
    auto summarized_proc_health = psme::rest::endpoint::HealthRollup<agent_framework::model::System>()
        .get(system.get_uuid(), agent_framework::model::enums::Component::Processor);

    response[Common::OEM][Common::RACKSCALE][System::PROCESSOR_SOCKETS] = processors_count;
    response[constants::System::PROCESSOR_SUMMARY][Common::STATUS][Common::HEALTH] = summarized_proc_health;
    response[constants::System::PROCESSOR_SUMMARY][Common::STATUS][Common::HEALTH_ROLLUP] = summarized_proc_health;

    auto is_processor_present = [](const agent_framework::model::Processor& p) {
        return (p.get_status().get_state() == agent_framework::model::enums::State::Absent) ? 0 : 1;
    };
    auto available_processors = psme::rest::endpoint::utils::query_entries<agent_framework::model::Processor, std::uint32_t>(system.get_uuid(), is_processor_present);
    response[constants::System::PROCESSOR_SUMMARY][constants::System::COUNT] =
        std::accumulate(available_processors.begin(), available_processors.end(), 0);
}

void add_memory_summary(const agent_framework::model::System &system, json::Value &response)
{
    const agent_framework::model::enums::State ENABLED = agent_framework::model::enums::State::Enabled;

    auto memory_modules = agent_framework::module::get_manager<agent_framework::model::Memory>()
        .get_keys(system.get_uuid());

    OptionalField<std::uint32_t> total_size_mb{};
    auto get_memory_size = [](const agent_framework::model::Memory& m) { return m.get_capacity_mb(); };
    auto memory_sizes = psme::rest::endpoint::utils::query_entries<agent_framework::model::Memory, OptionalField<std::uint32_t>>(system.get_uuid(), get_memory_size);

    for (const auto &mem_size : memory_sizes)
    {
        total_size_mb = total_size_mb + mem_size;
    }

    response[constants::System::MEMORY_SUMMARY][constants::System::TOTAL_SYSTEM_MEMORY_GIB] =
        (total_size_mb.has_value() ? json::Value(endpoint::utils::round(endpoint::utils::mb_to_gib(total_size_mb), 2)) : json::Value(json::Value::Type::NIL));

    response[constants::System::MEMORY_SUMMARY][Common::STATUS][Common::STATE] = ENABLED.to_string();
    auto summarized_memory_health = psme::rest::endpoint::HealthRollup<agent_framework::model::System>()
        .get(system.get_uuid(), agent_framework::model::enums::Component::Memory);

    response[constants::System::MEMORY_SUMMARY][Common::STATUS][Common::HEALTH] = summarized_memory_health;
    response[constants::System::MEMORY_SUMMARY][Common::STATUS][Common::HEALTH_ROLLUP] = summarized_memory_health;

    response[Common::OEM][Common::RACKSCALE][System::MEMORY_SOCKETS] = memory_modules.size();
}

void add_system_relations(const agent_framework::model::System &system, json::Value &response)
{
    // PCI DEVICES
    for (const auto &pci : system.get_pci_devices())
    {
        json::Value pci_device;
        pci_device[Common::VENDOR_ID] = pci.get_vendor_id();
        pci_device[Common::DEVICE_ID] = pci.get_device_id();
        response[Common::OEM][Common::RACKSCALE][System::PCI_DEVICES].push_back(std::move(pci_device));
    }

    // CABLE IDS/PCIE CONNECTIONS
    for (const auto &cable_id : system.get_cable_ids())
    {
        response[Common::OEM][Common::RACKSCALE][System::PCIE_CONNECTION_ID].push_back(cable_id);
    }

    // PCIe Devices
    // the manager for this system is also the manager for all the pcie devices
    const auto& manager_uuid = system.get_parent_uuid();
    for (const auto& device_uuid : agent_framework::module::get_manager<agent_framework::model::PcieDevice>().get_keys(
             manager_uuid))
    {
        json::Value device_link;
        device_link[Common::ODATA_ID] = endpoint::utils::get_component_url(
            agent_framework::model::enums::Component::PcieDevice, device_uuid);
        response[Chassis::PCIE_DEVICES].push_back(std::move(device_link));
    }

    const auto fabric_uuids = agent_framework::module::get_manager<agent_framework::model::Fabric>().get_keys(
        manager_uuid);
}

void make_children_links(const server::Request &req, json::Value &response)
{
    using psme::rest::endpoint::PathBuilder;
    response[constants::System::PROCESSORS][Common::ODATA_ID] =
        PathBuilder(req).append(constants::System::PROCESSORS).build();

#ifndef COCP		
    response[constants::System::ETHERNET_INTERFACES][Common::ODATA_ID] =
        PathBuilder(req).append(constants::System::ETHERNET_INTERFACES).build();
#endif

	response[constants::System::STORAGE][Common::ODATA_ID] =
        PathBuilder(req).append(constants::System::STORAGE).build();
    response[constants::System::MEMORY][Common::ODATA_ID] =
        PathBuilder(req).append(constants::System::MEMORY).build();

#ifdef CSYSTEM_NETWORK	
	response[constants::System::NETWORK_INTERFACES][Common::ODATA_ID] =
        PathBuilder(req).append(constants::System::NETWORK_INTERFACES).build();
#endif
}

static const std::map<std::string, std::string> gami_to_rest_attributes = {
    {agent_framework::model::literals::System::BOOT_OVERRIDE,
        endpoint::PathBuilder(constants::System::BOOT)
         .append(constants::System::BOOT_SOURCE_OVERRIDE_ENABLED)
         .build()},
    {agent_framework::model::literals::System::BOOT_OVERRIDE_MODE,
        endpoint::PathBuilder(constants::System::BOOT)
         .append(constants::System::BOOT_SOURCE_OVERRIDE_ENABLED)
         .build()},
    {agent_framework::model::literals::System::BOOT_OVERRIDE_TARGET,
        endpoint::PathBuilder(constants::System::BOOT)
         .append(constants::System::BOOT_SOURCE_OVERRIDE_TARGET)
         .build()},
    {agent_framework::model::literals::System::ASSET_TAG,
     constants::Common::ASSET_TAG}};

void make_parent_links(const agent_framework::model::System &system, json::Value &response)
{
    // find manager of this system
    try
    {
        json::Value managed_by;
        managed_by[Common::ODATA_ID] = psme::rest::endpoint::utils::get_component_url(
            agent_framework::model::enums::Component::Manager, system.get_parent_uuid());
        response[Common::LINKS][Common::MANAGED_BY].push_back(managed_by);
    }
    catch (const agent_framework::exceptions::InvalidUuid &)
    {
    }

    // find chassis containing this system
    try
    {
        json::Value chassis;
        chassis[Common::ODATA_ID] =
            psme::rest::endpoint::utils::get_component_url(agent_framework::model::enums::Component::Chassis,
                                                           system.get_chassis());
        response[Common::LINKS][constants::Common::CHASSIS].push_back(chassis);
    }
    catch (const agent_framework::exceptions::InvalidUuid &)
    {
}
}
} // namespace

endpoint::System::System(const std::string& path) : EndpointBase(path) {}

endpoint::System::~System() {}

void endpoint::System::get(const server::Request &req, server::Response &res)
{
    auto &secrf_pal = ecrf_pal_helper::Switch::get_instance();
    auto response = make_prototype();
    response[Common::ODATA_ID] = PathBuilder(req).build();

    auto system = psme::rest::model::Find<agent_framework::model::System>(req.params[PathParam::SYSTEM_ID]).get();
    make_parent_links(system, response);

    response[constants::Common::ODATA_ID] = PathBuilder(req).build();
    response[constants::Common::ID] = req.params[PathParam::SYSTEM_ID];
    response[constants::System::SYSTEM_TYPE] = system.get_system_type();
    response[Common::MANUFACTURER] = secrf_pal.get_manufacturer().c_str(); 
    response[Common::MODEL] = secrf_pal.get_platform_name().c_str();       
    response[Common::SERIAL] = secrf_pal.get_serial_number().c_str();      
    response[Common::PART_NUMBER] = secrf_pal.get_part_number().c_str();   
	response[constants::Common::UUID] = ServiceUuid::get_instance()->get_service_uuid();
    endpoint::status_to_json(system, response);
    response[Common::STATUS][Common::HEALTH_ROLLUP] = endpoint::HealthRollup<agent_framework::model::System>().get(system.get_uuid());
    response[constants::System::POWER_STATE] = system.get_power_state().to_string();

    char command[HT_BUFFER_LEN] = {0};
    char resultA[HT_BUFFER_LEN] = {0};
		sprintf(command, "%s" ,"dmidecode | grep 'BIOS Revision' | awk '{print $3}'");
		memset(resultA,0x0, sizeof(resultA));
    HelperTools::exec_shell(command, resultA, 0);

		if(strlen(resultA) != 0)
		{
			resultA[strcspn(resultA, "\r\n")]=0;
			response[constants::System::BIOS_VERSION] = resultA;
		}

    response[constants::System::SKU] = json::Value::Type::NIL; // system.get_sku();
    response[constants::Common::ASSET_TAG] = system.get_asset_tag();
    response[constants::System::INDICATOR_LED] = system.get_indicator_led();

    response[constants::System::BOOT][constants::System::BOOT_SOURCE_OVERRIDE_TARGET] =        system.get_boot_override_target().to_string();

    response[constants::System::BOOT][constants::System::BOOT_SOURCE_OVERRIDE_ENABLED] =        system.get_boot_override().to_string();
/*
    for (const auto& allowable : system.get_boot_override_supported()) {
        response[constants::System::BOOT][constants::System::BOOT_SOURCE_OVERRIDE_TARGET_ALLOWABLE_VALUES].
            push_back(allowable.to_string());
    }

    response[constants::System::BOOT][constants::System::BOOT_SOURCE_OVERRIDE_MODE] =        system.get_boot_override_mode().to_string();
*/
    std::string hash_computer_system_reset(constants::Common::HASH);
    hash_computer_system_reset.append(constants::System::COMPUTER_SYSTEM_RESET);
    response[constants::Common::ACTIONS][hash_computer_system_reset][constants::System::TARGET] =
        PathBuilder(req)
            .append(constants::Common::ACTIONS)
            .append(constants::System::COMPUTER_SYSTEM_RESET)
            .build();

    // do NOT remove, needed for PODM integration purposes
    response[constants::Common::OEM][constants::Common::RACKSCALE]
    [constants::System::DISCOVERY_STATE] = constants::System::BASIC;

    add_processors_summary(system, response);
    add_memory_summary(system, response);
    add_system_relations(system, response);
    make_children_links(req, response);

    /*For get Hostname/FQDN*/
    memset(command,0x0, sizeof(command));	
    memset(resultA,0x0, sizeof(resultA));	
    sprintf(command, "%s" ,"hostname");
    memset(resultA,0x0, sizeof(resultA));
    HelperTools::exec_shell(command, resultA, 0);
    
    if(strlen(resultA) != 0)
    {
    	resultA[strcspn(resultA, "\r\n")]=0;
    	response[constants::System::HOST_NAME] = resultA;
    }

    set_response(res, response);
}

void endpoint::System::patch(const server::Request &request, server::Response &response)
{
    std::string agent_id{};
    std::string system_uuid{};
    std::string parent_uuid{};
    BootOverrideSupported override_supported{};

    auto system = model::Find<agent_framework::model::System>(request.params[PathParam::SYSTEM_ID]).get();

    agent_id = system.get_agent_id();
    system_uuid = system.get_uuid();
    parent_uuid = system.get_parent_uuid();
    override_supported = system.get_boot_override_supported();

    const auto& json = JsonValidator::validate_request_body<schema::SystemPatchSchema>(request);
    agent_framework::model::attribute::Attributes attributes{};

    if (json.is_member(constants::Common::ASSET_TAG))
    {
        const auto& asset_tag = json[constants::Common::ASSET_TAG].as_string();
        attributes.set_value(agent_framework::model::literals::System::ASSET_TAG, asset_tag);
        char command[HT_BUFFER_LEN] = {0};
        char resultA[HT_BUFFER_LEN] = {0};
		sprintf(command, "echo %s > /etc/psme/ASSET_TAG", asset_tag.c_str());	
        HelperTools::exec_shell(command, resultA, 0);
		system.set_asset_tag(asset_tag);	
    }
    get(request, response);
}
