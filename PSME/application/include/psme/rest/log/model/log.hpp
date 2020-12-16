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
#include "agent-framework/module/enum/enum_builder.hpp"
#include <string>
#include <vector>

namespace json {
    class Value;
}



namespace psme {
namespace rest {
namespace log {
namespace model {



/*!
 * @brief Subscription representation
 */
class Log final {
public:
    /*!
     * @brief Get log id
     *
     * @return log id
     */
    uint64_t get_id() const {
        return m_id;
    }

    /*!
     * @brief Set log id
     *
     * @param id log id
     */
    void set_id(uint64_t id) {
        m_id = id;
    }

   /*!
     * @brief Creates json representation of log
     *
     * @return JSON representation of subscription
     */
    json::Value to_json() const;


    /*!
     * @brief Creates model representation from log JSON
     *
     * @param json JSON representation of log
     * @return Model representation of log
     */
    static Log from_json(const json::Value& json);
private:
    uint64_t m_id{};
};


}
}
}
}
