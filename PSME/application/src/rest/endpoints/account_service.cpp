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

#include "psme/rest/endpoints/account_service.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/account/model/accountservice.hpp"
#include "json/json.hpp"

#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/accountservice.hpp"

using namespace psme::rest;
using namespace psme::rest::constants;
using account::model::Accountservice;



namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#AccountService.AccountService";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#AccountService.v1_0_0.AccountService";
    r[Common::ID] = "AccountService";
    r[Common::NAME] = "Account Service";
    r[Common::DESCRIPTION] = "Account Service ";
    r[Common::STATUS][Common::STATE] = "Enabled";
    r[Common::STATUS][Common::HEALTH] = "OK";
    r[Common::STATUS][Common::HEALTH_ROLLUP] = "OK";
    r[Common::OEM] = json::Value::Type::OBJECT;
    r[AccountService::SERVICE_ENABLED] = true;
    r[AccountService::AUTHFAILURELOGGINGTHRESHOLD] = Accountservice::get_instance()->get_aflt();
    r[AccountService::MINPASSWORDLENGTH] = 6;
    r[AccountService::MAXPASSWORDLENGTH] = 24;    
    r[AccountService::ACCOUNTLOCKOUTTHRESHOLD] = Accountservice::get_instance()->get_alt();
    r[AccountService::ACCOUNTLOCKOUTDURATION] = Accountservice::get_instance()->get_ald();
    r[AccountService::ACCOUNTLOCKOUTCOUNTERRESETAFTER] = Accountservice::get_instance()->get_alcra();
    r[AccountService::ACCOUNTS][Common::ODATA_ID]= "/redfish/v1/AccountService/Accounts";
    r[AccountService::ROLES][Common::ODATA_ID]= "/redfish/v1/AccountService/Roles";

    return r;
}
}

endpoint::AccountService::AccountService(const std::string& path) : EndpointBase(path) {}

endpoint::AccountService::~AccountService() {}

void endpoint::AccountService::get(const server::Request& req, server::Response& res) {
    res.set_header("Link", "<http://redfish.dmtf.org/schemas/AccountService.json>;rel=\"describedby\"");

    auto r = make_prototype();
    r[Common::ODATA_ID] = PathBuilder(req).build();

    set_response(res, r);
}
using namespace psme::rest::validators;
using namespace psme::rest::validators::schema;

void endpoint::AccountService::patch(const server::Request& request, server::Response& response) {
    const auto& json = JsonValidator::validate_request_body<AccountServicePostSchema>(request);

    Accountservice::get_instance()->from_json(json);

    //endpoint::utils::set_location_header(response, PathBuilder(request).append(id).build());
    response.set_status(server::status_2XX::OK);
}
