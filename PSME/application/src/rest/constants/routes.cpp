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

#include "psme/rest/constants/routes.hpp"
#include "psme/rest/endpoints/path_builder.hpp"



using namespace psme::rest::constants;
using namespace psme::rest::endpoint;

// "/

const std::string Routes::BASE_ROOT =
    PathBuilder()
        .append("")
        .build();
// "/redfish
const std::string Routes::REDFISH_PATH =
    PathBuilder()
        .append(constants::PathParam::REDFISH)
        .build();

// "/redfish/v1"
const std::string Routes::ROOT_PATH =
    PathBuilder(constants::PathParam::BASE_URL)
        .build();

// "/redfish/v1/odata"
const std::string Routes::ODATA_SERVICE_DOCUMENT =
    PathBuilder(constants::PathParam::BASE_URL)
        .append(constants::PathParam::ODATA)
        .build();

// "/redfish/v1/RPVfoobar"
    const std::string Routes::RPVfoobar=
    PathBuilder(constants::PathParam::BASE_URL).append(constants::PathParam::RPVFOOBAR).build();

// "/redfish/v1/$metadata"
const std::string Routes::METADATA_ROOT_PATH =
    PathBuilder(constants::PathParam::BASE_URL)
        .append(constants::PathParam::METADATA_ROOT)
        .build();

// "/redfish/v1/metadata/{metadata_file:*}"
const std::string Routes::METADATA_PATH =
    PathBuilder(constants::PathParam::BASE_URL)
        .append(constants::PathParam::METADATA)
        .append_regex(constants::PathParam::METADATA_FILE, constants::PathParam::XML_REGEX)
        .build();

const std::string Routes::ACCOUNT_SERVICE_PATH =
    endpoint::PathBuilder(constants::PathParam::BASE_URL)
        .append(constants::Root::ACCOUNT_SERVICE)
        .build();
        
const std::string Routes::ACCOUNT_COLLECTION_PATH =
    PathBuilder(ACCOUNT_SERVICE_PATH)
        .append(constants::AccountService::ACCOUNTS)
        .build();
const std::string Routes::ROLE_COLLECTION_PATH =
    PathBuilder(ACCOUNT_SERVICE_PATH)
        .append(constants::AccountService::ROLES)
        .build();        

// "/redfish/v1/AccountService/Accounts/{accountId:[0-9]+}"
const std::string Routes::ACCOUNT_PATH =
    PathBuilder(ACCOUNT_COLLECTION_PATH)
        .append_regex(constants::PathParam::ACCOUNT_ID, constants::PathParam::USERNAME_REGEX)
        .build(); 
        
const std::string Routes::ROLE_PATH =
    PathBuilder(ROLE_COLLECTION_PATH)
        .append_regex(constants::PathParam::ROLE_ID, constants::PathParam::ROLENAME_REGEX)
        .build();                
        
// "/redfish/v1/TaskService"
const std::string Routes::TASK_SERVICE_PATH =
    endpoint::PathBuilder(constants::PathParam::BASE_URL)
        .append(constants::TaskService::TASK_SERVICE)
        .build();

// "/redfish/v1/TaskService/Tasks"
const std::string Routes::TASK_COLLECTION_PATH =
    PathBuilder(constants::PathParam::BASE_URL)
        .append(constants::TaskService::TASK_SERVICE)
        .append(constants::Root::TASKS)
        .build();

// "/redfish/v1/TaskService/Tasks/{TaskId:[0-9]+}"
const std::string Routes::TASK_PATH =
    PathBuilder(TASK_COLLECTION_PATH)
        .append_regex(constants::PathParam::TASK_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/TaskService/Tasks/{TaskId:[0-9]+}/Monitor"
const std::string Routes::MONITOR_PATH =
    PathBuilder(TASK_PATH)
        .append(constants::Monitor::MONITOR)
        .build();

// "/redfish/v1/Chassis"
const std::string Routes::CHASSIS_COLLECTION_PATH =
    PathBuilder(constants::PathParam::BASE_URL)
        .append(constants::Common::CHASSIS)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}"
const std::string Routes::CHASSIS_PATH =
    endpoint::PathBuilder(CHASSIS_COLLECTION_PATH)
        .append_regex(constants::PathParam::CHASSIS_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/Drives"
const std::string Routes::DRIVES_COLLECTION_PATH =
    PathBuilder(CHASSIS_PATH)
        .append(constants::Chassis::DRIVES)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/Drives/{driveId:[0-9]+}"
const std::string Routes::DRIVE_PATH =
    PathBuilder(DRIVES_COLLECTION_PATH)
        .append_regex(constants::PathParam::DRIVE_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/Drives/{driveId:[0-9]+}/Actions/Drive.SecureErase"
const std::string Routes::DRIVE_SECURE_ERASE_PATH =
    PathBuilder(DRIVE_PATH)
        .append(constants::Common::ACTIONS)
        .append(constants::PncDrive::SECURE_ERASE_ENDPOINT)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/PCIeDevices"
const std::string Routes::PCIE_DEVICES_COLLECTION_PATH =
    PathBuilder(CHASSIS_PATH)
        .append(constants::Chassis::PCIE_DEVICES)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/PCIeDevices/{deviceId:[0-9]+}"
const std::string Routes::PCIE_DEVICE_PATH =
    PathBuilder(PCIE_DEVICES_COLLECTION_PATH)
        .append_regex(constants::PathParam::DEVICE_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/PCIeDevices/{deviceId:[0-9]+}/Functions"
const std::string Routes::PCIE_FUNCTIONS_COLLECTION_PATH =
    PathBuilder(PCIE_DEVICE_PATH)
        .append(constants::PathParam::FUNCTIONS)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/PCIeDevices/{deviceId:[0-9]+}/Functions/{functionId:[0-9]+}"
const std::string Routes::PCIE_FUNCTION_PATH =
    PathBuilder(PCIE_FUNCTIONS_COLLECTION_PATH)
        .append_regex(constants::PathParam::FUNCTION_ID, constants::PathParam::ID_REGEX)
        .build();

/*Nick Added Begin: */
// "/redfish/v1/Chassis/{chassisId:[0-9]+}/Fans"
const std::string Routes::FANS_COLLECTION_PATH =
    PathBuilder(CHASSIS_PATH)
        .append(constants::Chassis::FANS)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/Power/PowerSupplies/"
const std::string Routes::PSU_COLLECTION_PATH =
    PathBuilder(CHASSIS_PATH)
        .append(constants::Chassis::PSU)    
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/Power/PowerSupplies/{PsuId:[0-9]+}"
const std::string Routes::PSUS_PATH =
    endpoint::PathBuilder(PSU_COLLECTION_PATH)   
        .append_regex(constants::PathParam::PSU_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/Thermals"
const std::string Routes::THERMALS_COLLECTION_PATH =
    PathBuilder(CHASSIS_PATH)
        .append(constants::Chassis::THERMALS)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/Thermal"
const std::string Routes::THERMAL_COLLECTION_PATH =
    PathBuilder(CHASSIS_PATH)
        .append(constants::Chassis::THERMAL)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/Thermal/Fans/{FanId:[0-9]+}"
const std::string Routes::FANS_PATH =
    endpoint::PathBuilder(THERMAL_COLLECTION_PATH)
        .append(constants::Chassis::FANS)
        .append_regex(constants::PathParam::FAN_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/Thermal/Temperatures/{ThermalId:[0-9]+}"
const std::string Routes::THERMALS_PATH =
    endpoint::PathBuilder(THERMAL_COLLECTION_PATH)
        .append(constants::Chassis::TEMPS)    
        .append_regex(constants::PathParam::THERMAL_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/SessionService"
const std::string Routes::SESSION_SERVICE_PATH =
    endpoint::PathBuilder(constants::PathParam::BASE_URL)
        .append(constants::SessionService::SESSION_SERVICE)
        .build();

// "/redfish/v1/SessionService/Services"
const std::string Routes::SESSION_COLLECTION_PATH =
    endpoint::PathBuilder(SESSION_SERVICE_PATH)
        .append(constants::SessionService::SERVICES)             
        .build();

// "/redfish/v1/SessionService/Services/Members"
const std::string Routes::MEMBERS_PATH =
    endpoint::PathBuilder(SESSION_COLLECTION_PATH)
        .append(constants::SessionService::MEMBERS)
        .build();

// "/redfish/v1/SessionService/Services/{SessionId:[a-zA-Z0-9_]+}"
const std::string Routes::SESSION_PATH =
    endpoint::PathBuilder(SESSION_COLLECTION_PATH)
        .append_regex(constants::PathParam::SESSION_ID, constants::PathParam::STRING_ID_REGEX)
        .build();

// "/redfish/v1/UpdateServices"
const std::string Routes::UPDATE_SERVICE_PATH =
    endpoint::PathBuilder(constants::PathParam::BASE_URL)
        .append(constants::UpdateService::UPDATE_SERVICE)      
        .build();


// "/redfish/v1/Chassis/{chassisId:[0-9]+}/Actions/Chassis.Reset"
const std::string Routes::CHASSIS_RESET_PATH =
    PathBuilder(CHASSIS_PATH)
        .append(constants::Common::ACTIONS)
        .append(constants::Chassis::CHASSIS_RESET)
        .build();

/*Nick Added End  : */

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/ThermalZones"
const std::string Routes::THERMAL_ZONE_COLLECTION_PATH =
    PathBuilder(CHASSIS_PATH)
        .append(constants::Chassis::THERMAL_ZONES)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/ThermalZones/{thermalZoneId:[0-9]+}"
const std::string Routes::THERMAL_ZONE_PATH =
    endpoint::PathBuilder(THERMAL_ZONE_COLLECTION_PATH)
        .append_regex(constants::PathParam::THERMAL_ZONE_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/PowerZones"
const std::string Routes::POWER_ZONE_COLLECTION_PATH =
    PathBuilder(CHASSIS_PATH)
        .append(constants::Chassis::POWER_ZONES)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/PowerZones/{powerZoneId:[0-9]+}"
const std::string Routes::POWER_ZONE_PATH =
    endpoint::PathBuilder(POWER_ZONE_COLLECTION_PATH)
        .append_regex(constants::PathParam::POWER_ZONE_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Managers"
const std::string Routes::MANAGER_COLLECTION_PATH =
    PathBuilder(constants::PathParam::BASE_URL)
        .append(constants::Root::MANAGERS)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}"
const std::string Routes::MANAGER_PATH =
    PathBuilder(MANAGER_COLLECTION_PATH)
        .append_regex(constants::PathParam::MANAGER_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}/EthernetInterfaces"
const std::string Routes::MANAGER_NETWORK_INTERFACE_COLLECTION_PATH =
    PathBuilder(MANAGER_PATH)
        .append(constants::Manager::ETHERNET_INTERFACES)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}/SerialInterfaces"
const std::string Routes::MANAGER_SERIAL_INTERFACE_COLLECTION_PATH =
    PathBuilder(MANAGER_PATH)
        .append(constants::Manager::SERIAL_INTERFACES)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}/EthernetInterfaces/{nicId:[0-9]+}"
const std::string Routes::MANAGER_NETWORK_INTERFACE_PATH =
    PathBuilder(MANAGER_NETWORK_INTERFACE_COLLECTION_PATH)
        .append_regex(constants::PathParam::NIC_ID, constants::PathParam::ID_REGEX)
        .build();


// "/redfish/v1/Managers/{managerId:[0-9]+}/EthernetInterfaces/{nicId:[0-9]+}/VLANs"
const std::string Routes::VLAN_MANAGER_NETWORK_INTERFACE_COLLECTION_PATH =
    PathBuilder(MANAGER_NETWORK_INTERFACE_PATH)
        .append(constants::EthernetSwitchPort::VLANS)
        .build();

// "/redfish/v1/Managers/{Managers:[0-9]+}/EthernetInterfaces/{nicId:[0-9]+}/VLANs/{vlanId:[0-9]+}"
const std::string Routes::VLAN_MANAGER_NETWORK_INTERFACE_PATH =
    PathBuilder(VLAN_MANAGER_NETWORK_INTERFACE_COLLECTION_PATH)
        .append_regex(constants::PathParam::VLAN_ID, constants::PathParam::ID_REGEX)
        .build();


// "/redfish/v1/Managers/{Managers:[0-9]+}/LogServices"
const std::string Routes::MANAGER_LOG_SERVICE_COLLECTION_PATH =
    PathBuilder(MANAGER_PATH)
        .append(constants::Manager::LOG_SERVICES)
        .build();

// "/redfish/v1/Managers/{Managers:[0-9]+}/LogServices/{logserId:[0-9]+}/
const std::string Routes::MANAGER_LOG_SERVICE_PATH =
    PathBuilder(MANAGER_LOG_SERVICE_COLLECTION_PATH)
        .append_regex(constants::PathParam::LOGSER_ID, constants::PathParam::ID_REGEX)
        .build();


// "/redfish/v1/Managers/{Managers:[0-9]+}/LogServices/{logserId:[0-9]+}/Entries/
const std::string Routes::MANAGER_LOG_SERVICE_ENTRIES_COLLECTION_PATH =
    PathBuilder(MANAGER_LOG_SERVICE_PATH)
        .append(constants::Manager::ENTRIES)
        .build();


// "/redfish/v1/Managers/{Managers:[0-9]+}/LogServices/{logserId:[0-9]+}/Entries/{logserId:[0-9]+}"
const std::string Routes::MANAGER_LOG_SERVICE_ENTRIES_PATH =
    PathBuilder(MANAGER_LOG_SERVICE_ENTRIES_COLLECTION_PATH)
        .append_regex(constants::PathParam::LOGSER_ID,constants::PathParam::ID_REGEX)      
        .build();

// "/redfish/v1/Managers/{Managers:[0-9]+}/LogServices/{LogId:[0-9]+}/Actions/LogService.Reset"
const std::string Routes::MANAGER_LOG_SERVICE_RESET_PATH =
    PathBuilder(MANAGER_LOG_SERVICE_PATH)
        .append(constants::Common::ACTIONS)
        .append(constants::Management::LOG_SERVICE_RESET)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}/SerialInterfaces/{nicId:[0-9]+}"
const std::string Routes::MANAGER_SERIAL_INTERFACE_PATH =
    PathBuilder(MANAGER_SERIAL_INTERFACE_COLLECTION_PATH)
        .append_regex(constants::PathParam::SER_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}/NetworkProtocol"
const std::string Routes::NETWORK_PROTOCOL_PATH =
    PathBuilder(MANAGER_PATH)
        .append(constants::Manager::NETWORK_PROTOCOL)
        .build();

// "/redfish/v1/Services"
const std::string Routes::STORAGE_SERVICE_COLLECTION_PATH =
    PathBuilder(constants::PathParam::BASE_URL)
        .append(constants::Root::SERVICES)
        .build();

// "/redfish/v1/Services/{serviceId:[0-9]+}"
const std::string Routes::STORAGE_SERVICE_PATH =
    PathBuilder(STORAGE_SERVICE_COLLECTION_PATH)
        .append_regex(constants::PathParam::SERVICE_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Services/{serviceId:[0-9]+}/Drives"
const std::string Routes::PHYSICAL_DRIVES_COLLECTION_PATH =
    PathBuilder(STORAGE_SERVICE_PATH)
        .append(constants::StorageService::DRIVES)
        .build();

// "/redfish/v1/Services/{serviceId:[0-9]+}/Drives/{driveId:[0-9]+}"
const std::string Routes::PHYSICAL_DRIVE_PATH =
    PathBuilder(PHYSICAL_DRIVES_COLLECTION_PATH)
        .append_regex(constants::PathParam::DRIVE_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Services/{serviceId:[0-9]+}/LogicalDrives"
const std::string Routes::LOGICAL_DRIVE_COLLECTION_PATH =
    PathBuilder(STORAGE_SERVICE_PATH)
        .append(constants::StorageService::LOGICAL_DRIVES)
        .build();

// "/redfish/v1/Services/{serviceId:[0-9]+}/LogicalDrives/{logicalDriveId:[0-9]+}"
const std::string Routes::LOGICAL_DRIVE_PATH =
    PathBuilder(LOGICAL_DRIVE_COLLECTION_PATH)
        .append_regex(constants::PathParam::LOGICAL_DRIVE_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Services/{serviceId:[0-9]+}/Targets"
const std::string Routes::REMOTE_TARGET_COLLECTION_PATH =
    PathBuilder(STORAGE_SERVICE_PATH)
        .append(constants::StorageService::TARGETS)
        .build();

// "/redfish/v1/Services/{serviceId:[0-9]+}/Targets/{remoteTargetId:[0-9]+}"
const std::string Routes::REMOTE_TARGET_PATH =
    PathBuilder(REMOTE_TARGET_COLLECTION_PATH)
        .append_regex(constants::PathParam::REMOTE_TARGET_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Fabrics"
const std::string Routes::FABRICS_COLLECTION_PATH =
    PathBuilder(constants::PathParam::BASE_URL)
        .append(constants::Root::FABRICS)
        .build();

// "/redfish/v1/Fabrics/{fabricId}"
const std::string Routes::FABRIC_PATH =
    PathBuilder(FABRICS_COLLECTION_PATH)
        .append_regex(constants::PathParam::FABRIC_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Fabrics/{fabricId}/Endpoints"
const std::string Routes::ENDPOINTS_COLLECTION_PATH =
    PathBuilder(FABRIC_PATH)
        .append(constants::Fabric::ENDPOINTS)
        .build();

// "/redfish/v1/Fabrics/{fabricId}/Endpoints/{endpointId}"
const std::string Routes::ENDPOINT_PATH =
    PathBuilder(ENDPOINTS_COLLECTION_PATH)
        .append_regex(constants::PathParam::ENDPOINT_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Fabrics/{fabricId}/Switches"
const std::string Routes::SWITCHES_COLLECTION_PATH =
    PathBuilder(FABRIC_PATH)
        .append(constants::Fabric::SWITCHES)
        .build();

// "/redfish/v1/Fabrics/{fabricId}/Switches/{switchId}"
const std::string Routes::SWITCH_PATH =
    PathBuilder(SWITCHES_COLLECTION_PATH)
        .append_regex(constants::PathParam::SWITCH_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Fabrics/{fabricId}/Switches/{switchId}/Actions/Switch.Reset"
const std::string Routes::SWITCH_RESET_PATH =
    PathBuilder(SWITCH_PATH)
        .append(constants::Common::ACTIONS)
        .append(constants::Switch::SWITCH_RESET_ENDPOINT)
        .build();

// "/redfish/v1/Fabrics/{fabricId}/Zones"
const std::string Routes::ZONES_COLLECTION_PATH =
    PathBuilder(FABRIC_PATH)
        .append(constants::Fabric::ZONES)
        .build();

// "/redfish/v1/Fabrics/{fabricId}/Zones/{zoneId:[0-9]+}"
const std::string Routes::ZONE_PATH =
    PathBuilder(ZONES_COLLECTION_PATH)
        .append_regex(constants::PathParam::ZONE_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Fabrics/{fabricId}/Switches/{switchId}/Ports"
const std::string Routes::PORTS_COLLECTION_PATH =
    PathBuilder(SWITCH_PATH)
        .append(constants::Switch::PORTS)
        .build();

// "/redfish/v1/Fabrics/{fabricId}/Switches/{switchId}/Ports/{portId:[0-9]+}"
const std::string Routes::PORT_PATH =
    PathBuilder(PORTS_COLLECTION_PATH)
        .append_regex(constants::PathParam::PORT_ID, constants::PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Fabrics/{fabricId}/Switches/{switchId}/Ports/{portId:[0-9]+}/Actions/Port.Reset"
const std::string Routes::PORT_RESET_PATH =
    PathBuilder(PORT_PATH)
        .append(constants::Common::ACTIONS)
        .append(constants::Port::PORT_RESET_ENDPOINT)
        .build();