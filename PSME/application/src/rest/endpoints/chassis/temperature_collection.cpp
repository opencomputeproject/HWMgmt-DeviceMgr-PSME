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
#include "agent-framework/module/chassis_components.hpp" 
#include "psme/rest/endpoints/chassis/temperature_collection.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/utils.hpp"

using namespace psme::rest;
using namespace psme::rest::endpoint;
using namespace psme::rest::constants;
using agent_framework::module::ChassisComponents;


namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#TemperatureCollection.TemperatureCollection";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#TemperatureCollection.TemperatureCollection";
    r[Common::NAME] = "Temperature Collection";
    r[Common::DESCRIPTION] = "Collection of Temperature Sensors";
    r[Collection::ODATA_COUNT] = json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;

    return r;
}
}

TemperatureCollection::TemperatureCollection(const std::string& path) : EndpointBase(path) {}
TemperatureCollection::~TemperatureCollection() {}

void TemperatureCollection::get(const server::Request& req, server::Response& res) {
    auto json = ::make_prototype();

    json[Common::ODATA_ID] = PathBuilder(req).build();

    auto chassis_ids = ChassisComponents::get_instance()->get_thermal_zone_manager().get_ids();	

    json[Collection::ODATA_COUNT] = std::uint32_t(chassis_ids.size());


    for (const auto& id : chassis_ids) {
        json::Value link;
        link[Common::ODATA_ID] = PathBuilder(req).append(id).build();
        json[Collection::MEMBERS].push_back(std::move(link));
    }	

    set_response(res, json);
}
