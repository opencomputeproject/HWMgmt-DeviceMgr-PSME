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
namespace account {
	
ENUM(PrivilegeType, uint32_t, Login, ConfigureManager, ConfigureUsers,
        ConfigureSelf, ConfigureComponents);
	
	
namespace model {

/*!
 * @brief PrivilegeTypeVec PrivilegeType Vector type
 */
using PrivilegeTypeVec = std::vector<PrivilegeType>;

/*!
 * @brief PrivilegeTypes class
 */
class PrivilegeTypes {
public:
    /*!
     * @brief Add PrivilegeType type
     *
     * @param privilege_type
     */
    void add(PrivilegeType privilege_type);

    /*!
     * @brief Get privilege type collection
     */
    PrivilegeTypeVec get() const;
    

private:
    PrivilegeTypeVec m_privilege_types{};
};

/*!
 * @brief Role representation
 */
class Role final {
public:
    /*!
     * @brief Set role id
     *
     * @param role id
     */
    void set_id(uint64_t id) {
        m_id = id;
    }

    /*!
     * @brief Get role id
     *
     * @return role id
     */
    uint64_t get_id() const {
        return m_id;
    }

    /*!
     * @brief Set role name
     *
     * @param role name
     */
    void set_name(const std::string& name) {
        m_name = name;
    }

    /*!
     * @brief Get role name
     *
     * @return role name
     */
    const std::string& get_name() const {
        return m_name;
    }

    /*!
     * @brief Set role ispredefined
     *
     * @param role ispredefined
     */
    void set_ispredefined(const bool& ispredefined) {
        m_ispredefined = ispredefined;
    }

    /*!
     * @brief Get role ispredefined
     *
     * @return role ispredefined
     */
    const bool& get_ispredefined() const {
        return m_ispredefined;
    }

    /*!
     * @brief Set role privilege types
     *
     * @param privilege_types role privilege types
     */
    void set_privilege_types(const PrivilegeTypes& privilege_types) {
        m_privilege_types = privilege_types;
    }

    /*!
     * @brief Get role privilege types
     *
     * @return role privilege types
     */
    const PrivilegeTypes& get_privilege_types() const {
        return m_privilege_types;
    }

    /*!
     * @brief Set role roleid
     *
     * @param roleid Roleid
     */
    void set_roleid(const std::string& roleid) {
        m_roleid= roleid;
    }

    /*!
     * @brief Get role roleid
     *
     * @return roleid Roleid
     */
    const std::string& get_roleid() const {
        return m_roleid;
    }

    /*!
     * @brief Creates json representation of role
     *
     * @return JSON representation of role
     */
    json::Value to_json() const;

    /*!
     * @brief Creates model representation from role JSON
     *
     * @param json JSON representation of role
     * @return Model representation of role
     */
    static Role from_json(const json::Value& json);

private:
    uint64_t m_id{};
    std::string m_name{};
    bool m_ispredefined{};
    PrivilegeTypes m_privilege_types{};
    std::string m_roleid{};
};

/*! Role vector type */
using RoleVec = std::vector<Role>;

}
}
}
}
