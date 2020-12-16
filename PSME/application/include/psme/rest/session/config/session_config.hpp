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
#include "psme/rest/session/model/session.hpp"
#include "psme/rest/session/manager/session_manager.hpp"
#include <mutex>
#include <map>

namespace psme {
namespace rest {
namespace session {
namespace config {

using namespace psme::rest::session;
using namespace psme::rest::session::manager;
using namespace psme::rest::session::model;
/*!
 * SessionConfig implementation
 */
class SessionConfig : public agent_framework::generic::Singleton<SessionConfig> {
public:
    /*!
     * @brief Default subscription file path
     */
    static constexpr const char DEFAULT_SESSION_FILE_PATH[] = "/etc/psme/sessions";

    /*!
     * @brief Set sessions configuration file
     * @param config_file_path Sessions configuration file path
     */
    void set_config_file(const std::string& config_file_path);

    /*!
     * @brief Save sessions to file
     */
    void saveSessionsConfig();

    /*!
     * @brief Load sessions from file
     */
    void loadSessionsConfig();

    /*!
     * @brief Destructor
     */
    virtual ~SessionConfig();
private:
    json::Value get_sessions_json(const std::string& json_content) const;
    std::mutex m_mutex{};
    std::string m_config_file_path{DEFAULT_SESSION_FILE_PATH};
};


}
}
}
}

