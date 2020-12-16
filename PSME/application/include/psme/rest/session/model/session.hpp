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
namespace session {
namespace model {



/*!
 * @brief Subscription representation
 */
class Session final {
public:
    /*!
     * @brief Set session time stamp
     *
     * @param id session time stamp
     */
    void set_time_stamp(uint64_t time_stamp) {
        m_time_stamp = time_stamp;
    }

    /*!
     * @brief Get session time stamp
     *
     */
    uint64_t get_time_stamp() const{
        return m_time_stamp;
    }	

    /*!
     * @brief Get session id
     *
     * @return session id
     */
    uint64_t get_id() const {
        return m_id;
    }

    /*!
     * @brief Set session id
     *
     * @param id session id
     */
    void set_id(uint64_t id) {
        m_id = id;
    }


    /*!
     * @brief Set session authe_token
     *
     * @param name session authe_token
     */
    void set_authen_token(const std::string& authe_token) {
        m_authe_token = authe_token;
    }

    /*!
     * @brief Get session authe_token
     *
     * @return session authe_token
     */
    const std::string& get_authen_token() const {
        return m_authe_token;
    }

    /*!
     * @brief Set session password
     *
     * @param  session password
     */
    void set_password(const std::string& password) {
        m_password = password;
    }

    /*!
     * @brief Get session password
     *
     * @return session password
     */
    const std::string& get_password() const {
        return m_password;
    }

    /*!
     * @brief Set session username
     *
     * @param session username
     */
    void set_username(const std::string& username) {
        m_username = username;
    }

    /*!
     * @brief Get session username
     *
     * @return session username
     */
    const std::string& get_username() const {
        return m_username;
    }


    /*!
     * @brief Set session user role
     *
     * @param session user role
     */
    void set_userrole(const std::string& role) {
        m_role = role;
    }

    /*!
     * @brief Get session user role
     *
     * @return session user role
     */
    const std::string& get_userrole() const {
        return m_role;
    }

    /*!
     * @brief Creates json representation of session
     *
     * @return JSON representation of subscription
     */
    json::Value to_json() const;


    /*!
     * @brief Creates model representation from session JSON
     *
     * @param json JSON representation of session
     * @return Model representation of session
     */
    static Session from_json(const json::Value& json);

    uint64_t m_time_stamp{};

private:
    uint64_t m_id{};
    std::string m_authe_token{};
    std::string m_srcip{};
    std::string m_username{};
    std::string m_password{};
    std::string m_role{};
	
};


}
}
}
}
