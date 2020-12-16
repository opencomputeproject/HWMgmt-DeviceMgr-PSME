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

#include "agent-framework/module/requests/common.hpp"
#include "agent-framework/module/responses/common.hpp"
#include "agent-framework/module/constants/compute.hpp"

#include "psme/core/agent/agent_manager.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/reset.hpp"
#include "psme/rest/endpoints/system/log_reset.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/model/handlers/generic_handler_deps.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"
#include "eclog_helper/eclog_helper.hpp"


using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::validators;
using namespace eclog_helper;

endpoint::LogReset::LogReset(const std::string& path) : EndpointBase(path) {}


endpoint::LogReset::~LogReset() {}


void endpoint::LogReset::post(const server::Request& request, server::Response& response) 
{
    //dummy //
    PathBuilder(request).build();
    eclog_helper::RFLogEntry &Entry = eclog_helper::RFLogEntry::get_instance();
    Entry.clean_log();
    response.set_status(server::status_2XX::OK);
}
