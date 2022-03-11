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
#include "psme/rest/utils/time_utils.hpp"
#include <sys/time.h>


using namespace psme::rest::utils;
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

Account to_model(const json::Value &json)
{
    Account s;    
    if (json.is_member(Common::NAME))
    {
    const auto& name = json[Common::NAME].as_string();
        s.set_name(name);
    }

    if(json.is_member(AccountConst::ENABLED))
    {
    const auto& enabled = json[AccountConst::ENABLED].as_bool();
        s.set_enabled(enabled);
    }
               
    if(json.is_member(AccountConst::LOCKED))
    {
        const auto &locked = json[AccountConst::LOCKED].as_bool();
        s.set_locked(locked);
    }

    if(json.is_member(AccountConst::USERNAME))
    {
        const auto &username = json[AccountConst::USERNAME].as_string();
    s.set_username(username);
    }

    if(json.is_member(AccountConst::PASSWORD))
    {
        const auto &password = json[AccountConst::PASSWORD].as_string();
    s.set_password(password,true);
    }
    
    if(json.is_member(AccountConst::ROLEID))
    {
        const auto &roleid = json[AccountConst::ROLEID].as_string();
        s.set_roleid(roleid);
    }
    return s;
}
}


AccountCollection::AccountCollection(const std::string& path) : EndpointBase(path) {}


AccountCollection::~AccountCollection() {}


void AccountCollection::get(const server::Request& req, server::Response& res) {
    std::chrono::steady_clock::time_point m_timestamp{std::chrono::steady_clock::now()};
    auto r = ::make_prototype();
    r[Common::ODATA_ID] = PathBuilder(req).build();
    r[Collection::ODATA_COUNT] = AccountManager::get_instance()->Account_size();

    for (const auto& item : AccountManager::get_instance()->getAccount()) {
        json::Value link_elem(json::Value::Type::OBJECT);
        const auto& account = item.second;
        link_elem[Common::ODATA_ID] = PathBuilder(req).append(account.get_username()).build();
        r[Collection::MEMBERS].push_back(std::move(link_elem));
    }
    
    std::string time_s = "W/\"" + TimeUtils::get_time_with_zone(m_timestamp) + '\"';     
    res.set_header("ETag", time_s);    
    res.set_header("Link", "<http://redfish.dmtf.org/schemas/ManagerAccountCollection.json>;rel=\"describedby\""); 
    set_response(res, r);
}

void AccountCollection::post(const server::Request &request, server::Response &response)
{
    using namespace psme::rest::error;
    std::chrono::steady_clock::time_point m_timestamp{std::chrono::steady_clock::now()};
    std::string time_s = "W/\"" + TimeUtils::get_time_with_zone(m_timestamp) + '\"';
    const auto& json = JsonValidator::validate_request_body<schema::AccountCollectionPostSchema>(request);
    json::Value r(json::Value::Type::OBJECT);
    
    if (!(json.is_member(AccountConst::ROLEID)) || !json.is_member(AccountConst::USERNAME) || !json.is_member(AccountConst::PASSWORD))
    {
        log_error(GET_LOGGER("rest"), "POST lack of mandotary field");
        response.set_header("Allow", "GET HEAD");
        response.set_status(server::status_4XX::METHOD_NOT_ALLOWED);
        return;
    }    
    
    Account account = to_model(json);
    account.set_etag(time_s);
    AccountManager::get_instance()->addAccount(account);
    AccountConfig::get_instance()->saveAccounts();
    endpoint::utils::set_location_header(response, PathBuilder(request).append(json[AccountConst::USERNAME].as_string()).build());
   
    set_response(response, r);
    response.set_header("ETag", time_s);
    response.set_status(server::status_2XX::CREATED);
}
