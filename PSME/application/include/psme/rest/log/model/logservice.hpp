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
#include "agent-framework/module/enum/enum_builder.hpp"
#include <json/json.hpp>
#include <string>
#include <vector>

namespace psme {
namespace rest {
namespace log {
namespace model {

#define UNUSED(x) (void)(x)

/*!
 * @brief Subscription representation
 */
class Logservice : public agent_framework::generic::Singleton<Logservice>
{
public:
    /*!
     * @brief Creates json representation of subscription
     *
     * @return JSON representation of subscription
     */
    json::Value to_json() const;

    /*!
     * @brief Creates model representation from subscription JSON
     *
     * @param json JSON representation of subscription
     * @return Model representation of subscription
     */
    void from_json(const json::Value &json);

private:
};
}
}
}
}
