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

#include "psme/rest/validators/schemas/flow.hpp"
#include "psme/rest/constants/constants.hpp"

using namespace psme::rest;
using namespace psme::rest::validators::schema;

const jsonrpc::ProcedureValidator &FlowPostSchema::get_procedure()
{
    static jsonrpc::ProcedureValidator procedure{
        "flow_post",
        jsonrpc::PARAMS_BY_NAME,
        constants::PathParam::ONU_ID, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::FLOW_ID, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::FLOW_TYPE, VALID_OPTIONAL(VALID_JSON_STRING),
        constants::OFlow::PKT_TAG_TYPE, VALID_OPTIONAL(VALID_JSON_STRING),
        constants::OFlow::NNI_ID, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::PORT_ID, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::GEMPORT_ID, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::CLASSIFIER, VALID_OPTIONAL(VALID_ARRAY_OF(VALID_JSON_STRING)),
        constants::OFlow::ACTION, VALID_OPTIONAL(VALID_JSON_STRING),
        constants::OFlow::ACTION_CMD, VALID_OPTIONAL(VALID_JSON_STRING),
        constants::OFlow::ACTION_VAL, VALID_OPTIONAL(VALID_ATTRIBUTE(ActionVal)),
        constants::OFlow::CLASS_VAL, VALID_OPTIONAL(VALID_ARRAY_OF(VALID_ATTRIBUTE(ClassVal))),
        nullptr};
    return procedure;
}

const jsonrpc::ProcedureValidator &FlowDelSchema::get_procedure()
{
    static jsonrpc::ProcedureValidator procedure{
        "flow_delete",
        jsonrpc::PARAMS_BY_NAME,
        constants::OFlow::FLOW_ID, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::FLOW_TYPE, VALID_OPTIONAL(VALID_JSON_STRING),
       nullptr};
    return procedure;
}

const jsonrpc::ProcedureValidator &FlowPostSchema::ActionVal::get_procedure()
{
    static jsonrpc::ProcedureValidator procedure{
        "action_val_post",
        jsonrpc::PARAMS_BY_NAME,
        constants::OFlow::OVID, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::OPBITS, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::OTPID, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::IVID, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::IPBITS, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::ITPID, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::ETHERTYPE, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::IPPROTO, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::SRCPORT, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::DSTPORT, VALID_OPTIONAL(VALID_JSON_INTEGER),
        nullptr};
    return procedure;
}

const jsonrpc::ProcedureValidator& FlowPostSchema::ClassVal::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "class_val_post",
        jsonrpc::PARAMS_BY_NAME,
        constants::OFlow::OVID, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::OPBITS, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::OTPID, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::IVID, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::IPBITS, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::ITPID, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::ETHERTYPE, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::IPPROTO, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::SRCPORT, VALID_OPTIONAL(VALID_JSON_INTEGER),
        constants::OFlow::DSTPORT, VALID_OPTIONAL(VALID_JSON_INTEGER),
        nullptr};
    return procedure;
}