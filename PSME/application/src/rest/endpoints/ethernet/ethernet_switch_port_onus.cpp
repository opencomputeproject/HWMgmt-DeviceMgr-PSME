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

#include "psme/rest/endpoints/ethernet/ethernet_switch_port_onus.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/utils/mapper.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/utils/lag_utils.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/model/handlers/handler_manager.hpp"
#include "psme/rest/model/handlers/generic_handler_deps.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"

#include "agent-framework/module/model/attributes/model_attributes.hpp"
#include "agent-framework/module/requests/network.hpp"
#include "agent-framework/module/responses/network.hpp"

#include "psme/core/agent/agent_manager.hpp"
#include <regex>
#ifdef BAL
#include "ecbal_api_dist_helper/ecbal_api_dist_helper.hpp"
using namespace ecbal_api_dist_helper;
#endif
#define UNUSED(x) (void)(x)
using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::endpoint;
using namespace psme::rest::error;
using namespace psme::rest::utils;
using namespace psme::rest::validators;
using namespace agent_framework::model;

namespace
{
json::Value make_prototype()
{
    json::Value r(json::Value::Type::OBJECT);
    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#EthernetSwitchONU.EthernetSwitchONU";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#EthernetSwitchONU.v1_0_0.EthernetSwitchONU";
    r[Common::NAME] = "PON Port Onus info.";
    r[Common::DESCRIPTION] = "Onu info.";
    return r;
}

} // namespace

EthernetSwitchPortOnus::EthernetSwitchPortOnus(const std::string &path) : EndpointBase(path) {}

EthernetSwitchPortOnus::~EthernetSwitchPortOnus() {}

void EthernetSwitchPortOnus::get(const server::Request &req, server::Response &res)
{
    auto json = ::make_prototype();
    int port_id = std::stoi(req.params[PathParam::SWITCH_PORT_ID]);
    json[Common::ODATA_ID] = PathBuilder(req).build();
    json[Common::ID] = req.params[PathParam::ONU_ID]; 
#ifdef BAL
    int onu_id = std::stoi(req.params[PathParam::ONU_ID]);
    auto &pOLT = Olt_Device::Olt_Device::get_instance();
    if (pOLT.is_bal_lib_init() == true)
        pOLT.rssi_measurement(onu_id, port_id - 1);
#endif
    auto network_components = agent_framework::module::NetworkComponents::get_instance();
    auto &port_manager = network_components->get_instance()->get_port_manager();
    auto port_uuids = port_manager.get_keys();
    for (const auto &port_uuid : port_uuids)
    {
        auto port_ = port_manager.get_entry_reference(port_uuid);
        if (port_->get_port_id() == (unsigned int)port_id)
        {
            json["TransceiverStatistics"] = port_->get_pon_trans_rx_pwr_info_json();
            break;
        }
    }
    set_response(res, json);
}

void EthernetSwitchPortOnus::del(const server::Request &req, server::Response &res)
{
    using namespace psme::rest::error;
    try
    {
#ifdef BAL
        auto json = ::make_prototype();
        int port_id = std::stoi(req.params[PathParam::SWITCH_PORT_ID]);
        json[Common::ODATA_ID] = PathBuilder(req).build();
        json[Common::ID] = req.params[PathParam::ONU_ID];
        int onu_id = std::stoi(req.params[PathParam::ONU_ID]);
        auto &OLT = Olt_Device::Olt_Device::get_instance();

        if (OLT.delete_onu(port_id - 1, onu_id))
            res.set_status(server::status_2XX::OK);
        else
            res.set_status(server::status_5XX::INTERNAL_SERVER_ERROR);
        return;
#else
        UNUSED(res);
        UNUSED(req);
#endif
    }
    catch (const agent_framework::exceptions::NotFound &ex)
    {
        return;
    }
}
