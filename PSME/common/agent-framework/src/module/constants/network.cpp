
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
 * @file network.cpp
 * @brief Contains constants used by network agents
 * */

#include "agent-framework/module/constants/network.hpp"

namespace agent_framework {
namespace model {
namespace literals {

constexpr const char NeighborInfo::NEIGHBOR_INFO[];
constexpr const char NeighborInfo::SWITCH_IDENTIFIER[];
constexpr const char NeighborInfo::PORT_IDENTIFIER[];
constexpr const char NeighborInfo::CABLE_ID[];

constexpr const char TransInfo::NEIGHBOR_INFO[];
constexpr const char TransInfo::SWITCH_IDENTIFIER[];
constexpr const char TransInfo::PORT_IDENTIFIER[];
constexpr const char TransInfo::CABLE_ID[];


constexpr const char TransInfo::SFP_VENDOR_NAME[];
constexpr const char TransInfo::PART_NUMBER[];
constexpr const char TransInfo::SERIAL_NUMBER[];
constexpr const char TransInfo::MANUFACTURE_DATE[];

constexpr const char TransInfo::TEMP_READING[];
constexpr const char TransInfo::TEMP_UPPER_THRESH_HOLD_FATAL[];
constexpr const char TransInfo::TEMP_UPPER_THRESH_HOLD_CRITICAL[];
constexpr const char TransInfo::TEMP_UPPER_THRESH_HOLD_NON_CRITICAL[];
constexpr const char TransInfo::TEMP_LOWER_THRESH_HOLD_FATAL[];
constexpr const char TransInfo::TEMP_LOWER_THRESH_HOLD_CRITICAL[];
constexpr const char TransInfo::TEMP_LOWER_THRESH_HOLD_NON_CRITICAL[];
constexpr const char TransInfo::TEMP_STATUS_STATE[];
constexpr const char TransInfo::TEMP_STATUS_HEALTH[];
constexpr const char TransInfo::TEMP_STATUS_HEALTH_ROLL_UP[];

constexpr const char TransInfo::BIAS_CURRENT_READING[];
constexpr const char TransInfo::BIAS_CURRENT_UPPER_THRESH_HOLD_FATAL[];
constexpr const char TransInfo::BIAS_CURRENT_UPPER_THRESH_HOLD_CRITICAL[];
constexpr const char TransInfo::BIAS_CURRENT_UPPER_THRESH_HOLD_NON_CRITICAL[];
constexpr const char TransInfo::BIAS_CURRENT_LOWER_THRESH_HOLD_FATAL[];
constexpr const char TransInfo::BIAS_CURRENT_LOWER_THRESH_HOLD_CRITICAL[];
constexpr const char TransInfo::BIAS_CURRENT_LOWER_THRESH_HOLD_NON_CRITICAL[];
constexpr const char TransInfo::BIAS_CURRENT_STATUS_STATE[];
constexpr const char TransInfo::BIAS_CURRENT_STATUS_HEALTH[];
constexpr const char TransInfo::BIAS_CURRENT_STATUS_HEALTH_ROLL_UP[];

constexpr const char TransInfo::VOLTAGE_READING[];
constexpr const char TransInfo::VOLTAGE_UPPER_THRESH_HOLD_FATAL[];
constexpr const char TransInfo::VOLTAGE_UPPER_THRESH_HOLD_CRITICAL[];
constexpr const char TransInfo::VOLTAGE_UPPER_THRESH_HOLD_NON_CRITICAL[];
constexpr const char TransInfo::VOLTAGE_LOWER_THRESH_HOLD_FATAL[];
constexpr const char TransInfo::VOLTAGE_LOWER_THRESH_HOLD_CRITICAL[];
constexpr const char TransInfo::VOLTAGE_LOWER_THRESH_HOLD_NON_CRITICAL[];
constexpr const char TransInfo::VOLTAGE_STATUS_STATE[];
constexpr const char TransInfo::VOLTAGE_STATUS_HEALTH[];
constexpr const char TransInfo::VOLTAGE_STATUS_HEALTH_ROLL_UP[];

constexpr const char TransInfo::TX_POWER_READING[];
constexpr const char TransInfo::TX_POWER_UPPER_THRESH_HOLD_FATAL[];
constexpr const char TransInfo::TX_POWER_UPPER_THRESH_HOLD_CRITICAL[];
constexpr const char TransInfo::TX_POWER_UPPER_THRESH_HOLD_NON_CRITICAL[];
constexpr const char TransInfo::TX_POWER_LOWER_THRESH_HOLD_FATAL[];
constexpr const char TransInfo::TX_POWER_LOWER_THRESH_HOLD_CRITICAL[];
constexpr const char TransInfo::TX_POWER_LOWER_THRESH_HOLD_NON_CRITICAL[];	
constexpr const char TransInfo::TX_POWER_STATUS_STATE[];
constexpr const char TransInfo::TX_POWER_STATUS_HEALTH[];
constexpr const char TransInfo::TX_POWER_STATUS_HEALTH_ROLL_UP[];

constexpr const char TransInfo::RX_POWER_READING[];
constexpr const char TransInfo::RX_POWER_UPPER_THRESH_HOLD_FATAL[];
constexpr const char TransInfo::RX_POWER_UPPER_THRESH_HOLD_CRITICAL[];
constexpr const char TransInfo::RX_POWER_UPPER_THRESH_HOLD_NON_CRITICAL[];
constexpr const char TransInfo::RX_POWER_LOWER_THRESH_HOLD_FATAL[];
constexpr const char TransInfo::RX_POWER_LOWER_THRESH_HOLD_CRITICAL[];
constexpr const char TransInfo::RX_POWER_LOWER_THRESH_HOLD_NON_CRITICAL[];
constexpr const char TransInfo::RX_POWER_STATUS_STATE[];
constexpr const char TransInfo::RX_POWER_STATUS_HEALTH[];
constexpr const char TransInfo::RX_POWER_STATUS_HEALTH_ROLL_UP[];

constexpr const char TransInfo::STATUS_STATE[];
constexpr const char TransInfo::STATUS_HEALTH[];
constexpr const char TransInfo::STATUS_HEALTH_ROLL_UP[];

constexpr const char TransInfo::SFP_10G_ETHERNET_COMPLIANCE_CODE[];
constexpr const char TransInfo::SFP_FIBRE_CHANNEL_LINK_LENGTH[];
constexpr const char TransInfo::SFP_FIBRE_CHANNEL_TECHNOLOGY[];
constexpr const char TransInfo::SFP_FIBRE_CHANNEL_TRANSMISSION_MEDIA[];
constexpr const char TransInfo::SFP_FIBRE_CHANNEL_SPEED[];

constexpr const char SwitchPortMembers::PORT[];
constexpr const char SwitchPortMembers::MEMBERS[];
constexpr const char SwitchPortMembers::OEM[];

constexpr const char EthernetSwitchPort::PORT[];
constexpr const char EthernetSwitchPort::SWITCH[];
constexpr const char EthernetSwitchPort::STATUS[];
constexpr const char EthernetSwitchPort::PORT_IDENTIFIER[];
constexpr const char EthernetSwitchPort::PORT_ID[];
constexpr const char EthernetSwitchPort::MODE[];
constexpr const char EthernetSwitchPort::MEMBERS[];
constexpr const char EthernetSwitchPort::PORT_CLASS[];
constexpr const char EthernetSwitchPort::PORT_TYPE[];
constexpr const char EthernetSwitchPort::PORT_MODE[];
constexpr const char EthernetSwitchPort::LINK_TECHNOLOGY[];
constexpr const char EthernetSwitchPort::LINK_SPEED_MBPS[];
constexpr const char EthernetSwitchPort::MAX_SPEED_MBPS[];
constexpr const char EthernetSwitchPort::OPERATIONAL_STATE[];
constexpr const char EthernetSwitchPort::ADMINISTRATIVE_STATE[];
constexpr const char EthernetSwitchPort::PORT_WIDTH[];
constexpr const char EthernetSwitchPort::FRAME_SIZE[];
constexpr const char EthernetSwitchPort::AUTO_SENSE[];
constexpr const char EthernetSwitchPort::FULL_DUPLEX[];
constexpr const char EthernetSwitchPort::IS_MANAGEMENT_PORT[];
constexpr const char EthernetSwitchPort::LAST_ERROR_CODE[];
constexpr const char EthernetSwitchPort::ERROR_CLEARED[];
constexpr const char EthernetSwitchPort::LAST_STATE_CHANGE_TIME[];
constexpr const char EthernetSwitchPort::MAC_ADDRESS[];
constexpr const char EthernetSwitchPort::IPV4_ADDRESS[];
constexpr const char EthernetSwitchPort::IPV6_ADDRESS[];
constexpr const char EthernetSwitchPort::NEIGHBOR_INFO[];
constexpr const char EthernetSwitchPort::TRANS_INFO[];
constexpr const char EthernetSwitchPort::TRANS_PON_RX_PWR_INFO[];

constexpr const char EthernetSwitchPort::SFP_VENDOR_NAME[];
constexpr const char EthernetSwitchPort::PART_NUMBER[];
constexpr const char EthernetSwitchPort::SERIAL_NUMBER[];
constexpr const char EthernetSwitchPort::MANUFACTURE_DATE[];

constexpr const char EthernetSwitchPort::TEMPERATURE[];
constexpr const char EthernetSwitchPort::VOLTAGE[];
constexpr const char EthernetSwitchPort::BIAS_CURRENT[];
constexpr const char EthernetSwitchPort::TX_POWER[];
constexpr const char EthernetSwitchPort::RX_POWER[];

constexpr const char EthernetSwitchPort::READING[];
constexpr const char EthernetSwitchPort::UPPER_THRESHOLD_FATAL[];
constexpr const char EthernetSwitchPort::UPPER_THRESHOLD_CRITICAL[];
constexpr const char EthernetSwitchPort::LOWER_THRESHOLD_CRITICAL[];
constexpr const char EthernetSwitchPort::LOWER_THRESHOLD_FATAL[];
constexpr const char EthernetSwitchPort::READING_UNIT[];

constexpr const char EthernetSwitchPort::NEIGHBOR_MAC[];
constexpr const char EthernetSwitchPort::VLAN_ENABLE[];
constexpr const char EthernetSwitchPort::DEFAULT_VLAN[];
constexpr const char EthernetSwitchPort::COLLECTIONS[];
constexpr const char EthernetSwitchPort::OEM[];

constexpr const char EthernetSwitch::SWITCH[];
constexpr const char EthernetSwitch::STATUS[];
constexpr const char EthernetSwitch::SWITCH_IDENTIFIER[];
constexpr const char EthernetSwitch::TECHNOLOGY[];
constexpr const char EthernetSwitch::MAC_ADDRESS[];
constexpr const char EthernetSwitch::FIRMWARE_NAME[];
constexpr const char EthernetSwitch::FIRMWARE_VERSION[];
constexpr const char EthernetSwitch::ROLE[];
constexpr const char EthernetSwitch::FRU_INFO[];
constexpr const char EthernetSwitch::MANUFACTURING_DATE[];
constexpr const char EthernetSwitch::LOCATION[];
constexpr const char EthernetSwitch::CHASSIS[];
constexpr const char EthernetSwitch::MAX_ACL_NUMBER[];
constexpr const char EthernetSwitch::COLLECTIONS[];
constexpr const char EthernetSwitch::OEM[];

constexpr const char EthernetSwitchPortVlan::PORT_VLAN[];
constexpr const char EthernetSwitchPortVlan::PORT[];
constexpr const char EthernetSwitchPortVlan::STATUS[];
constexpr const char EthernetSwitchPortVlan::VLAN_ID[];
constexpr const char EthernetSwitchPortVlan::VLAN_NAME[];
constexpr const char EthernetSwitchPortVlan::VLAN_ENABLE[];
constexpr const char EthernetSwitchPortVlan::TAGGED[];
constexpr const char EthernetSwitchPortVlan::OEM[];

constexpr const char Vlan::VLAN[];
constexpr const char Vlan::SWITCH[];
constexpr const char Vlan::STATUS[];
constexpr const char Vlan::VLAN_ID[];
constexpr const char Vlan::VLAN_NAME[];
constexpr const char Vlan::VLAN_ENABLE[];
constexpr const char Vlan::OEM[];

constexpr const char RemoteEthernetSwitch::SWITCH[];
constexpr const char RemoteEthernetSwitch::STATUS[];
constexpr const char RemoteEthernetSwitch::SWITCH_IDENTIFIER[];
constexpr const char RemoteEthernetSwitch::MAC_ADDRESS[];
constexpr const char RemoteEthernetSwitch::NEXT_HOP[];
constexpr const char RemoteEthernetSwitch::OEM[];

constexpr const char NextHop::METRIC[];
constexpr const char NextHop::PORT_IDENTIFIER[];
constexpr const char NextHop::IPV4_ADDRESS[];
constexpr const char NextHop::IPV6_ADDRESS[];

constexpr const char Acl::SWITCH[];
constexpr const char Acl::PORTS[];
constexpr const char Acl::OEM[];
constexpr const char Acl::ACL[];
constexpr const char Acl::STATUS[];
constexpr const char Acl::COLLECTIONS[];

constexpr const char AclRule::RULE[];
constexpr const char AclRule::STATUS[];
constexpr const char AclRule::OEM[];
constexpr const char AclRule::RULE_ID[];
constexpr const char AclRule::ACTION[];
constexpr const char AclRule::FORWARD_MIRROR_PORT[];
constexpr const char AclRule::MIRRORED_PORTS[];
constexpr const char AclRule::MIRROR_TYPE[];
constexpr const char AclRule::VLAN_ID[];
constexpr const char AclRule::SOURCE_IP[];
constexpr const char AclRule::DESTINATION_IP[];
constexpr const char AclRule::SOURCE_MAC[];
constexpr const char AclRule::DESTINATION_MAC[];
constexpr const char AclRule::SOURCE_L4_PORT[];
constexpr const char AclRule::DESTINATION_L4_PORT[];
constexpr const char AclRule::PROTOCOL[];
constexpr const char AclRule::ID[];
constexpr const char AclRule::ADDRESS[];
constexpr const char AclRule::MASK[];
constexpr const char AclRule::PORT[];

constexpr const char StaticMac::STATIC_MAC[];
constexpr const char StaticMac::PORT[];
constexpr const char StaticMac::OEM[];
constexpr const char StaticMac::STATUS[];
constexpr const char StaticMac::ADDRESS[];
constexpr const char StaticMac::VLAN_ID[];

}
}
}
