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

#include "psme/rest/validators/schemas/common.hpp"
#include "psme/rest/endpoints/ethernet/acl_collection.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/common.hpp"
#include "psme/rest/model/handlers/handler_manager.hpp"
#include "psme/rest/model/handlers/generic_handler_deps.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"
#include "psme/core/agent/agent_manager.hpp"

#include "agent-framework/module/requests/network.hpp"
#include "agent-framework/module/responses/network.hpp"
#include "psme/rest/endpoints/ethernet/rule.hpp"
#include "ecsys_helper/ecsys_helper.hpp"
#include "psme/rest/utils/ec_common_utils.hpp"
using namespace psme::rest::utils;

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::core::agent;
using namespace agent_framework::model;
using NetworkComponents = agent_framework::module::NetworkComponents;
using namespace ecsys_helper;

namespace
{
json::Value make_prototype()
{
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#EthernetSwitchACLCollection.EthernetSwitchACLCollection";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#EthernetSwitchACLCollection.EthernetSwitchACLCollection";
    r[Common::NAME] = "Ethernet Switch Access Control List Collection";
    r[Common::DESCRIPTION] = "Switch Access Control List. Each ACL entry can "
        "be bound to any switch port";
    r[Collection::ODATA_COUNT] = json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;

    return r;
}

} // namespace

endpoint::AclCollection::AclCollection(const std::string& path) : EndpointBase(path) {}

endpoint::AclCollection::~AclCollection() {}

void endpoint::AclCollection::get(const server::Request& req,
                                  server::Response &res)
{
    auto r = ::make_prototype();

    r[Common::ODATA_CONTEXT] = std::regex_replace(
        r[Common::ODATA_CONTEXT].as_string(),
        std::regex("__SWITCH_ID__"),
        req.params[PathParam::ETHERNET_SWITCH_ID]);

    r[Common::ODATA_ID] = PathBuilder(req).build();

    char resultA[BUFFER_LEN] = {0};
    char command[BUFFER_LEN] = {0};

    sprintf(command, "acl.sh get acl_count");
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);

    if(strlen(resultA) != 0)
    {
        int icount = std::stoi(resultA);
        int i =0;
        r[Collection::ODATA_COUNT] = static_cast<std::uint32_t>(icount);

        for (i = 0; i < icount; i++)
        {
            json::Value link_elem(json::Value::Type::OBJECT);
    
            sprintf(command, "acl.sh get acl_index_content  %d", i);
            memset(resultA,0x0, sizeof(resultA));
            EcCommonUtils::exec_shell(command, resultA, 0);
    
           if(strlen(resultA) != 0)
           {
               link_elem[Common::ODATA_ID] = PathBuilder(req).append(atoi(resultA)).build();
               r[Collection::MEMBERS].push_back(std::move(link_elem));
           }
        }	
    }
    else
        r[Collection::ODATA_COUNT] =0;

    set_response(res, r);
}

void endpoint::AclCollection::post(const server::Request& req,
                                   server::Response &res)
{
    validators::JsonValidator::validate_empty_request(req);

    char resultA[BUFFER_LEN] = {0};
    char command[BUFFER_LEN] = {0};

    sprintf(command, "acl.sh get acl_count");  
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);

    if(atoi(resultA) >= ACL_MAX)
    {
        res.set_status(server::status_5XX::SERVICE_UNAVAILABLE);
        return;
    }

    sprintf(command, "acl.sh set add_acl");
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);
	
    if(strlen(resultA) != 0)
    {
        endpoint::utils::set_location_header(res, PathBuilder(req).append(atoi(resultA)).build());    
        res.set_status(server::status_2XX::CREATED);
    }
}
