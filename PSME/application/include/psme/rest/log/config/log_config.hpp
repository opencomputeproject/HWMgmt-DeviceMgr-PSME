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

#pragma once
#include "agent-framework/generic/singleton.hpp"
#include "psme/rest/log/model/log.hpp"
#include "psme/rest/log/manager/log_manager.hpp"
#include <mutex>
#include <map>

namespace psme
{
namespace rest
{
namespace log
{
namespace config
{

using namespace psme::rest::log;
using namespace psme::rest::log::manager;
using namespace psme::rest::log::model;
/*!
 * LogConfig implementation
 */
class LogConfig : public agent_framework::generic::Singleton<LogConfig>
{
public:
    /*!
     * @brief Default subscription file path
     */
    static constexpr const char DEFAULT_LOG_FILE_PATH[] = "/etc/psme/logs";
    /*!
     * @brief Destructor
     */
    virtual ~LogConfig();

private:
   std::mutex m_mutex{};
};

} // namespace config
} // namespace log
} // namespace rest
} // namespace psme
