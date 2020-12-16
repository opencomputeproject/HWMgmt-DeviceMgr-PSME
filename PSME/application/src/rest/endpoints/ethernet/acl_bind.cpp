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
#include "agent-framework/module/requests/network.hpp"
#include "agent-framework/module/responses/network.hpp"

#include "psme/rest/constants/constants.hpp"
#include "psme/core/agent/agent_manager.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/endpoints/ethernet/acl_bind.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/model/handlers/generic_handler_deps.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"
#include "ecsys_helper/ecsys_helper.hpp"
#include "psme/rest/utils/ec_common_utils.hpp"
using namespace psme::rest::utils;
using namespace psme::core::agent;
using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::validators;
using namespace agent_framework::exceptions;
using namespace agent_framework::model;
using NetworkComponents = agent_framework::module::NetworkComponents;
using namespace ecsys_helper;

namespace
{
json::Value validate_post_request(const server::Request &request)
{
    json::Value schema({JsonValidator::object(constants::Acl::BIND_PORT, {JsonValidator::mandatory(Common::ODATA_ID,
                                                                                                   JsonValidator::has_type(JsonValidator::STRING_TYPE))})});
    return  JsonValidator::validate_request_body(request, schema);
}
} // namespace

namespace psme {
namespace rest {
namespace endpoint {

// Action Bind
template<>
void endpoint::AclBind<true>::post(const server::Request &request, server::Response &response)
{
    char resultA[2048] = {0};	
    char command[BUFFER_LEN] = {0};

    const auto json = validate_post_request(request);
    const auto&  path = json[constants::Acl::BIND_PORT][Common::ODATA_ID].as_string();

    std::string temp_path = std::regex_replace(   path.c_str(),    std::regex("/redfish/v1/EthernetSwitches/1/Ports/"), "");
    int iport = atoi(temp_path.c_str());	
    int acl_id = atoi(request.params[PathParam::ACL_ID].c_str());
	
    sprintf(command, " psme.sh get max_port_num");
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);

    if(strlen(resultA) != 0)
    {
        int Max_port = atoi(resultA);
        if (acl_id >0 && acl_id <= Max_port)
        {
            memset(resultA,0x0, sizeof(resultA));
            sprintf(command, "acl.sh set bind_acl %d %d",acl_id, iport);	
            EcCommonUtils::exec_shell(command, resultA, 0);
	 }
    }
    memset(resultA,0x0, sizeof(resultA));
    sprintf(command,"%s", "acl_init.sh clean; acl_init.sh start ");	
    EcCommonUtils::exec_shell(command, resultA, 0);
    response.set_status(server::status_2XX::NO_CONTENT);
}

// Action UnBind
template<>
void endpoint::AclBind<false>::post(const server::Request &request, server::Response &response)
{
    char resultA[2048] = {0};	
    char command[BUFFER_LEN] = {0};
    const auto json = validate_post_request(request);
    const auto&  path = json[constants::Acl::BIND_PORT][Common::ODATA_ID].as_string();

    std::string temp_path = std::regex_replace(   path.c_str(),    std::regex("/redfish/v1/EthernetSwitches/1/Ports/"), "");
    int iport = atoi(temp_path.c_str());	
    int acl_id = atoi(request.params[PathParam::ACL_ID].c_str());
	
    sprintf(command, " psme.sh get max_port_num");
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);

    if(strlen(resultA) != 0)
    {
        int Max_port = atoi(resultA);
        if (acl_id >0 && acl_id <= Max_port)
        {
            memset(resultA,0x0, sizeof(resultA));
            sprintf(command, "acl.sh set unbind_acl %d %d",acl_id, iport);	
            EcCommonUtils::exec_shell(command, resultA, 0);
	}
    }
    memset(resultA,0x0, sizeof(resultA));
    sprintf(command,"%s", "acl_init.sh clean");	
    EcCommonUtils::exec_shell(command, resultA, 0);

    memset(resultA,0x0, sizeof(resultA));
    sprintf(command,"%s", "acl_init.sh start");	
    EcCommonUtils::exec_shell(command, resultA, 0);

    response.set_status(server::status_2XX::NO_CONTENT);
}
}
}
}
