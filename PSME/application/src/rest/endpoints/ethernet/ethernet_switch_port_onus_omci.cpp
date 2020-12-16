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

#include "psme/rest/endpoints/ethernet/ethernet_switch_port_onus_omci.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/utils/mapper.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/omci.hpp"
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

EthernetSwitchPortOnusOmci::EthernetSwitchPortOnusOmci(const std::string &path) : EndpointBase(path) {}

EthernetSwitchPortOnusOmci::~EthernetSwitchPortOnusOmci() {}

void EthernetSwitchPortOnusOmci::post(const server::Request &req, server::Response &res)
{
    using namespace psme::rest::error;
    try
    {
#ifdef BAL
        int port_id = std::stoi(req.params[PathParam::SWITCH_PORT_ID]);
        int onu_id = std::stoi(req.params[PathParam::ONU_ID]);

        const auto json = JsonValidator::validate_request_body<schema::OmciPostSchema>(req);
        if (json.is_member(constants::Olt::OMCI))
        {
            char m_raw_omci[256] = {0x0};
            memcpy(m_raw_omci, json[constants::Olt::OMCI].as_string().c_str(), 90);

            auto &pOLT = Olt_Device::Olt_Device::get_instance();
            if (pOLT.is_bal_lib_init() == true)
            {
                if (pOLT.omci_msg_out(port_id - 1, onu_id, m_raw_omci))
                {
                    printf("Send OMCI OK!\r\n");
                    res.set_status(server::status_2XX::OK);
                    return;
                }
                else
                {
                    printf("Send OMCI NG!\r\n");
                    res.set_status(server::status_5XX::INTERNAL_SERVER_ERROR);
                    return;
                }
            }
        }
        return ;
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
