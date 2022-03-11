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
#include "psme/rest/account/manager/account_manager.hpp"
using namespace eclog_helper;
using namespace psme::rest::validators;
using namespace psme::rest::session::manager;
using namespace agent_framework::module;
using namespace agent_framework::model::enums;
using namespace psme::rest;
using namespace psme::rest::error;
using namespace psme::rest::account::manager;

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
    std::string reqID = request.params[constants::PathParam::SESSION_ID].c_str();
        json::Value r = make_session_prototype();
        r[constants::Common::ODATA_ID] = PathBuilder(request).build();
        r[constants::Common::ID] = request.params[constants::PathParam::SESSION_ID];

    const auto &Sessions = SessionManager::get_instance()->getSessionByStringId(reqID);
		
        r[constants::Common::USER_NAME]  = Sessions.get_username();
        set_response(response, r);
    return;	
}

void endpoint::Session::del(const server::Request &req, server::Response &res)
{
        json::Value r ="{}"; 
        std::string del_id = req.params[constants::PathParam::SESSION_ID].c_str();
    std::string req_token = req.get_header("xAuthGen");
    std::string req_username = req.get_header("UserName");
    std::string req_password = req.get_header("Password");
    //Admin or himself can delete its own session //
    if (req_token.size() != 0)
    {
        const auto &Sessions_from_token = SessionManager::get_instance()->getSession_by_Token(req_token);
        std::string role = Sessions_from_token.get_userrole();
        std::string session_from_token_id = Sessions_from_token.get_string_id();
        gADbg.acc_printf(LEVEL_INFO, "Session del: token[%s] role[%s] want del string id[%s]", req_token.c_str(), role.c_str(), del_id.c_str());

        if ("Administrator" == role || "Operator" == role || ( session_from_token_id == del_id))
        {
            const auto &Sessions_from_id = SessionManager::get_instance()->getSessionByStringId(del_id);
            if (("Operator" == Sessions_from_token.get_userrole()) && Sessions_from_id.get_userrole() == "Administrator")
            {
                //Operator can't del Admin's Session //
                gADbg.acc_printf(LEVEL_WARN, "Session del: Operator cannot del Admin Session");
                res.set_status(server::status_4XX::UNAUTHORIZED);
            }
            else
            {
        SessionManager::get_instance()->delSession_by_string_id(del_id);
                gADbg.acc_printf(LEVEL_INFO, "role[%s] Session logout OK!!", role.c_str());
        set_response(res, r);
        res.set_status(server::status_2XX::OK);
    }
        }
        else
        {
            gADbg.acc_printf(LEVEL_INFO, "Session del: UNAUTH");
            res.set_status(server::status_4XX::UNAUTHORIZED);
        }
    }
    else if (req_username.size() != 0 && req_password.size() != 0)
    {
        const auto &account = AccountManager::get_instance()->getAccount(req_username);
        std::string role = account.get_roleid();

        if ("Administrator" == role || "Operator" == role)
        {
            const auto &Sessions_from_id = SessionManager::get_instance()->getSessionByStringId(del_id);
            SessionManager::get_instance()->delSession_by_string_id(del_id);
            gADbg.acc_printf(LEVEL_INFO, "Use Basic Authen to role[%s] Session logout session[%s] OK!!", role.c_str(), del_id.c_str());
            set_response(res, r);
            res.set_status(server::status_2XX::OK);
        }
        else
        {
            gADbg.acc_printf(LEVEL_INFO, "Session del: UNAUTH");
            res.set_status(server::status_4XX::UNAUTHORIZED);
        }
    }
    else
    {
        gADbg.acc_printf(LEVEL_INFO, "No session enable ,Session del: OK");
        set_response(res, r);
        res.set_status(server::status_2XX::OK);
    }
}
