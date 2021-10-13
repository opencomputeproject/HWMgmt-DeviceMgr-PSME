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
 * @file network.hpp
 * @brief Contains constants used by network agents
 * */

#pragma once
namespace agent_framework {
namespace model {
namespace literals {

/*!
 * @brief Class consisting of literals for NeighborInfo model objects
 * */
class NeighborInfo {
public:
    static constexpr const char NEIGHBOR_INFO[] = "neighborInfo";
    static constexpr const char SWITCH_IDENTIFIER[] = "switchIdentifier";
    static constexpr const char PORT_IDENTIFIER[] = "portIdentifier";
    static constexpr const char CABLE_ID[] = "cableId";
};

class TransInfo {
public:
    static constexpr const char NEIGHBOR_INFO[] = "neighborInfo";
    static constexpr const char SWITCH_IDENTIFIER[] = "switchIdentifier";
    static constexpr const char PORT_IDENTIFIER[] = "portIdentifier";
    static constexpr const char CABLE_ID[] = "cableId";

    static constexpr const char SFP_VENDOR_NAME[] = "SfpVendorName";
    static constexpr const char PART_NUMBER[] = "PartNumber";
    static constexpr const char SERIAL_NUMBER[] = "SerialNumber";
    static constexpr const char MANUFACTURE_DATE[] = "ManufactureDate";
	
    static constexpr const char TEMP_READING[] = "TemperatureReading";
    static constexpr const char TEMP_UPPER_THRESH_HOLD_FATAL[] = "TemperatureUpperThresholdFatal";
    static constexpr const char TEMP_UPPER_THRESH_HOLD_CRITICAL[] = "TemperatureUpperThresholdCritical";
    static constexpr const char TEMP_UPPER_THRESH_HOLD_NON_CRITICAL[] = "TemperatureUpperThresholdNonCritical";
    static constexpr const char TEMP_LOWER_THRESH_HOLD_FATAL[] = "TemperatureLowerThresholdFatal";
    static constexpr const char TEMP_LOWER_THRESH_HOLD_CRITICAL[] = "TemperatureLowerThresholdCritical";
    static constexpr const char TEMP_LOWER_THRESH_HOLD_NON_CRITICAL[] = "TemperatureLowerThresholdNonCritical";
    static constexpr const char TEMP_STATUS_STATE[] = "TemperatureState";
    static constexpr const char TEMP_STATUS_HEALTH[] ="TemperatureHealth";
    static constexpr const char TEMP_STATUS_HEALTH_ROLL_UP[] = "TemperatureHealthRollup";	

    static constexpr const char BIAS_CURRENT_READING[] = "BiasCurrentReading";
    static constexpr const char BIAS_CURRENT_UPPER_THRESH_HOLD_FATAL[] = "BiasCurrentUpperThresholdFatal";
    static constexpr const char BIAS_CURRENT_UPPER_THRESH_HOLD_CRITICAL[] = "BiasCurrentUpperThresholdCritical";
    static constexpr const char BIAS_CURRENT_UPPER_THRESH_HOLD_NON_CRITICAL[] = "BiasCurrentUpperThresholdNonCritical";
    static constexpr const char BIAS_CURRENT_LOWER_THRESH_HOLD_FATAL[] = "BiasCurrentLowerThresholdFatal";
    static constexpr const char BIAS_CURRENT_LOWER_THRESH_HOLD_CRITICAL[] = "BiasCurrentLowerThresholdCritical";
    static constexpr const char BIAS_CURRENT_LOWER_THRESH_HOLD_NON_CRITICAL[] = "BiasCurrentLowerThresholdNonCritical";
    static constexpr const char BIAS_CURRENT_STATUS_STATE[] = "BiasCurrentState";
    static constexpr const char BIAS_CURRENT_STATUS_HEALTH[] ="BiasCurrentHealth";
    static constexpr const char BIAS_CURRENT_STATUS_HEALTH_ROLL_UP[] = "BiasCurrentHealthRollup";		

    static constexpr const char VOLTAGE_READING[] = "VoltageReading";
    static constexpr const char VOLTAGE_UPPER_THRESH_HOLD_FATAL[] = "VoltageUpperThresholdFatal";
    static constexpr const char VOLTAGE_UPPER_THRESH_HOLD_CRITICAL[] = "VoltageUpperThresholdCritical";
    static constexpr const char VOLTAGE_UPPER_THRESH_HOLD_NON_CRITICAL[] = "VoltageUpperThresholdNonCritical";
    static constexpr const char VOLTAGE_LOWER_THRESH_HOLD_FATAL[] = "VoltageLowerThresholdFatal";
    static constexpr const char VOLTAGE_LOWER_THRESH_HOLD_CRITICAL[] = "VoltageLowerThresholdCritical";
    static constexpr const char VOLTAGE_LOWER_THRESH_HOLD_NON_CRITICAL[] = "VoltageLowerThresholdNonCritical";
    static constexpr const char VOLTAGE_STATUS_STATE[] = "VoltageState";
    static constexpr const char VOLTAGE_STATUS_HEALTH[] ="VoltageHealth";
    static constexpr const char VOLTAGE_STATUS_HEALTH_ROLL_UP[] = "VoltageHealthRollup";		
	

    static constexpr const char TX_POWER_READING[] = "TxPowerReading";
    static constexpr const char TX_POWER_UPPER_THRESH_HOLD_FATAL[] = "TxPowerUpperThresholdFatal";
    static constexpr const char TX_POWER_UPPER_THRESH_HOLD_CRITICAL[] = "TxPowerUpperThresholdCritical";
    static constexpr const char TX_POWER_UPPER_THRESH_HOLD_NON_CRITICAL[] = "TxPowerUpperThresholdNonCritical";
    static constexpr const char TX_POWER_LOWER_THRESH_HOLD_FATAL[] = "TxPowerLowerThresholdFatal";
    static constexpr const char TX_POWER_LOWER_THRESH_HOLD_CRITICAL[] = "TxPowerLowerThresholdCritical";
    static constexpr const char TX_POWER_LOWER_THRESH_HOLD_NON_CRITICAL[] = "TxPowerLowerThresholdNonCritical";	
    static constexpr const char TX_POWER_STATUS_STATE[] = "TxPowerState";
    static constexpr const char TX_POWER_STATUS_HEALTH[] ="TxPowerHealth";
    static constexpr const char TX_POWER_STATUS_HEALTH_ROLL_UP[] = "TxPowerHealthRollup";	

    static constexpr const char RX_POWER_READING[] = "RxPowerReading";
    static constexpr const char RX_POWER_UPPER_THRESH_HOLD_FATAL[] = "RxPowerUpperThresholdFatal";
    static constexpr const char RX_POWER_UPPER_THRESH_HOLD_CRITICAL[] = "RxPowerUpperThresholdCritical";
    static constexpr const char RX_POWER_UPPER_THRESH_HOLD_NON_CRITICAL[] = "RxPowerUpperThresholdNonCritical";
    static constexpr const char RX_POWER_LOWER_THRESH_HOLD_FATAL[] = "RxPowerLowerThresholdFatal";
    static constexpr const char RX_POWER_LOWER_THRESH_HOLD_CRITICAL[] = "RxPowerLowerThresholdCritical";
    static constexpr const char RX_POWER_LOWER_THRESH_HOLD_NON_CRITICAL[] = "RxPowerLowerThresholdNonCritical";	
    static constexpr const char RX_POWER_STATUS_STATE[] = "RxPowerState";
    static constexpr const char RX_POWER_STATUS_HEALTH[] ="RxPowerHealth";
    static constexpr const char RX_POWER_STATUS_HEALTH_ROLL_UP[] = "RxPowerHealthRollup";

    static constexpr const char SFP_10G_ETHERNET_COMPLIANCE_CODE[] = "10g40gEthernetComplianceCode";
    static constexpr const char SFP_FIBRE_CHANNEL_LINK_LENGTH[] = "FibreChannelLinkLength";
    static constexpr const char SFP_FIBRE_CHANNEL_TECHNOLOGY[] = "FibreChannelTechnology";
    static constexpr const char SFP_FIBRE_CHANNEL_TRANSMISSION_MEDIA[] = "FibreChannelTransmissionMedia";
    static constexpr const char SFP_FIBRE_CHANNEL_SPEED[] = "FibreChannelSpeed";

    static constexpr const char STATUS_STATE[] = "State";
    static constexpr const char STATUS_HEALTH[] ="Health";
    static constexpr const char STATUS_HEALTH_ROLL_UP[] = "HealthRollup";	
};

/*!
 * @brief Class consisting of literals for SwitchPortMembers
 * */
class SwitchPortMembers {
public:
    static constexpr const char PORT[] = "port";
    static constexpr const char MEMBERS[] = "members";
    static constexpr const char OEM[] = "oem";
};

/*!
 * @brief Class consisting of literals for EthernetSwitchPort model objects
 * */
class EthernetSwitchPort {
public:
    static constexpr const char PORT[] = "port";
    static constexpr const char SWITCH[] = "switch";
    static constexpr const char STATUS[] = "Status";
    static constexpr const char PORT_IDENTIFIER[] = "portIdentifier";
    static constexpr const char PORT_ID[] = "portId";	
    static constexpr const char MODE[] = "mode";
    static constexpr const char MEMBERS[] = "members";
    static constexpr const char PORT_CLASS[] = "portClass";
    static constexpr const char PORT_TYPE[] = "portType";
    static constexpr const char PORT_MODE[] = "portMode";
    static constexpr const char LINK_TECHNOLOGY[] = "linkTechnology";
    static constexpr const char LINK_SPEED_MBPS[] = "linkSpeedMbps";
    static constexpr const char MAX_SPEED_MBPS[] = "maxSpeedMbps";
    static constexpr const char OPERATIONAL_STATE[] = "operationalState";
    static constexpr const char ADMINISTRATIVE_STATE[] = "administrativeState";
    static constexpr const char PORT_WIDTH[] = "portWidth";
    static constexpr const char FRAME_SIZE[] = "frameSize";
    static constexpr const char AUTO_SENSE[] = "autoSense";
    static constexpr const char FULL_DUPLEX[] = "fullDuplex";
    static constexpr const char IS_MANAGEMENT_PORT[] = "isManagementPort";
    static constexpr const char LAST_ERROR_CODE[] = "lastErrorCode";
    static constexpr const char ERROR_CLEARED[] = "errorCleared";
    static constexpr const char LAST_STATE_CHANGE_TIME[] =
        "lastStateChangeTime";
    static constexpr const char MAC_ADDRESS[] = "macAddress";
    static constexpr const char IPV4_ADDRESS[] = "ipv4Address";
    static constexpr const char IPV6_ADDRESS[] = "ipv6Address";
    static constexpr const char NEIGHBOR_INFO[] = "neighborInfo";
    static constexpr const char NEIGHBOR_MAC[] = "neighborMac";
    static constexpr const char VLAN_ENABLE[] = "vlanEnable";
    static constexpr const char DEFAULT_VLAN[] = "defaultVlan";
    static constexpr const char COLLECTIONS[] = "collections";
    static constexpr const char OEM[] = "oem";
    static constexpr const char TRANS_INFO[] = "transInfo";
    static constexpr const char TRANS_PON_RX_PWR_INFO[] = "transInfo_rx_pwr_info";

    static constexpr const char SFP_VENDOR_NAME[] = "SFP Vendor Name";
    static constexpr const char PART_NUMBER[] = "Part Number";
    static constexpr const char SERIAL_NUMBER[] = "Serial Number";
    static constexpr const char MANUFACTURE_DATE[] = "Manufacture Date";

    static constexpr const char TEMPERATURE[] = "Temperature";
    static constexpr const char VOLTAGE[] = "Voltage";
    static constexpr const char BIAS_CURRENT[] ="BiasCurrent";
    static constexpr const char TX_POWER[] =  "TxPower";
    static constexpr const char RX_POWER[] = "RxPower";

    static constexpr const char READING[] = "Reading";
    static constexpr const char UPPER_THRESHOLD_FATAL[] = "UpperThresholdFatal";
    static constexpr const char UPPER_THRESHOLD_CRITICAL[] = "UpperThresholdCritical";
    static constexpr const char LOWER_THRESHOLD_CRITICAL[] = "LowerThresholdCritical";
    static constexpr const char LOWER_THRESHOLD_FATAL[] ="LowerThresholdFatal";
    static constexpr const char READING_UNIT[] ="ReadingUnits";
    static constexpr const char TX_DISABLED_STATE[] = "TxDisabledState";
};

/*!
 * @brief Class consisting of literals for Vlan model objects
 * */
class Vlan {
public:
    static constexpr const char SWITCH[] = "switch";
    static constexpr const char VLAN[] = "vlan";
    static constexpr const char STATUS[] = "status";
    static constexpr const char VLAN_ID[] = "vlanId";
    static constexpr const char VLAN_NAME[] = "vlanName";
    static constexpr const char VLAN_ENABLE[] = "vlanEnable";
    static constexpr const char OEM[] = "oem";
};

/*!
 * @brief Class consisting of literals for EthernetSwitchPortVlan model objects
 * */
class EthernetSwitchPortVlan {
public:
    static constexpr const char PORT_VLAN[] = "portVlan";
    static constexpr const char PORT[] = "port";
    static constexpr const char STATUS[] = "status";
    static constexpr const char VLAN_ID[] = "vlanId";
    static constexpr const char VLAN_NAME[] = "vlanName";
    static constexpr const char VLAN_ENABLE[] = "vlanEnable";
    static constexpr const char TAGGED[] = "tagged";
    static constexpr const char OEM[] = "oem";
};

/*!
 * @brief Class consisting of literals for EthernetSwitch model objects
 * */
class EthernetSwitch {
public:
    static constexpr const char SWITCH[] = "switch";
    static constexpr const char STATUS[] = "status";
    static constexpr const char SWITCH_IDENTIFIER[] = "switchIdentifier";
    static constexpr const char TECHNOLOGY[] = "technology";
    static constexpr const char MAC_ADDRESS[] = "macAddress";
    static constexpr const char FIRMWARE_NAME[] = "firmwareName";
    static constexpr const char FIRMWARE_VERSION[] = "firmwareVersion";
    static constexpr const char ROLE[] = "role";
    static constexpr const char FRU_INFO[] = "fruInfo";
    static constexpr const char MANUFACTURING_DATE[] = "manufacturingDate";
    static constexpr const char LOCATION[] = "location";
    static constexpr const char CHASSIS[] = "chassis";
    static constexpr const char MAX_ACL_NUMBER[] = "maxAclNumber";
    static constexpr const char COLLECTIONS[] = "collections";
    static constexpr const char OEM[] = "oem";
};

/*!
 * @brief Class consisting of literals for Remote model objects
 * */
class RemoteEthernetSwitch {
public:
    static constexpr const char SWITCH[] = "switch";
    static constexpr const char STATUS[] = "status";
    static constexpr const char SWITCH_IDENTIFIER[] = "switchIdentifier";
    static constexpr const char MAC_ADDRESS[] = "macAddress";
    static constexpr const char NEXT_HOP[] = "nextHop";
    static constexpr const char OEM[] = "oem";
};

/*!
 * @brief Class consisting of literals for NextHop model objects
 * */
class NextHop {
public:
    static constexpr const char METRIC[] = "metric";
    static constexpr const char PORT_IDENTIFIER[] = "portIdentifier";
    static constexpr const char IPV4_ADDRESS[] = "ipv4Address";
    static constexpr const char IPV6_ADDRESS[] = "ipv6Address";
};

/*!
 * @brief Class consisting of literals for ACL
 * */
class Acl {
public:
    static constexpr const char SWITCH[] = "switch";
    static constexpr const char PORTS[] = "ports";
    static constexpr const char OEM[] = "oem";
    static constexpr const char ACL[] = "acl";
    static constexpr const char STATUS[] = "status";
    static constexpr const char COLLECTIONS[] = "collections";
};

/*!
 * @brief Class consisting of literals for Rule
 * */
class AclRule {
public:
    static constexpr const char RULE[] = "rule";
    static constexpr const char STATUS[] = "status";
    static constexpr const char OEM[] = "oem";
    static constexpr const char RULE_ID[] = "ruleId";
    static constexpr const char ACTION[] = "action";
    static constexpr const char FORWARD_MIRROR_PORT[] = "forwardMirrorPort";
    static constexpr const char MIRRORED_PORTS[] = "mirroredPorts";
    static constexpr const char MIRROR_TYPE[] = "mirrorType";
    static constexpr const char VLAN_ID[] = "vlanId";
    static constexpr const char SOURCE_IP[] = "sourceIp";
    static constexpr const char DESTINATION_IP[] = "destinationIp";
    static constexpr const char SOURCE_MAC[] = "sourceMac";
    static constexpr const char DESTINATION_MAC[] = "destinationMac";
    static constexpr const char SOURCE_L4_PORT[] = "sourceL4Port";
    static constexpr const char DESTINATION_L4_PORT[] = "destinationL4Port";
    static constexpr const char PROTOCOL[] = "protocol";
    static constexpr const char ID[] = "id";
    static constexpr const char MASK[] = "mask";
    static constexpr const char ADDRESS[] = "address";
    static constexpr const char PORT[] = "port";
};

/*!
 * @brief Class consisting of literals for Static MACs
 */
class StaticMac {
public:
    static constexpr const char STATIC_MAC[] = "staticMac";
    static constexpr const char PORT[] = "port";
    static constexpr const char OEM[] = "oem";
    static constexpr const char STATUS[] = "status";
    static constexpr const char ADDRESS[] = "macAddress";
    static constexpr const char VLAN_ID[] = "vlanId";
};

}
}
}

