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
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * */

#include "psme/rest/constants/constants.hpp"



namespace psme {
namespace rest {
namespace constants {

namespace Metadata {
const char METADATA_ROOT_FILE[] = "metadata_root.xml";
}

namespace PathParam {
const char BASE_ROOT[] = "/";
const char REDFISH[] = "redfish";
const char METADATA_ROOT[] = "$metadata";
const char METADATA[] = "metadata";
const char BASE_URL[] = "/redfish/v1";
const char METADATA_FILE[] = "metadataFile";
const char SUBSCRIPTION_ID[] = "subscriptionId";
const char ACCOUNT_ID[]= "accountId"; //viclin add
const char ROLE_ID[]= "roleId"; //viclin add
const char CHASSIS_ID[] = "chassisId";
const char THERMAL_ZONE_ID[] = "thermalZoneId";
const char POWER_ZONE_ID[] = "powerZoneId";
const char SYSTEM_ID[] = "systemId";
const char ADAPTER_ID[] = "adapterId";
const char DEVICE_ID[] = "deviceId";
const char SERVICE_ID[] = "serviceId";
const char DRIVE_ID[] = "driveId";
const char LOGICAL_DRIVE_ID[] = "logicalDriveId";
const char REMOTE_TARGET_ID[] = "remoteTargetId";
const char MANAGER_ID[] = "managerId";
const char NIC_ID[] = "nicId";
const char NETWORK_INTERFACE_ID[] = "networkInterfaceId";
const char NETWORK_DEVICE_FUNCTION_ID[] = "networkDeviceFunctionId";
const char NODE_ID[] = "nodeId";
const char ETHERNET_SWITCH_ID[] = "ethernetSwitchId";
const char SWITCH_PORT_ID[] = "switchPortId";
const char VLAN_ID[] = "vlanId";
const char PROCESSOR_ID[] = "processorId";
const char SIMPLE_STORAGE_ID[] = "simpleStorageId";
const char MEMORY_ID[] = "memoryId";
const char FABRIC_ID[] = "fabricId";
const char ENDPOINT_ID[] = "endpointId";
const char SWITCH_ID[] = "switchId";
const char FUNCTION_ID[] = "functionId";
const char ZONE_ID[] = "zoneId";
const char PORT_ID[] = "portId";
const char FUNCTIONS[] = "Functions";
const char ACL_ID[] = "aclId";
const char RULE_ID[] = "ruleId";
const char STORAGE_ID[] = "storageId";
const char STATIC_MAC_ID[] = "staticMacId";
const char TASK_ID[] = "taskId";
const char ODATA[] = "odata";
const char RPVFOOBAR[] = "RPVfoobar";
const char MESSAGE_REGISTRY_FILE_ID[] = "messageRegistryFileId";

const char PATH_SEP = '/';
const char VARIABLE_BEGIN = '{';
const char VARIABLE_END = '}';
const char REGEX_MARK = ':';
const char ID_REGEX[] = "[0-9]+";
const char STRING_ID_REGEX[] = "[a-zA-Z0-9_]+";
const char USERNAME_REGEX[] = "[a-zA-Z0-9_]+";
const char ROLENAME_REGEX[] = "[a-zA-Z0-9_]+";
const char XML_REGEX[] = ".*.xml";
const char FAN_ID[] = "FanId";  
const char PSU_ID[] = "PsuId";  
const char THERMAL_ID[] = "ThermalId";
const char SESSION_ID[] = "SessionId";
const char SER_ID[] = "serId";
const char LOGSER_ID[] = "logserId";
const char ONU_ID[] = "onuId";
}

namespace Olt
{
const char BAL_STATE[] = "BalState";
const char OLT_OPTR_STATE[] = "OltOperateState";
const char OMCI[] = "Omci";
const char FLOWID_REGEX[] = "[a-zA-Z0-9_]+";
} // namespace Olt

namespace OFlow
{
const char FLOW_ID[] = "FlowId";
const char NNI_ID[] = "NniId";
const char PORT_ID[] = "PortId";
const char FLOW_TYPE[] = "FlowType";
const char PKT_TAG_TYPE[] = "PktTagType";
const char GEMPORT_ID[] = "GemportId";
const char CLASSIFIER[] = "Classifier";
const char ACTION[] = "Action";
const char ACTION_CMD[] = "ActionCmd";
const char ACTION_VAL[] = "ActionVal";
const char CLASS_VAL[] = "ClassVal";
const char OVID[] = "OVid";
const char OPBITS[] = "OPbits";
const char OTPID[] = "OTpid";
const char IVID[] = "IVid";
const char IPBITS[] = "IPbits";
const char ITPID[] = "ITpid";
const char ETHERTYPE[] = "EtherType";
const char IPPROTO[] = "IpProto";
const char SRCPORT[] = "SrcPort";
const char DSTPORT[] = "DstPort";
} // namespace OltFlow

namespace Common {
const char INTEL[] = "Intel Corporation";
const char ODATA_CONTEXT[] = "@odata.context";
const char ODATA_ID[] = "@odata.id";
const char ODATA_TYPE[] = "@odata.type";
const char ID[] = "Id";
const char MEMBER_ID[] = "MemberId";
const char NAME[] = "Name";
const char LINKS[] = "Links";
const char MANUFACTURER[] = "Manufacturer";
const char MODEL[] = "Model";
const char SERIAL[] = "SerialNumber";
const char PART_NUMBER[] = "PartNumber";
const char ASSET_TAG[] = "AssetTag";
const char SERVICE_TAG[] = "ServiceTag";
const char STATUS[] = "Status";
const char STATE[] = "State";
const char HEALTH[] = "Health";
const char HEALTH_ROLLUP[] = "HealthRollup";
const char ENUM_STATUS[] = "EnumStatus";
const char OEM[] = "Oem";
const char RACKSCALE[] = "Intel_RackScale";
const char API_VERSION[] = "ApiVersion";
const char LOCATION[] = "Location";
const char ACT_OEM[] = "Accton_Oem";
const char CONTAINED_BY[] = "ContainedBy";
const char MANAGED_BY[] = "ManagedBy";
const char COOLED_BY[] = "CooledBy";
const char POWERED_BY[] = "PoweredBy";
const char DESCRIPTION[] = "Description";
const char VENDOR_ID[] = "VendorId";
const char DEVICE_ID[] = "DeviceId";
const char FIRMWARE_VERSION[] = "FirmwareVersion";
const char UUID[] = "UUID";
const char ACTIONS[] = "Actions";
const char HASH[] = "#";
const char CHASSIS[] = "Chassis";
const char RESET_TYPE[] = "ResetType";
const char MAC_ADDRESS[] = "MACAddress";
const char USER_NAME[] = "UserName";
const char PASSWORD[] = "Password";
const char ENTRIES[] = "Entries";
const char SPARE_PART_NUMBER[] = "SparePartNumber";
}
namespace ChassisThreshold
{
const char LOWERTHRESHOLDNONCRITICAL[] = "LowerThresholdNonCritical";
const char UPPERTHRESHOLDNONCRITICAL[] = "UpperThresholdNonCritical";
const char UPPERTHRESHOLDCRITICAL[] = "UpperThresholdCritical";
const char UPPERTHRESHOLDFATAL[] = "UpperThresholdFatal";
const char MEMBER_ID[] = "MemberId";
const char TEMPERATURES[] = "Temperatures";
}

namespace ErrorMessage {
const char ERROR[] = "error";
const char CODE[] = "code";
const char MESSAGE[] = "message";
const char EXTENDED_INFO[] = "@Message.ExtendedInfo";
}

namespace MessageObject {
const char JSON_POINTER_PREFIX[] = "#/";
const char MESSAGE_ID[] = "MessageId";
const char MESSAGE[] = "Message";
const char MESSAGE_ARGS[] = "MessageArgs";
const char RELATED_PROPERTIES[] = "RelatedProperties";
const char SEVERITY[] = "Severity";
const char RESOLUTION[] = "Resolution";
}

namespace Collection {
const char ODATA_COUNT[] = "Members@odata.count";
const char MEMBERS[] = "Members";
}

namespace Root {
const char REDFISH_VERSION[] = "RedfishVersion";
const char EVENT_SERVICE[] = "EventService";
const char TASKS[] = "Tasks";
const char SYSTEMS[] = "Systems";
const char SERVICES[] = "Services";
const char MANAGERS[] = "Managers";
const char ETHERNET_SWITCHES[] = "EthernetSwitches";
const char ETHERNET_INTERFACE[]= "EthernetInterface";
const char REGISTRIES[] = "Registries";
const char FABRICS[] = "Fabrics";
const char ACCOUNT_SERVICE[] = "AccountService";
const char FANS[]="Fans" ; 
const char THERMALZONES[]="ThermalZones" ;
const char POWERZONES[]="PowerZones" ;
const char PSUS[]="PowerSupplies" ; 
const char PCTRL[] = "PowerControl";
const char PVOLTAGE[] = "Voltages";
const char PSU[]="Power" ; 
const char THERMALS[]="Thermals" ;
const char TEMPS[]="Temperatures" ;
const char THERMAL[]="Thermal" ;
const char SESSION_SERVICE[] = "SessionService";
}

namespace Redfish {
const char V1[] = "v1";
}

namespace OdataServiceDocument {
const char VALUE[] = "value";
const char NAME[] = "name";
const char KIND[] = "kind";
const char URL[] = "url";
}

namespace Manager {
const char TARGET[] = "target";
const char MANAGER_TYPE[] = "ManagerType";
const char POWER_STATE[] = "PowerState";
const char DATE_TIME[] = "DateTime";
const char DATE_TIME_LOCAL_OFFSET[] = "DateTimeLocalOffset";
const char SERVICE_ENTRY_POINT_UUID[] = "ServiceEntryPointUUID";
const char GRAPHICAL_CONSOLE[] = "GraphicalConsole";
const char MAX_CONCURRENT_SESSIONS[] = "MaxConcurrentSessions";
const char CONNECT_TYPES_SUPPORTED[] = "ConnectTypesSupported";
const char ENABLED[] = "Enabled";
const char SERVICE_ENABLED[] = "ServiceEnabled";
const char SERIAL_CONSOLE[] = "SerialConsole";
const char COMMAND_SHELL[] = "CommandShell";
const char MANAGER_FOR_CHASSIS[] = "ManagerForChassis";
const char MANAGER_FOR_SERVERS[] = "ManagerForServers";
const char MANAGER_FOR_SWITCHES[] = "ManagerForSwitches";
const char MANAGER_IN_CHASSIS[] = "ManagerInChassis";
const char MANAGER_FOR_SERVICES[] = "ManagerForServices";
const char MANAGER_FOR_REMOTE_TARGETS[] = "ManagerForRemoteTargets";
const char MANAGER_FOR_LOGICAL_DRIVES[] = "ManagerForLogicalDrives";
const char MANAGER_FOR_PHYSICAL_DRIVES[] = "ManagerForPhysicalDrives";
const char NETWORK_PROTOCOL[] = "NetworkProtocol";
const char MANAGER_FOR_CTRL_X86_IP[] = "Controller-x86-IP";
const char MANAGER_FOR_CTRL_BMC_IP[] = "Controller-BMC-IP";
const char ETHERNET_INTERFACES[] = "EthernetInterfaces";
const char SERIAL_INTERFACES[] = "SerialInterfaces";
const char LOG_SERVICES[] = "LogServices";
const char ENTRIES[] = "Entries";

const char MAX_NUM_RECS[] = "MaxNumberOfRecords";
const char OVERWRITE_POLICY[] = "OverWritePolicy";

const char ENTRYTYPE[] = "EntryType";
const char OEM_RECORD_FORMAT[] = "OemRecordFormat";
const char SEVERITY[] = "Severity";
const char CREATED[] = "Created";
const char ENTRY_CODE[] = "EntryCode";
const char SENSOR_TYPE[] = "SensorType";
const char SENSOR_NUMBER[] = "SensorNumber";
const char MESSAGE[] = "Message";
}

namespace NetworkProtocol {
const char HOST_NAME[] = "HostName";
const char FQDN[] = "FQDN";
const char HTTP[] = "HTTP";
const char HTTPS[] = "HTTPS";
const char SNMP[] = "SNMP";
const char VIRTUAL_MEDIA[] = "VirtualMedia";
const char TELNET[] = "Telnet";
const char SSDP[] = "SSDP";
const char IPMI[] = "IPMI";
const char SSH[] = "SSH";
const char KVMIP[] = "KVMIP";
const char PROTOCOL_ENABLED[] = "ProtocolEnabled";
const char PORT[] = "Port";
const char NOTIFY_IPV6_SCOPE[] = "NotifyIPv6Scope";
const char NOTIFY_MULTICAST_INTERVAL[] = "NotifyMulticastIntervalSeconds";
const char NOTIFY_TTL[] = "NotifyTTL";
}

namespace IpAddress {
const char ADDRESS[] = "Address";
const char ADDRESS_ORIGIN[] = "AddressOrigin";
const char GATEWAY[] = "Gateway";
const char HOST_NAME[] = "HostName";
const char FQDN[] = "FQDN";
const char SUBNET_MASK[] = "SubnetMask";
const char PREFIX_LENGTH[] = "PrefixLength";
const char SCOPE[] = "Scope";
const char ADDRESS_STATE[] = "AddressState";
}

namespace NetworkInterface {
const char MAC_ADDRESS[] = "MACAddress";
const char PERMANENT_MAC_ADDRESS[] = "PermanentMACAddress";
const char SPEED[] = "SpeedMbps";
const char AUTO_NEG[] = "AutoNeg";
const char FULL_DUPLEX[] = "FullDuplex";
const char MTU_SIZE[] = "MTUSize";
const char HOST_NAME[] = "HostName";
const char FQDN[] = "FQDN";
const char MAX_IPv6_STATIC_ADDRESSES[] = "MaxIPv6StaticAddresses";
const char IPv4_ADDRESSES[] = "IPv4Addresses";
const char IPv4_STACIC_ADDRESSES[] = "IPv4StaticAddresses";
const char IPv6_ADDRESS_POLICY_TABLE[] = "IPv6AddressPolicyTable";
const char IPv4_DHCP[] = "DHCPv4";
const char IPv4_DHCP_ENABLED[] = "DHCPEnabled";
const char IPv4_DHCP_USE_DNS_SERVERS[] = "UseDNSServers";
const char IPv4_DHCP_USE_DOMAIN_NAME[] = "UseDomainName";
const char IPv4_DHCP_USE_GATEWAY[] = "UseGateway";
const char IPv4_DHCP_USE_NTP_SERVERS[] = "UseNTPServers";
const char IPv4_DHCP_USE_STATIC_ROUTES[] = "UseStaticRoutes";
const char IPv4_DHCP_CONFIG[] = "DHCPv4Configuration";
const char STATELESS_ADDRESS_AUTO_CONFIG[] = "StatelessAddressAutoConfig";

const char PREFIX[] = "Prefix";
const char PRECEDENCE[] = "Precedence";
const char LABEL[] = "Label";
const char IPv6_STATIC_ADDRESSES[] = "IPv6StaticAddresses";
const char IPv6_ADDRESSES[] = "IPv6Addresses";
const char IPv6_DEFAULT_GATEWAY[] = "IPv6DefaultGateway";
const char IPv6_AUTO_CONFIG_ENABLED[] = "IPv6AutoConfigEnabled";


const char IPv6_Static_DEFAULT_GATEWAY[] = "IPv6StaticDefaultGateways";
const char IPv6_DHCP[] = "DHCPv6";
const char IPv6_DHCP_OPERATINMODE[] = "OperatingMode";
const char IPv6_DHCP_USE_DNS_SERVERS[] = "UseDNSServers";
const char IPv6_DHCP_USE_NTP_SERVERS[] = "UseNTPServers";
const char IPv6_DHCP_USE_RAPID_COMMIT[] = "UseRapidCommit";

const char NAME_SERVERS[] = "NameServers";
const char STATIC_NAME_SERVERS[] = "StaticNameServers";
const char INTERFACE_ENABLED[] = "InterfaceEnabled";
const char UEFI_DEVICE_PATH[] = "UefiDevicePath";
const char VLANS[] = "VLANs";
const char VLAN[] = "VLAN";
const char NEIGHBOR_PORT[] = "NeighborPort";
const char LINK_STATUS[] = "LinkStatus";
}

namespace EventService {
const char SERVICE_ENABLED[] = "ServiceEnabled";
const char DELIVERY_RETRY_ATTEMPTS[] = "DeliveryRetryAttempts";
const char DELIVERY_RETRY_INTERVAL_SECONDS[] = "DeliveryRetryIntervalSeconds";
const char EVENT_TYPES_FOR_SUBSCRIPTION[] = "EventTypesForSubscription";
const char STATUS_CHANGE[] = "StatusChange";
const char RESOURCE_UPDATED[] = "ResourceUpdated";
const char RESOURCE_ADDED[] = "ResourceAdded";
const char RESOURCE_REMOVED[] = "ResourceRemoved";
const char ALERT[] = "Alert";
const char SUBSCRIPTIONS[] = "Subscriptions";
const char ACTIONS[] = "Actions";
const char SEND_TEST_EVENT[] = "#EventService.SendTestEvent";
const char TARGET[] = "target";
const char ALLOWABLE_EVENT_TYPES[] = "EventType@Redfish.AllowableValues";
}

namespace AccountService {
const char  SERVICE_ENABLED[] = "ServiceEnabled";
const char  AUTHFAILURELOGGINGTHRESHOLD[] = "AuthFailureLoggingThreshold" ;
const char  MINPASSWORDLENGTH[] = "MinPasswordLength";
const char  MAXPASSWORDLENGTH[] = "MaxPasswordLength";
const char  ACCOUNTLOCKOUTTHRESHOLD[] = "AccountLockoutThreshold" ;
const char  ACCOUNTLOCKOUTDURATION[] = "AccountLockoutDuration" ;
const char  ACCOUNTLOCKOUTCOUNTERRESETAFTER[] = "AccountLockoutCounterResetAfter" ;
const char  ACCOUNTS[] = "Accounts";
const char  ROLES[]= "Roles";
}
namespace AccountConst{
const char ENABLED[] = "Enabled";
const char SALT[] = "Salt";
const char PASSWORD[] = "Password";
const char USERNAME[] = "UserName";
const char ROLEID[] = "RoleId";
const char LOCKED[] = "Locked";
const char ROLE[] = "Role";

}

namespace AccountRole {
const char ISPREDEFINED[] = "IsPredefined" ;
const char ROLEID[] = "RoleId" ;
const char PRIVILEGE_TYPES[] = "AssignedPrivileges";
	
}

namespace EventSubscription {
const char ID[] = "Id";
const char DESTINATION[] = "Destination";
const char CONTEXT[] = "Context";
const char PROTOCOL[] = "Protocol";
const char EVENT_TYPES[] = "EventTypes";
}

namespace Capability {
extern const char CHASSIS[] = "Chassis";
extern const char COMPUTE[] = "Compute";
extern const char NETWORK[] = "Network";
extern const char STORAGE[] = "Storage";
}

namespace TaskService {
const char TASK_SERVICE[] = "TaskService";
const char SERVICE_ENABLED[] = "ServiceEnabled";
const char DATETIME[] = "DateTime";
const char COMPLETED_TASK_OVERWRITE_POLICY[] = "CompletedTaskOverWritePolicy";
const char LIFE_CYCLE_EVENT_ON_TASK_STATE_CHANGE[] = "LifeCycleEventOnTaskStateChange";
const char TASKS[] = "Tasks";
}

namespace SessionService {
const char SESSION_SERVICE[] = "SessionService";
const char SERVICE_ENABLED[] = "ServiceEnabled";
const char BASICAUTHEN_ENABLED[] = "BasicAuthenEnabled";
const char SERVICE_TIMEOUT[] = "SessionTimeout";
const char SERVICE_MAXSESSIONS[] = "SessionMaxSessions";
const char SERVICES[] = "Sessions";
const char MEMBERS[] = "Members";
const char ENABLED[] = "Enabled";
const char PASSWORD[] = "Password";
const char USERNAME[] = "UserName";
const char AUTHENTOKEN[] = "AuthenToken";
const char TIMESTAMP[] = "TimeStamp";
}
namespace UpdateService {
const char UPDATE_SERVICE[] = "UpdateService";
const char FIRMWARE_INVENTORY[] = "FirmwareInventory";
const char MULTI_UPDATER[] = "MU";
const char NOS[] = "NOS";
const char UPDATE_STATE[] = "UpdateState";
const char IMAGE_URI[] = "ImageURI";
}

namespace SerialInterface {
const char INTERFACE_ENABLED[] = "InterfaceEnabled";
const char SIGNAL_TYPE[] = "SignalType";
const char BIT_RATE[] = "BitRate";
const char PARITY[] = "Parity";
const char DATA_BIT[] = "DataBits";
const char STOP_BITS[] = "StopBits";
const char FLOW_CONTROL[] = "FlowControl";
const char CONNECTOR_TYPE[] = "ConnectorType";
const char PIN_OUT[] = "PinOut";
}

namespace Task {
const char TASK_STATE[] = "TaskState";
const char START_TIME[] = "StartTime";
const char END_TIME[] = "EndTime";
const char TASK_STATUS[] = "TaskStatus";
const char MESSAGES[] = "Messages";
}

namespace Monitor {
const char MONITOR[] = "Monitor";
}

namespace MessageRegistryFile {
const char LANGUAGES[] = "Languages";
const char REGISTRY[] = "Registry";
const char LOCATION[] = "Location";
const char LANGUAGE[] = "Language";
const char URI[] = "Uri";
const char ARCHIVE_URI[] = "ArchiveUri";
const char PUBLICATION_URI[] = "PublicationUri";
const char ARCHIVE_FILE[] = "ArchiveFile";
}

namespace ThermalZoneCollection
{
const char FANS[] = "Fans";
const char FAN_NAME[] = "FanName";
const char PHYSICAL_CONTEXT[] = "PhysicalContext";
const char READING[] = "Reading";
const char READING_UNITS[] = "ReadingUnits";
const char DESIRED_SPEED_RPM[] = "DesiredSpeedRPM";
const char MAX_FANS_SUPPORTED[] = "MaxFansSupported";
const char PWM[] = "PWM";
const char POLICY[] = "Policy";
const char TEMPERATURES[] = "Temperatures";
const char READING_CELSIUS[] = "ReadingCelsius";
const char SENSOR_NUMBER[] = "SensorNumber";
const char STATUS[] = "State";
const char HEALTH[] = "Health";
const char UPPER_THRESHOLD_FATAL[] = "UpperThresholdFatal";
const char UPPER_THRESHOLD_CRITICAL[] = "UpperThresholdCritical";
const char LOWER_THRESHOLD_CRITICAL[] = "LowerThresholdCritical";
const char LOWER_THRESHOLD_FATAL[] = "LowerThresholdFatal";
const char RELATED_ITEM[] = "RelatedItem";
} // namespace ThermalZone
}
}
}
