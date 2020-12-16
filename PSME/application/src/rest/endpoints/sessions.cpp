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

#include "psme/core/agent/agent_manager.hpp"
#include "psme/rest/endpoints/sessions.hpp"
#include "psme/rest/utils/status_helpers.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/model/handlers/handler_manager.hpp"
#include "agent-framework/module/requests/common/delete_task.hpp"
#include "agent-framework/module/responses/common/delete_task.hpp"
#include "psme/rest/session/manager/session_manager.hpp"
#include "psme/rest/session/model/session.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/sessions.hpp"
#include "eclog_helper/eclog_helper.hpp"
using namespace eclog_helper;
using namespace psme::rest::validators;
using namespace psme::rest::session::manager;
using namespace agent_framework::module;
using namespace agent_framework::model::enums;
using namespace psme::rest;
using namespace psme::rest::error;

namespace {

auto &gADbg = ADbg::get_instance();

json::Value make_session_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[constants::Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#Session.Session";
    r[constants::Common::ODATA_ID] = json::Value::Type::NIL;
    r[constants::Common::ODATA_TYPE] = "#Session.v1_0_0.Session";
    r[constants::Common::NAME] = "User Session";
    r[constants::Common::DESCRIPTION] = "User Session";
    r["UserName"] =  json::Value::Type::NIL;
    return r;
}

}
endpoint::Session::Session(const std::string& path) : EndpointBase(path) {}
endpoint::Session::~Session() {}

void endpoint::Session::get(const server::Request &request, server::Response &response)
{

    uint64_t MaxSessions = SessionManager::get_instance()->GetSessionConfigMaxSessions();
	
    uint64_t reqID = atoi(request.params[constants::PathParam::SESSION_ID].c_str());
	
    if(reqID > MaxSessions)
    {
        response.set_status(server::status_4XX::NOT_FOUND); 
    }
    else
    {
        json::Value r = make_session_prototype();
        r[constants::Common::ODATA_ID] = PathBuilder(request).build();
        r[constants::Common::ID] = request.params[constants::PathParam::SESSION_ID];

        const auto& Sessions= SessionManager::get_instance()->getSession(reqID);	
		
        r[constants::Common::USER_NAME]  = Sessions.get_username();
        set_response(response, r);
    }
    return;	
}



void endpoint::Session::del(const server::Request& req, server::Response& res) {


        uint64_t deld = atoi(req.params[constants::PathParam::SESSION_ID].c_str());

        SessionManager::get_instance()->delSession(deld);

        res.set_status(server::status_2XX::OK);
    }


