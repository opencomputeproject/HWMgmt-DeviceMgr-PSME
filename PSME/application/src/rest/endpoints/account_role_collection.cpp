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
#include "psme/rest/server/error/error_factory.hpp"

#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/account_roles_collection.hpp"
#include "psme/rest/account/config/role_config.hpp"


#include "psme/rest/validators/json_validator.hpp"




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

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#RoleCollection.RoleCollection";	//For pass DMTF conference check //
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#RoleCollection.RoleCollection";
    r[Common::NAME] = "Roles Collection";
    r[Collection::ODATA_COUNT] = json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;

    return r;
}

#if 0
Accountrole to_model(const json::Value& json) {
    Accountrole s;
    
    const auto& name = json[Common::NAME].as_string();
    const auto& destination = json[EventSubscription::DESTINATION].as_string();
    const auto& context = json[EventSubscription::CONTEXT].as_string();
    const auto& protocol = json[EventSubscription::PROTOCOL].as_string();
    EventTypes event_types;
    for (const auto& event_type : json[EventSubscription::EVENT_TYPES]) {
        event_types.add(EventType::from_string(event_type.as_string()));
    }
    s.set_name(name);
    s.set_destination(destination);
    s.set_context(context);
    s.set_protocol(protocol);
    s.set_event_types(event_types);

    return s;
}
#endif
}


AccountRoleCollection::AccountRoleCollection(const std::string& path) : EndpointBase(path) {}


AccountRoleCollection::~AccountRoleCollection() {}


void AccountRoleCollection::get(const server::Request& req, server::Response& res) {
    auto r = ::make_prototype();
    r[Common::ODATA_ID] = PathBuilder(req).build();
    r[Collection::ODATA_COUNT] = AccountManager::get_instance()->Role_size();
    for (const auto& item : AccountManager::get_instance()->getRole()) {
        json::Value link_elem(json::Value::Type::OBJECT);
        const auto& role = item.second;
        link_elem[Common::ODATA_ID] = PathBuilder(req).append(role.get_name()).build();
        r[Collection::MEMBERS].push_back(std::move(link_elem));
    }
       
    set_response(res, r);
}


void AccountRoleCollection::post(const server::Request& request, server::Response& response) {
    //Dummy 
    PathBuilder(request).build();
#if 0	
    const auto& json = JsonValidator::validate_request_body<schema::SubscriptionCollectionPostSchema>(request);
    Subscription subscription = to_model(json);
    uint64_t id = AccountManager::get_instance()->add(subscription);
    SubscriptionConfig::get_instance()->save();
    endpoint::utils::set_location_header(response, PathBuilder(request).append(id).build());
#endif
    response.set_status(server::status_2XX::CREATED);
}
