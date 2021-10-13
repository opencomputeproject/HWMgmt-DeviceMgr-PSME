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

#include "psme/rest/endpoints/role.hpp"
#include "psme/rest/endpoints/utils.hpp"
#include "psme/rest/account/config/role_config.hpp"
//#include "psme/rest/validators/schemas/role.hpp"
#include "psme/rest/validators/json_validator.hpp"


#include "psme/rest/constants/constants.hpp"
#include "json/json.hpp"

#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/server/error/server_exception.hpp"

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

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#Role.Role";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#Role.v1_0_0.Role";
    r[Common::ID] = json::Value::Type::NIL;
    r[Common::NAME] = json::Value::Type::NIL;
    r[Common::DESCRIPTION] = json::Value::Type::NIL;
//    r[AccountRole::ROLEID] = json::Value::Type::NIL;  //To pass DMTF Validator
    r[AccountRole::ISPREDEFINED] = json::Value::Type::NIL;
    r[AccountRole::PRIVILEGE_TYPES] = json::Value::Type::NIL;

    return r;
}

void to_json(const Role& role, json::Value& json) {
    json[Common::NAME] = role.get_name();
//    json[AccountRole::ROLEID] = role.get_roleid();  //To pass DMTF Validator
    json[AccountRole::ISPREDEFINED] = role.get_ispredefined();
    json::Value priv_types_json(json::Value::Type::ARRAY);

    for (const auto& priv_type : role.get_privilege_types().get()){
        priv_types_json.push_back(priv_type.to_string());
    }
    
    
    json[AccountRole::PRIVILEGE_TYPES] = priv_types_json;

}
}

endpoint::Role::Role(const std::string& path) : EndpointBase(path) {}

endpoint::Role::~Role() {}

void endpoint::Role::get(const server::Request& request, server::Response& response) {
    auto r = make_prototype();
    r[Common::ODATA_ID] = request.get_url();
    r[Common::ID] = request.params[PathParam::ROLE_ID];
    const auto& role = AccountManager::get_instance()->getRole(request.params[PathParam::ROLE_ID]);
    to_json(role, r);
    set_response(response, r);
}

void endpoint::Role::del(const server::Request& request, server::Response& response) {
    const auto& id = id_to_uint64(request.params[PathParam::ROLE_ID]);
    AccountManager::get_instance()->delRole(id);
    response.set_status(server::status_2XX::NO_CONTENT);
}

void endpoint::Role::patch(const server::Request &request, server::Response &response)
{
#if 0        
    using namespace psme::rest::account::model;
    const auto& id = id_to_uint64(request.params[PathParam::ROLE_ID]);
   
    account::model::Account account = AccountManager::get_instance()->getAccount(id);    
    const auto json = JsonValidator::validate_request_body<schema::AccountCollectionPostSchema>(request);
    
    const auto& old_username = account.get_username();
    const auto& enabled = json[AccountConst::ENABLED].as_bool();
    const auto& locked = json[AccountConst::LOCKED].as_bool();
    std::string new_username = json[AccountConst::USERNAME].as_string();
    const auto& password = json[AccountConst::PASSWORD].as_string();
    const auto& roleid = json[AccountConst::ROLEID].as_string();

    account.set_username(new_username);
    account.set_password(password, ? );
    account.set_roleid(roleid);
    account.set_enabled(enabled);
    account.set_locked(locked);   
    
    AccountManager::get_instance()->modAccount(old_username,account); 
    AccountConfig::get_instance()->saveAccounts();
#endif
    (void) (request);
    response.set_status(server::status_4XX::METHOD_NOT_ALLOWED);
    return;
}