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



#include "psme/rest/account/model/role.hpp"
#include "psme/rest/constants/constants.hpp"
#include <json/json.hpp>
#include <stdexcept>

using namespace psme::rest::constants;

namespace psme {
namespace rest {
namespace account {
namespace model {

json::Value Role::to_json() const {
    json::Value json(json::Value::Type::OBJECT);
    json[Common::NAME] = m_name;
    json[AccountRole::ISPREDEFINED] = m_ispredefined;
    json[AccountRole::ROLEID] = m_roleid;
    json::Value privilege_types_json(json::Value::Type::ARRAY);
    for (const auto& privilege_type : m_privilege_types.get()){
        privilege_types_json.push_back(privilege_type.to_string());
    }
    json[AccountRole::PRIVILEGE_TYPES] = privilege_types_json;
    return json;
}

Role Role::from_json(const json::Value& json) {
    const auto& name = json[Common::NAME].as_string();
    const auto& ispredefined = json[AccountRole::ISPREDEFINED].as_bool();
    const auto& roleid = json[AccountRole::ROLEID].as_string();
    PrivilegeTypes privilege_types;
    for (const auto& privilege_type : json[AccountRole::PRIVILEGE_TYPES]){
        privilege_types.add(PrivilegeType::from_string(privilege_type.as_string()));
    }
    Role role;
    role.set_name(name);
    role.set_ispredefined(ispredefined);
    role.set_roleid(roleid);
    role.set_privilege_types(privilege_types);
    return role;
}

void PrivilegeTypes::add(PrivilegeType priv_type) {
   m_privilege_types.push_back(priv_type);
}

PrivilegeTypeVec PrivilegeTypes::get() const {
    return m_privilege_types;
}



}
}
}
}
