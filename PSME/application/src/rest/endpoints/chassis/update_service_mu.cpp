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
#include "psme/rest/endpoints/chassis/update_service_mu.hpp"
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
#include <ecrf_pal_helper/api/ecrf_pal_helper.hpp>
#include <sys/sysinfo.h>
#include <unistd.h>

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

static const string CHK_TIMEOUT_CMD = "res=$?;if [ $res -eq 124 ];then echo 'Timeout' ;elif [ $res -eq 0 ];then echo 'Success' ;else echo 'Failure';fi";

json::Value make_mu_update_service_prototype();
json::Value make_mu_update_service_prototype()
{
    json::Value r(json::Value::Type::OBJECT);
    return r;
}
namespace psme
{
namespace rest
{
namespace endpoint
{
auto &gADbg = ADbg::get_instance();

endpoint::UpdateServiceMU::UpdateServiceMU(const std::string &path) : EndpointBase(path)
{
}

void endpoint::UpdateServiceMU::get(const server::Request &request, server::Response &response)
{
    response.set_status(server::status_5XX::NOT_IMPLEMENTED);
    return;
}

void endpoint::UpdateServiceMU::post(const server::Request &request, server::Response &response)
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
