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
 * @brief Thermal zone
 * */

#pragma once
#include "agent-framework/module/model/attributes/model_attributes.hpp"
#include "agent-framework/module/model/resource.hpp"
#include "agent-framework/module/enum/common.hpp"

namespace agent_framework {
namespace model {

/*! Thermal zone class */
class ThermalZone : public Resource {
public:

    explicit ThermalZone(const std::string& parent_uuid = {}, enums::Component parent_type = enums::Component::None);
    ~ThermalZone();

    ThermalZone(const ThermalZone&) = default;
    ThermalZone& operator=(const ThermalZone&) = default;
    ThermalZone(ThermalZone&&) = default;
    ThermalZone& operator=(ThermalZone&&) = default;

    /*!
     * @brief construct an object of class ThermalZone from JSON
     *
     * @param json the Json::Value deserialized to object
     *
     * @return the newly constructed ThermalZone object
     */
    static ThermalZone from_json(const Json::Value& json);

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
        return ThermalZone::collection_name;
    }

    /*!
     * @brief Get component name
     * @return component name
     */
    static enums::Component get_component() {
        return ThermalZone::component;
    }

    /*!
     * @brief return temperature of type double
     *
     * @return the temperature value
     */
    OptionalField<double> get_temperature() const {
        return m_temperature;
    }

    /*!
     * @brief setter for temperature attribute
     *
     * @param temperature of type double
     */
    void set_temperature(const OptionalField<double> temperature) {
        m_temperature = temperature;
    }

   /*!
     * @brief setter for chassis attribute
     *
     * @param chassis of type std::string
     */
    void set_chassis(const OptionalField<std::string>& chassis) {
        m_chassis = chassis;
            log_debug(GET_LOGGER("ThermalZone"), " set_chassis:" << m_chassis);
    }

    /*!
     * @brief return an object chassis of class std::string
     *
     * @return the chassis value
     */
    const OptionalField<std::string>& get_chassis() const {
               log_debug(GET_LOGGER("ThermalZone"), " get_chassis:" << m_chassis);
        return m_chassis;
    }


    /*!
     * @brief setter for fan_id attribute
     *
     * @param fan_id of type uint32_t
     */
    void set_tz_id(const OptionalField<uint32_t> fan_id) {
        m_tz_id = fan_id;
    }

    /*!
     * @brief return fan_id of type std::uint32_t
     *
     * @return the fan_id value
     */
    OptionalField<std::uint32_t> get_tz_id() const {
        return m_tz_id;
    }

 /*!
     * @brief return warning_lower_temp of type std::uint32_t
     *
     * @return the warning_lower_temp value
     */
    OptionalField<double> get_warning_lower_temp() const {
        return m_warning_lower_temp;
    }

    /*!
     * @brief setter for warning_lower_temp attribute
     *
     * @param warning_temp of type uint32_t
     */
    void set_warning_lower_temp(const double warning_temp) {
        m_warning_lower_temp = warning_temp;
    }

 /*!
     * @brief return warning_temp of type std::uint32_t
     *
     * @return the warning_upper_temp value
     */
    OptionalField<double> get_warning_upper_temp() const {
        return m_warning_upper_temp;
    }

    /*!
     * @brief setter for warning_upper_temp attribute
     *
     * @param warning_temp of type uint32_t
     */
    void set_warning_upper_temp(const double warning_temp) {
        m_warning_upper_temp = warning_temp;
    }

    /*!
     * @brief return error_temp of type std::uint32_t
     *
     * @return the error_temp value
     */
    OptionalField<double> get_error_temp() const {
        return m_error_temp;
    }

    /*!
     * @brief setter for error_temp attribute
     *
     * @param error_temp of type uint32_t
     */
    void set_error_temp(const double error_temp) {
        m_error_temp = error_temp;
    }


  /*!
     * @brief return shutdown_tempof type std::uint32_t
     *
     * @return the shutdown_temp value
     */
    OptionalField<double> get_shutdown_temp() const {
        return m_shutdown_temp;
    }

    /*!
     * @brief setter for shutdown_temp attribute
     *
     * @param shutdown_temp of type uint32_t
     */
    void set_shutdown_temp(const double shutdown_temp) {
        m_shutdown_temp = shutdown_temp;
    }

    /*!
     * @brief return thermal_type of type std::uint32_t
     *
     * @return the thermal_type value
     */
    OptionalField<std::int32_t> get_thermal_type() const {
        return m_thermal_type;
    }

    /*!
     * @brief setter for thermal_type attribute
     *
     * @param thermal_type of type uint32_t
     */
    void set_thermal_type(const OptionalField<int32_t> thermal_type) {
        m_thermal_type = thermal_type;
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

    OptionalField<std::uint32_t> m_tz_id{0u}; 
    OptionalField<std::string> m_chassis{}; 
    OptionalField<std::int32_t> m_thermal_type{0};	
    OptionalField<double> m_warning_upper_temp{0};	
    OptionalField<double> m_warning_lower_temp{0};	
    OptionalField<double> m_error_temp{0};	
    OptionalField<double> m_shutdown_temp{0};	
    OptionalField<std::string> m_status_health{};
    OptionalField<std::string> m_status_state{};
	
    OptionalField<double> m_temperature{0.0};

    static const enums::CollectionName collection_name;
    static const enums::Component component;

};

}
}
