/*!
 * @copyright
 * Copyright (c) 2015-2017 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * */

#include "psme/rest/endpoints/event_subscription.hpp"
#include "psme/rest/endpoints/utils.hpp"
#include "psme/rest/eventing/manager/subscription_manager.hpp"
#include "psme/rest/eventing/config/subscription_config.hpp"
#include "psme/rest/constants/constants.hpp"
#include "json/json.hpp"

#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/subscription.hpp"

#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/server/error/server_exception.hpp"

using namespace psme::rest;
using namespace psme::rest::validators;
using namespace psme::rest::constants;
using namespace psme::rest::server;
using namespace psme::rest::eventing::manager;
using namespace psme::rest::eventing::config;
using namespace psme::rest::endpoint::utils;

namespace
{
json::Value make_prototype()
{
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#EventDestination.EventDestination";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#EventDestination.v1_0_0.EventDestination";
    r[Common::ID] = json::Value::Type::NIL;
    r[Common::NAME] = json::Value::Type::NIL;
    r[Common::DESCRIPTION] = json::Value::Type::NIL;
    r[EventSubscription::DESTINATION] = json::Value::Type::NIL;
    r[EventSubscription::CONTEXT] = json::Value::Type::NIL;
    r[EventSubscription::PROTOCOL] = json::Value::Type::NIL;
    r[EventSubscription::EVENT_TYPES] = json::Value::Type::ARRAY;
    return r;
}

void to_json(const Subscription &subscription, json::Value &json)
{
    json[Common::NAME] = subscription.get_name();
    json[EventSubscription::DESTINATION] = subscription.get_destination();
    json[EventSubscription::CONTEXT] = subscription.get_context();
    json[EventSubscription::PROTOCOL] = subscription.get_protocol();
    json::Value event_types_json(json::Value::Type::ARRAY);
    for (const auto &event_type : subscription.get_event_types().get())
    {
        event_types_json.push_back(event_type.to_string());
    }
    json[EventSubscription::EVENT_TYPES] = event_types_json;
}

Subscription to_model(const json::Value &json)
{
    Subscription s;
    const auto &name = json[Common::NAME].as_string();
    const auto &destination = json[EventSubscription::DESTINATION].as_string();
    const auto &context = json[EventSubscription::CONTEXT].as_string();
    const auto &protocol = json[EventSubscription::PROTOCOL].as_string();
    EventTypes event_types;
    for (const auto &event_type : json[EventSubscription::EVENT_TYPES])
    {
        event_types.add(EventType::from_string(event_type.as_string()));
}
    s.set_id(0); //0 means add from subscribe //!0 get from file
    s.set_name(name);
    s.set_destination(destination);
    s.set_context(context);
    s.set_protocol(protocol);
    s.set_event_types(event_types);
    return s;
}
} // namespace

endpoint::Subscription::Subscription(const std::string& path) : EndpointBase(path) {}

endpoint::Subscription::~Subscription() {}

void endpoint::Subscription::get(const server::Request& request, server::Response& response) {
    auto r = make_prototype();
    r[Common::ODATA_ID] = request.get_url();
    r[Common::ID] = request.params[PathParam::SUBSCRIPTION_ID];
    const auto& id = id_to_uint64(request.params[PathParam::SUBSCRIPTION_ID]);
    const auto& subscription = SubscriptionManager::get_instance()->get(id);
    to_json(subscription, r);
    set_response(response, r);
}

void endpoint::Subscription::del(const server::Request& request, server::Response& response) {
    const auto& id = id_to_uint64(request.params[PathParam::SUBSCRIPTION_ID]);
    SubscriptionManager::get_instance()->del(id);
    SubscriptionConfig::get_instance()->save();
	
    response.set_status(server::status_2XX::NO_CONTENT);
}

void endpoint::Subscription::patch(const server::Request& request, server::Response& response)
{
    bool mod = false;
    const auto &json = JsonValidator::validate_request_body<schema::SubscriptionPatchSchema>(request);

    auto r = make_prototype();
    const auto &id_ = id_to_uint64(request.params[PathParam::SUBSCRIPTION_ID]);
    const auto &subscription_ = SubscriptionManager::get_instance()->get(id_);
    to_json(subscription_, r);

    if (json.is_member(constants::Common::NAME))
    {
        if (r[constants::Common::NAME] != json[constants::Common::NAME])
        {
            r[constants::Common::NAME] = json[constants::Common::NAME];
            mod = true;
        }
    }

    if (json.is_member(constants::EventSubscription::DESTINATION))
    {
        if (r[constants::EventSubscription::DESTINATION] != json[constants::EventSubscription::DESTINATION])
        {
            r[constants::EventSubscription::DESTINATION] = json[constants::EventSubscription::DESTINATION];
            mod = true;
        }
    }

    if (json.is_member(constants::EventSubscription::CONTEXT))
    {
        if (r[constants::EventSubscription::CONTEXT] != json[constants::EventSubscription::CONTEXT])
        {
            r[constants::EventSubscription::CONTEXT] = json[constants::EventSubscription::CONTEXT];
            mod = true;
        }
    }

    if(mod == true)
    {
        SubscriptionManager::get_instance()->del(id_);

        const auto &subscription = to_model(r);
        uint64_t id = SubscriptionManager::get_instance()->add(subscription);
        if (id != 0)
        {
            SubscriptionConfig::get_instance()->save();
            response.set_status(server::status_2XX::OK);
        }
        else
        {
            response.set_status(server::status_4XX::METHOD_NOT_ALLOWED);
        }
    }
    else
    {
        response.set_status(server::status_2XX::OK);
    }
}