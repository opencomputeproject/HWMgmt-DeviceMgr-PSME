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
#include "psme/rest/endpoints/chassis/update_service_nos.hpp"
#include "psme/rest/utils/status_helpers.hpp"
#include "psme/rest/utils/ec_common_utils.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/model/handlers/handler_manager.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/endpoints/monitor_content_builder.hpp"
#include "psme/rest/endpoints/task_service_utils.hpp"
#include "psme/rest/constants/constants.hpp"
#include "agent-framework/module/requests/common.hpp"
#include "agent-framework/module/responses/common.hpp"
#include "agent-framework/action/task_runner.hpp"
#include "agent-framework/action/task_creator.hpp"
#include "agent-framework/eventing/event_data.hpp"
#include "agent-framework/eventing/events_queue.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"
#include "psme/rest/eventing/manager/subscription_manager.hpp"
#include <eclog_helper/eclog_helper.hpp>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <ecrf_pal_helper/api/ecrf_pal_helper.hpp>

using namespace ecrf_pal_helper;
using namespace psme::rest::validators;
using namespace agent_framework::model::enums;
using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::error;
using namespace psme::rest::utils;

using namespace psme::rest::eventing;
using namespace psme::rest::model;
using SubscriptionManager = psme::rest::eventing::manager::SubscriptionManager;
using namespace eclog_helper;


json::Value make_nos_update_service_prototype();
json::Value make_nos_update_service_prototype()
{
    json::Value r(json::Value::Type::OBJECT);

    r[constants::Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#SoftwareInventory.SoftwareInventory";
    r[constants::Common::ODATA_ID] = json::Value::Type::NIL;
    r[constants::Common::ID] = "NOS Updater";
    r[constants::Common::ODATA_TYPE] = "#SoftwareInventory.v1_2_1.SoftwareInventory";
    r[constants::Common::NAME] = "Edgecore NOS Updater information";
    r[constants::Common::DESCRIPTION] = "Update Service";
    return r;
}

namespace psme
{
namespace rest
{
namespace endpoint
{
endpoint::UpdateServiceNOS::UpdateServiceNOS(const std::string &path) : EndpointBase(path)
{
}

void endpoint::UpdateServiceNOS::get(const server::Request &request, server::Response &response)
{
    response.set_status(server::status_5XX::NOT_IMPLEMENTED);
    return;
}

string get_name_from_device_path(const string &device_path);

void endpoint::UpdateServiceNOS::post(const server::Request &request, server::Response &response)
{
    try
    {
        response.set_status(server::status_5XX::NOT_IMPLEMENTED);
        return;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
}

} // namespace endpoint
} // namespace rest
} // namespace psme
