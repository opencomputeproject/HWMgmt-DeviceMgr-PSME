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


#include "psme/rest/session/config/session_config.hpp"

#include <json/json.hpp>
#include <fstream>

namespace psme {
namespace rest {
namespace session {
namespace config {

constexpr const char SessionConfig::DEFAULT_SESSION_FILE_PATH[];


json::Value SessionConfig::get_sessions_json(const std::string& json_content) const {
    json::Deserializer deserializer(json_content);
    if (deserializer.is_invalid()) {
        throw std::runtime_error("Malformed JSON: " + json_content);
    }
    json::Value json;
    deserializer >> json;
    return json;
}

void SessionConfig::saveSessionsConfig() {
    log_debug(GET_LOGGER("app"), "Saving sessions config file: " << m_config_file_path);
    std::lock_guard<std::mutex> lock{m_mutex};
    std::ofstream out(m_config_file_path, std::ios::binary);
    if (out.fail()) {
        log_warning(GET_LOGGER("app"), "Unable to open config file: " << m_config_file_path);
        return;
    }
  
    const auto& subs_json= SessionManager::get_instance()->to_json();
    json::Serializer serializer(subs_json);
    std::string json_content;
    json_content << serializer;
    out << json_content;
    out.close();
  
}

void SessionConfig::loadSessionsConfig() {
    log_debug(GET_LOGGER("app"), "Reading sessions config file: " << m_config_file_path);
    std::ifstream in(m_config_file_path);
    if (in.fail()) {
        log_warning(GET_LOGGER("app"), "Unable to open config file: " << m_config_file_path);
        return;
    }
    std::stringstream buffer;
    buffer << in.rdbuf();
    in.close();
    const auto& content = buffer.str();

    if (content.empty()) {
        log_warning(GET_LOGGER("app"), "Empty sessions configuration file: " << m_config_file_path);
        return;
    }
    
     log_debug(GET_LOGGER("app"), "Reading sessions config file has  : " << content);
    
    try {
        const auto& sessions_json = get_sessions_json(content);
        log_debug(GET_LOGGER("app"), "Reading sessions after serialized  : " << json::Serializer(sessions_json) );
        SessionManager::get_instance()->addSessionConfig(sessions_json);
    }
    catch (const std::exception& ex) {
        log_warning(GET_LOGGER("app"), "Unable to load session config: " << ex.what());
    }
}

void SessionConfig::set_config_file(const std::string& config_file_path) {
    m_config_file_path = config_file_path;
}

SessionConfig::~SessionConfig() {
}

}
}
}
}
