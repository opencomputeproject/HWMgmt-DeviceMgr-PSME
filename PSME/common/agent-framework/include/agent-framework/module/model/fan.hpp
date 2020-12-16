/*!
 * @copyright
 * Copyright (c) 2015-2017 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * @file fan.hpp
 * @brief Fan model interface
 * */
#pragma once
#include "agent-framework/module/model/attributes/model_attributes.hpp"
#include "agent-framework/module/model/resource.hpp"
#include "agent-framework/module/enum/chassis.hpp"
#include "agent-framework/module/enum/common.hpp"
#include "logger/logger_factory.hpp" 


namespace agent_framework {
namespace model {

/*! Fan */
class Fan : public Resource {
public:
    explicit Fan(const std::string& parent_uuid = {}, enums::Component parent_type = enums::Component::None);
    ~Fan();

    Fan(const Fan&) = default;
    Fan& operator=(const Fan&) = default;
    Fan(Fan&&) = default;
    Fan& operator=(Fan&&) = default;

    /*!
     * @brief construct an object of class Fan from JSON
     *
     * @param json the Json::Value deserialized to object
     *
     * @return the newly constructed Fan object
     */
    static Fan from_json(const Json::Value& json);

    /*!
     * @brief transform the object to JSon
     *
     * @return the object serialized to Json::Value
     */
    Json::Value to_json() const;

    /*!
     * @brief Get collection name
     * @return collection name
     */
    static enums::CollectionName get_collection_name() {
        return Fan::collection_name;
    }

    /*!
     * @brief Get component name
     * @return component name
     */
    static enums::Component get_component() {
        return Fan::component;
    }

    /*!
     * @brief return slot of type std::uint32_t
     *
     * @return the slot value
     */
    OptionalField<std::uint32_t> get_slot() const {
        return m_slot;
    }

    /*!
     * @brief setter for slot attribute
     *
     * @param slot of type uint32_t
     */
    void set_slot(const OptionalField<uint32_t> slot) {
        m_slot = slot;
    }

    /*!
     * @brief setter for fan_id attribute
     *
     * @param fan_id of type uint32_t
     */
    void set_fan_id(const OptionalField<uint32_t> fan_id) {
        m_fan_id = fan_id;
    }

    /*!
     * @brief return fan_id of type std::uint32_t
     *
     * @return the fan_id value
     */
    OptionalField<std::uint32_t> get_fan_id() const {
        return m_fan_id;
    }

    /*!
     * @brief return current_speed of type std::uint32_t
     *
     * @return the current_speed value
     */
    OptionalField<std::int32_t> get_current_speed() const {
        return m_current_speed;
    }

    /*!
     * @brief setter for current_speed attribute
     *
     * @param current_speed of type uint32_t
     */
    void set_current_speed(const OptionalField<int32_t> current_speed) {
        m_current_speed = current_speed;
    }


    /*!
     * @brief return fan_type of type std::uint32_t
     *
     * @return the fan_type value
     */
    OptionalField<std::int32_t> get_fan_type() const {
        return m_fan_type;
    }

    /*!
     * @brief setter for fan_type attribute
     *
     * @param fan_type of type uint32_t
     */
    void set_fan_type(const OptionalField<int32_t> thermal_type) {
        m_fan_type = thermal_type;
    }

    /*!
     * @brief return desired_speed of type std::uint32_t
     *
     * @return the desired_speed value
     */
    OptionalField<std::uint32_t> get_desired_speed() const {
        return m_desired_speed;
    }

    /*!
     * @brief setter for desired_speed attribute
     *
     * @param desired_speed of type uint32_t
     */
    void set_desired_speed(const OptionalField<uint32_t> desired_speed) {
        m_desired_speed = desired_speed;
    }

    /*!
     * @brief return an object fru_info of class attribute::FruInfo
     *
     * @return the fru_info value
     */
    const attribute::FruInfo& get_fru_info() const {
        return m_fru_info;
    }

    /*!
     * @brief setter for fru_info attribute
     *
     * @param fru_info of type attribute::FruInfo
     */
    void set_fru_info(const attribute::FruInfo& fru_info) {
        m_fru_info = fru_info;
    }

   /*!
     * @brief setter for chassis attribute
     *
     * @param chassis of type std::string
     */
    void set_chassis(const OptionalField<std::string>& chassis) {
        m_chassis = chassis;
            log_debug(GET_LOGGER("fan"), " set_chassis:" << m_chassis);
    }

    /*!
     * @brief return an object chassis of class std::string
     *
     * @return the chassis value
     */
    const OptionalField<std::string>& get_chassis() const {
               log_debug(GET_LOGGER("fan"), " get_chassis:" << m_chassis);
        return m_chassis;
    }

   /*!
     * @brief setter for status_health attribute
     *
     * @param status_health of type std::string
     */
    void set_status_health(const OptionalField<std::string>& status_health) {
        m_status_health = status_health;
    }

    /*!
     * @brief return an object status_health of class std::string
     *
     * @return the status_health value
     */
    const OptionalField<std::string>& get_status_health() const {
        return m_status_health;
    }

   /*!
     * @brief setter for status_state attribute
     *
     * @param status_state of type std::string
     */
    void set_status_state(const OptionalField<std::string>& status_state) {
        m_status_state = status_state;
    }

    /*!
     * @brief return an object status_state of class std::string
     *
     * @return the status_state value
     */
    const OptionalField<std::string>& get_status_state() const {
        return m_status_state;
    }

private:

    OptionalField<std::uint32_t> m_slot{0u};
    OptionalField<std::uint32_t> m_fan_id{0u};  
    OptionalField<std::int32_t> m_current_speed{0};
    OptionalField<std::int32_t> m_fan_type{0};	
    OptionalField<std::string> m_status_health{};
    OptionalField<std::string> m_status_state{};
    OptionalField<std::uint32_t> m_desired_speed{0u};
    attribute::FruInfo m_fru_info{};

    static const enums::CollectionName collection_name;
    static const enums::Component component;
    OptionalField<std::string> m_chassis{};  //Nick Added 

};

}
}

