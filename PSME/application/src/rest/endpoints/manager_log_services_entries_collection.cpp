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
#include "psme/rest/endpoints/manager_log_services_entries_collection.hpp"
#include "psme/rest/log/manager/log_manager.hpp"
#include "psme/rest/constants/constants.hpp"
#include <arpa/inet.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fstream>

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace agent_framework::model::enums;
using namespace psme::rest::log::manager;

namespace
{
json::Value make_prototype()
{
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#LogEntryCollection.LogEntryCollection";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#LogEntryCollection.LogEntryCollection";
    r[Common::NAME] = "Log Service Collection";
    r[Common::DESCRIPTION] = "Collection of Logs for this System";
    r[Collection::ODATA_COUNT] = 0;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;
	
    return r;
}
} // namespace

endpoint::ManagerLogServicesEntriesCollection::ManagerLogServicesEntriesCollection(const std::string& path) : EndpointBase(path) {}
endpoint::ManagerLogServicesEntriesCollection::~ManagerLogServicesEntriesCollection() {}

void endpoint::ManagerLogServicesEntriesCollection::get(const server::Request &req, server::Response &res)
{
    try 
    {
        auto JsonRes = ::make_prototype();
        JsonRes[Common::ODATA_ID] = PathBuilder(req).build();
        JsonRes[Collection::MEMBERS] = LogManager::get_instance()->to_json();
        JsonRes[Collection::ODATA_COUNT] = JsonRes[Collection::MEMBERS].size();
        set_response(res, JsonRes);
    }	
    catch (const std::exception& ex)
    {
        log_warning(GET_LOGGER("rest"),"Read manage error!!");
    }
}
