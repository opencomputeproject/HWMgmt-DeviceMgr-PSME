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

#include "psme/core/agent/agent_manager.hpp"
#include "psme/rest/endpoints/ethernet/static_mac.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/static_mac.hpp"
#include "psme/rest/model/handlers/handler_manager.hpp"
#include "psme/rest/model/handlers/generic_handler_deps.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"
#include "ecsys_helper/ecsys_helper.hpp"

#include "agent-framework/module/requests/common.hpp"
#include "agent-framework/module/responses/common.hpp"
#include "agent-framework/module/requests/network.hpp"
#include "agent-framework/module/responses/network.hpp"

#include "psme/rest/utils/ec_common_utils.hpp"
using namespace psme::rest::utils;
using namespace psme::rest;
using namespace psme::rest::validators;
using namespace psme::rest::endpoint;
using namespace psme::rest::model::handler;
using namespace psme::core::agent;
using namespace psme::rest::constants;
using namespace agent_framework::module;
using namespace agent_framework::model;
using NetworkComponents = agent_framework::module::NetworkComponents;
using namespace ecsys_helper;

namespace
{
json::Value make_prototype()
{
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#EthernetSwitchStaticMAC.EthernetSwitchStaticMAC";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#EthernetSwitchStaticMAC.v1_0_0.EthernetSwitchStaticMAC";
    r[Common::ID] = json::Value::Type::NIL;
    r[Common::NAME] = "Static MAC";
    r[Common::DESCRIPTION] = "Static MAC address rule";

    r[constants::Common::MAC_ADDRESS] = json::Value::Type::NIL;
    r[constants::StaticMac::VLAN_ID] = json::Value::Type::NIL;
    r[Common::OEM] = json::Value::Type::OBJECT;

    return r;
}

static const std::map<std::string, std::string> gami_to_rest_attributes = {
    {agent_framework::model::literals::StaticMac::ADDRESS,      constants::Common::MAC_ADDRESS},
    {agent_framework::model::literals::StaticMac::VLAN_ID, constants::StaticMac::VLAN_ID},
};

} // namespace

endpoint::StaticMac::StaticMac(const std::string& path) : EndpointBase(path) {}
endpoint::StaticMac::~StaticMac() {}

void endpoint::StaticMac::get(const server::Request &req, server::Response &res)
{
    auto json = ::make_prototype();

    json[Common::ODATA_CONTEXT] = std::regex_replace(
            json[Common::ODATA_CONTEXT].as_string(),
            std::regex("__SWITCH_ID__"),
            req.params[PathParam::ETHERNET_SWITCH_ID]);

    json[Common::ODATA_CONTEXT] = std::regex_replace(
            json[Common::ODATA_CONTEXT].as_string(),
            std::regex("__PORT_ID__"),
            req.params[PathParam::SWITCH_PORT_ID]);

    json[Common::ODATA_ID] = PathBuilder(req).build();
    json[Common::ID] = req.params[PathParam::STATIC_MAC_ID];
    json[Common::NAME] = constants::StaticMac::STATIC_MAC + req.params[PathParam::STATIC_MAC_ID];

    int index = std::stoi(req.params[PathParam::STATIC_MAC_ID]);
    int iPort = std::stoi(req.params[PathParam::SWITCH_PORT_ID]);

    char command[BUFFER_LEN] = {0};
    char resultA[BUFFER_LEN] = {0};

    sprintf(command, "l2.sh get static_mac_count %d" , iPort);
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);

    if(strlen(resultA) != 0)
    {
        sprintf(command, "l2.sh get static_mac %d %d" , iPort, index);
        memset(resultA,0x0, sizeof(resultA));
        EcCommonUtils::exec_shell(command, resultA, 0);
        if(strlen(resultA) != 0)
        {
            resultA[strcspn(resultA, "\r\n")]=0;
            json[constants::Common::MAC_ADDRESS] = resultA;
        }
        sprintf(command, "l2.sh get static_mac_vlan %d %d" , iPort, index);
        memset(resultA,0x0, sizeof(resultA));
        EcCommonUtils::exec_shell(command, resultA, 0);
        if(strlen(resultA) != 0)
        {
            json[constants::StaticMac::VLAN_ID] = atoi(resultA);
        }		
    }
    set_response(res, json);
}

void endpoint::StaticMac::del(const server::Request &req, server::Response &res)
{
    int index = std::stoi(req.params[PathParam::STATIC_MAC_ID]);
    int iPort = std::stoi(req.params[PathParam::SWITCH_PORT_ID]);
    char command[BUFFER_LEN] = {0};
    char resultA[BUFFER_LEN] = {0};
    sprintf(command, "l2.sh  set static_mac_del_idx %d  %d" , iPort,  index);
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);
    res.set_status(server::status_2XX::NO_CONTENT);
}

void endpoint::StaticMac::patch(const server::Request &request, server::Response &response)
{
    const auto json = JsonValidator::validate_request_body<schema::StaticMacPatchSchema>(request);
    auto mac = json[constants::Common::MAC_ADDRESS].as_string();
    int iPort = std::stoi(request.params[PathParam::SWITCH_PORT_ID]);
    int index = std::stoi(request.params[PathParam::STATIC_MAC_ID]);
    auto vlan_id = json[constants::StaticMac::VLAN_ID].as_uint();
    char command[BUFFER_LEN] = {0};
    char resultA[BUFFER_LEN] = {0};
    sprintf(command, "l2.sh set static_mac_patch %d %d %s %d" , iPort, index, mac.c_str(), vlan_id);
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);
    get(request, response);
}
