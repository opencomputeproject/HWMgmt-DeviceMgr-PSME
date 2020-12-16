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

#include "agent-framework/module/requests/common.hpp"
#include "agent-framework/module/responses/common.hpp"
#include "agent-framework/module/constants/compute.hpp"
#include "agent-framework/action/task_runner.hpp"
#include "agent-framework/action/task_creator.hpp"
#include "psme/rest/endpoints/task_service_utils.hpp"
#include "psme/core/agent/agent_manager.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/reset.hpp"
#include "psme/rest/endpoints/system/system_reset.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/model/handlers/generic_handler_deps.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"
#include "ecsys_helper/ecsys_helper.hpp"
#include <unistd.h>
#include <sys/reboot.h>
#include <chrono>
#include <thread>
#include "psme/rest/utils/ec_common_utils.hpp"
#include <eclog_helper/eclog_helper.hpp>
using namespace eclog_helper;
using namespace psme::rest::utils;
using namespace std;
using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::validators;
using namespace ecsys_helper;


endpoint::SystemReset::SystemReset(const std::string& path) : EndpointBase(path) {}

endpoint::SystemReset::~SystemReset() {}

auto &g_ADbg = ADbg::get_instance();
static void GracefulRestartAfter2Seconds()
{
    auto exec_reboot_task = []() {
        //Start reboot
        eclog_helper::RFLogEntry &Entry = eclog_helper::RFLogEntry::get_instance();
        std::string event("Event");
        std::string sensor_type("SystemBoot/Restart");
        std::string serverity("OK");
        std::string message;
        message = "System graceful restart";
        Entry.get_current_time();
        Entry.set_log_entry(event, sensor_type, serverity, message, 0);

        char command[BUFFER_LEN] = {0};
        std::string resultA;
    sync();
        sprintf(command, "%s", "sleep 5;reboot");
        EcCommonUtils::exec_shell_(command, resultA, 0);
        g_ADbg.acc_printf(LEVEL_WARN, "System graceful restart!!");
    };

    agent_framework::action::TaskCreator task_creator{};
    task_creator.prepare_task();
    task_creator.add_subtask(std::bind(exec_reboot_task));
    task_creator.register_task();
    auto task = task_creator.get_task();
    agent_framework::action::TaskRunner::get_instance().run(task);
}

static void GracefulShutdownAfter2Seconds()
{
    auto exec_reboot_task = []() {
        //Start reboot
        eclog_helper::RFLogEntry &Entry = eclog_helper::RFLogEntry::get_instance();
        std::string event("Event");
        std::string sensor_type("OS Stop/Shutdown");
        std::string serverity("OK");
        std::string message;
        message = "System graceful shutdown";
        Entry.get_current_time();
        Entry.set_log_entry(event, sensor_type, serverity, message, 0);

        char command[BUFFER_LEN] = {0};
        std::string resultA;
    sync();
        sprintf(command, "%s", "sleep 5;shutdown -h now");
        EcCommonUtils::exec_shell_(command, resultA, 0);
        g_ADbg.acc_printf(LEVEL_WARN, "System graceful shutdown!!");
    };

    agent_framework::action::TaskCreator task_creator{};
    task_creator.prepare_task();
    task_creator.add_subtask(std::bind(exec_reboot_task));
    task_creator.register_task();
    auto task = task_creator.get_task();
    agent_framework::action::TaskRunner::get_instance().run(task);
}

void endpoint::SystemReset::post(const server::Request &request, server::Response &response)
{
    std::string agent_id{};
    std::string system_uuid{};
    std::string parent_uuid{};

    char command[HT_BUFFER_LEN] = {0};
    char resultA[HT_BUFFER_LEN] = {0};

    // Gets necessary data from model and does not block system reference
    auto system = model::Find<agent_framework::model::System>(request.params[PathParam::SYSTEM_ID]).get();
    
    agent_id = system.get_agent_id();
    system_uuid = system.get_uuid();
    parent_uuid = system.get_parent_uuid();

    const auto& json = JsonValidator::validate_request_body<schema::ResetPostSchema>(request);
    agent_framework::model::attribute::Attributes attributes{};

    if (json.is_member(constants::Common::RESET_TYPE))
    {
        const auto& reset_type = json[constants::Common::RESET_TYPE].as_string();
        attributes.set_value(agent_framework::model::literals::System::POWER_STATE, reset_type);

		using ResetType = agent_framework::model::enums::ResetType;

		const auto reset_type_e = ResetType::from_string(reset_type);

        switch (reset_type_e)
        {
			case agent_framework::model::enums::ResetType::None:
			case agent_framework::model::enums::ResetType::ForceOn:
			case agent_framework::model::enums::ResetType::Nmi:
			default:
				break;
			case agent_framework::model::enums::ResetType::On:
			case agent_framework::model::enums::ResetType::GracefulRestart:
                     {
            GracefulRestartAfter2Seconds();
				break;
                     }
			case agent_framework::model::enums::ResetType::PushPowerButton:
			case agent_framework::model::enums::ResetType::ForceRestart:
				sprintf(command, "%s" ,"psme.sh set force_restart");	
            EcCommonUtils::exec_shell(command, resultA, 0);
				break;
			case agent_framework::model::enums::ResetType::ForceOff:
				sprintf(command, "%s" ,"psme.sh set force_off");	
            EcCommonUtils::exec_shell(command, resultA, 0);
				break;
			case agent_framework::model::enums::ResetType::GracefulShutdown:
                     {
            GracefulShutdownAfter2Seconds();
				break;
		}
		}
    }

    if(!strncmp(resultA, "NOT support.", 12))
        response.set_status(server::status_5XX::NOT_IMPLEMENTED);
    else
        response.set_status(server::status_2XX::OK);
}
