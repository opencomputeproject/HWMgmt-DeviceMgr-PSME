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

#include "psme/rest/endpoints/manager_log_services_entries.hpp"
#include "psme/rest/constants/constants.hpp"
#include <arpa/inet.h>

/*To get terminal info. Begin:*/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fstream>
#include "psme/rest/log/manager/log_manager.hpp"
using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::log::manager;
using namespace agent_framework::model::enums;

namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#LogEntry.LogEntry";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#LogEntry.v1_0_0.LogEntry";
    r[Common::NAME] = "Log Entry";
    r[Manager::ENTRYTYPE] = json::Value::Type::NIL;
    r[Manager::SEVERITY] = json::Value::Type::NIL;
    r[Manager::CREATED] = json::Value::Type::NIL;
    r[Manager::ENTRY_CODE] = json::Value::Type::NIL;
    r[Manager::SENSOR_TYPE] = json::Value::Type::NIL;
    r[Manager::SENSOR_NUMBER] = json::Value::Type::NIL;
    r[Manager::MESSAGE] = json::Value::Type::NIL;
    return r;
}
}

endpoint::ManagerLogServicesEntries::ManagerLogServicesEntries(const std::string& path) : EndpointBase(path) {}

endpoint::ManagerLogServicesEntries::~ManagerLogServicesEntries() {}

void endpoint::ManagerLogServicesEntries::get(const server::Request &req, server::Response &res)
    {       
    try
        {              
        unsigned RequestID = atoi(req.params[PathParam::LOGSER_ID].c_str());
        json::Value EntryMember;
        EntryMember = LogManager::get_instance()->to_json();
        if (EntryMember.size() > 0 && ((RequestID <= EntryMember.size()) && (RequestID > 0)))
            {
            auto r = ::make_prototype();
            r[Common::ODATA_ID] = PathBuilder(req).build();
            r[Common::ID] = req.params[PathParam::LOGSER_ID].c_str();

            int EntryID = atoi(req.params[PathParam::LOGSER_ID].c_str()) - 1;
            json::Value s2 = EntryMember[EntryID];
            r[Manager::ENTRYTYPE] = s2[Manager::ENTRYTYPE].as_string();
            r[Manager::SEVERITY] = s2[Manager::SEVERITY].as_string();
            r[Manager::CREATED] = s2[Manager::CREATED].as_string();
            r[Manager::ENTRY_CODE] = s2[Manager::ENTRY_CODE].as_string();
            r[Manager::SENSOR_TYPE] = s2[Manager::SENSOR_TYPE].as_string();
            r[Manager::SENSOR_NUMBER] = s2[Manager::SENSOR_NUMBER].as_int();
            r[Manager::MESSAGE] = s2[Manager::MESSAGE].as_string();
            set_response(res, r);
                }
        else
                {
            res.set_status(server::status_4XX::BAD_REQUEST); //Download Failure
        }	
    }
    catch (const std::exception& ex)
    {
        log_warning(GET_LOGGER("rest"),"Read manage error!!");
    }
    
}
