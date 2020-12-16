/*!
 * @section LICENSE
 *
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
 * @section DESCRIPTION
 *
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * */

#include "agent-framework/module/model/attributes/trans_info.hpp"
#include "agent-framework/module/constants/network.hpp"

using namespace agent_framework::model::attribute;

TransInfo::TransInfo() { }

TransInfo::~TransInfo() { }

Json::Value TransInfo::to_json() const {
    Json::Value result;

    result[literals::TransInfo::SFP_VENDOR_NAME] = get_spf_vendor_name();
    result[literals::TransInfo::PART_NUMBER] = get_part_number();
    result[literals::TransInfo::SERIAL_NUMBER] = get_serial_number();
    result[literals::TransInfo::MANUFACTURE_DATE] = get_manufacture_date();
	
    result[literals::TransInfo::TEMP_READING] = get_temp_reading();
    result[literals::TransInfo::TEMP_UPPER_THRESH_HOLD_FATAL] = get_temp_upper_th_fatal();
    result[literals::TransInfo::TEMP_UPPER_THRESH_HOLD_CRITICAL] = get_temp_upper_th_critical();
    result[literals::TransInfo::TEMP_UPPER_THRESH_HOLD_NON_CRITICAL] = get_temp_upper_th_non_critical();
    result[literals::TransInfo::TEMP_LOWER_THRESH_HOLD_FATAL] = get_temp_lower_th_fatal();
    result[literals::TransInfo::TEMP_LOWER_THRESH_HOLD_CRITICAL] = get_temp_lower_th_critical();
    result[literals::TransInfo::TEMP_LOWER_THRESH_HOLD_NON_CRITICAL] = get_temp_lower_th_non_critical();
    result[literals::TransInfo::TEMP_STATUS_STATE] = get_temp_status_state();
    result[literals::TransInfo::TEMP_STATUS_HEALTH] = get_temp_status_health();

    result[literals::TransInfo::BIAS_CURRENT_READING] = get_bias_current_reading();
    result[literals::TransInfo::BIAS_CURRENT_UPPER_THRESH_HOLD_FATAL] = get_bias_current_upper_th_fatal();
    result[literals::TransInfo::BIAS_CURRENT_UPPER_THRESH_HOLD_CRITICAL] = get_bias_current_upper_th_critical();
    result[literals::TransInfo::BIAS_CURRENT_UPPER_THRESH_HOLD_NON_CRITICAL] = get_bias_current_upper_th_non_critical();
    result[literals::TransInfo::BIAS_CURRENT_LOWER_THRESH_HOLD_FATAL] = get_bias_current_lower_th_fatal();
    result[literals::TransInfo::BIAS_CURRENT_LOWER_THRESH_HOLD_CRITICAL] = get_bias_current_lower_th_critical();
    result[literals::TransInfo::BIAS_CURRENT_LOWER_THRESH_HOLD_NON_CRITICAL] = get_bias_current_lower_th_non_critical();
    result[literals::TransInfo::BIAS_CURRENT_STATUS_STATE] = get_bias_current_status_state();
    result[literals::TransInfo::BIAS_CURRENT_STATUS_HEALTH] = get_bias_current_status_health();
	

    result[literals::TransInfo::VOLTAGE_READING] = get_voltage_reading();
    result[literals::TransInfo::VOLTAGE_UPPER_THRESH_HOLD_FATAL] = get_voltage_upper_th_fatal();
    result[literals::TransInfo::VOLTAGE_UPPER_THRESH_HOLD_CRITICAL] = get_voltage_upper_th_critical();
    result[literals::TransInfo::VOLTAGE_UPPER_THRESH_HOLD_NON_CRITICAL] = get_voltage_upper_th_non_critical();
    result[literals::TransInfo::VOLTAGE_LOWER_THRESH_HOLD_FATAL] = get_voltage_lower_th_fatal();
    result[literals::TransInfo::VOLTAGE_LOWER_THRESH_HOLD_CRITICAL] = get_voltage_lower_th_critical();
    result[literals::TransInfo::VOLTAGE_LOWER_THRESH_HOLD_NON_CRITICAL] = get_voltage_lower_th_non_critical();
    result[literals::TransInfo::VOLTAGE_STATUS_STATE] = get_voltage_status_state();
    result[literals::TransInfo::VOLTAGE_STATUS_HEALTH] = get_voltage_status_health();	

    result[literals::TransInfo::TX_POWER_READING] = get_tx_power_reading();
    result[literals::TransInfo::TX_POWER_UPPER_THRESH_HOLD_FATAL] = get_tx_power_upper_th_fatal();
    result[literals::TransInfo::TX_POWER_UPPER_THRESH_HOLD_CRITICAL] = get_tx_power_upper_th_critical();
    result[literals::TransInfo::TX_POWER_UPPER_THRESH_HOLD_NON_CRITICAL] = get_tx_power_upper_th_non_critical();
    result[literals::TransInfo::TX_POWER_LOWER_THRESH_HOLD_FATAL] = get_tx_power_lower_th_fatal();
    result[literals::TransInfo::TX_POWER_LOWER_THRESH_HOLD_CRITICAL] = get_tx_power_lower_th_critical();
    result[literals::TransInfo::TX_POWER_LOWER_THRESH_HOLD_NON_CRITICAL] = get_tx_power_lower_th_non_critical();
    result[literals::TransInfo::TX_POWER_STATUS_STATE] = get_tx_power_status_state();
    result[literals::TransInfo::TX_POWER_STATUS_HEALTH] = get_tx_power_status_health();	

    result[literals::TransInfo::RX_POWER_READING] = get_rx_power_reading();
    result[literals::TransInfo::RX_POWER_UPPER_THRESH_HOLD_FATAL] = get_rx_power_upper_th_fatal();
    result[literals::TransInfo::RX_POWER_UPPER_THRESH_HOLD_CRITICAL] = get_rx_power_upper_th_critical();
    result[literals::TransInfo::RX_POWER_UPPER_THRESH_HOLD_NON_CRITICAL] = get_rx_power_upper_th_non_critical();
    result[literals::TransInfo::RX_POWER_LOWER_THRESH_HOLD_FATAL] = get_rx_power_lower_th_fatal();
    result[literals::TransInfo::RX_POWER_LOWER_THRESH_HOLD_CRITICAL] = get_rx_power_lower_th_critical();
    result[literals::TransInfo::RX_POWER_LOWER_THRESH_HOLD_NON_CRITICAL] = get_rx_power_lower_th_non_critical();
    result[literals::TransInfo::RX_POWER_STATUS_STATE] = get_rx_power_status_state();
    result[literals::TransInfo::RX_POWER_STATUS_HEALTH] = get_rx_power_status_health();

	
    return result;
}

TransInfo TransInfo::from_json(const Json::Value& json) {
    attribute::TransInfo trnas_info;


    trnas_info.set_spf_vendor_name(json[literals::TransInfo::SFP_VENDOR_NAME]);
    trnas_info.set_part_number(json[literals::TransInfo::PART_NUMBER]);
    trnas_info.set_serial_number(json[literals::TransInfo::SERIAL_NUMBER]);
    trnas_info.set_manufacture_date(json[literals::TransInfo::MANUFACTURE_DATE]);

    trnas_info.set_temp_reading(json[literals::TransInfo::TEMP_READING]);
    trnas_info.set_temp_upper_th_fatal(json[literals::TransInfo::TEMP_UPPER_THRESH_HOLD_FATAL]);
    trnas_info.set_temp_upper_th_critical(json[literals::TransInfo::TEMP_UPPER_THRESH_HOLD_CRITICAL]);
    trnas_info.set_temp_upper_th_non_critical(json[literals::TransInfo::TEMP_UPPER_THRESH_HOLD_NON_CRITICAL]);
    trnas_info.set_temp_lower_th_fatal(json[literals::TransInfo::TEMP_LOWER_THRESH_HOLD_FATAL]);
    trnas_info.set_temp_lower_th_critical(json[literals::TransInfo::TEMP_LOWER_THRESH_HOLD_CRITICAL]);
    trnas_info.set_temp_lower_th_non_critical(json[literals::TransInfo::TEMP_LOWER_THRESH_HOLD_NON_CRITICAL]);
    trnas_info.set_temp_status_state(json[literals::TransInfo::TEMP_STATUS_STATE]);
    trnas_info.set_temp_status_health(json[literals::TransInfo::TEMP_STATUS_HEALTH]);

    trnas_info.set_bias_current_reading(json[literals::TransInfo::BIAS_CURRENT_READING]);
    trnas_info.set_bias_current_upper_th_fatal(json[literals::TransInfo::BIAS_CURRENT_UPPER_THRESH_HOLD_FATAL]);
    trnas_info.set_bias_current_upper_th_critical(json[literals::TransInfo::BIAS_CURRENT_UPPER_THRESH_HOLD_CRITICAL]);
    trnas_info.set_bias_current_upper_th_non_critical(json[literals::TransInfo::BIAS_CURRENT_UPPER_THRESH_HOLD_NON_CRITICAL]);
    trnas_info.set_bias_current_lower_th_fatal(json[literals::TransInfo::BIAS_CURRENT_LOWER_THRESH_HOLD_FATAL]);
    trnas_info.set_bias_current_lower_th_critical(json[literals::TransInfo::BIAS_CURRENT_LOWER_THRESH_HOLD_CRITICAL]);
    trnas_info.set_bias_current_lower_th_non_critical(json[literals::TransInfo::BIAS_CURRENT_LOWER_THRESH_HOLD_NON_CRITICAL]);
    trnas_info.set_bias_current_status_state(json[literals::TransInfo::BIAS_CURRENT_STATUS_STATE]);
    trnas_info.set_bias_current_status_health(json[literals::TransInfo::BIAS_CURRENT_STATUS_HEALTH]);

    trnas_info.set_voltage_reading(json[literals::TransInfo::VOLTAGE_READING]);
    trnas_info.set_voltage_upper_th_fatal(json[literals::TransInfo::VOLTAGE_UPPER_THRESH_HOLD_FATAL]);
    trnas_info.set_voltage_upper_th_critical(json[literals::TransInfo::VOLTAGE_UPPER_THRESH_HOLD_CRITICAL]);
    trnas_info.set_voltage_upper_th_non_critical(json[literals::TransInfo::VOLTAGE_UPPER_THRESH_HOLD_NON_CRITICAL]);
    trnas_info.set_voltage_lower_th_fatal(json[literals::TransInfo::VOLTAGE_LOWER_THRESH_HOLD_FATAL]);
    trnas_info.set_voltage_lower_th_critical(json[literals::TransInfo::VOLTAGE_LOWER_THRESH_HOLD_CRITICAL]);
    trnas_info.set_voltage_lower_th_non_critical(json[literals::TransInfo::VOLTAGE_LOWER_THRESH_HOLD_NON_CRITICAL]);
    trnas_info.set_voltage_status_state(json[literals::TransInfo::VOLTAGE_STATUS_STATE]);
    trnas_info.set_voltage_status_health(json[literals::TransInfo::VOLTAGE_STATUS_HEALTH]);

    trnas_info.set_tx_power_reading(json[literals::TransInfo::TX_POWER_READING]);
    trnas_info.set_tx_power_upper_th_fatal(json[literals::TransInfo::TX_POWER_UPPER_THRESH_HOLD_FATAL]);
    trnas_info.set_tx_power_upper_th_critical(json[literals::TransInfo::TX_POWER_UPPER_THRESH_HOLD_CRITICAL]);
    trnas_info.set_tx_power_upper_th_non_critical(json[literals::TransInfo::TX_POWER_UPPER_THRESH_HOLD_NON_CRITICAL]);
    trnas_info.set_tx_power_lower_th_fatal(json[literals::TransInfo::TX_POWER_LOWER_THRESH_HOLD_FATAL]);
    trnas_info.set_tx_power_lower_th_critical(json[literals::TransInfo::TX_POWER_LOWER_THRESH_HOLD_CRITICAL]);
    trnas_info.set_tx_power_lower_th_non_critical(json[literals::TransInfo::TX_POWER_LOWER_THRESH_HOLD_NON_CRITICAL]);
    trnas_info.set_tx_power_status_state(json[literals::TransInfo::TX_POWER_STATUS_STATE]);
    trnas_info.set_tx_power_status_health(json[literals::TransInfo::TX_POWER_STATUS_HEALTH]);

    trnas_info.set_rx_power_reading(json[literals::TransInfo::RX_POWER_READING]);
    trnas_info.set_rx_power_upper_th_fatal(json[literals::TransInfo::RX_POWER_UPPER_THRESH_HOLD_FATAL]);
    trnas_info.set_rx_power_upper_th_critical(json[literals::TransInfo::RX_POWER_UPPER_THRESH_HOLD_CRITICAL]);
    trnas_info.set_rx_power_upper_th_non_critical(json[literals::TransInfo::RX_POWER_UPPER_THRESH_HOLD_NON_CRITICAL]);
    trnas_info.set_rx_power_lower_th_fatal(json[literals::TransInfo::RX_POWER_LOWER_THRESH_HOLD_FATAL]);
    trnas_info.set_rx_power_lower_th_critical(json[literals::TransInfo::RX_POWER_LOWER_THRESH_HOLD_CRITICAL]);
    trnas_info.set_rx_power_lower_th_non_critical(json[literals::TransInfo::RX_POWER_LOWER_THRESH_HOLD_NON_CRITICAL]);
    trnas_info.set_rx_power_status_state(json[literals::TransInfo::RX_POWER_STATUS_STATE]);
    trnas_info.set_rx_power_status_health(json[literals::TransInfo::RX_POWER_STATUS_HEALTH]);	
	
    return trnas_info;
}

Onu_Trans_Rx_Pwr_Info::Onu_Trans_Rx_Pwr_Info() { }

Onu_Trans_Rx_Pwr_Info::~Onu_Trans_Rx_Pwr_Info() { }

Json::Value Onu_Trans_Rx_Pwr_Info::to_json() const {
    Json::Value result;

    result[literals::TransInfo::RX_POWER_READING] = get_rx_power_reading();
    result[literals::TransInfo::RX_POWER_UPPER_THRESH_HOLD_FATAL] = get_rx_power_upper_th_fatal();
    result[literals::TransInfo::RX_POWER_UPPER_THRESH_HOLD_CRITICAL] = get_rx_power_upper_th_critical();
    result[literals::TransInfo::RX_POWER_UPPER_THRESH_HOLD_NON_CRITICAL] = get_rx_power_upper_th_non_critical();
    result[literals::TransInfo::RX_POWER_LOWER_THRESH_HOLD_FATAL] = get_rx_power_lower_th_fatal();
    result[literals::TransInfo::RX_POWER_LOWER_THRESH_HOLD_CRITICAL] = get_rx_power_lower_th_critical();
    result[literals::TransInfo::RX_POWER_LOWER_THRESH_HOLD_NON_CRITICAL] = get_rx_power_lower_th_non_critical();
    result[literals::TransInfo::RX_POWER_STATUS_STATE] = get_rx_power_status_state();
    result[literals::TransInfo::RX_POWER_STATUS_HEALTH] = get_rx_power_status_health();
	
    return result;
}

Onu_Trans_Rx_Pwr_Info Onu_Trans_Rx_Pwr_Info::from_json(const Json::Value& json) {
    attribute::Onu_Trans_Rx_Pwr_Info trnas_info;

    trnas_info.set_rx_power_reading(json[literals::TransInfo::RX_POWER_READING]);
    trnas_info.set_rx_power_upper_th_fatal(json[literals::TransInfo::RX_POWER_UPPER_THRESH_HOLD_FATAL]);
    trnas_info.set_rx_power_upper_th_critical(json[literals::TransInfo::RX_POWER_UPPER_THRESH_HOLD_CRITICAL]);
    trnas_info.set_rx_power_upper_th_non_critical(json[literals::TransInfo::RX_POWER_UPPER_THRESH_HOLD_NON_CRITICAL]);
    trnas_info.set_rx_power_lower_th_fatal(json[literals::TransInfo::RX_POWER_LOWER_THRESH_HOLD_FATAL]);
    trnas_info.set_rx_power_lower_th_critical(json[literals::TransInfo::RX_POWER_LOWER_THRESH_HOLD_CRITICAL]);
    trnas_info.set_rx_power_lower_th_non_critical(json[literals::TransInfo::RX_POWER_LOWER_THRESH_HOLD_NON_CRITICAL]);
    trnas_info.set_rx_power_status_state(json[literals::TransInfo::RX_POWER_STATUS_STATE]);
    trnas_info.set_rx_power_status_health(json[literals::TransInfo::RX_POWER_STATUS_HEALTH]);	
	
    return trnas_info;
}
