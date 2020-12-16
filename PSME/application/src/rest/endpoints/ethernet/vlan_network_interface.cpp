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
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/ethernet/vlan_network_interface.hpp"
#include "psme/rest/model/handlers/handler_manager.hpp"
#include "psme/rest/model/handlers/generic_handler_deps.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"
#include "psme/rest/utils/status_helpers.hpp"
#include "ecsys_helper/ecsys_helper.hpp"
#include "psme/rest/utils/ec_common_utils.hpp"

#include "agent-framework/module/requests/network.hpp"
#include "agent-framework/module/responses/network.hpp"
#include "agent-framework/module/network_components.hpp"

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

    json::Value rs;
    rs[Common::ODATA_TYPE] = "#Intel.Oem.VLanNetworkInterface";
    rs[::Vlan::TAGGED] = json::Value::Type::NIL;
    rs[Common::STATUS][Common::STATE] = json::Value::Type::NIL;
    rs[Common::STATUS][Common::HEALTH] = json::Value::Type::NIL;
    rs[Common::STATUS][Common::HEALTH_ROLLUP] = json::Value::Type::NIL;
    r[Common::OEM][Common::RACKSCALE] = std::move(rs);

    return r;
}

} // namespace

endpoint::VlanNetworkInterface::VlanNetworkInterface(const std::string& path) : EndpointBase(path) {}

endpoint::VlanNetworkInterface::~VlanNetworkInterface() {}

void endpoint::VlanNetworkInterface::get(const server::Request &req, server::Response &res)
{
    auto r = make_prototype();

    r[Common::ODATA_ID] = PathBuilder(req).build();
    r[Common::ID] = req.params[PathParam::VLAN_ID];
    r[Common::NAME] = Vlan::VLAN + req.params[PathParam::VLAN_ID];

    char resultA[BUFFER_LEN] = {0};
    char command[BUFFER_LEN] = {0};
    int iPort = std::stoi(req.params[PathParam::SWITCH_PORT_ID]);

    r[Vlan::VLAN_ID] =std::stoi(req.params[PathParam::VLAN_ID]); 
    r[Vlan::VLAN_ENABLE] = true;  // Todo ??

    r[Common::OEM][Common::RACKSCALE][Common::STATUS][Common::STATE] = "Enabled";
    r[Common::OEM][Common::RACKSCALE][Common::STATUS][Common::HEALTH] = "OK";
    r[Common::OEM][Common::RACKSCALE][Common::STATUS][Common::HEALTH_ROLLUP] ="OK";

    sprintf(command, "vlan.sh get vlan_port_untag_value %d %d" , std::stoi(req.params[PathParam::VLAN_ID]), iPort);
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);

    if(strlen(resultA) != 0)
    {
        if(!strncmp("untag", resultA, 5))
            r[Common::OEM][Common::RACKSCALE][::Vlan::TAGGED] = false;		
        else if(!strncmp("none", resultA, 3))
            r[Common::OEM][Common::RACKSCALE][::Vlan::TAGGED] = true;	
        else
        {
            r[Vlan::VLAN_ENABLE] = false;  // Todo ??
            r[Common::OEM][Common::RACKSCALE][Common::STATUS][Common::STATE] = json::Value::Type::NIL;
            r[Common::OEM][Common::RACKSCALE][Common::STATUS][Common::HEALTH] = json::Value::Type::NIL;
            r[Common::OEM][Common::RACKSCALE][Common::STATUS][Common::HEALTH_ROLLUP] = json::Value::Type::NIL; 

            res.set_status(server::status_2XX::NO_CONTENT);
	 }
    }
    set_response(res, r);
}

void endpoint::VlanNetworkInterface::del(const server::Request &req, server::Response &res)
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
