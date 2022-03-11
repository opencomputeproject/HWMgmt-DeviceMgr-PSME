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

#include "psme/rest/endpoints/account.hpp"
#include "psme/rest/endpoints/utils.hpp"
#include "psme/rest/account/config/account_config.hpp"
#include "psme/rest/validators/schemas/account.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/session/manager/session_manager.hpp"
#include "psme/rest/utils/time_utils.hpp"
#include "psme/rest/utils/ec_common_utils.hpp"
#include <sys/time.h>


#include "psme/rest/constants/constants.hpp"
#include "json/json.hpp"

#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/server/error/server_exception.hpp"

#include "psme/rest/server/parameters.hpp"

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::server;
using namespace psme::rest::account::manager;
using namespace psme::rest::account::config;
using namespace psme::rest::session::manager;

using namespace psme::rest::endpoint::utils;
using namespace psme::rest::validators;
using namespace psme::rest::utils;



namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#ManagerAccount.ManagerAccount";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#ManagerAccount.v1_0_0.ManagerAccount";
    r[Common::ID] = json::Value::Type::NIL;
    r[Common::NAME] = json::Value::Type::NIL;
    r[Common::DESCRIPTION] = json::Value::Type::NIL;
    r[AccountConst::USERNAME] = json::Value::Type::NIL;
    r[AccountConst::PASSWORD] = json::Value::Type::NIL;
    r[AccountConst::LOCKED] = json::Value::Type::NIL;
    r[AccountConst::ENABLED] = json::Value::Type::NIL;
    r[AccountConst::ROLEID] = json::Value::Type::NIL;
    r[Common::LINKS][AccountConst::ROLE] = json::Value::Type::NIL;
    

    return r;
}

void to_json(const Account& account, json::Value& json) {
    json[Common::NAME] = "User Account";

    if (account.get_enabled() == false)
        json[AccountConst::USERNAME] = "";
    else
    json[AccountConst::USERNAME] = account.get_username();

    json[Common::DESCRIPTION] = "User Account"; 
    //json[AccountConst::PASSWORD] = account.get_password();
    json[AccountConst::LOCKED] = account.get_locked();
    json[AccountConst::ROLEID] = account.get_roleid();
    json[AccountConst::ENABLED] = account.get_enabled();
}
    
}

endpoint::Account::Account(const std::string& path) : EndpointBase(path) {}

endpoint::Account::~Account() {}

void endpoint::Account::get(const server::Request &request, server::Response &response)
{
    auto r = make_prototype();
    r[Common::ODATA_ID] = request.get_url();
    r[Common::ID] = request.params[PathParam::ACCOUNT_ID];
    if (!AccountManager::get_instance()->checkAccount(request.params[PathParam::ACCOUNT_ID]))
    {
        json::Value br(json::Value::Type::OBJECT);
        json::Value body = request.get_body();
        br["error"]["@Message.ExtendedInfo"].push_back(body);
        br["error"]["@Message.ExtendedInfo"]["Message"] = "Get Error!";
        set_response(response, br);
        response.set_status(server::status_4XX::BAD_REQUEST);
        return;
    }
    const auto& account = AccountManager::get_instance()->getAccount(request.params[PathParam::ACCOUNT_ID]);
    const auto& role = AccountManager::get_instance()->getRole(account.get_roleid());
    to_json(account, r);
    json::Value link;
    link[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL)
                    .append(constants::Root::ACCOUNT_SERVICE)
                    .append(constants::AccountService::ROLES)
                                 .append(account.get_roleid())
                                 .build();
    r[Common::LINKS][AccountConst::ROLE] = std::move(link);    
   
    log_info(GET_LOGGER("rest"), "GET account ::" << account.get_name() << "etag value" << account.get_etag());
    response.set_header("ETag", account.get_etag());
    set_response(response, r);
}

void endpoint::Account::del(const server::Request& request, server::Response& response)
{
    json::Value r(json::Value::Type::OBJECT) ;

    if (!AccountManager::get_instance()->checkAccount(request.params[PathParam::ACCOUNT_ID]))
    {
        json::Value br(json::Value::Type::OBJECT);
        json::Value body = request.get_body();
        br["error"]["@Message.ExtendedInfo"].push_back(body);
        br["error"]["@Message.ExtendedInfo"]["Message"] = "Del Error!";
        set_response(response, br);
        response.set_status(server::status_4XX::BAD_REQUEST);
        return;
    }

    std::string token = request.get_header("xAuthGen");
    if (token.size() != 0)
    {
        Session new_session = SessionManager::get_instance()->getSession_by_Token(token);

        if (new_session.get_userrole() == "Administrator")
        {
        AccountManager::get_instance()->delAccount(request.params[PathParam::ACCOUNT_ID]);
        AccountConfig::get_instance()->saveAccounts();
            set_response(response, r);
        response.set_status(server::status_2XX::OK);
    }
        else
        {
            response.set_status(server::status_4XX::UNAUTHORIZED);
        }
    }
    else
    {
        AccountManager::get_instance()->delAccount(request.params[PathParam::ACCOUNT_ID]);
        AccountConfig::get_instance()->saveAccounts();
        set_response(response, r);
        response.set_status(server::status_2XX::OK);
    }
}

void endpoint::Account::patch(const server::Request &request, server::Response &response)
{
    using namespace psme::rest::error;
    using namespace psme::rest::account::model;
    bool authorized = false;
    bool include_non_validator_string = false;
    std::chrono::steady_clock::time_point m_timestamp{std::chrono::steady_clock::now()};
    
    std::vector<std::string> validators_string =
        {
            AccountConst::USERNAME,
            AccountConst::PASSWORD,
            Common::NAME,
            AccountConst::ENABLED,
            AccountConst::LOCKED,
            AccountConst::ROLEID};

    json::Value json;
    
    json::Deserializer deserializer(request.get_body());

    if (deserializer.is_invalid())
                {
        log_info(GET_LOGGER("rest"), "invalid json format");
        response.set_status(server::status_4XX::BAD_REQUEST);
        return;
            }

    deserializer >> json;

    // Non exist account //
    if (!AccountManager::get_instance()->checkAccount(request.params[PathParam::ACCOUNT_ID]))
        {
        json::Value br(json::Value::Type::OBJECT);
        json::Value message(json::Value::Type::OBJECT);
        message["Message"] = json;
        message["MessageId"] = "Base.1.4.0.NoOperation";
        br["error"]["@Message.ExtendedInfo"].push_back(message);
        br["error"]["message"] = "A general erro occur causing by PATCH, non exist account name";
        br["error"]["code"] = "Base.1.4.0.NoOperation";

        set_response(response, br);
        response.set_status(server::status_4XX::BAD_REQUEST);
        return;
        }

    account::model::Account account = AccountManager::get_instance()->getAccount(request.params[PathParam::ACCOUNT_ID]);
       
    std::string If_Match_etag = request.get_header("If-Match");
    log_info(GET_LOGGER("rest"), "account etag::" << account.get_etag());
    log_info(GET_LOGGER("rest"), "If-Match::" << If_Match_etag);

    const auto old_username = account.get_username();
    std::string token = request.get_header("xAuthGen");
    bool is_admin = false;
    bool is_set = false;
    bool is_authen_service_enable = SessionManager::get_instance()->GetSessionConfigEnable();
    //If authen is enabled, there should be a token here, otherwise, the authen is disable.
    if (!is_authen_service_enable)
    {
        //Authen disabled 
        authorized = true;
        }
        else
        {
        //Authen enabled 
        // Only Admin and him/herself can modify
        Session new_session = SessionManager::get_instance()->getSession_by_Token(token);
        if (!is_authen_service_enable || new_session.get_userrole() == "Administrator" || (new_session.get_username() == old_username))
            authorized = true;

        if (new_session.get_userrole() == "Administrator")
            is_admin = true;
        }

    if (authorized == true)
        {
        // Is requst include validator string ? //
        for (auto it = json.cbegin(); it != json.cend(); ++it)
        {
            std::string s_it = it.key();
            if (std::find(validators_string.begin(), validators_string.end(), s_it) != validators_string.end())
            {
                if (json.is_member(AccountConst::PASSWORD) && AccountConst::PASSWORD == s_it)
        {
            const auto &password = json[AccountConst::PASSWORD].as_string();
            account.set_password(password, true);
                    log_info(GET_LOGGER("rest"), "Set Password");
                    is_set = true;
        }

                if (json.is_member(Common::NAME) && Common::NAME == s_it)
        {
            const auto &name = json[Common::NAME].as_string();
            account.set_name(name);
                    log_info(GET_LOGGER("rest"), "Set Name");
                    is_set = true;
        }

                // Following items only Admin or non authen mode can modify //
    
                if ((json.is_member(AccountConst::ENABLED) && ((is_admin && is_authen_service_enable) || !is_authen_service_enable)) && AccountConst::ENABLED == s_it)
        {
            const auto &enabled = json[AccountConst::ENABLED].as_bool();
            account.set_enabled(enabled);
                    log_info(GET_LOGGER("rest"), "Set Enabled");
                    is_set = true;
        }

                if ((json.is_member(AccountConst::LOCKED) && ((is_admin && is_authen_service_enable) || !is_authen_service_enable)) && AccountConst::LOCKED == s_it)
        {
            const auto &locked = json[AccountConst::LOCKED].as_bool();
            account.set_locked(locked);
                    log_info(GET_LOGGER("rest"), "Set Locked");
                    is_set = true;
        }

                if ((json.is_member(AccountConst::USERNAME) && ((is_admin && is_authen_service_enable) || !is_authen_service_enable)) && AccountConst::USERNAME == s_it)
        {
            const std::string &new_username = json[AccountConst::USERNAME].as_string();
            if (new_username.compare(old_username) != 0)
            {
                        log_info(GET_LOGGER("rest"), "Patch new name " << new_username);
                const auto &Existed = AccountManager::get_instance()->AccountExisted(new_username);

                if (Existed == true)
                            log_info(GET_LOGGER("rest"), "Patch name already exist " << new_username);
        else
        {
            account.set_username(new_username);
                            is_set = true;
                            log_info(GET_LOGGER("rest"), "Set UserName " << new_username);
                        }
        }
        }

                if ((json.is_member(AccountConst::ROLEID) && ((is_admin && is_authen_service_enable) || !is_authen_service_enable)) && AccountConst::ROLEID == s_it)
        {
            try
        {
                const auto &role = AccountManager::get_instance()->getRole(json[AccountConst::ROLEID].as_string());
            }
            catch (const agent_framework::exceptions::NotFound &ex)
            {
                        response.set_status(server::status_4XX::BAD_REQUEST);
                return;
            }
            const auto &roleid = json[AccountConst::ROLEID].as_string();
            account.set_roleid(roleid);
                    is_set = true;
                    log_info(GET_LOGGER("rest"), "Set Role");
                }
        }
        else
        {
                include_non_validator_string = true;
        }
        }

        if (include_non_validator_string == true && is_set == false)
        {
            // Modify a single property that can never be updated. For example, a property that is read-only, unknown, or unsupported.
            json::Value br(json::Value::Type::OBJECT);
            json::Value message(json::Value::Type::OBJECT);
            message["Message"] = json;
            message["MessageId"] = "Base.1.4.0.NoOperation";
            br["error"]["@Message.ExtendedInfo"].push_back(message);
            br["error"]["message"] = "A general erro occur causing by PATCH";
            br["error"]["code"] = "Base.1.4.0.NoOperation";
            set_response(response, br);
    
            if (json.size() == 1)
            {
                // {"aabb" = "x"}
                response.set_status(server::status_4XX::BAD_REQUEST);
            }
            else
            {
                // {"aabb" = "x", "ccdd" = "y"}
                response.set_status(server::status_4XX::METHOD_NOT_ALLOWED);
            }
            return;
        }
        else if (is_set == true)
        {
            if (If_Match_etag != "" && If_Match_etag != account.get_etag())
            {
                response.set_status(server::status_4XX::PRECONDITION_FAILED);
                return;
            }
            else
            {
                char command[BUFFER_LEN] = {0};
                std::string random_string;
                sprintf(command, "%s", "openssl rand -hex 4");
                EcCommonUtils::exec_shell_(command, random_string, 1);
                std::string time_s = "W/\"" + TimeUtils::get_time_with_zone(m_timestamp) + random_string.erase(random_string.size() - 1) + '\"';
                // Modify several properties where one or more properties can never be updated. For example, when a property is read-only, unknown, or unsupported.
                // {"aabb" = "x", "UserName" = "ValidName"}
                json::Value br(json::Value::Type::OBJECT);
                json::Value message(json::Value::Type::OBJECT);
                message["Message"] = json;
                message["MessageId"] = "Base.1.4.0.NoOperation";
                br["@odata.id"] = "";
                br["@Message.ExtendedInfo"] = "";
                set_response(response, br);
                account.set_etag(time_s);
                log_info(GET_LOGGER("rest"), "new etag::" << time_s);
        AccountManager::get_instance()->modAccount(old_username, account); //patch is equal to modify account
        AccountConfig::get_instance()->saveAccounts();
                response.set_header("ETag", time_s);
        response.set_status(server::status_2XX::OK);
    }
        }
    else
    {
            json::Value br(json::Value::Type::OBJECT);
            json::Value message(json::Value::Type::OBJECT);
            message["Message"] = json;
            message["MessageId"] = "Base.1.4.0.NoOperation";
            br["error"]["@Message.ExtendedInfo"].push_back(message);
            br["error"]["message"] = "A general erro occur causing by PATCH";
            br["error"]["code"] = "Base.1.4.0.NoOperation";
            set_response(response, br);
            response.set_status(server::status_4XX::BAD_REQUEST);
            return;
    }
}
    else
        response.set_status(server::status_4XX::BAD_REQUEST);
}