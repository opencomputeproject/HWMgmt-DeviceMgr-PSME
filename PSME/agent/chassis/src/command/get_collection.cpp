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
 * @file command/get_collection.cpp
 * @brief GetCollection acc-chassis implementation
 * */

#include "agent-framework/module/common_components.hpp"
#include "agent-framework/module/chassis_components.hpp"
#include "agent-framework/module/network_components.hpp"
#include "agent-framework/module/compute_components.hpp"
#include "agent-framework/module/model/model_compute.hpp"

#include "agent-framework/command-ref/registry.hpp"
#include "agent-framework/command-ref/chassis_commands.hpp"
#include "agent-framework/command-ref/compute_commands.hpp"

#include "base64/base64.hpp"

using namespace agent_framework::command_ref;
using namespace agent_framework::module;
using namespace agent_framework::model;
using namespace agent_framework::model::attribute;

using agent_framework::module::ChassisComponents;
using agent_framework::module::ComputeComponents;

namespace
{

using KeysVec = std::vector<std::string>;
using Collections = attribute::Array<attribute::Collection>;

void response_add_subcomponents(const KeysVec &keys,
                                GetCollection::Response &response)
{
    for (const auto &key : keys)
    {
        SubcomponentEntry s;
        s.set_subcomponent(key);
        response.add_entry(s);
    }
}

void process_switch(const Collection &collection, const std::string &uuid,
                    GetCollection::Response &response,
                    const std::string &name)
{
    // Three possible collection types of a switch
    if (enums::CollectionType::EthernetSwitchPorts == collection.get_type())
    {
        response_add_subcomponents(NetworkComponents::get_instance()->get_port_manager().get_keys(uuid), response);
    }
    else if (enums::CollectionType::Vlans == collection.get_type())
    {
        response_add_subcomponents(NetworkComponents::get_instance()->get_vlan_manager().get_keys(uuid), response);
    }
    else if (enums::CollectionType::NeighborSwitches ==
             collection.get_type())
    {
        response_add_subcomponents(NetworkComponents::get_instance()->get_remote_switch_manager().get_keys(uuid), response);
    }
    else if (enums::CollectionType::Acls == collection.get_type())
    {
        response_add_subcomponents(NetworkComponents::get_instance()->get_acl_manager().get_keys(uuid), response);
    }
    else
    {
        THROW(::agent_framework::exceptions::InvalidCollection, "network-agent",
              "No collection named \'" + name + "\' for Ethernet Switch " + uuid + "!");
    }
}

void process_port(const Collection &collection, const std::string &uuid,
                  GetCollection::Response &response,
                  const std::string &name)
{
    // Two possible collection types of a port
    if (enums::CollectionType::EthernetSwitchPortVlans == collection.get_type())
    {
        response_add_subcomponents(NetworkComponents::get_instance()->get_port_vlan_manager().get_keys(uuid), response);
    }
    else if (enums::CollectionType::PortMembers == collection.get_type())
    {
        response_add_subcomponents(NetworkComponents::get_instance()->get_port_members_manager().get_children(uuid), response);
    }
    else if (enums::CollectionType::Acls == collection.get_type())
    {
        response_add_subcomponents(NetworkComponents::get_instance()->get_acl_port_manager().get_parents(uuid), response);
    }
    else if (enums::CollectionType::StaticMacs == collection.get_type())
    {
        response_add_subcomponents(NetworkComponents::get_instance()->get_static_mac_manager().get_keys(uuid), response);
    }
    else
    {
        THROW(::agent_framework::exceptions::InvalidCollection, "network-agent",
              "No collection named \'" + name + "\' for Ethernet Switch Port " + uuid + "!");
    }
}

void process_manager(const Collection &collection, const std::string &uuid,
                     GetCollection::Response &response, const std::string &name)
{
    if (enums::CollectionType::Chassis == collection.get_type())
    {
        response_add_subcomponents(CommonComponents::get_instance()->get_chassis_manager().get_keys(uuid), response);
    }
    else if (enums::CollectionType::Managers == collection.get_type())
    {
        response_add_subcomponents(CommonComponents::get_instance()->get_module_manager().get_keys(uuid), response);
    }
    else if (enums::CollectionType::Fans == collection.get_type())
    {
        std::cerr << "==Fans CollectionType process_manager== " << std::endl;
        response_add_subcomponents(ChassisComponents::get_instance()->get_fan_manager().get_keys(uuid), response);
    }
    else if (enums::CollectionType::ThermalZones == collection.get_type())
    {
        std::cerr << "==ThermalZones CollectionType process_manager== " << std::endl;
        response_add_subcomponents(ChassisComponents::get_instance()->get_thermal_zone_manager().get_keys(uuid), response);
    }
    else if (enums::CollectionType::PSUs == collection.get_type())
    {
        std::cerr << "==Psus CollectionType process_manager== " << std::endl;
        response_add_subcomponents(ChassisComponents::get_instance()->get_psu_manager().get_keys(uuid), response);
    }
    else if (enums::CollectionType::Systems == collection.get_type())
    {
        std::cerr << "==Computer System CollectionType process_manager== " << std::endl;
        response_add_subcomponents(CommonComponents::get_instance()->get_system_manager().get_keys(uuid), response);
    }
    else if (enums::CollectionType::EthernetSwitches == collection.get_type())
    {
        std::cerr << "==EthernetSwitches CollectionType process_manager== " << std::endl;
        response_add_subcomponents(NetworkComponents::get_instance()->get_switch_manager().get_keys(uuid), response);
    }
    else
    {
        THROW(agent_framework::exceptions::InvalidCollection, "chassis-agent",
              "Invalid collection name: \'" + name + "\'");
    }
}

const Collection find_collection(const Collections &collections,
                                 const std::string &name)
{
    for (const auto &collection : collections)
    {
        if (name == collection.get_name())
        {
            return collection;
        }
    }
    Collection c;
    c.set_type(enums::CollectionType::None);
    return c;
}

#ifdef CSUBSYSTEM
void process_chassis(const Collection &collection, const std::string &uuid,
                     GetCollection::Response &response, const std::string &name)
{

    // One possible collection type for a chassis
    if (enums::CollectionType::Drives == collection.get_type())
    {

        std::cerr << "System Components acc  process_chassis add drive====" << std::endl;
        response_add_subcomponents(CommonComponents::get_instance()->get_drive_manager().get_keys(uuid), response);
    }
    else
    {
        THROW(agent_framework::exceptions::InvalidCollection,
              "compute-agent", "Collection not found: \'" + name + "\'.");
    }
}

void process_storage_subsystem(const Collection &collection, const std::string &uuid,
                               GetCollection::Response &response, const std::string &name)
{
    // Two possible collection type of a storage subsystem
    if (enums::CollectionType::Drives == collection.get_type())
    {
        // StorageSubsystem has the same drives as Chassis
        // So, find the StorageSubsystem's parent System and grandparent Manager.
        // Then, find the chassis under the manager.
        // and find it's Drives.
        std::cerr << "==process_system process_storage_subsystem Drives== " << std::endl;
        auto system_uuid = CommonComponents::get_instance()->get_storage_subsystem_manager().get_entry_reference(uuid)->get_parent_uuid();
        auto manager_uuid = CommonComponents::get_instance()->get_system_manager().get_entry_reference(system_uuid)->get_parent_uuid();
        auto chassis_uuid = CommonComponents::get_instance()->get_chassis_manager().get_keys(manager_uuid).front();
        response_add_subcomponents(CommonComponents::get_instance()->get_drive_manager().get_keys(chassis_uuid), response);
    }
    else if (enums::CollectionType::StorageControllers == collection.get_type())
    {
        std::cerr << "==process_system process_storage_subsystem StorageControllers== " << std::endl;
        response_add_subcomponents(ComputeComponents::get_instance()->get_storage_controller_manager().get_keys(uuid), response);
    }
    else
    {
        THROW(::agent_framework::exceptions::InvalidCollection,
              "compute-agent", "Collection not found: \'" + name + "\'.");
    }
}

void process_system(const Collection &collection, const std::string &uuid,
                    GetCollection::Response &response, const std::string &name)
{

    // Six possible collection types of a system
    if (enums::CollectionType::Processors == collection.get_type())
    {
        std::cerr << "==process_system CollectionType Processors == " << std::endl;
        response_add_subcomponents(ComputeComponents::get_instance()->get_processor_manager().get_keys(uuid), response);
    }
    else if (enums::CollectionType::Memory == collection.get_type())
    {
        std::cerr << "==process_system CollectionType Memory == " << std::endl;
        response_add_subcomponents(ComputeComponents::get_instance()->get_memory_manager().get_keys(uuid), response);
    }
    else if (enums::CollectionType::StorageSubsystems == collection.get_type())
    {
        std::cerr << "==process_system CollectionType Storage == " << std::endl;
        response_add_subcomponents(CommonComponents::get_instance()->get_storage_subsystem_manager().get_keys(uuid), response);
    }
    else if (enums::CollectionType::NetworkInterfaces == collection.get_type())
    {
        response_add_subcomponents(ComputeComponents::get_instance()->get_network_interface_manager().get_keys(uuid), response);
    }
    else if (enums::CollectionType::NetworkDevices == collection.get_type())
    {
        response_add_subcomponents(ComputeComponents::get_instance()->get_network_device_manager().get_keys(uuid), response);
    }
    else
    {
        THROW(agent_framework::exceptions::InvalidCollection,
              "compute-agent", "Collection not found: \'" + name + "\'.");
    }
}
#endif

void get_collection(const GetCollection::Request &request, GetCollection::Response &response)
{
    const auto &uuid = request.get_uuid();
    const auto &name = request.get_name();

    if (CommonComponents::get_instance()->get_module_manager().entry_exists(uuid))
    {
        const Manager manager = CommonComponents::get_instance()->get_module_manager().get_entry(uuid);
        const Collection collection = find_collection(manager.get_collections(), name);
        process_manager(collection, uuid, response, name);
    }
    else if (NetworkComponents::get_instance()->get_port_manager().entry_exists(uuid))
    {

        const EthernetSwitchPort &port = NetworkComponents::get_instance()->get_port_manager().get_entry(uuid);
        const Collection &collection = find_collection(port.get_collections(), name);
        process_port(collection, uuid, response, name);
    }

    else if (NetworkComponents::get_instance()->get_switch_manager().entry_exists(uuid))
    {
        const EthernetSwitch &sw = NetworkComponents::get_instance()->get_switch_manager().get_entry(uuid);
        const Collection &collection = find_collection(sw.get_collections(), name);
        process_switch(collection, uuid, response, name);
    }
#ifdef CSUBSYSTEM
    else if (CommonComponents::get_instance()->get_system_manager().entry_exists(uuid))
    {
        const System &sys = CommonComponents::get_instance()->get_system_manager().get_entry(uuid);
        const Collection &collection = find_collection(sys.get_collections(), name);
        process_system(collection, uuid, response, name);
    }
    else if (CommonComponents::get_instance()->get_storage_subsystem_manager().entry_exists(uuid))
    {
        const StorageSubsystem &subsystem = CommonComponents::get_instance()->get_storage_subsystem_manager().get_entry(uuid);
        const Collection &collection = find_collection(subsystem.get_collections(), name);
        process_storage_subsystem(collection, uuid, response, name);
    }
    else if (CommonComponents::get_instance()->get_chassis_manager().entry_exists(uuid))
    {
        const Chassis &chassis = CommonComponents::get_instance()->get_chassis_manager().get_entry(uuid);
        const Collection &collection = find_collection(chassis.get_collections(), name);
        process_chassis(collection, uuid, response, name);
    }
#endif
    else
    {
        THROW(::agent_framework::exceptions::InvalidUuid, "chassis-agent",
              "Component not found, invalid UUID: " + uuid);
    }
}
} // namespace

REGISTER_COMMAND(GetCollection, ::get_collection);