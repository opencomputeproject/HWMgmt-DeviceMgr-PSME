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

#include "psme/rest/endpoints/manager_network_interface.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/utils/network_interface_info.hpp"
#include "configuration/configuration.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/ethernet_interface.hpp"
#include <arpa/inet.h>
#include "ecnet_helper/ecnet_helper.hpp"
#include "ecrf_pal_helper/api/ecrf_pal_helper.hpp"
#include <limits.h>
#include "ecsys_helper/ecsys_helper.hpp"
#include "psme/rest/utils/ec_common_utils.hpp"
using namespace psme::rest::utils;
using namespace ecrf_pal_helper;
using namespace ecsys_helper;

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace agent_framework::model::enums;
using namespace psme::rest::validators;
using namespace ecnet_helper;

bool isValidIpAddress(char *ipAddress);

namespace
{
json::Value make_prototype()
{
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#EthernetInterface.EthernetInterface";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#EthernetInterface.v1_4_0.EthernetInterface";
    r[Common::ID] = json::Value::Type::NIL;
	r[Common::NAME] = "Management port";
	r[Common::DESCRIPTION] = "Management port settings";
    r[Common::STATUS][Common::STATE] = json::Value::Type::NIL;
    r[Common::STATUS][Common::HEALTH] = json::Value::Type::NIL;
    r[Common::STATUS][Common::HEALTH_ROLLUP] = json::Value::Type::NIL;
    r[Common::OEM] = json::Value::Type::OBJECT;
    r[Common::MAC_ADDRESS] = json::Value::Type::NIL;
    r[NetworkInterface::PERMANENT_MAC_ADDRESS] = json::Value::Type::NIL;
    r[NetworkInterface::SPEED] = json::Value::Type::NIL;
    r[NetworkInterface::AUTO_NEG] = json::Value::Type::NIL;
    r[NetworkInterface::FULL_DUPLEX] = json::Value::Type::NIL;
    r[NetworkInterface::MTU_SIZE] = json::Value::Type::NIL;
    r[NetworkInterface::HOST_NAME] = json::Value::Type::NIL;
    r[NetworkInterface::FQDN] = json::Value::Type::NIL;
    r[NetworkInterface::MAX_IPv6_STATIC_ADDRESSES] = json::Value::Type::NIL;
    r[NetworkInterface::INTERFACE_ENABLED] = json::Value::Type::NIL;
    r[NetworkInterface::LINK_STATUS] = json::Value::Type::NIL;
    r[NetworkInterface::IPv4_STACIC_ADDRESSES] = json::Value::Type::ARRAY;
    r[NetworkInterface::IPv4_ADDRESSES] = json::Value::Type::ARRAY;
    r[NetworkInterface::IPv4_DHCP] = json::Value::Type::NIL;
    r[NetworkInterface::STATELESS_ADDRESS_AUTO_CONFIG]= json::Value::Type::NIL;
    r[NetworkInterface::IPv6_DHCP] = json::Value::Type::NIL;
    r[NetworkInterface::IPv6_ADDRESS_POLICY_TABLE] = json::Value::Type::ARRAY;
    r[NetworkInterface::IPv6_ADDRESSES] = json::Value::Type::ARRAY;
    r[NetworkInterface::IPv6_DEFAULT_GATEWAY] = json::Value::Type::NIL;
    r[NetworkInterface::IPv6_Static_DEFAULT_GATEWAY] = json::Value::Type::ARRAY;
    r[NetworkInterface::STATIC_NAME_SERVERS] = json::Value::Type::ARRAY;
    r[NetworkInterface::IPv6_STATIC_ADDRESSES] = json::Value::Type::ARRAY;
    r[NetworkInterface::NAME_SERVERS] = json::Value::Type::ARRAY;

    json::Value links;
    links[Fabric::ENDPOINTS] = json::Value::Type::ARRAY;
    links[Common::OEM][Common::RACKSCALE][Common::ODATA_TYPE] = "#Intel.Oem.EthernetInterface";
    r[Common::LINKS] = std::move(links);

    return r;
}
} // namespace

bool isValidIpAddress(char *ipAddress)
{
    struct sockaddr_in sa;
    struct sockaddr_in6 sa6;	
	
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
	
    if ( result  == 0)
         result = inet_pton(AF_INET6, ipAddress, &(sa6.sin6_addr));		
	
    return result != 0;
}

endpoint::ManagerNetworkInterface::ManagerNetworkInterface(const std::string& path) : EndpointBase(path) {}

endpoint::ManagerNetworkInterface::~ManagerNetworkInterface() {}

void endpoint::ManagerNetworkInterface::get(const server::Request &req, server::Response &res)
{
	auto manager = psme::rest::model::Find<agent_framework::model::Manager>(req.params[PathParam::MANAGER_ID]).get();
	auto switch_ids = agent_framework::module::NetworkComponents::get_instance()->get_switch_manager().get_ids(manager.get_uuid());

	auto r = ::make_prototype();
	r[Common::ODATA_ID] = PathBuilder(req).build();
	r[Common::ID] = req.params[PathParam::NIC_ID];

	try
	{
		const json::Value config = configuration::Configuration::get_instance().to_json();
		const auto& nic_name = config["server"]["network-interface-name"].as_string();

		char command[HT_BUFFER_LEN] = {0};
		char resultA[HT_BUFFER_LEN] = {0};

		json::Value ipv6_confg;
		ipv6_confg[NetworkInterface::IPv6_DHCP_OPERATINMODE] = json::Value::Type::NIL;
		ipv6_confg[NetworkInterface::IPv6_DHCP_USE_DNS_SERVERS] = false;
		ipv6_confg[NetworkInterface::IPv6_DHCP_USE_NTP_SERVERS] = false;
		ipv6_confg[NetworkInterface::IPv6_DHCP_USE_RAPID_COMMIT] = false;					 
		r[NetworkInterface::IPv6_DHCP] = ipv6_confg;

		/*For DHCPv4/6 Config.*/
		/*For get Hostname/FQDN*/

		HostnameConfig hostcfg;
		std::string chostname;
		
		if(hostcfg.Get_Hostname(chostname) != -1)
		{
			r[IpAddress::HOST_NAME] = chostname;
			r[IpAddress::FQDN] = chostname;
		}

		/*For get addressOrigin*/
		psme::rest::utils::NetworkInterfaceInfo nic_info(nic_name);							
		const auto& nic_address = nic_info.get_interface_address();
		InterfaceConfig intf_ip;

		/*For DHCPv4/6 Config.*/
		json::Value ip_confg;
		ip_confg[NetworkInterface::IPv4_DHCP_USE_DNS_SERVERS]       = true;    //If use dhcp client, use it's dns server
		ip_confg[NetworkInterface::IPv4_DHCP_USE_DOMAIN_NAME]       = false;					 
		ip_confg[NetworkInterface::IPv4_DHCP_USE_GATEWAY]              = true;    //If use dhcp client, use it's gateway
		ip_confg[NetworkInterface::IPv4_DHCP_USE_NTP_SERVERS]        = false;
		ip_confg[NetworkInterface::IPv4_DHCP_USE_STATIC_ROUTES]    = false;					 

		if (!nic_address.get_ip_address().empty()) 
		{
			const auto& ip_address = nic_address.get_ip_address();
			const auto& subnet_mask = nic_address.get_netmask();
			const auto& mac_address = nic_address.get_mac_address();

			json::Value ipv4_address;
			ipv4_address[IpAddress::ADDRESS]        = ip_address;
			ipv4_address[IpAddress::SUBNET_MASK] = subnet_mask;

			std::string snic_name(nic_name.c_str());
			intf_ip.Get_Interface_Ip_Info(snic_name);
                     
                     if(intf_ip.get_dhcpv6_enable()== false)
                     {
                         if(intf_ip.get_ipv6_auto_enable()== true)
                         {
                              json::Value ip_auto_confg;
                              ip_auto_confg[NetworkInterface::IPv6_AUTO_CONFIG_ENABLED] = true;
                              r[NetworkInterface::STATELESS_ADDRESS_AUTO_CONFIG]=ip_auto_confg;
                         }
                         else
                         {					 	
                              json::Value ip_auto_confg;
                              ip_auto_confg[NetworkInterface::IPv6_AUTO_CONFIG_ENABLED] = false;
                              r[NetworkInterface::STATELESS_ADDRESS_AUTO_CONFIG]=ip_auto_confg;						  
                         }
		           r[NetworkInterface::IPv6_DHCP][NetworkInterface::IPv6_DHCP_OPERATINMODE] =  "Disabled";						 
                     }
                     else
                     {
                         //DHCPv6 only support Stateful mode //
		           r[NetworkInterface::IPv6_DHCP][NetworkInterface::IPv6_DHCP_OPERATINMODE] =  "Stateful";
                     }		 	
					 
			if(intf_ip.get_dhcpv4_enable()== true)
			{
				ipv4_address[IpAddress::ADDRESS_ORIGIN] = "DHCP";
				ip_confg[NetworkInterface::IPv4_DHCP_ENABLED] = true;
			}
			else
			{
				ipv4_address[IpAddress::ADDRESS_ORIGIN]         = "Static";
				ip_confg[NetworkInterface::IPv4_DHCP_ENABLED] = false;
				
			       json::Value ipv4_static_address;
				ipv4_static_address[IpAddress::ADDRESS] = ip_address;
				r[NetworkInterface::IPv4_STACIC_ADDRESSES].push_back(ipv4_static_address);
			}

			r[NetworkInterface::STATIC_NAME_SERVERS].push_back( intf_ip.Get_Static_DNS_Name_Server(snic_name).c_str());
			
			r[NetworkInterface::IPv4_DHCP] = ip_confg;	

			/*For get IP gateway*/
			sprintf(command, "route -n | grep UG | grep %s | awk '{print $2}'",  nic_name.c_str());
			memset(resultA,0x0, sizeof(resultA));
			EcCommonUtils::exec_shell(command, resultA, 0);

			if(strlen(resultA) != 0)
			{
				resultA[strcspn(resultA, "\r\n")]=0;
				ipv4_address[IpAddress::GATEWAY] = resultA;
			}	
			else
				ipv4_address[IpAddress::GATEWAY] = json::Value::Type::NIL;

			r[NetworkInterface::IPv4_ADDRESSES].push_back(std::move(ipv4_address));

			r[Common::MAC_ADDRESS] = mac_address;

			r[NetworkInterface::PERMANENT_MAC_ADDRESS] =mac_address;
		}
		else
		{
			log_warning(GET_LOGGER("rest"),
			"Empty IPv4 Address in the network interface read from the configuration file.");
		}

		r[NetworkInterface::SPEED] = intf_ip.get_link_speed();

		if(!strncmp("full", intf_ip.get_full_hal_status().c_str(), 4))
			r[NetworkInterface::FULL_DUPLEX] = true;
		else
			r[NetworkInterface::FULL_DUPLEX] = false;

		r[NetworkInterface::AUTO_NEG] = intf_ip.get_auto_nego_status();

		/*For link status*/

		if(intf_ip.get_link_up_down() | IFF_UP)
		{
			r[NetworkInterface::INTERFACE_ENABLED] = true;
			r[Common::STATUS][Common::STATE]  = "Enabled";
			r[Common::STATUS][Common::HEALTH] = "OK";
			r[Common::STATUS][Common::HEALTH_ROLLUP] = "OK";	
			r[NetworkInterface::LINK_STATUS] = "LinkUp";
		}					 	
		else
		{
			r[NetworkInterface::LINK_STATUS] = "LinkDown";	
			r[NetworkInterface::INTERFACE_ENABLED] = false;
			r[Common::STATUS][Common::STATE]  = "Disabled";
			r[Common::STATUS][Common::HEALTH] = "OK";
			r[Common::STATUS][Common::HEALTH_ROLLUP] = "OK";	
		}					 	

		r[NetworkInterface::MTU_SIZE] =   intf_ip.get_mtu();

		/*For support at least 4 DNS servers*/
		{
#define MAX_DNS 4
			char tmp1[MAX_DNS][128];

			sprintf(command, "cat /etc/resolv.conf | grep nameserver | awk '{print $2}'");
			memset(resultA,0x0, sizeof(resultA));
			EcCommonUtils::exec_shell(command, resultA, 0);

			sscanf(resultA, "%s\r\n%s\r\n%s\r\n%s" ,  tmp1[0] , tmp1[1],  tmp1[2] , tmp1[3]);

			for(int i = 0; i < MAX_DNS; i++)
			{
				if(strlen(tmp1[i])!=0 && isValidIpAddress(tmp1[i]))
				{
					json::Value nameservers_address;
					nameservers_address[IpAddress::ADDRESS] = tmp1[i];
					r[NetworkInterface::NAME_SERVERS].push_back(std::move( tmp1[i]));
				}
			}
		}

		/*For VALN*/
		sprintf(command, "mgmt_vlan.sh get vlan_port_count %s", nic_name.c_str());
		memset(resultA,0x0, sizeof(resultA));
		EcCommonUtils::exec_shell(command, resultA, 0);

		if(strlen(resultA) != 0)
		{
			int count=atoi(resultA);
			if(count == 1)
			{
				memset(resultA,0x0, sizeof(resultA));
				sprintf(command, "mgmt_vlan.sh get vlan_port_value %s 0", nic_name.c_str());

				EcCommonUtils::exec_shell(command, resultA, 0);
				if(strlen(resultA) == 1)
				{
					json::Value vlan;
					unsigned int vlanid= atoi(resultA);									      

					sprintf(command, "cat /sys/class/net/%s.%d/operstate", nic_name.c_str(), vlanid );	
					EcCommonUtils::exec_shell(command, resultA, 0);

					if(!strncmp("up", resultA, 2))
					 	vlan[Vlan::VLAN_ENABLE] = true;										
					else
					 	vlan[Vlan::VLAN_ENABLE] = false;

					vlan[Vlan::VLAN_ID] = vlanid;									 
					r[NetworkInterface::VLAN].push_back(std::move(vlan));
				}								 
			}
			else if( count > 1 )
			{
				json::Value rl(json::Value::Type::OBJECT);
				rl[Common::ODATA_ID] = PathBuilder(req).append(constants::EthernetSwitchPort::VLANS).build();
				r[NetworkInterface::VLANS].push_back(std::move(rl));
			}
		}		 

		/*For IPv6*/

		sprintf(command, "ipv6_status.sh get  gateway %s", nic_name.c_str());
		memset(resultA,0x0, sizeof(resultA));
		EcCommonUtils::exec_shell(command, resultA, 0);

		if(strlen(resultA) != 0)
		{
			resultA[strcspn(resultA, "\r\n")]=0;
			r[NetworkInterface::IPv6_DEFAULT_GATEWAY] = resultA;
		}
		else
			r[NetworkInterface::IPv6_DEFAULT_GATEWAY] = json::Value::Type::NIL;

		sprintf(command, "ipv6_status.sh get ipv6_count %s", nic_name.c_str());
		memset(resultA,0x0, sizeof(resultA));
		EcCommonUtils::exec_shell(command, resultA, 0);

		if(strlen(resultA) != 0)
		{
			int i = 0;
			int count=atoi(resultA);
			for(i=0 ; i< count; i++)
			{
			       bool bstatic = false;
				char tmp1[3][256];
				int prefix=0;
				sprintf(command, "ipv6_status.sh get ipv6_info %s %d", nic_name.c_str(), i);
				memset(resultA,0x0, sizeof(resultA));
				EcCommonUtils::exec_shell(command, resultA, 0);

				sscanf(resultA, "%s %d %s" ,  tmp1[0] , &prefix, tmp1[2]);
				json::Value ipv6_address;
				ipv6_address[IpAddress::ADDRESS] = tmp1[0];
				ipv6_address[IpAddress::PREFIX_LENGTH] = prefix;

				std::string DHCPv6("DHCPv6");

                            if((tmp1[2] == DHCPv6) && intf_ip.get_dhcpv6_enable() == false)
                            {
                                bstatic = true;								
				    ipv6_address[IpAddress::ADDRESS_ORIGIN] =  "Static";		
                            }
				else
				    ipv6_address[IpAddress::ADDRESS_ORIGIN] =  tmp1[2];
				
				ipv6_address[IpAddress::ADDRESS_STATE] = json::Value::Type::NIL;
				r[constants::NetworkInterface::IPv6_ADDRESSES].push_back(std::move(ipv6_address));		

				if(bstatic)
				{
					json::Value ipv6_static_address;
					ipv6_static_address[IpAddress::ADDRESS] = tmp1[0];
					ipv6_static_address[IpAddress::PREFIX_LENGTH] = prefix;
					r[constants::NetworkInterface::IPv6_STATIC_ADDRESSES].push_back(std::move(ipv6_static_address));		
				}
			}
		}		 
	}
	catch (const std::exception &ex)
	{
		log_warning(GET_LOGGER("rest"),"Read manage error!!");
	}

	set_response(res, r);
}

void endpoint::ManagerNetworkInterface::patch(const server::Request &request, server::Response &response)
{
	try
	{
		auto &secrf_pal = ecrf_pal_helper::Switch::get_instance();

		if (secrf_pal.get_nos_type() == "sonic")
		{
			response.set_status(server::status_5XX::NOT_IMPLEMENTED);
			return;
		}

    auto json = JsonValidator::validate_request_body<schema::EthernetInterfacePatchSchema>(request);
   
    const json::Value config = configuration::Configuration::get_instance().to_json();
    std::string intf_name = config["server"]["network-interface-name"].as_string(); 
    bool DHCPv4Enable = false;
    bool IPv6SLASSEnable = false;
    std::string DHCPIPv6Mode = "Disabled";

    InterfaceConfig intf_ip;

    if(json.is_member(constants::EthernetInterface::DHCPv4) && json[constants::EthernetInterface::DHCPv4].is_member(constants::EthernetInterface::DHCP_Enabled))
    {
        DHCPv4Enable = json[constants::EthernetInterface::DHCPv4][constants::EthernetInterface::DHCP_Enabled].as_bool();
		
        if(DHCPv4Enable == false)
        {
            if (json.is_member(constants::EthernetInterface::HOSTNAME)) 
            {
                std::string hostname = json[constants::EthernetInterface::HOSTNAME].as_string();
                if(hostname.length() <= HOST_NAME_MAX)
                {
                    HostnameConfig hostcfg;
                    hostcfg.Set_Hostname(hostname);		
                }
                else
                    response.set_status(server::status_4XX::BAD_REQUEST);		
            }
        
            if (json.is_member(constants::EthernetInterface::IPv4_ADDRESSES)) 
            {
                //Set Static IPv4 address here //
                std::string intf_ipaddr                     = json[constants::EthernetInterface::IPv4_ADDRESSES][0][constants::EthernetInterface::ADDRESSES].as_string();
                std::string intf_ipaddrmask            = json[constants::EthernetInterface::IPv4_ADDRESSES][0][constants::EthernetInterface::NETMASK].as_string();	
                std::string intf_gateway                  = json[constants::EthernetInterface::IPv4_ADDRESSES][0][constants::EthernetInterface::GATEWAY].as_string();	
                std::string intf_static_nameserver = json[constants::EthernetInterface::STATIC_NAME_SERVERS][0][constants::EthernetInterface::ADDRESSES].as_string();	
                intf_ip.Set_Interface_Ipv4_Info(intf_name, intf_ipaddr, intf_ipaddrmask,intf_gateway, intf_static_nameserver);
            }
        }
        else if(DHCPv4Enable == true)
        {
            intf_ip.Set_Interface_DHCPv4_Enable(intf_name);
        }
    }

    if( json.is_member(constants::NetworkInterface::IPv6_DHCP) && json[constants::NetworkInterface::IPv6_DHCP].is_member(constants::NetworkInterface::IPv6_DHCP_OPERATINMODE))
    {
        //DHCPv6 mode
        DHCPIPv6Mode = json[constants::NetworkInterface::IPv6_DHCP][NetworkInterface::IPv6_DHCP_OPERATINMODE].as_string();
        if(DHCPIPv6Mode != "Disabled")
        {
            if(DHCPIPv6Mode == "Stateful")
                intf_ip.Set_Interface_IPv6_DHCP_Enable(intf_name);
        }
        else if(DHCPIPv6Mode == "Disabled")
            intf_ip.Set_Interface_IPv6_Disable(intf_name);
    }
    else
    {
        // IPv6 SLASS/STATIC mode //
        if(json.is_member(NetworkInterface::STATELESS_ADDRESS_AUTO_CONFIG) && json[NetworkInterface::STATELESS_ADDRESS_AUTO_CONFIG].is_member(NetworkInterface::IPv6_AUTO_CONFIG_ENABLED))
        {
            IPv6SLASSEnable = json[NetworkInterface::STATELESS_ADDRESS_AUTO_CONFIG][NetworkInterface::IPv6_AUTO_CONFIG_ENABLED].as_bool();
            if(IPv6SLASSEnable == false)
            {
                if (json.is_member(NetworkInterface::IPv6_ADDRESSES)) 
                {	
                    //Set Static IPv6 address here //
                    intf_ip.Set_Interface_IPv6_Disable(intf_name);
        			
                    std::string intf_ipaddr = {};
                    std::string intf_gateway= {};	
                    std::string intf_static_nameserver= {};	
                    std::string intf_ipaddrmask = {};  
        			
                    if (json[NetworkInterface::IPv6_ADDRESSES][0].is_member(IpAddress::ADDRESS)) 
                        intf_ipaddr = json[NetworkInterface::IPv6_ADDRESSES][0][IpAddress::ADDRESS].as_string();
        			
                    if (json[NetworkInterface::IPv6_ADDRESSES][0].is_member(IpAddress::PREFIX_LENGTH)) 
                         intf_ipaddrmask = json[NetworkInterface::IPv6_ADDRESSES][0][IpAddress::PREFIX_LENGTH].as_string();
        
                    if (json.is_member(constants::NetworkInterface::IPv6_Static_DEFAULT_GATEWAY)) 
                        intf_gateway = json[constants::NetworkInterface::IPv6_Static_DEFAULT_GATEWAY].as_string();	
        
                    if (json.is_member(constants::EthernetInterface::STATIC_NAME_SERVERS)) 	
                        intf_static_nameserver = json[constants::EthernetInterface::STATIC_NAME_SERVERS][0][constants::EthernetInterface::ADDRESSES].as_string();	
                    
                    intf_ip.Set_Interface_Ipv6_Info(intf_name, intf_ipaddr, intf_ipaddrmask,intf_gateway, intf_static_nameserver);	
                }
            }
            else
            {
                intf_ip.Set_Interface_IPv6_SLAAC_Enable(intf_name);    
            }	
        }
    }

    intf_ip.Restart();
}
	catch (const std::exception &ex)
	{
		log_warning(GET_LOGGER("rest"), "ManagerNetworkInterface::patch error!!");
	}
}
