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

#include "agent-framework/module/network_components.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/utils.hpp"
#include "psme/rest/endpoints/manager_serial_interface_collection.hpp"

using namespace psme::rest;
using namespace psme::rest::endpoint;
using namespace psme::rest::constants;

namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#SerialInterfaceCollection.SerialInterfaceCollection";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#SerialInterfaceCollection.SerialInterfaceCollection";
    r[Common::NAME] = "Serial Interface Collection";
    r[Common::DESCRIPTION] = "a Collection of Serial Interfaces";
    r[Collection::ODATA_COUNT] = json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;

    return r;
}
}

ManagerSerialInterfaceCollection::ManagerSerialInterfaceCollection(const std::string& path) : EndpointBase(path) {}

ManagerSerialInterfaceCollection::~ManagerSerialInterfaceCollection() {}

void ManagerSerialInterfaceCollection::get(const server::Request& req, server::Response& res) {
    auto json = ::make_prototype();
    auto manager_uuid = psme::rest::model::Find<agent_framework::model::Manager>(
        req.params[PathParam::MANAGER_ID]).get_uuid();
    auto switch_ids = agent_framework::module::NetworkComponents::get_instance()->get_switch_manager().get_ids(manager_uuid);

    json[Common::ODATA_ID] = PathBuilder(req).build();

    json[Collection::ODATA_COUNT] = 1;
    json::Value link_elem(json::Value::Type::OBJECT);
    link_elem[Common::ODATA_ID] = PathBuilder(req).append(1).build();
    json[Collection::MEMBERS].push_back(std::move(link_elem));
  
    set_response(res, json);
}
