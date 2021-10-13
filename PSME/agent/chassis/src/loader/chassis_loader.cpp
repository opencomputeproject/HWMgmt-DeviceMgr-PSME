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
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * @section DESCRIPTION
 *
 * */
#include "loader/chassis_loader.hpp"
#include "configuration/configuration.hpp"
#include "logger/logger_factory.hpp"
#include "agent-framework/module/network_components.hpp"
#include "agent-framework/module/compute_components.hpp"
#include "agent-framework/module/chassis_components.hpp"
#include "agent-framework/module/common_components.hpp"
#include "agent-framework/module/model/processor.hpp"
#include "agent-framework/version.hpp"
#include "agent-framework/service_uuid.hpp"
#include "ecsys_helper/ecsys_helper.hpp"
#include "ecopenbmc_helper/ecopenbmc_helper.hpp"
#include <chrono>
#include <thread>
using namespace agent::chassis::loader;
using namespace agent_framework::generic;
using namespace agent_framework::model;
using namespace agent_framework::module;
using namespace agent_framework::model::attribute;
using namespace ecsys_helper;
using namespace ecopenbmc_helper;

#ifdef CSUBSYSTEM
void make_processors(Manager&  manager, const std::string& system_parent_uuid);
void make_memories(Manager&  manager, const std::string& system_parent_uuid);
void make_storages(Manager&  manager, const std::string& system_parent_uuid , const std::string& chassis_uuid);
#endif
System make_system_default(const std::string& parent, const std::string& chassis);
void create_system(Manager&  manager, const std::string& chassis , const json::Value& json );

namespace
{
inline std::string decrypt_value(const std::string &value)
{
    return value;
}

EthernetSwitchPortVlan make_port_vlan(const std::string &parent, bool tagged = true, int vlan_id = 0, bool enable = false)
{

/*Add VLANs ID based on Port ID*/

	EthernetSwitchPortVlan vlan{parent};
	vlan.set_status(attribute::Status(
	agent_framework::model::enums::State::Enabled,
	agent_framework::model::enums::Health::OK));
	vlan.set_vlan_id(vlan_id);
	vlan.set_vlan_name("VLAN Name");
	
	if(enable==true)
		vlan.set_vlan_enable(true);
	else
		vlan.set_vlan_enable(false);

	vlan.set_tagged(tagged);
	vlan.set_oem(attribute::Oem());
	return vlan;
}

EthernetSwitchPort make_port(const std::string &parent, bool lag = false)
{
    EthernetSwitchPort port{parent};
    port.set_status(attribute::Status(
        agent_framework::model::enums::State::Enabled,
        agent_framework::model::enums::Health::OK));
    port.set_port_identifier("Port ID");
    port.set_port_class(
        lag ? enums::PortClass::Logical : enums::PortClass::Physical);
    port.set_port_type(enums::PortType::Upstream);
    port.set_port_mode(
        lag ? enums::PortMode::LinkAggregationDynamic : enums::PortMode::LinkAggregationStatic);
    port.set_link_technology(enums::LinkTechnology::Ethernet);
    port.set_link_speed_mbps(5000);
    port.set_max_speed_mbps(10000);
    port.set_operational_state(enums::OperationalState::Up);
    port.set_administrative_state(enums::AdministrativeState::Up);
    port.set_port_width(1);
    port.set_frame_size(1400);
    port.set_auto_sense(true);
    port.set_full_duplex(false);
    port.set_is_management_port(false);
    port.set_last_error_code(0);
    port.set_error_cleared(false);
    port.set_last_state_change_time("2015-02-23T14:44:00+00:00");
    port.set_mac_address("AA:BB:CC:DD:EE:FF");
    port.set_ipv4_address(attribute::Ipv4Address(
            "10.0.2.10",
            "255.255.255.0",
            enums::Ipv4AddressOrigin::DHCP,
            "10.0.2.1"));
    port.set_ipv6_address(attribute::Ipv6Address(
            "fe80:1ec1:deff:fe6f:1c37",
            16,
            enums::Ipv6AddressOrigin::DHCP,
            enums::Ipv6AddressState::Preferred));
    port.set_neighbor_info(attribute::NeighborInfo(
            "NeighborID",
            "portID",
            "cableID"));
    port.set_neighbor_mac("80:00:27:2C:FF:00");
    port.set_vlan_enable(true);
    port.set_default_vlan("");

    port.add_collection(attribute::Collection(
            enums::CollectionName::PortMembers,
            enums::CollectionType::PortMembers,
            "SlotMask"));

    port.add_collection(attribute::Collection(
             enums::CollectionName::EthernetSwitchPortVlans,
             enums::CollectionType::EthernetSwitchPortVlans,
             "SlotMask"));

    port.set_oem(attribute::Oem());
    return port;
}

EthernetSwitch make_ethernet_switch(const std::string &parent, const std::string &chassis)
{
    EthernetSwitch eth_switch{parent};
    eth_switch.set_status(attribute::Status(
        agent_framework::model::enums::State::Enabled,
        agent_framework::model::enums::Health::OK));

    eth_switch.set_role(enums::SwitchRole::TOR);
    eth_switch.set_chassis(chassis);
    eth_switch.add_collection(attribute::Collection(
        enums::CollectionName::EthernetSwitchPorts,
        enums::CollectionType::EthernetSwitchPorts,
        "SlotMask"));
    eth_switch.add_collection(attribute::Collection(
        enums::CollectionName::Vlans,
        enums::CollectionType::Vlans,
        "SlotMask"));
 
    return eth_switch;
}

void create_port_vlan(const std::string &parent)
{

	/* Check Port Id and how many VLANs it has */
    char command[HT_BUFFER_LEN] = {0};
    char resultA[HT_BUFFER_LEN] = {0};
    char IfName[HT_BUFFER_LEN] = "ma1";
	unsigned int count = 0, vlan_id =0, enable=0;

	sprintf(command, "psme.sh get mgmt_port_name");	
    HelperTools::exec_shell(command, resultA, 0);
	
	if(strlen(resultA) !=0 )	
       {
           sprintf(IfName, "%s", resultA);
	}

	sprintf(command, "psme.sh get inf_vlans_count %s", IfName);	
    HelperTools::exec_shell(command, resultA, 0);

	if(strlen(resultA) !=0 )	
		count = atoi(resultA);

	for (auto i = 0u; i < count; ++i) 
	{
		sprintf(command, "psme.sh get inf_vlans_value %s %d", IfName , i+1 );	
        HelperTools::exec_shell(command, resultA, 0);
		if(strlen(resultA) !=0 )	
		{
			vlan_id = atoi(resultA);
			sprintf(command, "cat /sys/class/net/%s.%d/operstate", IfName , vlan_id);	
            HelperTools::exec_shell(command, resultA, 0);

			if(strlen(resultA) !=0 )	
			{
			if(!strncmp("up", resultA, 2))
				enable=true;
			else
				enable=false;
			}
			
			auto port_vlan = ::make_port_vlan(parent ,false, vlan_id, enable);
			log_debug(GET_LOGGER("rpc"), "Adding port VLAN: " << port_vlan.get_uuid());
			NetworkComponents::get_instance()->get_port_vlan_manager().add_entry(port_vlan);
		}
	}
}

std::vector<std::string> create_port(const std::string &parent, int port_num, bool is_mgmt_port)
{
    std::vector<std::string> ports{};
	
    for (auto i = 0; i < port_num; ++i)
    {
        auto port = ::make_port(parent);
        port.set_port_id(i+1);
        log_debug(GET_LOGGER("rpc"), "Adding port: " << port.get_uuid());
        NetworkComponents::get_instance()->get_port_manager().add_entry(port);
        //create_static_macs(port.get_uuid());
        if(is_mgmt_port == true)
        {
        	create_port_vlan(port.get_uuid());
        	//auto vlan = ::create_default_port_vlan(port.get_uuid());
        	//port.set_default_vlan(vlan);
        }
	 ports.push_back(port.get_uuid());
    }
    return ports;
}

void create_ethernet_switch(Manager &manager, const std::string &chassis)
{
	 const std::string& parent = manager.get_uuid();
	 int port_num = manager.get_max_port_num();
        port_num = MAX_LAG +port_num;
	 
        auto eth_switch = ::make_ethernet_switch(parent, chassis);
        log_debug(GET_LOGGER("rpc"), "Adding ethernet switch: " << eth_switch.get_uuid());
        NetworkComponents::get_instance()->get_switch_manager().add_entry(eth_switch);

        create_port(eth_switch.get_uuid(), port_num, false);    
}

} // namespace

using agent_framework::module::ChassisComponents;
using agent_framework::module::CommonComponents;

std::mutex ChassisLoader::m_mutex{};
std::condition_variable ChassisLoader::m_cv{};
std::atomic<bool> ChassisLoader::m_is_loaded{false};

System make_system_default(const std::string& parent, const std::string& chassis) {
	System system{parent};
	system.set_uefi_target("/");
	system.set_bios_version("N/A");
	system.set_oem(agent_framework::model::attribute::Oem{});
       system.set_boot_override(enums::BootOverride::Disabled);
       system.set_boot_override_target(enums::BootOverrideTarget::None);

	system.add_boot_override_supported(enums::BootOverrideTarget::Hdd);
	system.add_boot_override_supported(enums::BootOverrideTarget::Pxe);
	system.add_boot_override_supported(enums::BootOverrideTarget::None);

	system.set_power_state(enums::PowerState::On);
	system.set_status({
			agent_framework::model::enums::State::Enabled,
			agent_framework::model::enums::Health::OK
			});
	system.set_chassis(chassis);
	system.set_sku("SKU");
	system.set_asset_tag("N/A");
	system.set_indicator_led(enums::IndicatorLed::Lit);
#ifdef CSUBSYSTEM
	system.add_collection(attribute::Collection(
	enums::CollectionName::Processors,
	enums::CollectionType::Processors,
	"SlotMask"));

	system.add_collection(attribute::Collection(
	enums::CollectionName::Memory,
	enums::CollectionType::Memory,
	"SlotMask"));

	system.add_collection(attribute::Collection(
       enums::CollectionName::StorageSubsystems,
       enums::CollectionType::StorageSubsystems,
       "SlotMask"));

#endif
	return system;
}


void create_system(Manager&  manager, const std::string& chassis_uuid , const json::Value& json ) {
	auto system = ::make_system_default(manager.get_uuid(), chassis_uuid);

	system.set_asset_tag(json["asset_tag"].as_string());                                                                
	system.set_guid(ServiceUuid::get_instance()->get_service_uuid());                                                                

std::cerr << "service uuid uuid[" << ServiceUuid::get_instance()->get_service_uuid() << "]"  << std::endl;

	attribute::FruInfo fru_info;
	fru_info.set_manufacturer(json["manufacturer"].as_string()); 
	fru_info.set_model_number(json["model_number"].as_string());	        
	fru_info.set_part_number(json["model_number"].as_string());	        
	fru_info.set_serial_number(json["serial_number"].as_string());
	system.set_fru_info(std::move(fru_info));
	CommonComponents::get_instance()->get_system_manager().add_entry(system);
	
#ifdef CSUBSYSTEM
	make_processors(manager, system.get_uuid());

	make_memories(manager, system.get_uuid());

	make_storages(manager, system.get_uuid(), chassis_uuid);
#endif		
}
ChassisLoader::~ChassisLoader() { }

class LoadManagers
{
public:
    void read_managers(const json::Value &json)
    {
        for (const auto &elem : json["managers"].as_array())
        {
            auto manager = make_manager(elem);
            read_manager(manager, elem);
        }
    }

    void read_managers(const std::string &parent, const json::Value &json)
    {
        for (const auto &elem : json["managers"].as_array())
        {
            auto manager = make_manager(parent, elem);
            read_manager(manager, elem);
        }
    }

    void generate_acc_sw_chassis(Manager &manager, const std::string &chassis_uuid, const json::Value &json)
    {
	    int index1;
           int num = manager.get_max_fan_num();
           int t_s_num = manager.get_max_thermal_sensor_num();
           int psu_num = manager.get_max_psu_num();

           /*Add Fans Nodes*/

        for (index1 = 1; index1 <= num; index1++)
        {
	        Fan fan{manager.get_uuid()};
		 fan.set_chassis(chassis_uuid); 
		 fan.set_current_speed(0);
		 fan.set_fan_type(0);
               fan.set_status_health("unknown");
               fan.set_status_state("unknown");
		 fan.set_fan_id(index1);
	        ChassisComponents::get_instance()->get_fan_manager().add_entry(fan);
	    }

        for (index1 = 1; index1 <= t_s_num; index1++)
        {
	        ThermalZone tzone{manager.get_uuid()};
		 tzone.set_chassis(chassis_uuid); 
		 tzone.set_temperature(0);
		 tzone.set_tz_id(index1);
		 tzone.set_warning_upper_temp(0);
		 tzone.set_warning_lower_temp(0);
		 tzone.set_error_temp(0);
		 tzone.set_shutdown_temp(0);
		 tzone.set_status_health("unknow");
		 tzone.set_status_state("unknow");

	        ChassisComponents::get_instance()->get_thermal_zone_manager().add_entry(tzone);
	    }

        for (index1 = 1; index1 <= psu_num; index1++)
        {
	        Psu psu{manager.get_uuid()};
		 psu.set_chassis(chassis_uuid); 
		 psu.set_psu_id(index1);
	        ChassisComponents::get_instance()->get_psu_manager().add_entry(psu);
	    }

	    create_system(manager, chassis_uuid, json);

           create_ethernet_switch(manager, chassis_uuid);
    }

private:
    void read_manager(Manager &manager, const json::Value &json)
    {

        log_debug(GET_LOGGER("agent"), "Adding manager:" << manager.get_uuid());
        CommonComponents::get_instance()->get_module_manager().add_entry(manager);

        if (json["chassis"].is_object())
        {
            auto chassis = make_chassis(manager.get_uuid(), json["chassis"]);
            log_debug(GET_LOGGER("agent"), "Adding chassis:" << chassis.get_uuid()
                                           << " to manager " << manager.get_uuid());
            CommonComponents::get_instance()->get_chassis_manager().add_entry(chassis);
		    generate_acc_sw_chassis(manager, chassis.get_uuid(),  json["chassis"] );
        }
        if (json["managers"].is_array())
        {
            log_debug(GET_LOGGER("agent"), "Adding children managers to manager:" << manager.get_uuid());
            read_managers(manager.get_uuid(), json);
        }
    }

    Manager make_manager(const json::Value &json)
    {
        Manager manager{};

        //Chassis collection is added only to top level manager.
        manager.add_collection(attribute::Collection(
            enums::CollectionName::Chassis,
            enums::CollectionType::Chassis,
            "slotMask"));

        manager.add_collection(attribute::Collection(
        enums::CollectionName::Fans,
        enums::CollectionType::Fans,
        "SlotMask"));

        manager.add_collection(attribute::Collection(
        enums::CollectionName::ThermalZones,
        enums::CollectionType::ThermalZones,
        "SlotMask"));

        manager.add_collection(attribute::Collection(
        enums::CollectionName::Psus,
        enums::CollectionType::PSUs,
        "SlotMask"));

	    manager.add_collection(attribute::Collection(
				    enums::CollectionName::Systems,
				    enums::CollectionType::Systems,
        "slotMask"));

        manager.add_collection(attribute::Collection(
        enums::CollectionName::EthernetSwitches,
        enums::CollectionType::EthernetSwitches,
        "SlotMask"));
	 
        make_manager_internals(manager, json);
        return manager;
    }

    Manager make_manager(const std::string &parent, const json::Value &json)
    {
        Manager manager{parent};
        make_manager_internals(manager, json);
        return manager;
    }

    void make_manager_internals(Manager &manager, const json::Value &json)
    {
        manager.set_firmware_version(Version::VERSION_STRING);
        manager.set_manager_model("Redfish PSME");
        manager.set_slot(uint8_t(json["slot"].as_uint()));

        manager.set_max_fan_num(uint8_t(json["ecrf_pal"]["fan_max_num"].as_uint()));
        manager.set_max_thermal_sensor_num(uint8_t(json["ecrf_pal"]["thermal_sen_max_num"].as_uint()));
        manager.set_max_psu_num(uint8_t(json["ecrf_pal"]["psu_max_num"].as_uint()));
        manager.set_max_port_num(uint8_t(json["ecrf_pal"]["port_max_num"].as_uint()));

        try
        {
            if (json["chassis"].is_object() &&
                "Drawer" == json["chassis"]["type"].as_string())
            {
                manager.set_manager_type(
                    enums::ManagerInfoType::EnclosureManager);
            }

            manager.set_ipv4_address(json["ipv4"].as_string());

            ConnectionData connection_data{};
            connection_data.set_ip_address(json["ipv4"].as_string());
            connection_data.set_port(json["port"].as_uint());
            log_debug(GET_LOGGER("agent"), "Manager connection data loaded. Ip="
                                               << connection_data.get_ip_address()
                                               << ", port=" << connection_data.get_port());

            manager.set_connection_data(connection_data);
        }
        catch (std::runtime_error &e)
        {
            log_warning(GET_LOGGER("agent"), "Cannot read connection data.");
            log_debug(GET_LOGGER("agent"), e.what());
        }

        manager.set_status({agent_framework::model::enums::State::Enabled,
                            agent_framework::model::enums::Health::OK});
        static constexpr bool ENABLED = true;
        static constexpr enums::NetworkServiceName NAME_SSH{enums::NetworkServiceName::SSH};
        static constexpr uint32_t SSH_PORT = 22;
        agent_framework::model::attribute::NetworkService service_ssh(NAME_SSH, SSH_PORT, ENABLED);
        manager.add_network_service(service_ssh);

        attribute::SerialConsole serial{};
        serial.set_bitrate(115200);
        serial.set_data_bits(8);
        serial.set_enabled(true);
        serial.set_flow_control(enums::SerialConsoleFlowControl::None);
        serial.set_parity(enums::SerialConsoleParity::None);
        serial.set_pin_out(enums::SerialConsolePinOut::Cisco);
        serial.set_stop_bits(1);
        serial.set_signal_type(enums::SerialConsoleSignalType::Rs232);

        manager.set_serial_console(std::move(serial));
    }

    Chassis make_chassis(const std::string &parent, const json::Value &json)
    {
        Chassis chassis{parent};

        try
        {
            chassis.set_type(enums::ChassisType::from_string(json["type"].as_string()));
            chassis.set_size(json["size"].as_uint());
            chassis.set_location_offset(json["locationOffset"].as_uint());
            chassis.set_asset_tag(json["asset_tag"].as_string());        
            const auto& parent_id_json = json["parentId"];
            if (parent_id_json.is_uint())
            { // for backward compatibility
                chassis.set_parent_id(std::to_string(parent_id_json.as_uint()));
            }
            else
            {
                chassis.set_parent_id(parent_id_json.as_string());
            }
        }
        catch (const std::runtime_error &e)
        {
            log_error(GET_LOGGER("agent"), "Invalid chassis configuration.");
            log_debug(GET_LOGGER("agent"), e.what());
        }

        if (json["platform"].is_string())
        {
            chassis.set_platform(enums::PlatformType::from_string(json["platform"].as_string()));
        }
        if (json["networkInterface"].is_string())
        {
            chassis.set_network_interface(json["networkInterface"].as_string());
        }

        chassis.add_collection(attribute::Collection(
            enums::CollectionName::Drives,
            enums::CollectionType::Drives,
            "slotMask"));

        chassis.set_status({agent_framework::model::enums::State::Enabled,
                            agent_framework::model::enums::Health::OK});

	agent_framework::model::attribute::FruInfo fru_info{};
	fru_info.set_manufacturer(json["manufacturer"].as_string());
	fru_info.set_model_number(json["model_number"].as_string());
	fru_info.set_part_number(json["model_number"].as_string());
	fru_info.set_serial_number(json["serial_number"].as_string()); 
	chassis.set_fru_info(std::move(fru_info));
        return chassis;
    }
};

#ifdef CSUBSYSTEM
void make_processors(Manager&  manager, const std::string& system_parent_uuid)
{
	unsigned int cpu_index = 0;

	log_debug(GET_LOGGER("basic-discovery"), "Sending GetProcessorInfo. CPU: " << std::uint32_t(cpu_index));

	for( cpu_index = 1; cpu_index <= manager.get_max_cpu_num() ; cpu_index++ )
	{
		char str_tmp[256] = {0};

		attribute::Status status{};
		Processor processor{system_parent_uuid};

		sprintf(str_tmp, "CPU%d", cpu_index-1);
		processor.set_socket(str_tmp);
		
		log_debug(GET_LOGGER("basic-discovery"), "Discovering Processor (" << processor.get_socket() << ").");

		processor.set_processor_type(enums::ProcessorType::CPU);

		if(manager.get_cpu_manu() == "x86_64")
		{
			processor.set_processor_architecture(enums::ProcessorArchitecture::x86);
			processor.set_instruction_set(enums::ProcessorInstructionSet::x86_64);		
			processor.set_manufacturer(literals::Intel::INTEL_CORP);			
		}
		else
		{
			processor.set_processor_architecture(enums::ProcessorArchitecture::ARM);  //Todo Currently support 2 arch , //
			processor.set_instruction_set(enums::ProcessorInstructionSet::UNKNOWN);			
		}	

		processor.set_model_name(manager.get_cpu_model_name());
		processor.set_model(enums::ProcessorModel::Unknown);
		processor.set_max_speed_mhz(std::uint32_t(manager.get_cpu_max_speed()));
		
		processor.set_total_cores(manager.get_cpu_total_core());
		processor.set_total_threads(std::uint32_t(manager.get_cpu_total_thread()));
		CpuId cpu_id{};
		cpu_id.set_vendor_id(manager.get_cpu_vid());
		processor.set_cpu_id(std::move(cpu_id));
		status.set_state(enums::State::Enabled);
		processor.set_status(status);
		ComputeComponents::get_instance()->get_processor_manager().add_entry(std::move(processor));
	}
	log_debug(GET_LOGGER("basic-discovery"), "GetProcessorInfo successful.");
}

void make_memories(Manager&  manager, const std::string& system_parent_uuid)
{
	unsigned int mem_index = 0;
    char command[HT_BUFFER_LEN] = {0};
    char resultA[HT_BUFFER_LEN] = {0};
	char tmp[32] = {0};

	log_debug(GET_LOGGER("basic-discovery"), "Sending GetMemoryInfo. Memory: " << std::uint32_t(mem_index));

	for( mem_index = 1; mem_index <= manager.get_max_mem_num() ; mem_index++ )
	{
        Memory memory{system_parent_uuid};
        attribute::Status status{};
		sprintf(command, "collect_mem_info.sh %d TYPE", mem_index  );	
        HelperTools::exec_shell(command, resultA, 0);
		if(strlen(resultA) !=0 )	
		{
                     memory.set_device_type(enums::DeviceType::DDR3_SDRAM);					 	

		       if(!strncmp("DDR", resultA, 3))
                         memory.set_device_type(enums::DeviceType::DDR);
		       if(!strncmp("DDR2", resultA, 4))
                         memory.set_device_type(enums::DeviceType::DDR2);
		       if(!strncmp("DDR3", resultA, 4))
                         memory.set_device_type(enums::DeviceType::DDR3);		
		       if(!strncmp("DDR4", resultA, 4))
                         memory.set_device_type(enums::DeviceType::DDR4);				   
        	}

		sprintf(command, "collect_mem_info.sh %d LOCATOR", mem_index  );	
        HelperTools::exec_shell(command, resultA, 0);
		if(strlen(resultA) !=0 )	
		{
		    resultA[strcspn(resultA, "\r\n")]=0;
	           memory.set_device_locator(resultA);
		}
		
		sprintf(command, "collect_mem_info.sh %d SPEED", mem_index  );	
        HelperTools::exec_shell(command, resultA, 0);
		if(strlen(resultA) !=0 )	
		{
	           memory.set_operating_speed_mhz(atoi(resultA));
		}

		sprintf(command, "collect_mem_info.sh %d DWIDTH", mem_index  );	
        HelperTools::exec_shell(command, resultA, 0);
		if(strlen(resultA) !=0 )	
		{
	           memory.set_data_width_bits(atoi(resultA));
		}
	
        attribute::Region region{};
		 sprintf(tmp,"%d", mem_index);
	        region.set_region_id(tmp);
        region.set_offset_mb(0);

		sprintf(command, "collect_mem_info.sh %d MSIZE", mem_index  );	
        HelperTools::exec_shell(command, resultA, 0);
		if(strlen(resultA) !=0 )	
		{
   	           memory.set_capacity_mb(atoi(resultA));
                  region.set_size_mb(atoi(resultA));
		}	        
        memory.add_region(std::move(region));
        status.set_state(enums::State::Enabled);

        memory.set_status(status);
        ComputeComponents::get_instance()->get_memory_manager().add_entry(std::move(memory));
	}
        log_debug(GET_LOGGER("basic-discovery"), "GetDimmInfo successful.");
}

void make_storages(Manager&  manager, const std::string& system_parent_uuid , const std::string& chassis_uuid)
{

	unsigned int drv_index = 0;
    char command[HT_BUFFER_LEN] = {0};
    char resultA[HT_BUFFER_LEN] = {0};

       attribute::Status status{};
       status.set_state(enums::State::Enabled);
       status.set_health(enums::Health::OK);
                  
       StorageSubsystem storage{system_parent_uuid};
       storage.add_collection(attribute::Collection(
       enums::CollectionName::Drives,
       enums::CollectionType::Drives,
        "slotMask"));
       
       storage.set_status(status);
           
	for( drv_index = 1; drv_index <= manager.get_max_drv_num() ; drv_index++ )
	{
           Drive drive{chassis_uuid};	

           sprintf(command, "collect_drv_info.sh %d MSIZE", drv_index);	
        HelperTools::exec_shell(command, resultA, 0);
           if(strlen(resultA) !=0 )	
           {  
               double bytes=(double)atoll(resultA);
		 log_debug(GET_LOGGER("agent"), "Drive size [" << (bytes)  << "B]");  
	        drive.set_capacity_gb((bytes/1000000000));
           }

           sprintf(command, "collect_drv_info.sh %d REVISION", drv_index);	
        HelperTools::exec_shell(command, resultA, 0);
           if(strlen(resultA) !=0 )	
           {  
               resultA[strcspn(resultA, "\r\n")]=0;           
	        drive.set_revision(resultA);
           }
		   
           attribute::FruInfo fru;

           sprintf(command, "collect_drv_info.sh %d SERIAL", drv_index);	
        HelperTools::exec_shell(command, resultA, 0);
           if(strlen(resultA) !=0 )	
           {  
               resultA[strcspn(resultA, "\r\n")]=0;
               fru.set_serial_number(resultA);
           }		   

           sprintf(command, "collect_drv_info.sh %d MODEL_ID", drv_index);	
        HelperTools::exec_shell(command, resultA, 0);
           if(strlen(resultA) !=0 )	
           {  
               resultA[strcspn(resultA, "\r\n")]=0;
               fru.set_model_number(resultA);
           }		   

           drive.set_fru_info(std::move(fru));
           drive.set_status(status);
           CommonComponents::get_instance()->get_drive_manager().add_entry(std::move(drive));	
       }
	CommonComponents::get_instance()->get_storage_subsystem_manager().add_entry(std::move(storage));

	log_debug(GET_LOGGER("basic-discovery"), "Drive V2 was discovered.");
}

#endif

bool ChassisLoader::load(const json::Value& json) {
    try {
        LoadManagers lm{};
        lm.read_managers(json);
        m_is_loaded = true;
    }
    catch (const json::Value::Exception& e) {
        log_error(GET_LOGGER("discovery"),
                  "Load agent configuration failed: " << e.what());
    }

    m_cv.notify_all();
    return m_is_loaded;
}

void ChassisLoader::wait_for_complete() {
    std::unique_lock<std::mutex> lock{m_mutex};
    m_cv.wait(lock, []{return true == ChassisLoader::m_is_loaded;});
}
