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

#include "agent-framework/command-ref/registry.hpp"
#include "agent-framework/command-ref/network_commands.hpp"
#include "agent-framework/module/network_components.hpp"

using namespace agent_framework::model;
using namespace agent_framework::command_ref;

namespace {
    void get_port_vlan_info(const GetPortVlanInfo::Request& request,
                            GetPortVlanInfo::Response& response) {
        auto& manager = agent_framework::module::NetworkComponents::
            get_instance()->get_port_vlan_manager();
        std::string uuid = request.get_uuid();

        if (manager.entry_exists(uuid)) {
            response = manager.get_entry(uuid);
        }
        else {
            THROW(::agent_framework::exceptions::InvalidUuid, "network-agent",
                "Port VLAN not found, UUID = '" + uuid + "'.");
        }
    }
}

REGISTER_COMMAND(GetPortVlanInfo, ::get_port_vlan_info);
