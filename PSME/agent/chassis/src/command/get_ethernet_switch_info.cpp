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
#include "agent-framework/command-ref/registry.hpp"
#include "agent-framework/command-ref/network_commands.hpp"

using namespace agent_framework::command_ref;
using namespace agent_framework::model;

namespace {
    void get_switch_info(const GetEthernetSwitchInfo::Request& request,
                         GetEthernetSwitchInfo::Response& response) {
        auto& manager = agent_framework::module::NetworkComponents::
            get_instance()->get_switch_manager();
        std::string uuid = request.get_uuid();
        EthernetSwitch eth_switch{};

        if (manager.entry_exists(uuid)) {
            eth_switch = manager.get_entry(uuid);
        }
        else {
            THROW(::agent_framework::exceptions::InvalidUuid, "network-agent",
                    "Ethernet Switch not found, uuid = " + uuid);
        }
        response = eth_switch;
    }
}

REGISTER_COMMAND(GetEthernetSwitchInfo, ::get_switch_info);
