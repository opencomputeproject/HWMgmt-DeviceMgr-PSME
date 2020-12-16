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

#include "psme/rest/model/handlers/handler_manager.hpp"
#include "psme/rest/model/handlers/generic_handler_deps.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"

#include "psme/rest/utils/mapper.hpp"
#include "psme/rest/utils/lag_utils.hpp"
#include "psme/rest/utils/status_helpers.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/ethernet_switch_port.hpp"

#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/endpoints/endpoints.hpp"

#include "agent-framework/module/requests/network.hpp"
#include "agent-framework/module/responses/network.hpp"
#include "agent-framework/module/responses/common.hpp"
#include "configuration/configuration.hpp"
#include <arpa/inet.h>

#include "psme/rest/endpoints/manager_network_interface.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/utils/network_interface_info.hpp"

#include <stdlib.h>
#include "ecrf_pal_helper/api/ecrf_pal_helper.hpp"
using namespace ecrf_pal_helper;

#ifdef BAL 
#include "ecbal_api_dist_helper/ecbal_api_dist_helper.hpp"
using namespace ecbal_api_dist_helper;
#endif

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::error;
using namespace psme::rest::utils;
using namespace psme::rest::validators;
using namespace agent_framework::module;
using namespace agent_framework::model;
using namespace psme::rest::constants;

using PatchMembersRequest = std::tuple<requests::AddEthernetSwitchPortMembers, requests::DeleteEthernetSwitchPortMembers>;

namespace
{
json::Value make_prototype()
{
    json::Value r(json::Value::Type::OBJECT);
    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#EthernetSwitchPort.EthernetSwitchPort";
    return r;
}

std::string get_switch(const server::Request &req)
{
    return endpoint::PathBuilder(PathParam::BASE_URL)
        .append(Root::ETHERNET_SWITCHES)
        .append(req.params[PathParam::ETHERNET_SWITCH_ID])
        .build();
}

} // namespace

endpoint::PonPortOnus::PonPortOnus(const std::string &path) : EndpointBase(path) {}

endpoint::PonPortOnus::~PonPortOnus() {}

void endpoint::PonPortOnus::get(const server::Request &req, server::Response &res)
{
    // Port status //
    auto r = ::make_prototype();

    r[Common::ODATA_ID] = PathBuilder(req).build();
    auto switch_id = psme::rest::model::Find<agent_framework::model::EthernetSwitch>( req.params[PathParam::ETHERNET_SWITCH_ID]) .get_one() ->get_id();
    r[Common::ODATA_CONTEXT] = std::regex_replace(r[Common::ODATA_CONTEXT].as_string(), std::regex("__SWITCH_ID__"), std::to_string(switch_id));
    int port_id = std::stoi(req.params[PathParam::SWITCH_PORT_ID]);
    auto network_components = agent_framework::module::NetworkComponents::get_instance();
    auto &port_manager = network_components->get_instance()->get_port_manager();
    auto port_uuids = port_manager.get_keys();

    for (const auto &port_uuid : port_uuids)
    {
        auto port_ = port_manager.get_entry_reference(port_uuid);
        if (port_->get_port_id() == (unsigned int)port_id)
        {
            r["TEST"] = port_->get_pon_trans_rx_pwr_info_json();
        }
    }
    set_response(res, r);
}

void endpoint::PonPortOnus::patch(const server::Request &request, server::Response &response)
{
    get(request, response);
}

void endpoint::PonPortOnus::del(const server::Request &req, server::Response &res)
{
    res.set_status(server::status_2XX::NO_CONTENT);
}
