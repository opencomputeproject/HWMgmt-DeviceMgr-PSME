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

#include "psme/rest/endpoints/ethernet/acl.hpp"
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

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#EthernetSwitchACL.EthernetSwitchACL" ;	//For pass DMTF conference check //
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#EthernetSwitchACL.v1_0_0.EthernetSwitchACL";
    r[Common::ID] = json::Value::Type::NIL;
    r[Common::NAME] = "ACL";

    r[Common::DESCRIPTION] = "Access Control List";
    r[Common::OEM] = json::Value::Type::OBJECT;
    r[constants::Acl::RULES] = json::Value::Type::OBJECT;

    r[Common::LINKS][constants::Acl::BOUND_PORTS] = json::Value::Type::ARRAY;
    r[Common::LINKS][Common::OEM] = json::Value::Type::OBJECT;

    json::Value actions;
    std::string hash_bind_acl = constants::Common::HASH + std::string(constants::Acl::BIND_ACL);
    actions[hash_bind_acl][constants::Acl::TARGET] = json::Value::Type::NIL;
    actions[hash_bind_acl][constants::Acl::PORT_ALLOWABLE_VALUES] = json::Value::Type::ARRAY;

    std::string hash_unbind_acl = constants::Common::HASH + std::string(constants::Acl::UNBIND_ACL);
    actions[hash_unbind_acl][constants::Acl::TARGET] = json::Value::Type::NIL;
    actions[hash_unbind_acl][constants::Acl::PORT_ALLOWABLE_VALUES] = json::Value::Type::ARRAY;
    r[Common::ACTIONS] = std::move(actions);

    return r;
}
} // namespace

endpoint::Acl::Acl(const std::string& path) : EndpointBase(path) {}
endpoint::Acl::~Acl() {}

void endpoint::Acl::get(const server::Request &req, server::Response &res)
{
    auto r = ::make_prototype();

    r[Common::ODATA_CONTEXT] = std::regex_replace(
        r[Common::ODATA_CONTEXT].as_string(),
        std::regex("__SWITCH_ID__"),
        req.params[PathParam::ETHERNET_SWITCH_ID]);

    r[Common::ODATA_ID] = PathBuilder(req).build();
    r[Common::ID] = req.params[PathParam::ACL_ID];
    r[Common::NAME] = constants::Acl::ACL + req.params[PathParam::ACL_ID];

    r[constants::Acl::RULES][Common::ODATA_ID] =
        PathBuilder(req).append(constants::Acl::RULES).build();

    int iACL_ID = std::stoi(req.params[PathParam::ACL_ID]);
    json::Value link;
    char resultA[BUFFER_LEN] = {0};
    char command[BUFFER_LEN] = {0};

    sprintf(command, "acl.sh get acl_count");
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);

    if(atoi(resultA) != 0)
    {
        std::string hash_bind_acl = constants::Common::HASH + std::string(constants::Acl::BIND_ACL);
        std::string hash_unbind_acl = constants::Common::HASH + std::string(constants::Acl::UNBIND_ACL);
    
        sprintf(command, " psme.sh get max_port_num");
        memset(resultA,0x0, sizeof(resultA));
        EcCommonUtils::exec_shell(command, resultA, 0);

        if(strlen(resultA) != 0)
        {
            int max_port = atoi(resultA);
        
            sprintf(command, " acl.sh get acl_bindport %d", iACL_ID);
            memset(resultA,0x0, sizeof(resultA));
            EcCommonUtils::exec_shell(command, resultA, 0);
        // Has ports bind in this ACL	
            if(strlen(resultA) != 0)
            {
                unsigned long long  iMASK = 0x7FFFFFFFFFFFFEULL ;  
                unsigned long long  iPORT = atoll(resultA);
        		
                iPORT &=iMASK;
                r[constants::Common::ACTIONS][hash_unbind_acl][constants::Acl::TARGET] =
                PathBuilder(req).append(constants::Common::ACTIONS).append(constants::Acl::UNBIND_ACL).build();
        
                r[constants::Common::ACTIONS][hash_bind_acl][constants::Acl::TARGET] =
                PathBuilder(req).append(constants::Common::ACTIONS).append(constants::Acl::BIND_ACL).build();	
        
                for(int i = 1; i <= max_port ; i++)  //BIT 0 is CPU PORT, 
                {
                    char PATH_ID[128];
                    sprintf(PATH_ID,"/redfish/v1/EthernetSwitches/1/Ports/%d", i);
                
                    if( (1ULL << i) & iPORT)
                    {
                        link[Common::ODATA_ID] = PATH_ID;
                        r[Common::LINKS][constants::Acl::BOUND_PORTS].push_back(std::move(link));
                        r[constants::Common::ACTIONS][hash_unbind_acl][constants::Acl::PORT_ALLOWABLE_VALUES].push_back(std::move(link));				
                    }
                    else
                    {
                        link[Common::ODATA_ID] = PATH_ID;
                        r[constants::Common::ACTIONS][hash_bind_acl][constants::Acl::PORT_ALLOWABLE_VALUES].push_back(std::move(link));				
                    }
        	     }
            }
            else
            {

                r[constants::Common::ACTIONS][hash_bind_acl][constants::Acl::TARGET] =
                PathBuilder(req).append(constants::Common::ACTIONS).append(constants::Acl::BIND_ACL).build();	
        
                for(int i = 1; i <= max_port ; i++)  //BIT 0 is CPU PORT, 
                {
                    char PATH_ID[128];
                    sprintf(PATH_ID,"/redfish/v1/EthernetSwitches/1/Ports/%d", i);			
                    link[Common::ODATA_ID] = PATH_ID;
                    r[constants::Common::ACTIONS][hash_bind_acl][constants::Acl::PORT_ALLOWABLE_VALUES].push_back(std::move(link));				
        	     }
            }
        }
    }
    set_response(res, r);
}

void endpoint::Acl::del(const server::Request &req, server::Response &res)
{
    int iACL_ID = std::stoi(req.params[PathParam::ACL_ID]);
    char resultA[BUFFER_LEN] = {0};
    char command[BUFFER_LEN] = {0};

    sprintf(command, "acl.sh set del_acl %d ; acl_init.sh clean; acl_init.sh start", iACL_ID);
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);
	
    if(strlen(resultA) != 0)
    {
    res.set_status(server::status_2XX::NO_CONTENT);
}
 }
