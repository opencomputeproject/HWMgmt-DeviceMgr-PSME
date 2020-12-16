/*!
 * @brief Implementation of GetManagerInfo command.
 *
 * File contains all implementations of methods for GetManagerInfo command.
 *
 * @copyright Copyright (c) 2016-2017 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @header{Files}
 * @file get_manager_info.cpp
 */

#include "agent-framework/module/common_components.hpp"
#include "agent-framework/command-ref/registry.hpp"
#include "agent-framework/command-ref/network_commands.hpp"



using namespace agent_framework::command_ref;
using namespace agent_framework::module;

namespace {
void get_manager_info(const GetManagerInfo::Request& request,
                      GetManagerInfo::Response& response) {
    response = CommonComponents::get_instance()->
        get_module_manager().get_entry(request.get_uuid());
}
}

REGISTER_COMMAND(GetManagerInfo, ::get_manager_info);