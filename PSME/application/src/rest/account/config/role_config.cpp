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

#include "psme/rest/account/config/role_config.hpp"

#include <json/json.hpp>
#include <fstream>

namespace psme {
namespace rest {
namespace account {
namespace config {

constexpr const char RoleConfig::DEFAULT_ROLE_FILE_PATH[];

json::Value RoleConfig::get_roles_json(const RoleMap& roles) const {
    json::Value json(json::Value::Type::ARRAY);
    for (const auto& entry : roles) {
        const auto& role = entry.second;
        json.push_back(role.to_json());
    }
    return json;
}

json::Value RoleConfig::get_roles_json(const std::string& json_content) const {
    json::Deserializer deserializer(json_content);
    if (deserializer.is_invalid()) {
        throw std::runtime_error("Malformed JSON: " + json_content);
    }
    json::Value json;
    deserializer >> json;
    return json;
}

void RoleConfig::saveRoles() {
    log_debug(GET_LOGGER("app"), "Saving roles config file: " << m_config_file_path);
    std::lock_guard<std::mutex> lock{m_mutex};
    std::ofstream out(m_config_file_path, std::ios::binary);
    if (out.fail()) {
        log_warning(GET_LOGGER("app"), "Unable to open role config file: " << m_config_file_path);
        return;
    }
    auto roles = AccountManager::get_instance()->getRole();
    
    //we do remove those ispredefined ones and only save the others
    auto iter = roles.begin();
    while ( iter != roles.end() ) {
        const auto& role = iter->second;
        if (true == role.get_ispredefined()) {
            iter = roles.erase(iter);
        }
    }    
       
    const auto& roles_json = get_roles_json(roles);
    json::Serializer serializer(roles_json);
    std::string json_content;
    json_content << serializer;
    out << json_content;
    out.close();
}

void RoleConfig::loadPreRoles() {
    log_debug(GET_LOGGER("app"), "Loading Predefined Roles : ");

    try
    {
          Role Aadmin{};
          Role Aoperator{};
          Role Areadonlyuser{};
          
          PrivilegeTypes APrivTypes{};
          
          APrivTypes.add(PrivilegeType::from_string("Login"));
          APrivTypes.add(PrivilegeType::from_string("ConfigureManager"));
          APrivTypes.add(PrivilegeType::from_string("ConfigureUsers"));
          APrivTypes.add(PrivilegeType::from_string("ConfigureSelf"));
          APrivTypes.add(PrivilegeType::from_string("ConfigureComponents"));          
        Aadmin.set_ispredefined(true);
        Aadmin.set_privilege_types(APrivTypes);
        Aadmin.set_roleid("Administrator");
        Aadmin.set_name("Administrator");
          AccountManager::get_instance()->addRole(Aadmin);
                    
          PrivilegeTypes OPrivTypes{};
          OPrivTypes.add(PrivilegeType::from_string("Login"));
          OPrivTypes.add(PrivilegeType::from_string("ConfigureSelf"));
          OPrivTypes.add(PrivilegeType::from_string("ConfigureComponents"));          
        Aoperator.set_ispredefined(true);
        Aoperator.set_privilege_types(OPrivTypes);
        Aoperator.set_roleid("Operator");
        Aoperator.set_name("Operator");
          AccountManager::get_instance()->addRole(Aoperator);
 
          PrivilegeTypes RPrivTypes{};
          RPrivTypes.add(PrivilegeType::from_string("Login"));
          RPrivTypes.add(PrivilegeType::from_string("ConfigureSelf"));
        Areadonlyuser.set_ispredefined(true);
        Areadonlyuser.set_privilege_types(RPrivTypes);
        Areadonlyuser.set_roleid("ReadOnly");
        Areadonlyuser.set_name("ReadOnly");
          AccountManager::get_instance()->addRole(Areadonlyuser);
    }
    catch (const std::exception& ex) {
        log_warning(GET_LOGGER("app"), "Unable to add predefined role: " << ex.what());
    }
}




void RoleConfig::loadRoles() {
    log_debug(GET_LOGGER("app"), "Reading roles config file: " << m_config_file_path);
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
        log_warning(GET_LOGGER("app"), "Empty roles configuration file: " << m_config_file_path);
        return;
    }
    
     log_debug(GET_LOGGER("app"), "Reading roles config file has  : " << content);
    
    try {
        const auto& roles_json = get_roles_json(content);
        if (roles_json.is_array()) {
           log_debug(GET_LOGGER("app"), "Reading roles after serialized  : " << json::Serializer(roles_json) );
            AccountManager::get_instance();
            for (const auto& role_json : roles_json.as_array()) {
                const auto role = Role::from_json(role_json);
                AccountManager::get_instance()->addRole(role);
            }
        }
    }
    catch (const std::exception& ex) {
        log_warning(GET_LOGGER("app"), "Unable to add role: " << ex.what());
    }
}

void RoleConfig::set_config_file(const std::string& config_file_path) {
    m_config_file_path = config_file_path;
}

RoleConfig::~RoleConfig() {
}

}
}
}
}
