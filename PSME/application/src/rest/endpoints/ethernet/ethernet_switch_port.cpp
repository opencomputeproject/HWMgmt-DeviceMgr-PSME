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
#define UNUSED(x) (void)(x)
using namespace psme::rest;
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
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#EthernetSwitchPort.v1_0_0.EthernetSwitchPort";
    r[Common::ID] = json::Value::Type::NIL;
    r[Common::NAME] = "Ethernet Switch Port";
    r[Common::DESCRIPTION] = "Ethernet Switch Port description";
    r[constants::EthernetSwitchPort::PORT_ID] = json::Value::Type::NIL;
    r[Common::STATUS][Common::STATE] = json::Value::Type::NIL;
    r[Common::STATUS][Common::HEALTH] = json::Value::Type::NIL;
    r[Common::STATUS][Common::HEALTH_ROLLUP] = json::Value::Type::NIL;

    r[constants::EthernetSwitchPort::LINK_TYPE] = json::Value::Type::NIL;
    r[constants::EthernetSwitchPort::OPERATIONAL_STATE] = json::Value::Type::NIL;
    r[constants::EthernetSwitchPort::TX_DISABLED_STATE] = json::Value::Type::NIL;
    r[constants::EthernetSwitchPort::ADMINISTRATIVE_STATE] = json::Value::Type::NIL;
    r[constants::EthernetSwitchPort::LINK_SPEED] = json::Value::Type::NIL;

    r[constants::EthernetSwitchPort::NEIGHBOR_INFO]
     [constants::EthernetSwitchPort::SWITCH_ID] = json::Value::Type::NIL;
    r[constants::EthernetSwitchPort::NEIGHBOR_INFO]
     [constants::EthernetSwitchPort::PORT_ID] = json::Value::Type::NIL;
    r[constants::EthernetSwitchPort::NEIGHBOR_INFO]
     [constants::EthernetSwitchPort::CABLE_ID] = json::Value::Type::NIL;
    r[constants::EthernetSwitchPort::NEIGHBOR_MAC] = json::Value::Type::NIL;
    r[constants::EthernetSwitchPort::FRAME_SIZE] = json::Value::Type::NIL;
    r[constants::EthernetSwitchPort::AUTOSENSE] = json::Value::Type::NIL;
    r[constants::EthernetSwitchPort::FULL_DUPLEX] = json::Value::Type::NIL;
    r[constants::Common::MAC_ADDRESS] = json::Value::Type::NIL;
    r[constants::EthernetSwitchPort::PORT_CLASS] = json::Value::Type::NIL;
    r[constants::EthernetSwitchPort::PORT_MODE] = json::Value::Type::NIL;
    r[constants::EthernetSwitchPort::PORT_TYPE] = json::Value::Type::NIL;
    r[Common::OEM] = json::Value::Type::OBJECT;

    r[constants::EthernetSwitchPort::IPv4_ADDRESSES] =
        json::Value::Type::ARRAY;

    r[constants::EthernetSwitchPort::IPv6_ADDRESSES] =
        json::Value::Type::ARRAY;

    r[constants::EthernetSwitchPort::VLANS] =
        json::Value::Type::OBJECT;

    r[constants::EthernetSwitchPort::STATIC_MACS] =
        json::Value::Type::OBJECT;

    json::Value links;
    links[constants::EthernetSwitchPort::PRIMARY_VLAN] =
        json::Value::Type::NIL;
    links[constants::EthernetSwitchPort::SWITCH] =
        json::Value::Type::NIL;
    links[constants::EthernetSwitchPort::MEMBER_OF_PORT] =
        json::Value::Type::NIL;
    links[constants::EthernetSwitchPort::PORT_MEMBERS] =
        json::Value::Type::ARRAY;
    links[constants::EthernetSwitchPort::ACTIVE_ACLS] =
        json::Value::Type::ARRAY;
    links[Common::OEM] = json::Value::Type::OBJECT;

    links[Common::OEM][Common::RACKSCALE]
         [Common::ODATA_TYPE] = "#Intel.Oem.EthernetSwitchPort";

#ifndef COCP
    links[Common::OEM][Common::RACKSCALE][constants::EthernetSwitchPort::NEIGHBOR_INTERFACE][Common::ODATA_ID] = json::Value::Type::NIL;
#endif

    r[Common::LINKS] = std::move(links);
    return r;
}

std::string get_switch(const server::Request &req)
{
    return endpoint::PathBuilder(PathParam::BASE_URL)
        .append(Root::ETHERNET_SWITCHES)
        .append(req.params[PathParam::ETHERNET_SWITCH_ID])
        .build();
}

static const std::map<std::string, std::string> gami_to_rest_attributes = {
    {agent_framework::model::literals::EthernetSwitchPort::LINK_SPEED_MBPS, constants::EthernetSwitchPort::LINK_SPEED},
    {agent_framework::model::literals::EthernetSwitchPort::ADMINISTRATIVE_STATE, constants::EthernetSwitchPort::ADMINISTRATIVE_STATE},
    {agent_framework::model::literals::EthernetSwitchPort::OPERATIONAL_STATE, constants::EthernetSwitchPort::OPERATIONAL_STATE},
    {agent_framework::model::literals::EthernetSwitchPort::FRAME_SIZE, constants::EthernetSwitchPort::FRAME_SIZE},
    {agent_framework::model::literals::EthernetSwitchPort::AUTO_SENSE, constants::EthernetSwitchPort::AUTOSENSE},
    {agent_framework::model::literals::EthernetSwitchPort::DEFAULT_VLAN, constants::EthernetSwitchPort::PRIMARY_VLAN}};
} // namespace

endpoint::EthernetSwitchPort::EthernetSwitchPort(const std::string &path) : EndpointBase(path) {}

endpoint::EthernetSwitchPort::~EthernetSwitchPort() {}

bool isValidIpAddress(char *ipAddress);

bool endpoint::EthernetSwitchPort::trigger_rssi(int pon_id)
{
    //For OLT BAL API usage only
    //Send RSSI trigger command
    //Get ONUs list
#ifdef BAL
    Json::Reader onu_list_j_reader = {};
    Json::Value j_return_value;
    std::string onu_list_file_path = "/tmp/pon_" + std::to_string(pon_id - 1) + "_onu_list";
    printf("EthernetSwitchPort get_onus_list() onu_list_file_path[%s]\r\n", onu_list_file_path.c_str());
    std::ifstream if_onu_list_files(onu_list_file_path);
    bool isJson = (onu_list_j_reader.parse(if_onu_list_files, j_return_value));

    if (isJson)
    {
        int count = j_return_value["total_count"].asInt();
        if (count != 0)
        {
            //Always get onu id 1 RSSI value//
            int id = j_return_value["onu_1_id"].asInt();
            auto &pOLT = Olt_Device::Olt_Device::get_instance();
            if (pOLT.is_bal_lib_init() == true)
            {
                return pOLT.rssi_measurement(id, pon_id - 1);
            }
        }
    }
#else
    UNUSED(pon_id);
#endif
    return false;
}

void endpoint::EthernetSwitchPort::get(const server::Request &req, server::Response &res)
{
    // Port status //
    auto r = ::make_prototype();

    r[Common::ODATA_ID] = PathBuilder(req).build();
    auto switch_id = psme::rest::model::Find<agent_framework::model::EthernetSwitch>(req.params[PathParam::ETHERNET_SWITCH_ID]).get_one()->get_id();
    r[Common::ODATA_CONTEXT] = std::regex_replace(r[Common::ODATA_CONTEXT].as_string(), std::regex("__SWITCH_ID__"), std::to_string(switch_id));
    r[Common::ID] = req.params[PathParam::SWITCH_PORT_ID];
    r[Common::NAME] = constants::EthernetSwitchPort::PORT + req.params[PathParam::SWITCH_PORT_ID];

    auto port = psme::rest::model::Find<agent_framework::model::EthernetSwitchPort>(req.params[PathParam::SWITCH_PORT_ID])
                    .via<agent_framework::model::EthernetSwitch>(req.params[PathParam::ETHERNET_SWITCH_ID])
                    .get();

    int port_id = std::stoi(req.params[PathParam::SWITCH_PORT_ID]);

    r[constants::EthernetSwitchPort::PORT_ID] = port.get_port_identifier();
    endpoint::status_to_json(port, r);
    const json::Value config = configuration::Configuration::get_instance().to_json();

    auto network_components = agent_framework::module::NetworkComponents::get_instance();
    auto &port_manager = network_components->get_instance()->get_port_manager();
    auto port_uuids = port_manager.get_keys();
    auto &ecRF_Pal = ecrf_pal_helper::Switch::Switch::get_instance();

    for (const auto &port_uuid : port_uuids)
    {
        auto port_ = port_manager.get_entry_reference(port_uuid);
        if (port_->get_port_id() == (unsigned int)port_id)
        {
            if (port_->get_status().get_state() == enums::State::Enabled)
            {

                r[Common::STATUS][Common::STATE] = "Enabled";
                r[Common::STATUS][Common::HEALTH] = "OK";
                r[Common::STATUS][Common::HEALTH_ROLLUP] = "OK";
                r[constants::EthernetSwitchPort::LINK_TYPE] = "Ethernet";

                if (r[constants::EthernetSwitchPort::PORT_ID] != "RJ45 port")
                {
                    r[constants::EthernetSwitchPort::TRANS_STATIC] = port_->get_trans_info_json();
                }
            }
            else
            {
                r[Common::STATUS][Common::STATE] = "Absent";
                r[Common::STATUS][Common::HEALTH] = "Warning";
                r[Common::STATUS][Common::HEALTH_ROLLUP] = "Warning";
            }
        }
        r[Common::STATUS][Common::HEALTH_ROLLUP] = endpoint::HealthRollup<agent_framework::model::EthernetSwitchPort>().get(port.get_uuid());
        r[constants::EthernetSwitchPort::VLANS][Common::ODATA_ID] = PathBuilder(req).append(constants::EthernetSwitchPort::VLANS).build();
        r[Common::LINKS][constants::EthernetSwitchPort::SWITCH][Common::ODATA_ID] = get_switch(req);
        r[constants::EthernetSwitchPort::STATIC_MACS][Common::ODATA_ID] = PathBuilder(req).append(constants::EthernetSwitchPort::STATIC_MACS).build();
    }
    set_response(res, r);
}

void endpoint::EthernetSwitchPort::patch(const server::Request &request, server::Response &response)
{
    std::string admin;
    bool TxDisableState;
    auto json = JsonValidator::validate_request_body<schema::EthernetSwitchPortPatchSchema>(request);
    int iPort = std::stoi(request.params[PathParam::SWITCH_PORT_ID]);

    auto &ecRF_Pal = ecrf_pal_helper::Switch::Switch::get_instance();
    if (json.is_member(constants::EthernetSwitchPort::TX_DISABLED_STATE))
    {
        TxDisableState = json[constants::EthernetSwitchPort::TX_DISABLED_STATE].as_bool();
        if (false == TxDisableState)
            ecRF_Pal.set_port_tx_disable_status(iPort, false);
        else if (true == TxDisableState)
            ecRF_Pal.set_port_tx_disable_status(iPort, true);
    }

    get(request, response);
}

void endpoint::EthernetSwitchPort::del(const server::Request &req, server::Response &res)
{
   UNUSED(req);
   res.set_status(server::status_2XX::NO_CONTENT);
}
