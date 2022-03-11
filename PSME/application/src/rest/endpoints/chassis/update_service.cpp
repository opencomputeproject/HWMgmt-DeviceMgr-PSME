/*!
 * @copyright
 * Copyright (c) 2016-2017 Intel Corporation
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
 * */

#include "psme/core/agent/agent_manager.hpp"
#include "psme/rest/endpoints/chassis/update_service.hpp"
#include "psme/rest/utils/status_helpers.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/model/handlers/handler_manager.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/utils/ec_common_utils.hpp"

using namespace psme::rest::validators;
using namespace agent_framework::model::enums;
using namespace psme::rest;
using namespace psme::rest::error;
using namespace psme::rest::utils;

namespace
{

json::Value make_update_service_prototype()
{
    json::Value r(json::Value::Type::OBJECT);

    r[constants::Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#UpdateService.UpdateService";
    r[constants::Common::ODATA_ID] = json::Value::Type::NIL;
    r[constants::Common::ID] = "1";
    r[constants::Common::ODATA_TYPE] = "#UpdateService.v1_2_0.UpdateService";
    r[constants::Common::NAME] = "Update Service";
    r[constants::Common::DESCRIPTION] = "Update Service";

    json::Value AllowableValues;
    AllowableValues.push_back("/redfish/v1/UpdateService/FirmwareInventory/MU");
    AllowableValues.push_back("/redfish/v1/UpdateService/FirmwareInventory/NOS");

    json::Value In;
    In["@odata.id"] = "/redfish/v1/UpdateService/FirmwareInventory";
    r["FirmwareInventory"] = std::move(In);

    In["@odata.id"] = "/redfish/v1/UpdateService/SoftwareInventory";
    r["SoftwareInventory"] = std::move(In);

    json::Value actions;
    std::string update_service("#");
    update_service.append("UpdateService.SimpleUpdate");
    actions[update_service]["title"] = "SimpleUpdate";
    actions[update_service]["target"] = "/redfish/v1/UpdateService/Actions/UpdateService.SimpleUpdate";
    actions[update_service]["Targets@Redfish.AllowableValues"] = std::move(AllowableValues);

    //Check if support WGET 
    json::Value AllowableValues_Get_Method;
    if (EcCommonUtils::Support_WGET())
    {
        AllowableValues_Get_Method.push_back("HTTPS");
    }

    //Check if support TFTP 
    if (EcCommonUtils::Support_TFTP())
    {
        AllowableValues_Get_Method.push_back("TFTP");
    }
    actions[update_service]["TransferProtocol@Redfish.AllowableValues"] = std::move(AllowableValues_Get_Method);
    r["Actions"] = std::move(actions);
    return r;
}
} // namespace
namespace psme
{
namespace rest
{
namespace endpoint
{
endpoint::UpdateService::UpdateService(const std::string &path) : EndpointBase(path) {}
endpoint::UpdateService::~UpdateService() {}

void endpoint::UpdateService::get(const server::Request &request, server::Response &response)
{

    json::Value r = make_update_service_prototype();
    r[constants::Common::ODATA_ID] = PathBuilder(request).build();
    set_response(response, r);
    return;
}


} // namespace endpoint
} // namespace rest
} // namespace psme
