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

#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/test_event_subscription.hpp"
#include "psme/rest/eventing/manager/subscription_manager.hpp"
#include "eclog_helper/eclog_helper.hpp"
using namespace eclog_helper;
using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::endpoint;
using namespace psme::rest::eventing::manager;
using namespace psme::rest::eventing::model;



namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#TestEventDestinationCollection.TestEventDestinationCollection";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#TestEventDestinationCollection.TestEventDestinationCollection";
    r[Common::NAME] = "Test Event Subscription";
    r[Common::DESCRIPTION] = "Test Event Subscription description";
    r[Collection::ODATA_COUNT] = 0;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;

    return r;
}
}

TestEventSubscription::TestEventSubscription(const std::string& path) : EndpointBase(path) {}

TestEventSubscription::~TestEventSubscription() {}

void TestEventSubscription::get(const server::Request& req, server::Response& res) {
    auto r = ::make_prototype();
    r[Common::ODATA_ID] = PathBuilder(req).build();
	
    Event event4(EventType::Alert, "////TEST EVENT SUB ////");
    event4.set_message_id("ResourceEvent.1.0.3.ResourceWarningThresholdExceeded");
    event4.set_message("The resource property /redfish/v1/EthernetSwitches/1/Ports/1 has exceeded its warning threshold of value 100");
    event4.set_origin_of_condition("/redfish/v1/EthernetSwitches/1/Ports/1");
    Json::Value oem;
    oem["CurrentValue"] = 778;
    oem["State"] = "TxErrorRateExceededLowerThresholdNonCritical";
    oem["ThresholdValue"] = 100;
    event4.set_oem(oem);
    event4.set_severity("Warning");
    event4.set_context("Alert notification message");

    SubscriptionManager::get_instance()->notify(event4);
	
    auto &Entry = RFLogEntry::get_instance();
    Entry.get_current_time();
    Entry.get_zone_offset();
    std::string event_l("Event");
    std::string servrity("Warning");
    std::string sensor_type("Temperature");
    std::string message;
    message = std::string("This log message sent from test post!!!");
    Entry.set_log_entry(event_l, sensor_type, servrity, message, 1);
	
    set_response(res, r);
}
