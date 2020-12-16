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

#include "psme/rest/endpoints/ethernet/static_mac_collection.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/model/handlers/handler_manager.hpp"
#include "psme/rest/model/handlers/generic_handler_deps.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"
#include "psme/core/agent/agent_manager.hpp"
#include "ecsys_helper/ecsys_helper.hpp"

#include "agent-framework/module/requests/network.hpp"
#include "agent-framework/module/responses/network.hpp"

#include "psme/rest/utils/ec_common_utils.hpp"
using namespace psme::rest::utils;
using namespace psme::rest;
using namespace psme::rest::validators;
using namespace psme::rest::endpoint;
using namespace psme::core::agent;
using namespace psme::rest::constants;
using namespace agent_framework::model;
using NetworkComponents = agent_framework::module::NetworkComponents;
using namespace ecsys_helper;

namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#EthernetSwitchStaticMACCollection.EthernetSwitchStaticMACCollection";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#EthernetSwitchStaticMACCollection.EthernetSwitchStaticMACCollection";
    r[Common::NAME] = "Static MAC Collection";
    r[Common::DESCRIPTION] = "Collection of Static MACs";
    r[Collection::ODATA_COUNT] = json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;

    return r;
}

json::Value validate_post_request(const server::Request& request) {
    json::Value schema({
        JsonValidator::mandatory(constants::Common::MAC_ADDRESS,
            JsonValidator::has_type(JsonValidator::STRING_TYPE)),
        JsonValidator::optional(constants::StaticMac::VLAN_ID,
            JsonValidator::has_type(JsonValidator::UINT_TYPE))
    });

    return JsonValidator::validate_request_body(request, schema);
}
}

StaticMacCollection::StaticMacCollection(const std::string& path) : EndpointBase(path) {}
StaticMacCollection::~StaticMacCollection() {}

void StaticMacCollection::get(const server::Request &req, server::Response &res)
{
    char command[BUFFER_LEN] = {0};
    char resultA[BUFFER_LEN] = {0};
	
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

    int iPort = std::stoi(req.params[PathParam::SWITCH_PORT_ID]);

    sprintf(command, "l2.sh get static_mac_count  %d" , iPort);
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);

    if(strlen(resultA) != 0)
    {
        int i = 0;
        json[Collection::ODATA_COUNT] = atoi(resultA);	
        for(i = 1; i <= atoi(resultA) ; i++)
        {
            json::Value link_elem(json::Value::Type::OBJECT);
            link_elem[Common::ODATA_ID] = PathBuilder(req).append(i).build();
            json[Collection::MEMBERS].push_back(std::move(link_elem));
        }	
    }
    else
         json[Collection::ODATA_COUNT] = 0;
    set_response(res, json);
}

void endpoint::StaticMacCollection::post(const server::Request &req, server::Response &res)
{
    const auto json = validate_post_request(req);
    char command[BUFFER_LEN] = {0};
    char resultA[BUFFER_LEN] = {0};

    int iPort = std::stoi(req.params[PathParam::SWITCH_PORT_ID]);
    auto vlan_id = json[constants::StaticMac::VLAN_ID].as_uint();
    auto mac = json[constants::Common::MAC_ADDRESS].as_string();
	
    if (vlan_id !=  0)
        sprintf(command, "l2.sh set static_mac %d %s %d" , iPort, mac.c_str(), vlan_id);
    else
        sprintf(command, "l2.sh set static_mac %d %s 0" , iPort, mac.c_str());

	memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);
    res.set_status(server::status_2XX::CREATED);	
}
