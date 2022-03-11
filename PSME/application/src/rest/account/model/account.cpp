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


#include "psme/rest/account/model/account.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/utils/time_utils.hpp"
#include <json/json.hpp>
#include <stdexcept>

using namespace psme::rest::constants;
using namespace psme::rest::utils;

namespace psme {
namespace rest {
namespace account {
namespace model {

json::Value Account::to_json() const {
    json::Value json(json::Value::Type::OBJECT);
    json[Common::NAME] = m_name;
    json[AccountConst::USERNAME] = m_username;
    json[AccountConst::SALT]     = m_salt;
    json[AccountConst::PASSWORD] = m_password;
    json[AccountConst::ROLEID] = m_roleid;
    json[AccountConst::LOCKED] = m_locked;
    json[AccountConst::ENABLED] = m_enabled;
    json[AccountConst::ETAG] = m_etag;
    return json;
}

Account Account::from_json(const json::Value& json) {
    std::chrono::steady_clock::time_point m_timestamp{std::chrono::steady_clock::now()};
    std::string time_s = "W/\"" + TimeUtils::get_time_with_zone(m_timestamp) + '\"';
    const auto& name = json[Common::NAME].as_string();
    const auto& username = json[AccountConst::USERNAME].as_string();
    const auto& salt     = json[AccountConst::SALT].as_string();
    const auto& password = json[AccountConst::PASSWORD].as_string();
    const auto& roleid = json[AccountConst::ROLEID].as_string();    
    const auto& locked = json[AccountConst::LOCKED].as_bool();
    const auto& enabled = json[AccountConst::ENABLED].as_bool();

    Account account;
    account.set_name(name);
    account.set_username(username);
    account.set_salt(salt);
    account.set_password(password,false);
    account.set_enabled(enabled);
    account.set_locked(locked);
    account.set_roleid(roleid);
    account.set_etag(time_s);
    return account;
}



}
}
}
}
