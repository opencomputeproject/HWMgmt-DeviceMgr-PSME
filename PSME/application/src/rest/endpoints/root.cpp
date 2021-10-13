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

#include "agent-framework/service_uuid.hpp"
#include "configuration/configuration.hpp"

#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/root.hpp"

#include "json/json.hpp"
#include "version.hpp"



using namespace psme::rest;
using namespace psme::rest::constants;

using agent_framework::generic::ServiceUuid;

namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#ServiceRoot.ServiceRoot";
    r[Common::ODATA_ID] = "/redfish/v1";
    r[Common::ODATA_TYPE] = "#ServiceRoot.v1_1_1.ServiceRoot";
    r[Common::ID] = "RootService";
    r[Common::NAME] = "PSME Service Root";
    r[Common::DESCRIPTION] = "Service Root description";
    r[Root::REDFISH_VERSION] = "1.0.2";
    r[Common::UUID] = "00000000000-000000-0000000-00000000000000000";
#ifdef CACCOUNT	
    r[Root::ACCOUNT_SERVICE][Common::ODATA_ID] = "/redfish/v1/AccountService";
#endif

#ifdef CNTASK 	
    r[Root::TASKS][Common::ODATA_ID] = "/redfish/v1/TaskService";
#endif

    r[Common::CHASSIS][Common::ODATA_ID] = "/redfish/v1/Chassis";

#ifdef CSERVICES
    r[Root::SERVICES][Common::ODATA_ID] = "/redfish/v1/Services";
#endif

#ifdef CSESSION
    r[SessionService::SESSION_SERVICE][Common::ODATA_ID] = "/redfish/v1/SessionService";
#endif

#ifdef CUPDATE
    r[UpdateService::UPDATE_SERVICE][Common::ODATA_ID] = "/redfish/v1/UpdateService";
#endif

    r[Root::MANAGERS][Common::ODATA_ID] = "/redfish/v1/Managers";

#ifdef CFABRICS
    r[Root::FABRICS][Common::ODATA_ID] = "/redfish/v1/Fabrics";
#endif
    r[Common::LINKS][SessionService::SERVICES][Common::ODATA_ID]= "/redfish/v1/SessionService/Sessions";

    return r;
}
}


endpoint::Root::Root(const std::string& path) : EndpointBase(path) {
    const auto& config = configuration::Configuration::get_instance().to_json();
    service_root_name = config["rest"]["service-root-name"].as_string();
}


endpoint::Root::~Root() { }

void endpoint::Root::get(const server::Request &request, server::Response &response)
{
    std::string o_data_verion = request.get_header("OData-Version");

    if (o_data_verion != "")
    {
        if (o_data_verion != "4.0")
        {
            response.set_status(server::status_4XX::PRECONDITION_FAILED);
            return;
        }
    }

    auto json = make_prototype();

    json[Common::UUID] = ServiceUuid::get_instance()->get_service_uuid();
    json[Common::NAME] = service_root_name;

    response.set_header("Allow", "GET HEAD");
    response.set_header("Cache-Control", "no-cache");
    response.set_header("Link", "<http://redfish.dmtf.org/schemas/ServiceRoot.json>;rel=\"describedby\"");
    response.set_header("OData-Version", "4.0");
    set_response(response, json);
}

void endpoint::Root::trace(const server::Request& request, server::Response& response)
{
    response.set_header("Allow", "GET");
    http_method_not_allowed(request, response);
}

void endpoint::Root::head(const server::Request&, server::Response& response) {
    response.set_header("Allow", "GET HEAD");
    response.set_header("Cache-Control", "no-cache");
    response.set_header("Link", "<http://redfish.dmtf.org/schemas/ServiceRoot.json>;rel=\"describedby\"");
}
