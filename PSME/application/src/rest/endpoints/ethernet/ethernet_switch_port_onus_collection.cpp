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
#include "psme/rest/endpoints/ethernet/ethernet_switch_port_onus_collection.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/utils/mapper.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/ethernet_switch_port_onus_collection.hpp"
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
#include <string>
#include <ostream>
#include <iostream>
#include <fstream>
#include <istream>

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
    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#EthernetSwitchONUCollection.EthernetSwitchONUCollection";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#EthernetSwitchONUCollection.EthernetSwitchONUCollection";
    r[Common::NAME] = "PON Port Onus Collection";
    r[Common::DESCRIPTION] = "Collection of PON Port Onus";
    r[Collection::ODATA_COUNT] = json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;
    return r;
}

} // namespace

EthernetSwitchPortOnusCollection::EthernetSwitchPortOnusCollection(const std::string &path) : EndpointBase(path) {}

EthernetSwitchPortOnusCollection::~EthernetSwitchPortOnusCollection() {}

void EthernetSwitchPortOnusCollection::get(const server::Request &req, server::Response &res)
{
    try
    {
        int port_id = std::stoi(req.params[PathParam::SWITCH_PORT_ID]);
        Json::Reader onu_list_j_reader = {};
        Json::Value j_return_value;
        std::string onu_list_file_path = "/tmp/pon_" + std::to_string(port_id - 1) + "_onu_list";
        printf("EthernetSwitchPortOnusCollection() onu_list_file_path[%s]\r\n", onu_list_file_path.c_str());

        std::ifstream if_onu_list_files(onu_list_file_path);

        bool isJson = (onu_list_j_reader.parse(if_onu_list_files, j_return_value));

        if (isJson)
        {
            printf("Get onu file list file ok!!\r\n");
            int max_onus = j_return_value["total_count"].asInt();

            auto json = ::make_prototype();
            json[Common::ODATA_ID] = PathBuilder(req).build();
            json[Common::ODATA_CONTEXT] = std::regex_replace(json[Common::ODATA_CONTEXT].as_string(), std::regex("__SWITCH_ID__"), req.params[PathParam::ETHERNET_SWITCH_ID]);
            json[Collection::ODATA_COUNT] = max_onus;

            for (int i = 1; i <= max_onus; i++)
            {
                char onu_id[32] = {};
                sprintf(onu_id, "onu_%d_id", i);
                int id = j_return_value[onu_id].asInt();
                json::Value link_elem(json::Value::Type::OBJECT);
                link_elem[Common::ODATA_ID] = PathBuilder(req).append(id).build();
                json[Collection::MEMBERS].push_back(std::move(link_elem));
            }
            set_response(res, json);
        }
        else
            printf("Get sys fs mapping file error!!\r\n");
    }
    catch (const std::exception &e)
    {
        std::cout << "EthernetSwitchPortOnusCollection get() - exception : " << e.what() << std::endl;
    }
}
void EthernetSwitchPortOnusCollection::del(const server::Request &req, server::Response &res)
{
    UNUSED(res);
    UNUSED(req);
}

void EthernetSwitchPortOnusCollection::post(const server::Request &req, server::Response &res)
{
#define VENDOR_SPEC_SIZE 8
    using namespace psme::rest::error;
    try
    {
#ifdef BAL
        auto json = JsonValidator::validate_request_body<schema::EthernetSwitchPortOnusCollectionPostSchema>(req);
        int port_id = std::stoi(req.params[PathParam::SWITCH_PORT_ID]);
        int onu_id = json[constants::ONU::ONU_ID].as_int();
        std::string vendor_id = json[constants::ONU::VENDOR_ID].as_string();
        std::string vendor_spec = json[constants::ONU::VENDOR_SPECIFIC].as_string();

        auto &OLT = Olt_Device::Olt_Device::get_instance();

        long unsigned int buflen = vendor_spec.size();
        char cs_vendor_spec[VENDOR_SPEC_SIZE] = {0x0};
        uint16_t idx1 = 0;
        uint16_t idx2 = 0;
        char str1[20] = {0x0};
        char str2[20] = {0x0};

        if (buflen == VENDOR_SPEC_SIZE)
            memset(&cs_vendor_spec, 0, buflen);
        else
            return res.set_status(server::status_5XX::INTERNAL_SERVER_ERROR);

        for (idx1 = 0, idx2 = 0; idx1 < buflen; idx1++, idx2++)
        {
            sprintf(str1, "%c", vendor_spec[idx1]);
            sprintf(str2, "%c", vendor_spec[++idx1]);
            strcat(str1, str2);
            cs_vendor_spec[idx2] = (char)strtol(str1, NULL, 16);
        }

        printf("////////////Active ONU[%s][0x%02X][0x%02X][0x%02X][0x%02X] !!////////////\r\n",
               vendor_id.c_str(), cs_vendor_spec[0], cs_vendor_spec[1], cs_vendor_spec[2], cs_vendor_spec[3]);

        if (OLT.activate_onu(port_id - 1, onu_id, vendor_id.c_str(), cs_vendor_spec))
            res.set_status(server::status_2XX::CREATED);
        else
            res.set_status(server::status_5XX::INTERNAL_SERVER_ERROR);
#else
        UNUSED(res);
        UNUSED(req);
#endif
    }
    catch (const agent_framework::exceptions::NotFound &ex)
    {
        return;
    }
    return;
}
