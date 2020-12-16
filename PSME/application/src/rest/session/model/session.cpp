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



#include "psme/rest/session/model/session.hpp"
#include "psme/rest/constants/constants.hpp"
#include <json/json.hpp>
#include <stdexcept>

using namespace psme::rest::constants;

namespace psme {
namespace rest {
namespace session {
namespace model {

Session to_model(const json::Value& json);


json::Value Session::to_json() const {
    json::Value json(json::Value::Type::OBJECT);
    json[SessionService::AUTHENTOKEN] = m_authe_token;
    json[SessionService::USERNAME] = m_username;
    json[SessionService::PASSWORD] = m_password;
    json[SessionService::TIMESTAMP] = m_time_stamp;
	
    return json;
}

Session Session::from_json(const json::Value& json) {
    const auto& authe_token = json[SessionService::AUTHENTOKEN].as_string();
    const auto& time_stamp = json[SessionService::TIMESTAMP].as_uint64();

    const auto& username = json[SessionService::USERNAME].as_string();
    const auto& password = json[SessionService::PASSWORD].as_string();


    Session session;
    session.set_authen_token(authe_token);
    session.set_time_stamp(time_stamp);
    session.set_username(username);
    session.set_password(password);
    return session;
}



}
}
}
}
