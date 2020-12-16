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

#include "psme/rest/endpoints/ethernet/olt_flow.hpp"
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

#ifdef BAL
namespace
{
json::Value make_prototype()
{
    json::Value r(json::Value::Type::OBJECT);
    r[Common::NAME] = "Flow Collection";
    r[Common::DESCRIPTION] = "Collection of flows";
    r[Collection::ODATA_COUNT] = json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;
    return r;
}

} // namespace
typedef enum
{
    BCMOLT_ACTION_CMD_ID_NONE = 0,
    BCMOLT_ACTION_CMD_ID_ADD_OUTER_TAG = 0x0001,
    BCMOLT_ACTION_CMD_ID_REMOVE_OUTER_TAG = 0x0002,
    BCMOLT_ACTION_CMD_ID_XLATE_OUTER_TAG = 0x0004,
    BCMOLT_ACTION_CMD_ID_ADD_INNER_TAG = 0x0010,
    BCMOLT_ACTION_CMD_ID_REMOVE_INNER_TAG = 0x0020,
    BCMOLT_ACTION_CMD_ID_XLATE_INNER_TAG = 0x0040,
    BCMOLT_ACTION_CMD_ID_REMARK_OUTER_PBITS = 0x0100,
    BCMOLT_ACTION_CMD_ID_REMARK_INNER_PBITS = 0x0200,
} bcmolt_action_cmd_id;

typedef enum
{
    BCMOLT_ACTION_ID__BEGIN = 0,
    BCMOLT_ACTION_ID_CMDS_BITMASK = 0,
    BCMOLT_ACTION_ID_O_VID = 1,
    BCMOLT_ACTION_ID_O_PBITS = 2,
    BCMOLT_ACTION_ID_I_VID = 3,
    BCMOLT_ACTION_ID_I_PBITS = 4,
    BCMOLT_ACTION_ID__NUM_OF,

} bcmolt_action_id;

typedef enum
{
    BCMOLT_CLASSIFIER_ID__BEGIN = 0,
    BCMOLT_CLASSIFIER_ID_O_VID = 0,
    BCMOLT_CLASSIFIER_ID_I_VID = 1,
    BCMOLT_CLASSIFIER_ID_O_PBITS = 2,
    BCMOLT_CLASSIFIER_ID_I_PBITS = 3,
    BCMOLT_CLASSIFIER_ID_ETHER_TYPE = 4,
    BCMOLT_CLASSIFIER_ID_DST_MAC = 5,
    BCMOLT_CLASSIFIER_ID_SRC_MAC = 6,
    BCMOLT_CLASSIFIER_ID_IP_PROTO = 7,
    BCMOLT_CLASSIFIER_ID_DST_IP = 8,
    BCMOLT_CLASSIFIER_ID_SRC_IP = 9,
    BCMOLT_CLASSIFIER_ID_SRC_PORT = 10,
    BCMOLT_CLASSIFIER_ID_DST_PORT = 11,
    BCMOLT_CLASSIFIER_ID_PKT_TAG_TYPE = 12,
    BCMOLT_CLASSIFIER_ID_CLASSIFIER_BITMAP = 13,
    BCMOLT_CLASSIFIER_ID__NUM_OF,
} bcmolt_classifier_id;

std::map<std::string, bcmolt_action_cmd_id> action_cmd_id_map = {};
std::map<std::string, bcmolt_action_id> action_id_map = {};
std::map<std::string, bcmolt_classifier_id> classifier_id_map = {};

#endif

OltFlow::~OltFlow(){}
OltFlow::OltFlow(const std::string &path) : EndpointBase(path) 
{
#ifdef BAL
    action_cmd_id_map["BCMOLT_ACTION_CMD_ID_ADD_OUTER_TAG"] = BCMOLT_ACTION_CMD_ID_ADD_OUTER_TAG;
    action_cmd_id_map["BCMOLT_ACTION_CMD_ID_REMOVE_OUTER_TAG"] = BCMOLT_ACTION_CMD_ID_REMOVE_OUTER_TAG;
    action_cmd_id_map["BCMOLT_ACTION_CMD_ID_XLATE_OUTER_TAG"] = BCMOLT_ACTION_CMD_ID_XLATE_OUTER_TAG;
    action_cmd_id_map["BCMOLT_ACTION_CMD_ID_ADD_INNER_TAG"] = BCMOLT_ACTION_CMD_ID_ADD_INNER_TAG;
    action_cmd_id_map["BCMOLT_ACTION_CMD_ID_REMOVE_INNER_TAG"] = BCMOLT_ACTION_CMD_ID_REMOVE_INNER_TAG;
    action_cmd_id_map["BCMOLT_ACTION_CMD_ID_XLATE_INNER_TAG"] = BCMOLT_ACTION_CMD_ID_XLATE_INNER_TAG;
    action_cmd_id_map["BCMOLT_ACTION_CMD_ID_REMARK_OUTER_PBITS"] = BCMOLT_ACTION_CMD_ID_REMARK_OUTER_PBITS;
    action_cmd_id_map["BCMOLT_ACTION_CMD_ID_REMARK_INNER_PBITS"] = BCMOLT_ACTION_CMD_ID_REMARK_INNER_PBITS;

    action_id_map["BCMOLT_ACTION_ID_CMDS_BITMASK"] = BCMOLT_ACTION_ID_CMDS_BITMASK;
    action_id_map["BCMOLT_ACTION_ID_O_VID"] = BCMOLT_ACTION_ID_O_VID;
    action_id_map["BCMOLT_ACTION_ID_O_PBITS"] = BCMOLT_ACTION_ID_O_PBITS;
    action_id_map["BCMOLT_ACTION_ID_I_VID"] = BCMOLT_ACTION_ID_I_VID;
    action_id_map["BCMOLT_ACTION_ID_I_PBITS"] = BCMOLT_ACTION_ID_I_PBITS;

    classifier_id_map["BCMOLT_CLASSIFIER_ID_O_VID"] = BCMOLT_CLASSIFIER_ID_O_VID;
    classifier_id_map["BCMOLT_CLASSIFIER_ID_I_VID"] = BCMOLT_CLASSIFIER_ID_I_VID;
    classifier_id_map["BCMOLT_CLASSIFIER_ID_O_PBITS"] = BCMOLT_CLASSIFIER_ID_O_PBITS;
    classifier_id_map["BCMOLT_CLASSIFIER_ID_I_PBITS"] = BCMOLT_CLASSIFIER_ID_I_PBITS;
    classifier_id_map["BCMOLT_CLASSIFIER_ID_ETHER_TYPE"] = BCMOLT_CLASSIFIER_ID_ETHER_TYPE;
    classifier_id_map["BCMOLT_CLASSIFIER_ID_DST_MAC"] = BCMOLT_CLASSIFIER_ID_DST_MAC;
    classifier_id_map["BCMOLT_CLASSIFIER_ID_SRC_MAC"] = BCMOLT_CLASSIFIER_ID_SRC_MAC;
    classifier_id_map["BCMOLT_CLASSIFIER_ID_IP_PROTO"] = BCMOLT_CLASSIFIER_ID_IP_PROTO;
    classifier_id_map["BCMOLT_CLASSIFIER_ID_DST_IP"] = BCMOLT_CLASSIFIER_ID_DST_IP;
    classifier_id_map["BCMOLT_CLASSIFIER_ID_SRC_IP"] = BCMOLT_CLASSIFIER_ID_SRC_IP;
    classifier_id_map["BCMOLT_CLASSIFIER_ID_DST_PORT"] = BCMOLT_CLASSIFIER_ID_DST_PORT;
    classifier_id_map["BCMOLT_CLASSIFIER_ID_SRC_PORT"] = BCMOLT_CLASSIFIER_ID_SRC_PORT;
    classifier_id_map["BCMOLT_CLASSIFIER_ID_PKT_TAG_TYPE"] = BCMOLT_CLASSIFIER_ID_PKT_TAG_TYPE;
    classifier_id_map["BCMOLT_CLASSIFIER_ID_CLASSIFIER_BITMAP"] = BCMOLT_CLASSIFIER_ID_CLASSIFIER_BITMAP;
#endif
}

#ifdef BAL
bool set_classifier_val(struct class_val *class_val_c_val, json::Value j_json);
bool set_classifier_val(struct class_val *class_val_c_val, json::Value j_json)
{
    size_t size = j_json[constants::OFlow::CLASSIFIER].size();

    for (size_t i = 0; i < size; i++)
    {
        std::string classifier_map_index = j_json[constants::OFlow::CLASSIFIER][i].as_string();
        int class_e = classifier_id_map[classifier_map_index];

        switch (class_e)
        {
        case BCMOLT_CLASSIFIER_ID_O_VID:
            class_val_c_val->o_vid = (uint16_t)j_json[constants::OFlow::CLASS_VAL][0]["OVid"].as_uint();
            break;
        case BCMOLT_CLASSIFIER_ID_I_VID:
            class_val_c_val->i_vid = (uint16_t)j_json[constants::OFlow::CLASS_VAL][0]["IVid"].as_uint();
            break;
        case BCMOLT_CLASSIFIER_ID_O_PBITS:
            class_val_c_val->o_pbits = (uint8_t)j_json[constants::OFlow::CLASS_VAL][0]["OPbits"].as_uint();
            break;
        case BCMOLT_CLASSIFIER_ID_I_PBITS:
            class_val_c_val->i_pbits = (uint8_t)j_json[constants::OFlow::CLASS_VAL][0]["IPbits"].as_uint();
            break;
        case BCMOLT_CLASSIFIER_ID_ETHER_TYPE:
            class_val_c_val->ether_type = (uint16_t)j_json[constants::OFlow::CLASS_VAL][0]["EtherType"].as_uint();
            break;
        case BCMOLT_CLASSIFIER_ID_DST_MAC:
        case BCMOLT_CLASSIFIER_ID_SRC_MAC:
        case BCMOLT_CLASSIFIER_ID_DST_IP:
        case BCMOLT_CLASSIFIER_ID_SRC_IP:
        case BCMOLT_CLASSIFIER_ID_PKT_TAG_TYPE:
        case BCMOLT_CLASSIFIER_ID_CLASSIFIER_BITMAP:
            printf("no support!!\r\n");
            break;
        case BCMOLT_CLASSIFIER_ID_IP_PROTO:
            class_val_c_val->ip_proto = (uint8_t)j_json[constants::OFlow::CLASS_VAL][0]["IpProto"].as_uint();
            break;
        case BCMOLT_CLASSIFIER_ID_DST_PORT:
            class_val_c_val->dst_port = (uint16_t)j_json[constants::OFlow::CLASS_VAL][0]["DstPort"].as_uint();
            break;
        case BCMOLT_CLASSIFIER_ID_SRC_PORT:
            class_val_c_val->src_port = (uint16_t)j_json[constants::OFlow::CLASS_VAL][0]["SrcPort"].as_uint();
            break;
        default:
            break;
        }
    }
    return true;
}

bool set_action_val(struct action_val * action_val_a_val, json::Value j_json);
bool set_action_val(struct action_val * action_val_a_val, json::Value j_json)
{
    std::string action_map_index = j_json[constants::OFlow::ACTION].as_string();
    int action_e = action_id_map[action_map_index];
    switch (action_e)
    {
    case BCMOLT_ACTION_ID_CMDS_BITMASK:
        printf("not support!\r\n");
        break;
    case BCMOLT_ACTION_ID_O_VID:
        action_val_a_val->o_vid = (uint16_t)j_json[constants::OFlow::ACTION_VAL]["OVid"].as_uint();
        break;
    case BCMOLT_ACTION_ID_O_PBITS:
        action_val_a_val->o_pbits = (uint8_t)j_json[constants::OFlow::ACTION_VAL]["OPbits"].as_uint();
        break;
    case BCMOLT_ACTION_ID_I_VID:
        action_val_a_val->i_vid = (uint16_t)j_json[constants::OFlow::ACTION_VAL]["IVid"].as_uint();
        break;
    case BCMOLT_ACTION_ID_I_PBITS:
        action_val_a_val->i_pbits = (uint8_t)j_json[constants::OFlow::ACTION_VAL]["IPbits"].as_uint();
        break;
    default:
        break;
    }
    return true;
}
#endif

void OltFlow::post(const server::Request &req, server::Response &res)
{
    using namespace psme::rest::error;
    try
    {
#ifdef BAL
        std::string sft;  
        std::string sptt; 
        std::string action;
        std::string action_cmd; 
        std::string classifier; 
        int flow_id = 0; 
        int nni_id = 0; 
        int onu_id = 0; 
        int port_id = 0; 
        int gem_id = 0; 
        int action_in = 0;
        int action_cmd_in = 0;

        int classifier_in = 0;
        uint16_t in_action_val = 0;

        action_val action_val_a_val ={0};
        class_val class_val_c_val ={0};

        const auto json = JsonValidator::validate_request_body<schema::FlowPostSchema>(req);

        if (json.is_member(PathParam::ONU_ID))
        {
            onu_id = json[PathParam::ONU_ID].as_int();
        }

        if (json.is_member(constants::OFlow::FLOW_ID))
        {
            flow_id = json[constants::OFlow::FLOW_ID].as_int();
        }

        if (json.is_member(constants::OFlow::FLOW_TYPE))
        {
            sft = json[constants::OFlow::FLOW_TYPE].as_string() ;
        }

        if (json.is_member(constants::OFlow::PKT_TAG_TYPE))
        {
            sptt = json[constants::OFlow::PKT_TAG_TYPE].as_string();
        }

        if (json.is_member(constants::OFlow::PORT_ID))
        {
            port_id = json[constants::OFlow::PORT_ID].as_int();
        }

        if (json.is_member(constants::OFlow::NNI_ID))
        {
            nni_id = json[constants::OFlow::NNI_ID].as_int();
        }

        if (json.is_member(constants::OFlow::GEMPORT_ID))
        {
            gem_id = json[constants::OFlow::GEMPORT_ID].as_int();
        }

        if (json.is_member(constants::OFlow::CLASSIFIER))
        {
            if (json.is_member(constants::OFlow::CLASS_VAL))
            {
                set_classifier_val(&class_val_c_val, json);
            }
        }

        if (json.is_member(constants::OFlow::ACTION_CMD))
        {
            action_in = action_id_map[json[constants::OFlow::ACTION].as_string()];
            if (json.is_member(constants::OFlow::ACTION_VAL))
            {
                printf("in_action_val[%d]\r\n", in_action_val);
                set_action_val(&action_val_a_val, json);
            }
        }

        if (json.is_member(constants::OFlow::ACTION_CMD))
        {
            action_cmd = json[constants::OFlow::ACTION_CMD].as_string();
            printf("action_cmd[%s]\r\n", action_cmd.c_str());
            action_cmd_in = action_cmd_id_map[action_cmd];
            printf("action_cmd_in[%d]\r\n", action_cmd_in);
        }

        auto &OLT = Olt_Device::Olt_Device::get_instance();
        if (OLT.is_bal_lib_init() == true)
        {
            bool result = OLT.flow_add(onu_id, flow_id, sft, sptt, port_id, nni_id, gem_id, classifier_in,
                                    action_in, action_cmd_in, action_val_a_val, class_val_c_val);

            if (result)
                return res.set_status(server::status_2XX::OK);
            else
                return res.set_status(server::status_5XX::INTERNAL_SERVER_ERROR);
        }
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

void OltFlow::get(const server::Request &req, server::Response &res)
{
    using namespace psme::rest::error;
    try
    {
#ifdef BAL
        auto json = make_prototype();
        auto &OLT = Olt_Device::Olt_Device::get_instance();
        if (OLT.is_bal_lib_init() == true)
        {
            std::map<flow_pair, int32_t> flow_map_r = OLT.get_flow_map();
            long unsigned int count = flow_map_r.size();
            if (count != 0)
            {
                json[Collection::ODATA_COUNT] = count;
                std::map<flow_pair, int>::iterator it;
                for (it = flow_map_r.begin(); it != flow_map_r.end(); it++)
                {
                    json::Value link_elem(json::Value::Type::OBJECT);
                    std::string flow_name;
                    if (it->first.second == 0)
                        flow_name = std::to_string((unsigned int)it->first.first) + "_" + "upstream";
                    else
                        flow_name = std::to_string((unsigned int)it->first.first) + "_" + "downstream";

                    link_elem[Common::ODATA_ID] = PathBuilder(req).append(flow_name).build();
                    json[Collection::MEMBERS].push_back(std::move(link_elem));
                }
                set_response(res, json);
            }
            else
            {
                json[Collection::ODATA_COUNT] = 0;
                set_response(res, json);
            }
        }
        else
            set_response(res, json);
#else
        UNUSED(res);
        UNUSED(req);
#endif
    }
    catch (const agent_framework::exceptions::NotFound &ex)
    {
        printf("OltFlow get error!!\r\n");
        return;
    }
}
