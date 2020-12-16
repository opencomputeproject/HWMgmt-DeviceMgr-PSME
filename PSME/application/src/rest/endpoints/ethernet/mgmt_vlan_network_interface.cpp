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

#include "psme/core/agent/agent_manager.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/ethernet/mgmt_vlan_network_interface.hpp"
#include "psme/rest/model/handlers/handler_manager.hpp"
#include "psme/rest/model/handlers/generic_handler_deps.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"
#include "psme/rest/utils/status_helpers.hpp"
#include "psme/rest/utils/ec_common_utils.hpp"
#include "configuration/configuration.hpp"
#include "agent-framework/module/requests/network.hpp"
#include "agent-framework/module/responses/network.hpp"
#include "agent-framework/module/network_components.hpp"
#include "ecsys_helper/ecsys_helper.hpp"
#include "psme/rest/utils/ec_common_utils.hpp"

#include <json/json.hpp>

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace agent_framework::model::attribute;
using namespace ecsys_helper;
using namespace psme::rest::utils;

namespace
{
json::Value make_prototype()
{
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#VLanNetworkInterface.VLanNetworkInterface";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#VLanNetworkInterface.v1_0_1.VLanNetworkInterface";
    r[Common::ID] = json::Value::Type::NIL;
    r[Common::NAME] = "VLAN Network Interface";
    r[Common::DESCRIPTION] = "VLAN Network Interface description";
    r[Vlan::VLAN_ENABLE] = json::Value::Type::NIL;
    r[Vlan::VLAN_ID] = json::Value::Type::NIL;

    return r;
}
} // namespace

endpoint::Mgmt_VlanNetworkInterface::Mgmt_VlanNetworkInterface(const std::string& path) : EndpointBase(path) {}

endpoint::Mgmt_VlanNetworkInterface::~Mgmt_VlanNetworkInterface() {}

void endpoint::Mgmt_VlanNetworkInterface::get(const server::Request &req, server::Response &res)
{
    char resultA[BUFFER_LEN] = {0};
    char command[BUFFER_LEN] = {0};
    const json::Value config = configuration::Configuration::get_instance().to_json();
    const auto& nic_name = config["server"]["network-interface-name"].as_string();

    auto r = make_prototype();

    r[Common::ODATA_ID] = PathBuilder(req).build();
    r[Common::ID] = req.params[PathParam::VLAN_ID];
    r[Common::NAME] = Vlan::VLAN + req.params[PathParam::VLAN_ID];

    unsigned int vlanid= atoi( req.params[PathParam::VLAN_ID].c_str());									      
    r[Vlan::VLAN_ID] =  req.params[PathParam::VLAN_ID].c_str();
    
    sprintf(command, "cat /sys/class/net/%s.%d/operstate", nic_name.c_str(), vlanid );	
    EcCommonUtils::exec_shell(command, resultA, 0);
    
    if(!strncmp("up", resultA, 2))
        r[Vlan::VLAN_ENABLE] = true;										
    else
        r[Vlan::VLAN_ENABLE] = false;

    set_response(res, r);
}

void endpoint::Mgmt_VlanNetworkInterface::del(const server::Request &req, server::Response &res)
{

    int VLAN_ID = std::stoi(req.params[PathParam::VLAN_ID]);
    char resultA[BUFFER_LEN] = {0};
    char command[BUFFER_LEN] = {0};

    if(VLAN_ID ==1 )
    {
        res.set_status(server::status_5XX::SERVICE_UNAVAILABLE);
        return;
    }

    sprintf(command, "vlan.sh set vlan_value_destroy  %d" , VLAN_ID);
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);

    res.set_status(server::status_2XX::NO_CONTENT);
}
