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

#include "psme/rest/endpoints/ethernet/olt_flow_id.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/utils/mapper.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/flow.hpp"
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

OltFlowId::~OltFlowId(){}
OltFlowId::OltFlowId(const std::string &path) : EndpointBase(path) {}

namespace
{

#ifdef BAL
json::Value make_prototype()
{
    json::Value r(json::Value::Type::OBJECT);
    r[Common::NAME] = "Flow information";
    r[Common::DESCRIPTION] = "Detail information of flows";
    return r;
}
#endif

} // namespace



void OltFlowId::del(const server::Request &req, server::Response &res)
{
    using namespace psme::rest::error;
    try
    {
#ifdef BAL
        vector<std::string> array;
        std::string flow_id_type = req.params[OFlow::FLOW_ID];
        std::string sft;
        int flow_id;
        std::replace(flow_id_type.begin(), flow_id_type.end(), '_', ' '); // replace ':' by ' '
        stringstream ss(flow_id_type);
        std::string temp;
        while (ss >> temp)
        {
            array.push_back(temp);
        }
        flow_id = atoi(array[0].c_str());
        sft = array[1];
        auto &OLT = Olt_Device::Olt_Device::get_instance();
        if (OLT.is_bal_lib_init() == true)
        {
            bool result = OLT.flow_remove(flow_id, sft);

            if (result)
                return res.set_status(server::status_2XX::OK);
            else
                return res.set_status(server::status_5XX::INTERNAL_SERVER_ERROR);
        }
        else
            return res.set_status(server::status_5XX::INTERNAL_SERVER_ERROR);

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

void OltFlowId::get(const server::Request &req, server::Response &res)
{
    using namespace psme::rest::error;
    try
    {
#ifdef BAL
        auto r = ::make_prototype();
        vector<std::string> array;
        std::string flow_id_type = req.params[OFlow::FLOW_ID];
        std::string sft;
        int flow_id;
        std::replace(flow_id_type.begin(), flow_id_type.end(), '_', ' '); // replace ':' by ' '
        stringstream ss(flow_id_type);
        std::string temp;
        while (ss >> temp)
        {
            printf("[%s]\r\n", temp.c_str());
            array.push_back(temp);
        }
        flow_id = atoi(array[0].c_str());
        sft = array[1];
        auto &OLT = Olt_Device::Olt_Device::get_instance();
        if (OLT.is_bal_lib_init() == true)
        {
            r["FlowInformation"] = OLT.get_flow_info(flow_id, sft);
            set_response(res, r);
            return;
        }
        else
            return res.set_status(server::status_5XX::INTERNAL_SERVER_ERROR);
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
