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
#include "psme/rest/endpoints/session_service.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/session/model/sessionservice.hpp"
#include "psme/rest/session/manager/session_manager.hpp"
#include "psme/rest/session/config/session_config.hpp"
#include "json/json.hpp"


#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/sessionservice.hpp"

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::session::manager;
using namespace psme::rest::session::config;

namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#SessionService.SessionService";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#SessionService.v1_0_0.SessionService";
    r[Common::ID] = "SessionService";
    r[Common::NAME] = "Session Service";
    r[Common::DESCRIPTION] = "Session Service ";
    r[Common::STATUS][Common::STATE] = "Enabled";
    r[Common::STATUS][Common::HEALTH] = "OK";
    r[Common::STATUS][Common::HEALTH_ROLLUP] = "OK";
    r[Common::OEM] = json::Value::Type::OBJECT;
    r[SessionService::SERVICE_ENABLED] = true;
    r[SessionService::SERVICE_TIMEOUT] = 300;	
    r[SessionService::SERVICES][Common::ODATA_ID]= "/redfish/v1/SessionService/Sessions";

    return r;
}
}

endpoint::SessionService::SessionService(const std::string& path) : EndpointBase(path) {}

endpoint::SessionService::~SessionService() {}

void endpoint::SessionService::get(const server::Request& req, server::Response& res) {
    auto r = make_prototype();
    r[Common::ODATA_ID] = PathBuilder(req).build();
    r[constants::SessionService::SERVICE_TIMEOUT] = SessionManager::get_instance()->GetSessionConfigTimeOut();
    r[constants::SessionService::SERVICE_ENABLED] = SessionManager::get_instance()->GetSessionConfigEnable();

    set_response(res, r);
}
using namespace psme::rest::validators;
using namespace psme::rest::validators::schema;

void endpoint::SessionService::post(const server::Request& request, server::Response& response) {
    const auto& json = JsonValidator::validate_request_body<SessionServicePostSchema>(request);
    using session::model::Sessionservice;
	
    SessionManager::get_instance()->SetSessionConfigTimeOut(json[constants::SessionService::SERVICE_TIMEOUT].as_uint64());
    SessionManager::get_instance()->SetSessionConfigEnable(json[constants::SessionService::SERVICE_ENABLED].as_bool());
    SessionConfig::get_instance()->saveSessionsConfig();	

    response.set_status(server::status_2XX::OK);
}
