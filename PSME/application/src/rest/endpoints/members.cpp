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

#include "psme/rest/endpoints/session_collection.hpp"
#include "psme/rest/endpoints/members.hpp"
#include "psme/rest/utils/status_helpers.hpp"

#include "agent-framework/module/managers/utils/manager_utils.hpp"

#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/sessions.hpp"
#include "psme/rest/session/manager/session_manager.hpp"
#include "psme/rest/validators/schemas/session_collection.hpp"
#include "psme/rest/account/manager/account_manager.hpp"
#include "psme/rest/account/model/accountservice.hpp" 
#include "eclog_helper/eclog_helper.hpp"
#include "psme/rest/utils/ec_common_utils.hpp"
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace psme::rest::utils;
using namespace psme::rest::validators;
using namespace eclog_helper;
using namespace psme::rest;
using namespace psme::rest::session::manager;
using namespace psme::rest::constants;
using namespace agent_framework::module;
using namespace agent_framework::model::enums;
using namespace psme::rest::account::manager;

namespace
{
auto &gADbg = ADbg::get_instance();
/*
json::Value make_session_prototype()
{
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#SessionCollection.SessionCollection";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#SessionCollection.SessionCollection";
    r[Common::NAME] = "Session Collection";
    r[Common::DESCRIPTION] = "Session Collection";
    r[Collection::ODATA_COUNT] = json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;

    return r;
}
*/

} // namespace

namespace psme
{
namespace rest
{
namespace endpoint
{

Members::Members(const std::string& path) : EndpointBase(path) { }

Members::~Members() { }


Session to_members_model(const json::Value& json);
Session to_members_model(const json::Value &json)
{
    Session s;    
    const auto& username = json[SessionService::USERNAME].as_string();
    const auto& password = json[SessionService::PASSWORD].as_string();
               
    s.set_username(username);
    s.set_password(password);
    
    return s;
}

json::Value make_members_post_prototype();
json::Value make_members_post_prototype()
{
    json::Value r(json::Value::Type::OBJECT);

    r[constants::Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#Session.Session";
    r[constants::Common::ODATA_ID] = json::Value::Type::NIL;
    r[constants::Common::ODATA_TYPE] = "#Session.v1_0_0.Session";
    r[constants::Common::NAME] = "User Session";
    r[constants::Common::DESCRIPTION] = "User Session";
    r[constants::Common::USER_NAME] =  json::Value::Type::NIL;
    return r;
}

void endpoint::Members::post(const server::Request& request, server::Response& response)
{
    bool Enabled = SessionManager::get_instance()->GetSessionConfigEnable();
    if (Enabled == false)
    {
        gADbg.acc_printf(LEVEL_WARN, "Session service not enable!");
        response.set_status(server::status_5XX::SERVICE_UNAVAILABLE);
        return;
    }

    uint64_t Session_size = SessionManager::get_instance()->Session_size();

    uint64_t MaxSessions = SessionManager::get_instance()->GetSessionConfigMaxSessions();

    if (Session_size >= MaxSessions)
    {
        gADbg.acc_printf(LEVEL_WARN, "Session size over %d number", MaxSessions);
        response.set_status(server::status_5XX::SERVICE_UNAVAILABLE);
        return;
    }

    json::Value r = make_members_post_prototype();

    std::string username{};
    std::string password{};

    const auto &json = JsonValidator::validate_request_body<schema::SessionCollectionPostSchema>(request);
    Session session = to_members_model(json);

    if (json.is_member(constants::Common::USER_NAME) || json.is_member(constants::Common::PASSWORD))
    {
        char command[BUFFER_LEN] = {0};
        std::string AuthenToken;
        //Setup new session //
        username = json[constants::Common::USER_NAME].as_string();
        password = json[constants::Common::PASSWORD].as_string();
        gADbg.acc_printf(LEVEL_INFO, "Login UserName[%s], PassWord[%s] ", username.c_str(), password.c_str());

        sprintf(command, "%s", "openssl rand -hex 16");
        EcCommonUtils::exec_shell_(command, AuthenToken, 1);
        AuthenToken.erase(AuthenToken.length() - 1);
        /* Todo , above function create the same token if query in short time ?? 
        srand((unsigned int)time(0L));
        std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        unsigned long int pos;
        AuthenToken.clear();
        while (AuthenToken.size() != 32)
        { //Token len 32 bytes //
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            pos = ((rand() % (str.size() - 1)));
            gADbg.acc_printf(LEVEL_INFO, "pos[%d]", pos);
            AuthenToken += str.substr(pos, 1);
        }
*/
        if (((username.length() <= 256) && (username.length() > 0)) && ((password.length() <= 256) && (password.length() > 0)))
        {
            /*Check if this username locked*/
            if (!AccountManager::get_instance()->checkAccount(username))
            {
                response.set_status(server::status_4XX::UNAUTHORIZED);
                return;
            }

            const auto &account = AccountManager::get_instance()->getAccount(username);
            if (account.get_locked() == true)
            {
                response.set_status(server::status_5XX::SERVICE_UNAVAILABLE);
                gADbg.acc_printf(LEVEL_WARN, "Lock on user [%s]", username.c_str());
                return;
            }

            /*Check if correct username and password*/
            int res = AccountManager::get_instance()->login(username, password);
            if (res == 0)
            {
                /**/
                const auto &account_enable_chk = AccountManager::get_instance()->getAccount(username);
                if (account_enable_chk.get_enabled() != true)
                {
                    response.set_status(server::status_4XX::UNAUTHORIZED);
                    return;
                }

                const auto &role = AccountManager::get_instance()->getRole(account.get_roleid());

                if (SessionManager::get_instance()->checkSessionExist_by_name(username))
                {
                    gADbg.acc_printf(LEVEL_WARN, "Found exist ID and del first to cerate new one session");
                    //SessionManager::get_instance()->delSession_by_string_id(SessionManager::get_instance()->getSession(username).get_string_id());
                }
                else
                {
                    gADbg.acc_printf(LEVEL_WARN, "Not Found ID by name[%s]", username.c_str());
                }

                session.set_authen_token(AuthenToken);
                session.set_userrole(role.get_roleid());

                std::string id = SessionManager::get_instance()->addSession(session, false);
                gADbg.acc_printf(LEVEL_INFO, "SetToken[%s] session id[%d]", AuthenToken.c_str(), id.c_str());

                r[constants::Common::USER_NAME] = username;
                r[constants::Common::ID] = id;

                const std::string odata_path = "/redfish/v1/SessionService/Sessions/" + id;
                r[constants::Common::ODATA_ID] = odata_path;

                /*Set Authen Token in header */
                static const char *loginuri = "/redfish/v1/SessionService/Sessions";
#define XAUTH_NAME "X-Auth-Token"
#define LOC_ID "Location"
                char cxauthstr[256];
                char cxlocation[256];

                    gADbg.acc_printf(LEVEL_INFO, "SetToken[%s] session id[%s]", AuthenToken.c_str(), id.c_str());
                snprintf(cxauthstr, sizeof(cxauthstr), "%s", AuthenToken.c_str());

                const std::string Xauth = XAUTH_NAME;
                response.set_header(Xauth, cxauthstr);
                snprintf(cxlocation, sizeof(cxlocation), "%s/%s", loginuri, id.c_str());
                const std::string XLOCID = LOC_ID;
                response.set_header(XLOCID, cxlocation);
                response.set_status(server::status_2XX::CREATED);

                set_response(response, r);
            }
            else
            {
                if (res > Accountservice::get_instance()->get_aflt())
                    gADbg.acc_printf(LEVEL_WARN, "User[%s] over AuthFailureLoggingThreshold login times", username.c_str());

                response.set_status(server::status_4XX::UNAUTHORIZED);
                return;
            }
        }
        else
        {
            gADbg.acc_printf(LEVEL_WARN, "Username,Password not legal length");
            response.set_status(server::status_4XX::UNAUTHORIZED);
            return;
        }
    }
    return;
}

} // namespace endpoint
} // namespace rest
} // namespace psme
