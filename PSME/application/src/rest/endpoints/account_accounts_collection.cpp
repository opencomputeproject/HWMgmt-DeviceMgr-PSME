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

#include "psme/rest/server/request.hpp"

#include "psme/rest/server/error/server_exception.hpp"
#include "psme/rest/server/error/error_factory.hpp"

#include "psme/rest/account/config/account_config.hpp"

#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/account_accounts_collection.hpp"


#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/account_collection.hpp"


using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::endpoint;
using namespace psme::rest::account::config;
using namespace psme::rest::account::manager;
using namespace psme::rest::account::model;

using namespace psme::rest::validators;

namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#AccountService.AccountService";  //For pass DMTF conference check //
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#ManagerAccountCollection.ManagerAccountCollection";
    r[Common::NAME] = "Accounts Collection";
    r[Collection::ODATA_COUNT] = json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;

    return r;
}


Account to_model(const json::Value& json) {
    Account s;    
    const auto& name = json[Common::NAME].as_string();
    const auto& enabled = json[AccountConst::ENABLED].as_bool();
    const auto& locked = json[AccountConst::LOCKED].as_bool();
    const auto& username = json[AccountConst::USERNAME].as_string();
    const auto& password = json[AccountConst::PASSWORD].as_string();
    const auto& roleid = json[AccountConst::ROLEID].as_string();
               

    s.set_name(name);
    s.set_username(username);
    s.set_password(password,true);
    s.set_roleid(roleid);
    s.set_enabled(enabled);
    s.set_locked(locked);
    
    return s;
}



}


AccountCollection::AccountCollection(const std::string& path) : EndpointBase(path) {}


AccountCollection::~AccountCollection() {}


void AccountCollection::get(const server::Request& req, server::Response& res) {
    auto r = ::make_prototype();
    r[Common::ODATA_ID] = PathBuilder(req).build();
    r[Collection::ODATA_COUNT] = AccountManager::get_instance()->Account_size();

    for (const auto& item : AccountManager::get_instance()->getAccount()) {
        json::Value link_elem(json::Value::Type::OBJECT);
        const auto& account = item.second;
        link_elem[Common::ODATA_ID] = PathBuilder(req).append(account.get_username()).build();
        r[Collection::MEMBERS].push_back(std::move(link_elem));
    }
    
    set_response(res, r);
}


void AccountCollection::post(const server::Request& request, server::Response& response) {

    using namespace psme::rest::error;
    const auto& json = JsonValidator::validate_request_body<schema::AccountCollectionPostSchema>(request);
    Account account = to_model(json);
    
    try {
       const auto& role=AccountManager::get_instance()->getRole(json[AccountConst::ROLEID].as_string());    	
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
    
    
    
    uint64_t id = AccountManager::get_instance()->addAccount(account);
    AccountConfig::get_instance()->saveAccounts();
    endpoint::utils::set_location_header(response, PathBuilder(request).append(id).build());
   
    response.set_status(server::status_2XX::CREATED);
}
