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

#include "agent-framework/module/model/ethernet_switch_port.hpp"
#include "agent-framework/module/constants/network.hpp"

using namespace agent_framework::model;
using namespace agent_framework::model::utils;

const enums::Component EthernetSwitchPort::component = enums::Component::EthernetSwitchPort;
const enums::CollectionName EthernetSwitchPort::collection_name =
    enums::CollectionName::EthernetSwitchPorts;

EthernetSwitchPort::EthernetSwitchPort(const std::string &parent_uuid, enums::Component parent_type) : Resource{parent_uuid, parent_type} {}

EthernetSwitchPort::~EthernetSwitchPort() {}

Json::Value EthernetSwitchPort::to_json() const
{
    Json::Value result;
    result[literals::EthernetSwitchPort::STATUS] = get_status().to_json();
    result[literals::EthernetSwitchPort::PORT_IDENTIFIER] = get_port_identifier();
    result[literals::EthernetSwitchPort::PORT_ID] = get_port_id();
    result[literals::EthernetSwitchPort::PORT_CLASS] = get_port_class();
    result[literals::EthernetSwitchPort::PORT_TYPE] = get_port_type();
    result[literals::EthernetSwitchPort::PORT_MODE] = get_port_mode();
    result[literals::EthernetSwitchPort::LINK_TECHNOLOGY] = get_link_technology();
    result[literals::EthernetSwitchPort::LINK_SPEED_MBPS] = get_link_speed_mbps();
    result[literals::EthernetSwitchPort::MAX_SPEED_MBPS] = get_max_speed_mbps();
    result[literals::EthernetSwitchPort::OPERATIONAL_STATE] = get_operational_state();
    result[literals::EthernetSwitchPort::ADMINISTRATIVE_STATE] = get_administrative_state();
    result[literals::EthernetSwitchPort::PORT_WIDTH] = get_port_width();
    result[literals::EthernetSwitchPort::FRAME_SIZE] = get_frame_size();
    result[literals::EthernetSwitchPort::AUTO_SENSE] = get_auto_sense();
    result[literals::EthernetSwitchPort::FULL_DUPLEX] = get_full_duplex();
    result[literals::EthernetSwitchPort::IS_MANAGEMENT_PORT] = get_is_management_port();
    result[literals::EthernetSwitchPort::LAST_ERROR_CODE] = get_last_error_code();
    result[literals::EthernetSwitchPort::ERROR_CLEARED] = get_error_cleared();
    result[literals::EthernetSwitchPort::LAST_STATE_CHANGE_TIME] = get_last_state_change_time();
    result[literals::EthernetSwitchPort::MAC_ADDRESS] = get_mac_address();
    result[literals::EthernetSwitchPort::IPV4_ADDRESS] = get_ipv4_address().to_json();
    result[literals::EthernetSwitchPort::IPV6_ADDRESS] = get_ipv6_address().to_json();
    result[literals::EthernetSwitchPort::NEIGHBOR_INFO] = get_neighbor_info().to_json();
    result[literals::EthernetSwitchPort::NEIGHBOR_MAC] = get_neighbor_mac();
    result[literals::EthernetSwitchPort::VLAN_ENABLE] = get_vlan_enable();
    result[literals::EthernetSwitchPort::DEFAULT_VLAN] = get_default_vlan();
    result[literals::EthernetSwitchPort::COLLECTIONS] = get_collections().to_json();
    result[literals::EthernetSwitchPort::OEM] = get_oem().to_json();
    result[literals::EthernetSwitchPort::TRANS_INFO] = get_trans_info().to_json();
    result[literals::EthernetSwitchPort::TRANS_PON_RX_PWR_INFO] = get_pon_trans_rx_pwr_info().to_json();

    return result;
}

EthernetSwitchPort EthernetSwitchPort::from_json(const Json::Value &json)
{
    EthernetSwitchPort port;

    port.set_status(attribute::Status::from_json(json[literals::EthernetSwitchPort::STATUS]));
    port.set_port_identifier(json[literals::EthernetSwitchPort::PORT_IDENTIFIER]);
    port.set_port_id(json[literals::EthernetSwitchPort::PORT_ID]);
    port.set_port_class(json[literals::EthernetSwitchPort::PORT_CLASS]);
    port.set_port_type(json[literals::EthernetSwitchPort::PORT_TYPE]);
    port.set_port_mode(json[literals::EthernetSwitchPort::PORT_MODE]);
    port.set_link_technology(json[literals::EthernetSwitchPort::LINK_TECHNOLOGY]);
    port.set_link_speed_mbps(json[literals::EthernetSwitchPort::LINK_SPEED_MBPS]);
    port.set_max_speed_mbps(json[literals::EthernetSwitchPort::MAX_SPEED_MBPS]);
    port.set_operational_state(json[literals::EthernetSwitchPort::OPERATIONAL_STATE]);
    port.set_administrative_state(json[literals::EthernetSwitchPort::ADMINISTRATIVE_STATE]);
    port.set_port_width(json[literals::EthernetSwitchPort::PORT_WIDTH]);
    port.set_frame_size(json[literals::EthernetSwitchPort::FRAME_SIZE]);
    port.set_auto_sense(json[literals::EthernetSwitchPort::AUTO_SENSE]);
    port.set_full_duplex(json[literals::EthernetSwitchPort::FULL_DUPLEX]);
    port.set_is_management_port(json[literals::EthernetSwitchPort::IS_MANAGEMENT_PORT]);
    port.set_last_error_code(json[literals::EthernetSwitchPort::LAST_ERROR_CODE]);
    port.set_error_cleared(json[literals::EthernetSwitchPort::ERROR_CLEARED]);
    port.set_last_state_change_time(json[literals::EthernetSwitchPort::LAST_STATE_CHANGE_TIME]);
    port.set_mac_address(json[literals::EthernetSwitchPort::MAC_ADDRESS]);
    port.set_ipv4_address(attribute::Ipv4Address::from_json(json[literals::EthernetSwitchPort::IPV4_ADDRESS]));
    port.set_ipv6_address(attribute::Ipv6Address::from_json(json[literals::EthernetSwitchPort::IPV6_ADDRESS]));
    port.set_neighbor_info(attribute::NeighborInfo::from_json(json[literals::EthernetSwitchPort::NEIGHBOR_INFO]));
    port.set_neighbor_mac(json[literals::EthernetSwitchPort::NEIGHBOR_MAC]);
    port.set_vlan_enable(json[literals::EthernetSwitchPort::VLAN_ENABLE]);
    port.set_default_vlan(json[literals::EthernetSwitchPort::DEFAULT_VLAN]);
    port.set_collections(Collections::from_json(json[literals::EthernetSwitchPort::COLLECTIONS]));
    port.set_oem(attribute::Oem::from_json(json[literals::EthernetSwitchPort::OEM]));
    port.set_resource_hash(json);
    port.set_trans_info(attribute::TransInfo::from_json(json[literals::EthernetSwitchPort::TRANS_INFO]));
    port.set_trans_rx_pwr_info(attribute::Onu_Trans_Rx_Pwr_Info::from_json(json[literals::EthernetSwitchPort::TRANS_PON_RX_PWR_INFO]));

    return port;
}

json::Value EthernetSwitchPort::get_trans_info_json() const
{
    json::Value r(json::Value::Type::OBJECT);
    attribute::TransInfo tTransInfo;
    tTransInfo = m_trans_info;

    r[literals::EthernetSwitchPort::SFP_VENDOR_NAME] = tTransInfo.get_spf_vendor_name();
    r[literals::EthernetSwitchPort::PART_NUMBER] = tTransInfo.get_part_number();
    r[literals::EthernetSwitchPort::SERIAL_NUMBER] = tTransInfo.get_serial_number();
    r[literals::EthernetSwitchPort::MANUFACTURE_DATE] = tTransInfo.get_manufacture_date();

    //Temperature
    r[literals::EthernetSwitchPort::TEMPERATURE][literals::EthernetSwitchPort::READING_UNIT] = "Celsius";
    signed int rr = tTransInfo.get_temp_reading();
    double tmp = (rr / 1000.000);

    r[literals::EthernetSwitchPort::TEMPERATURE][literals::EthernetSwitchPort::READING] = tmp;

    rr = tTransInfo.get_temp_upper_th_fatal();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::TEMPERATURE][literals::EthernetSwitchPort::UPPER_THRESHOLD_FATAL] = tmp;

    rr = tTransInfo.get_temp_upper_th_critical();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::TEMPERATURE][literals::EthernetSwitchPort::UPPER_THRESHOLD_CRITICAL] = tmp;

    rr = tTransInfo.get_temp_lower_th_critical();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::TEMPERATURE][literals::EthernetSwitchPort::LOWER_THRESHOLD_CRITICAL] = tmp;

    rr = tTransInfo.get_temp_lower_th_fatal();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::TEMPERATURE][literals::EthernetSwitchPort::LOWER_THRESHOLD_FATAL] = tmp;

    r[literals::EthernetSwitchPort::TEMPERATURE][literals::EthernetSwitchPort::STATUS][literals::TransInfo::STATUS_STATE] = tTransInfo.get_temp_status_state();
    r[literals::EthernetSwitchPort::TEMPERATURE][literals::EthernetSwitchPort::STATUS][literals::TransInfo::STATUS_HEALTH] = tTransInfo.get_temp_status_health();

    //Voltage
    r[literals::EthernetSwitchPort::VOLTAGE][literals::EthernetSwitchPort::READING_UNIT] = "V";
    rr = tTransInfo.get_voltage_reading();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::VOLTAGE][literals::EthernetSwitchPort::READING] = tmp;

    rr = tTransInfo.get_voltage_upper_th_fatal();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::VOLTAGE][literals::EthernetSwitchPort::UPPER_THRESHOLD_FATAL] = tmp;

    rr = tTransInfo.get_voltage_upper_th_critical();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::VOLTAGE][literals::EthernetSwitchPort::UPPER_THRESHOLD_CRITICAL] = tmp;

    rr = tTransInfo.get_voltage_lower_th_critical();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::VOLTAGE][literals::EthernetSwitchPort::LOWER_THRESHOLD_CRITICAL] = tmp;

    rr = tTransInfo.get_voltage_lower_th_fatal();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::VOLTAGE][literals::EthernetSwitchPort::LOWER_THRESHOLD_FATAL] = tmp;

    r[literals::EthernetSwitchPort::VOLTAGE][literals::EthernetSwitchPort::STATUS][literals::TransInfo::STATUS_STATE] = tTransInfo.get_voltage_status_state();
    r[literals::EthernetSwitchPort::VOLTAGE][literals::EthernetSwitchPort::STATUS][literals::TransInfo::STATUS_HEALTH] = tTransInfo.get_voltage_status_health();

    // Biascurrent
    r[literals::EthernetSwitchPort::BIAS_CURRENT][literals::EthernetSwitchPort::READING_UNIT] = "mA";
    rr = tTransInfo.get_bias_current_reading();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::BIAS_CURRENT][literals::EthernetSwitchPort::READING] = tmp;

    rr = tTransInfo.get_bias_current_upper_th_fatal();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::BIAS_CURRENT][literals::EthernetSwitchPort::UPPER_THRESHOLD_FATAL] = tmp;

    rr = tTransInfo.get_bias_current_upper_th_critical();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::BIAS_CURRENT][literals::EthernetSwitchPort::UPPER_THRESHOLD_CRITICAL] = tmp;

    rr = tTransInfo.get_bias_current_lower_th_critical();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::BIAS_CURRENT][literals::EthernetSwitchPort::LOWER_THRESHOLD_CRITICAL] = tmp;

    rr = tTransInfo.get_bias_current_lower_th_fatal();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::BIAS_CURRENT][literals::EthernetSwitchPort::LOWER_THRESHOLD_FATAL] = tmp;

    r[literals::EthernetSwitchPort::BIAS_CURRENT][literals::EthernetSwitchPort::STATUS][literals::TransInfo::STATUS_STATE] = tTransInfo.get_bias_current_status_state();
    r[literals::EthernetSwitchPort::BIAS_CURRENT][literals::EthernetSwitchPort::STATUS][literals::TransInfo::STATUS_HEALTH] = tTransInfo.get_bias_current_status_health();

    // TxPower
    r[literals::EthernetSwitchPort::TX_POWER][literals::EthernetSwitchPort::READING_UNIT] = "mW";
    rr = tTransInfo.get_tx_power_reading();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::TX_POWER][literals::EthernetSwitchPort::READING] = tmp;

    rr = tTransInfo.get_tx_power_upper_th_fatal();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::TX_POWER][literals::EthernetSwitchPort::UPPER_THRESHOLD_FATAL] = tmp;

    rr = tTransInfo.get_tx_power_upper_th_critical();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::TX_POWER][literals::EthernetSwitchPort::UPPER_THRESHOLD_CRITICAL] = tmp;

    rr = tTransInfo.get_tx_power_lower_th_critical();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::TX_POWER][literals::EthernetSwitchPort::LOWER_THRESHOLD_CRITICAL] = tmp;

    rr = tTransInfo.get_tx_power_lower_th_fatal();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::TX_POWER][literals::EthernetSwitchPort::LOWER_THRESHOLD_FATAL] = tmp;


    r[literals::EthernetSwitchPort::TX_POWER][literals::EthernetSwitchPort::STATUS][literals::TransInfo::STATUS_STATE] = tTransInfo.get_tx_power_status_state();
    r[literals::EthernetSwitchPort::TX_POWER][literals::EthernetSwitchPort::STATUS][literals::TransInfo::STATUS_HEALTH] = tTransInfo.get_tx_power_status_health();

    // RxPower
    r[literals::EthernetSwitchPort::RX_POWER][literals::EthernetSwitchPort::READING_UNIT] = "mW";
    rr = tTransInfo.get_rx_power_reading();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::RX_POWER][literals::EthernetSwitchPort::READING] = tmp;

    rr = tTransInfo.get_rx_power_upper_th_fatal();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::RX_POWER][literals::EthernetSwitchPort::UPPER_THRESHOLD_FATAL] = tmp;

    rr = tTransInfo.get_rx_power_upper_th_critical();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::RX_POWER][literals::EthernetSwitchPort::UPPER_THRESHOLD_CRITICAL] = tmp;

    rr = tTransInfo.get_rx_power_lower_th_critical();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::RX_POWER][literals::EthernetSwitchPort::LOWER_THRESHOLD_CRITICAL] = tmp;

    rr = tTransInfo.get_rx_power_lower_th_fatal();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::RX_POWER][literals::EthernetSwitchPort::LOWER_THRESHOLD_FATAL] = tmp;

    r[literals::EthernetSwitchPort::RX_POWER][literals::EthernetSwitchPort::STATUS][literals::TransInfo::STATUS_STATE] = tTransInfo.get_rx_power_status_state();
    r[literals::EthernetSwitchPort::RX_POWER][literals::EthernetSwitchPort::STATUS][literals::TransInfo::STATUS_HEALTH] = tTransInfo.get_rx_power_status_health();

    return r;
}

json::Value EthernetSwitchPort::get_pon_trans_rx_pwr_info_json() const
{
    json::Value r(json::Value::Type::OBJECT);
    attribute::Onu_Trans_Rx_Pwr_Info tTransInfo;
    tTransInfo = m_pon_trans_rx_pwr_info;

    // RxPower
    signed int rr = tTransInfo.get_rx_power_reading();
    double tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::RX_POWER][literals::EthernetSwitchPort::READING] = tmp;

    rr = tTransInfo.get_rx_power_upper_th_fatal();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::RX_POWER][literals::EthernetSwitchPort::UPPER_THRESHOLD_FATAL] = tmp;

    rr = tTransInfo.get_rx_power_upper_th_critical();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::RX_POWER][literals::EthernetSwitchPort::UPPER_THRESHOLD_CRITICAL] = tmp;

    rr = tTransInfo.get_rx_power_lower_th_critical();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::RX_POWER][literals::EthernetSwitchPort::LOWER_THRESHOLD_CRITICAL] = tmp;

    rr = tTransInfo.get_rx_power_lower_th_fatal();
    tmp = (rr / 1000.000);
    r[literals::EthernetSwitchPort::RX_POWER][literals::EthernetSwitchPort::LOWER_THRESHOLD_FATAL] = tmp;

    r[literals::EthernetSwitchPort::RX_POWER][literals::EthernetSwitchPort::STATUS][literals::TransInfo::STATUS_STATE] = tTransInfo.get_rx_power_status_state();
    r[literals::EthernetSwitchPort::RX_POWER][literals::EthernetSwitchPort::STATUS][literals::TransInfo::STATUS_HEALTH] = tTransInfo.get_rx_power_status_health();

    return r;
}