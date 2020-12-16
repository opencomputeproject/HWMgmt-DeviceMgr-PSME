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

#include "psme/rest/account/config/account_config.hpp"

#include <json/json.hpp>
#include <fstream>

namespace psme {
namespace rest {
namespace account {
namespace config {

constexpr const char AccountConfig::DEFAULT_ACCOUNT_FILE_PATH[];

json::Value AccountConfig::get_accounts_json(const AccountMap& accounts) const {
    json::Value json(json::Value::Type::ARRAY);
    for (const auto& entry : accounts) {
        const auto& account = entry.second;
        json.push_back(account.to_json());
    }
    return json;
}

json::Value AccountConfig::get_accounts_json(const std::string& json_content) const {
    json::Deserializer deserializer(json_content);
    if (deserializer.is_invalid()) {
        throw std::runtime_error("Malformed JSON: " + json_content);
    }
    json::Value json;
    deserializer >> json;
    return json;
}

void AccountConfig::saveAccounts() {
    log_debug(GET_LOGGER("app"), "Saving accounts config file: " << m_config_file_path);
    std::lock_guard<std::mutex> lock{m_mutex};
    std::ofstream out(m_config_file_path, std::ios::binary);
    if (out.fail()) {
        log_warning(GET_LOGGER("app"), "Unable to open config file: " << m_config_file_path);
        return;
    }
    const auto& accounts = AccountManager::get_instance()->getAccount();
    const auto& subs_json = get_accounts_json(accounts);
    json::Serializer serializer(subs_json);
    std::string json_content;
    json_content << serializer;
    out << json_content;
    out.close();
}

void AccountConfig::load_admin() {
    log_debug(GET_LOGGER("app"), "Loading admin Account : ");

    
    try {

       Account account{};
       account.set_username("admin");
       account.set_password("redfish",true);
       account.set_enabled(true);
       account.set_locked(false);
       account.set_roleid("Administrator");
       AccountManager::get_instance()->addAccount(account);

    }
    catch (const std::exception& ex) {
        log_warning(GET_LOGGER("app"), "Unable to add predefined admin Account: " << ex.what());
    }
}





void AccountConfig::loadAccounts() {
    log_debug(GET_LOGGER("app"), "Reading accounts config file: " << m_config_file_path);
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
        log_warning(GET_LOGGER("app"), "Empty accounts configuration file: " << m_config_file_path);
        return;
    }
    
     log_debug(GET_LOGGER("app"), "Reading accounts config file has  : " << content);
    
    try {
        const auto& accounts_json = get_accounts_json(content);
        if (accounts_json.is_array()) {
           log_debug(GET_LOGGER("app"), "Reading accounts after serialized  : " << json::Serializer(accounts_json) );
            AccountManager::get_instance();
            for (const auto& account_json : accounts_json.as_array()) {
                const auto account = Account::from_json(account_json);
                if ( account.get_username() == "admin" )
                   AccountManager::get_instance()->modAccount("admin",account);
                else
                   AccountManager::get_instance()->addAccount(account);
            }
        }
    }
    catch (const std::exception& ex) {
        log_warning(GET_LOGGER("app"), "Unable to add account: " << ex.what());
    }
}

void AccountConfig::set_config_file(const std::string& config_file_path) {
    m_config_file_path = config_file_path;
}

AccountConfig::~AccountConfig() {
}

}
}
}
}
