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

using namespace psme::rest::endpoint::utils;
using namespace psme::rest::validators;



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
    json[Common::NAME] = account.get_name();
    json[AccountConst::USERNAME] = account.get_username();
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
#if 0    
    const auto& id = id_to_uint64(request.params[PathParam::ACCOUNT_ID]);
    const auto& account = AccountManager::get_instance()->getAccount(id);
#else
    const auto& account = AccountManager::get_instance()->getAccount(request.params[PathParam::ACCOUNT_ID]);
#endif    
    const auto& role = AccountManager::get_instance()->getRole(account.get_roleid());
    to_json(account, r);
        
    json::Value link;
    link[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL)
                    .append(constants::Root::ACCOUNT_SERVICE)
                    .append(constants::AccountService::ROLES)
                                 .append(role.get_id())
                                 .build();
    r[Common::LINKS][AccountConst::ROLE] = std::move(link);    
   
    set_response(response, r);
}

void endpoint::Account::del(const server::Request& request, server::Response& response) {
#if 0    
    const auto& id = id_to_uint64(request.params[PathParam::ACCOUNT_ID]);
    AccountManager::get_instance()->delAccount(id);
#else
        AccountManager::get_instance()->delAccount(request.params[PathParam::ACCOUNT_ID]);
#endif
        AccountConfig::get_instance()->saveAccounts();
        response.set_status(server::status_2XX::OK);
    }



void endpoint::Account::patch(const server::Request& request, server::Response& response) {
	
    using namespace psme::rest::error;
    using namespace psme::rest::account::model;
    
    std::cout << " patch req account id is " << request.params[PathParam::ACCOUNT_ID] << std::endl;

#if 0 
    const auto& id = id_to_uint64(request.params[PathParam::ACCOUNT_ID]);
    account::model::Account account = AccountManager::get_instance()->getAccount(id);  
#else
    account::model::Account account = AccountManager::get_instance()->getAccount(request.params[PathParam::ACCOUNT_ID]);
#endif      
    const auto json = JsonValidator::validate_request_body<schema::AccountPatchSchema>(request);
    
    const auto old_username = account.get_username();
    std::string n_username{};

        if (json.is_member(AccountConst::USERNAME))
        {
            const std::string &new_username = json[AccountConst::USERNAME].as_string();
            if (new_username.compare(old_username) != 0)
            {
                std::cout << " patch req old username " << old_username << " is not same as new username " << new_username << std::endl;
                //new_username account can not exist
                const auto &Existed = AccountManager::get_instance()->AccountExisted(new_username);
                if (Existed == true)
                {
                    throw agent_framework::exceptions::InvalidValue("new UserName already Existed");
                }
            }

            account.set_username(new_username);
            std::cout << " patch req old username " << old_username << " new username " << new_username << std::endl;
            n_username = new_username;

        }
        else
        {
            n_username = old_username;
            //const auto old_enabled = account.get_enabled();
            std::cout << " patch req keep USERNAME as old " << old_username << std::endl;
        }

        if (json.is_member(AccountConst::PASSWORD))
        {
            const auto &password = json[AccountConst::PASSWORD].as_string();
            account.set_password(password, true);
       

        }
        else
        {
            //const auto old_enabled = account.get_enabled();
            std::cout << " patch req keep PASSWORD as old " << std::endl;
        }


        if (json.is_member(Common::NAME))
        {
            const auto &name = json[Common::NAME].as_string();
            account.set_name(name);
        }
        else
        {
            //const auto old_enabled = account.get_enabled();
            std::cout << " patch req keep name as old " << std::endl;
        }

    
        if (json.is_member(AccountConst::ENABLED))
        {
            const auto &enabled = json[AccountConst::ENABLED].as_bool();
            account.set_enabled(enabled);
        }
        else
        {
            //const auto old_enabled = account.get_enabled();
            std::cout << " patch req keep ENABLED as old " << std::endl;
        }

        if (json.is_member(AccountConst::LOCKED))
        {
            const auto &locked = json[AccountConst::LOCKED].as_bool();
            account.set_locked(locked);
        }
        else
        {
            //const auto old_locked = account.get_locked();
            std::cout << " patch req keep LOCKED as old " << std::endl;
        }

        if (json.is_member(AccountConst::ROLEID))
        {
       try {
                const auto &role = AccountManager::get_instance()->getRole(json[AccountConst::ROLEID].as_string());
            }
       catch (const agent_framework::exceptions::NotFound& ex) {
                log_error(GET_LOGGER("rest"), "Not found exception: " << ex.what());
                ServerError server_error = ErrorFactory::create_error_from_gami_exception(
               agent_framework::exceptions::NotFound(ex.get_message(), request.get_url())
           );
                response.set_status(server_error.get_http_status_code());
           response.set_body(server_error.as_string());;
                return;
            }
            const auto &roleid = json[AccountConst::ROLEID].as_string();
            account.set_roleid(roleid);
        }
        else
        {
            std::cout << " patch req keep ROLEID as old " << std::endl;
        }

    
        AccountManager::get_instance()->modAccount(old_username, account); //patch is equal to modify account
        AccountConfig::get_instance()->saveAccounts();

        const std::string &key = PathParam::ACCOUNT_ID;

        server::Request get_request{request};
        get_request.params.set(key, n_username);
        std::cout << "change account_id to " << get_request.params[PathParam::ACCOUNT_ID] << std::endl;
        response.set_status(server::status_2XX::OK);
    
    
}