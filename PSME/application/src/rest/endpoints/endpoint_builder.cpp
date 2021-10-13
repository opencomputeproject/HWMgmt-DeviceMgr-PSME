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

#include "psme/rest/endpoints/endpoint_builder.hpp"
#include "psme/rest/endpoints/endpoints.hpp"
#include "psme/rest/server/multiplexer.hpp"
#include "psme/rest/server/utils.hpp"

using namespace psme::rest;
using namespace psme::rest::endpoint;
using namespace psme::rest::server;

EndpointBuilder::~EndpointBuilder() {}

void EndpointBuilder::build_endpoints(psme::rest::server::Multiplexer& mp) {
    mp.use_before([this](const Request&, Response& res) {
        res.set_header(ContentType::CONTENT_TYPE, ContentType::JSON);
    });

    // "/
    mp.register_handler(Redfish::UPtr(new Base_root(constants::Routes::BASE_ROOT)), AccessType::ALL);

    // "/redfish
    mp.register_handler(Redfish::UPtr(new Redfish(constants::Routes::REDFISH_PATH)), AccessType::ALL);

    // "/redfish/v1"
    mp.register_handler(Root::UPtr(new Root(constants::Routes::ROOT_PATH)), AccessType::ALL);

    // "/redfish/v1/odata"
    mp.register_handler(Root::UPtr(new OdataServiceDocument(constants::Routes::ODATA_SERVICE_DOCUMENT)), AccessType::ALL);

    // "/redfish/v1/RPVfoobar"
    mp.register_handler(Root::UPtr(new RPVfoobar(constants::Routes::RPVfoobar)), AccessType::ALL);

    // "/redfish/v1/$metadata"
    mp.register_handler(MetadataRoot::UPtr(new MetadataRoot(constants::Routes::METADATA_ROOT_PATH)), AccessType::ALL);

    // "/redfish/v1/metadata/{metadata_file:*}"
    mp.register_handler(Metadata::UPtr(new Metadata(constants::Routes::METADATA_PATH)), AccessType::ALL);

    // "/redfish/v1/AccountService"
    mp.register_handler(AccountService::UPtr(new AccountService(constants::Routes::ACCOUNT_SERVICE_PATH)));
    
    // "/redfish/v1/AccountService/Accounts"
    mp.register_handler(
        AccountCollection::UPtr(new AccountCollection(constants::Routes::ACCOUNT_COLLECTION_PATH)));    
    
    // "/redfish/v1/AccountService/Roles"
    mp.register_handler(
        AccountRoleCollection::UPtr(new AccountRoleCollection(constants::Routes::ROLE_COLLECTION_PATH)));
        
    // "/redfish/v1/AccountService/Accounts/{accountId:[0-9]+}"
    mp.register_handler(
        Account::UPtr(new Account(constants::Routes::ACCOUNT_PATH)));  
        
        // "/redfish/v1/AccountService/Roles/{roleId:[0-9]+}"
    mp.register_handler(
        Role::UPtr(new Role(constants::Routes::ROLE_PATH)));      

#ifdef CNTASK  
    // "/redfish/v1/TaskService"
    mp.register_handler(TaskService::UPtr(new TaskService(constants::Routes::TASK_SERVICE_PATH)));

    // "/redfish/v1/TaskService/Tasks"
    mp.register_handler(TaskCollection::UPtr(new TaskCollection(constants::Routes::TASK_COLLECTION_PATH)));

    // "/redfish/v1/Task/Service/Tasks/{taskId:[0-9]+}"
    mp.register_handler(Task::UPtr(new Task(constants::Routes::TASK_PATH)));

    // "/redfish/v1/Task/Service/Tasks/{taskId:[0-9]+}/Monitor"
#endif	
    mp.register_handler(Monitor::UPtr(new Monitor(constants::Routes::MONITOR_PATH)));

    // "/redfish/v1/Chassis"
    mp.register_handler(ChassisCollection::UPtr(new ChassisCollection(constants::Routes::CHASSIS_COLLECTION_PATH)));

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}"
    mp.register_handler(Chassis::UPtr(new Chassis(constants::Routes::CHASSIS_PATH)));

#ifndef NDEBUG
    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/Drives"
    mp.register_handler(Chassis::UPtr(new DrivesCollection(constants::Routes::DRIVES_COLLECTION_PATH)));
#endif

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/Drives/{driveId:[0-9]+}"
    mp.register_handler(Chassis::UPtr(new Drive(constants::Routes::DRIVE_PATH)));

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/Drives/{driveId:[0-9]+}/Actions/Drive.SecureErase"
    mp.register_handler(DriveSecureErase::UPtr(new DriveSecureErase(constants::Routes::DRIVE_SECURE_ERASE_PATH)));

#ifdef COMCAST
    // "/redfish/v1/Chassis/{systemId:[0-9]+}/Actions/Chassis.Reset"
    mp.register_handler(ChassisReset::UPtr(new ChassisReset(constants::Routes::CHASSIS_RESET_PATH)));


    // "/redfish/v1/Managers/{managerId:[0-9]+}/SerialInterfaces"
    mp.register_handler(ManagerSerialInterfaceCollection::UPtr(
        new ManagerSerialInterfaceCollection(constants::Routes::MANAGER_SERIAL_INTERFACE_COLLECTION_PATH)));


    // "/redfish/v1/Managers/{managerId:[0-9]+}/SerialInterfaces/{nicId:[0-9]+}"
    mp.register_handler(ManagerSerialInterface::UPtr(
        new ManagerSerialInterface(constants::Routes::MANAGER_SERIAL_INTERFACE_PATH)));


    // "/redfish/v1/Managers/{managerId:[0-9]+}/LogServices"
    mp.register_handler(ManagerLogServicesCollection::UPtr(
        new ManagerLogServicesCollection(constants::Routes::MANAGER_LOG_SERVICE_COLLECTION_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/LogServices/{nicId:[0-9]+}"
    mp.register_handler(ManagerLogServices::UPtr(
        new ManagerLogServices(constants::Routes::MANAGER_LOG_SERVICE_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/LogServices/{nicId:[0-9]+}/Entries"
    mp.register_handler(ManagerLogServicesEntriesCollection::UPtr(
        new ManagerLogServicesEntriesCollection(constants::Routes::MANAGER_LOG_SERVICE_ENTRIES_COLLECTION_PATH)));


    // "/redfish/v1/Managers/{managerId:[0-9]+}/LogServices/{nicId:[0-9]+}/Entries/{nicId:[0-9]+}"
    mp.register_handler(ManagerLogServicesEntries::UPtr(
        new ManagerLogServicesEntries(constants::Routes::MANAGER_LOG_SERVICE_ENTRIES_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/LogServices/{nicId:[0-9]+}/Actions/LogServies.Reset"
    mp.register_handler(LogReset::UPtr(new LogReset(constants::Routes::MANAGER_LOG_SERVICE_RESET_PATH)));

#endif 


#ifdef THERMAL_PSU 

// Step 1 
    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/Thermal"
    mp.register_handler(Chassis::UPtr(new ThermalCollection(constants::Routes::THERMAL_COLLECTION_PATH)));

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/Thermal/Temperatures/"
    mp.register_handler(Chassis::UPtr(new ThermalZone(constants::Routes::THERMALS_PATH)));

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/Thermal/Fans/{FanId:[0-9]+}"
    mp.register_handler(Chassis::UPtr(new Fan(constants::Routes::FANS_PATH)));

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/Power/PowerSupplies/"
    mp.register_handler(Chassis::UPtr(new PsuCollection(constants::Routes::PSU_COLLECTION_PATH)));

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/Power/PowerSupplies/{PsuID:[0-9]+}"
    mp.register_handler(Chassis::UPtr(new Psu(constants::Routes::PSUS_PATH)));
	
#endif

    // "/redfish/v1/SessionService"
    mp.register_handler(SessionService::UPtr(new SessionService(constants::Routes::SESSION_SERVICE_PATH)));
	
    // "/redfish/v1/SessionService/Sessions"
    mp.register_handler(SessionCollection::UPtr(new SessionCollection(constants::Routes::SESSION_COLLECTION_PATH)), AccessType::ALL);

    // "/redfish/v1/SessionService/Sessions/Members"
    mp.register_handler(Members::UPtr(new Members(constants::Routes::MEMBERS_PATH)),AccessType::ALL);	

    // "/redfish/v1/SessionService/Sessions/{SessionId:[a-zA-Z0-9_]+}"
    mp.register_handler(Session::UPtr(new Session(constants::Routes::SESSION_PATH)));	
	
#ifndef NDEBUG
    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/PCIeDevices"
    mp.register_handler(PcieDevicesCollection::UPtr(
        new PcieDevicesCollection(constants::Routes::PCIE_DEVICES_COLLECTION_PATH)));
#endif

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/PCIeDevices/{deviceId:[0-9]+}"
    mp.register_handler(PcieDevice::UPtr(new PcieDevice(constants::Routes::PCIE_DEVICE_PATH)));

#ifndef NDEBUG
    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/PCIeDevices/{deviceId:[0-9]+}/Functions"
    mp.register_handler(PcieFunctionsCollection::UPtr(
        new PcieFunctionsCollection(constants::Routes::PCIE_FUNCTIONS_COLLECTION_PATH)));
#endif

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/PCIeDevices/{deviceId:[0-9]+}/Functions/{functionId:[0-9]+}"
    mp.register_handler(PcieFunction::UPtr(new PcieFunction(constants::Routes::PCIE_FUNCTION_PATH)));

    // "/redfish/v1/Managers"
    mp.register_handler(ManagerCollection::UPtr(new ManagerCollection(constants::Routes::MANAGER_COLLECTION_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}"
    mp.register_handler(Manager::UPtr(new Manager(constants::Routes::MANAGER_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/EthernetInterfaces"
    mp.register_handler(ManagerNetworkInterfaceCollection::UPtr(
        new ManagerNetworkInterfaceCollection(constants::Routes::MANAGER_NETWORK_INTERFACE_COLLECTION_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/EthernetInterfaces/{nicId:[0-9]+}"
    mp.register_handler(ManagerNetworkInterface::UPtr(
        new ManagerNetworkInterface(constants::Routes::MANAGER_NETWORK_INTERFACE_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/EthernetInterfaces/{nicId:[0-9]+}/VLANs"
    mp.register_handler(Mgmt_VlanNetworkInterfaceCollection::UPtr(
        new Mgmt_VlanNetworkInterfaceCollection(constants::Routes::VLAN_MANAGER_NETWORK_INTERFACE_COLLECTION_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/EthernetInterfaces/{nicId:[0-9]+}/VLANs/{vlanId:[0-9]+}"
    mp.register_handler(Mgmt_VlanNetworkInterface::UPtr(
        new Mgmt_VlanNetworkInterface(constants::Routes::VLAN_MANAGER_NETWORK_INTERFACE_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/NetworkProtocol"
    mp.register_handler(NetworkProtocol::UPtr(new NetworkProtocol(constants::Routes::NETWORK_PROTOCOL_PATH)));

#ifdef CSERVICES
    // "/redfish/v1/Services"
    mp.register_handler(StorageServiceCollection::UPtr(
        new StorageServiceCollection(constants::Routes::STORAGE_SERVICE_COLLECTION_PATH)));

    // "/redfish/v1/Services/{serviceId:[0-9]+}"
    mp.register_handler(StorageService::UPtr(new StorageService(constants::Routes::STORAGE_SERVICE_PATH)));

    // "/redfish/v1/Services/{serviceId:[0-9]+}/Drives"
    mp.register_handler(PhysicalDrivesCollection::UPtr(
        new PhysicalDrivesCollection(constants::Routes::PHYSICAL_DRIVES_COLLECTION_PATH)));

    // "/redfish/v1/Services/{serviceId:[0-9]+}/Drives/{driveId:[0-9]+}"
    mp.register_handler(PhysicalDrive::UPtr(new PhysicalDrive(constants::Routes::PHYSICAL_DRIVE_PATH)));

    // "/redfish/v1/Services/{serviceId:[0-9]+}/LogicalDrives"
    mp.register_handler(
        LogicalDriveCollection::UPtr(new LogicalDriveCollection(constants::Routes::LOGICAL_DRIVE_COLLECTION_PATH)));

    // "/redfish/v1/Services/{serviceId:[0-9]+}/LogicalDrives/{logicalDriveId:[0-9]+}"
    mp.register_handler(LogicalDrive::UPtr(new LogicalDrive(constants::Routes::LOGICAL_DRIVE_PATH)));

    // "/redfish/v1/Services/{serviceId:[0-9]+}/Targets"
    mp.register_handler(RemoteTargetCollection::UPtr(
        new RemoteTargetCollection(constants::Routes::REMOTE_TARGET_COLLECTION_PATH)));

    // "/redfish/v1/Services/{serviceId:[0-9]+}/Targets/{remoteTargetId:[0-9]+}"
    mp.register_handler(RemoteTarget::UPtr(new RemoteTarget(constants::Routes::REMOTE_TARGET_PATH)));
#endif


#ifdef FABRICS
    // "/redfish/v1/Fabrics"
    mp.register_handler(FabricsCollection::UPtr(
        new FabricsCollection(constants::Routes::FABRICS_COLLECTION_PATH)));

    // "/redfish/v1/Fabrics/{fabricId}"
    mp.register_handler(Fabric::UPtr(
        new Fabric(constants::Routes::FABRIC_PATH)));

    // "/redfish/v1/Fabrics/{fabricId}/Endpoints"
    mp.register_handler(EndpointsCollection::UPtr(
        new EndpointsCollection(constants::Routes::ENDPOINTS_COLLECTION_PATH)));

    // "/redfish/v1/Fabrics/{fabricId}/Endpoints/{endpointId}"
    mp.register_handler(Endpoint::UPtr(
        new Endpoint(constants::Routes::ENDPOINT_PATH)));

    // "/redfish/v1/Fabrics/{fabricId}/Switches"
    mp.register_handler(SwitchesCollection::UPtr(
        new SwitchesCollection(constants::Routes::SWITCHES_COLLECTION_PATH)));

    // "/redfish/v1/Fabrics/{fabricId}/Switches/{switchId}"
    mp.register_handler(Switch::UPtr(new Switch(constants::Routes::SWITCH_PATH)));

    // "/redfish/v1/Fabrics/{fabricId}/Switches/{switchId}/Actions/Switch.Reset"
    mp.register_handler(SwitchReset::UPtr(new SwitchReset(constants::Routes::SWITCH_RESET_PATH)));

    // "/redfish/v1/Fabrics/{fabricId}/Zones"
    mp.register_handler(ZonesCollection::UPtr(
        new ZonesCollection(constants::Routes::ZONES_COLLECTION_PATH)));

    // "/redfish/v1/Fabrics/{fabricId}/Zones/{zoneId:[0-9]+}"
    mp.register_handler(Zone::UPtr(new Zone(constants::Routes::ZONE_PATH)));

    // "/redfish/v1/Fabrics/{fabricId}/Switches/{switchId}/Ports"
    mp.register_handler(PortsCollection::UPtr(
        new PortsCollection(constants::Routes::PORTS_COLLECTION_PATH)));

    // "/redfish/v1/Fabrics/{fabricId}/Switches/{switchId}/Ports/{portId:[0-9]+}"
    mp.register_handler(Port::UPtr(new Port(constants::Routes::PORT_PATH)));

    // "/redfish/v1/Fabrics/{fabricId}/Switches/{switchId}/Ports/{portId:[0-9]+}/Actions/Port.Reset"
    mp.register_handler(PortReset::UPtr(new PortReset(constants::Routes::PORT_RESET_PATH)));
#endif	

}
