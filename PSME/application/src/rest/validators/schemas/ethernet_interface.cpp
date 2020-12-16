/*
* Edgecore DeviceManager
* Copyright 2020-2021 Edgecore Networks, Inc.
 *
* Licensed to the Apache Software Foundation (ASF) under one
* or more contributor license agreements.  See the NOTICE file
* distributed with this work for additional information
* regarding copyright ownership.  The ASF licenses this file
* to you under the Apache License, Version 2.0 (the
* "License"); you may not use this file except in compliance
* with the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
* KIND, either express or implied.  See the License for the
* specific language governing permissions and limitations
* under the License.
*/

#include "psme/rest/validators/schemas/ethernet_interface.hpp"
#include "psme/rest/validators/schemas/common.hpp"
#include "psme/rest/constants/constants.hpp"

#include "agent-framework/module/enum/network.hpp"

using namespace psme::rest;
using namespace psme::rest::validators::schema;
using namespace agent_framework::model;

const jsonrpc::ProcedureValidator& EthernetInterfacePatchSchema::DHCPv6Schema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "ethernet_interface_dhcpv6_patch",
        jsonrpc::PARAMS_BY_NAME,
        constants::NetworkInterface::IPv6_DHCP_OPERATINMODE, VALID_OPTIONAL(VALID_JSON_STRING),
        nullptr
    };
    return procedure;
}

const jsonrpc::ProcedureValidator& EthernetInterfacePatchSchema::IPv6Schema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "ethernet_interface_ipv6_patch",
        jsonrpc::PARAMS_BY_NAME,
        constants::IpAddress::ADDRESS, VALID_OPTIONAL(VALID_JSON_STRING),
        constants::IpAddress::PREFIX_LENGTH, VALID_OPTIONAL(VALID_JSON_STRING),
        nullptr
    };
    return procedure;
}


const jsonrpc::ProcedureValidator& EthernetInterfacePatchSchema::IPv4Schema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "ethernet_interface_ipv4_patch",
        jsonrpc::PARAMS_BY_NAME,
        constants::IpAddress::ADDRESS, VALID_OPTIONAL(VALID_JSON_STRING),
        constants::IpAddress::SUBNET_MASK, VALID_OPTIONAL(VALID_JSON_STRING),
        constants::IpAddress::GATEWAY, VALID_OPTIONAL(VALID_JSON_STRING),
        nullptr
    };
    return procedure;
}

const jsonrpc::ProcedureValidator& EthernetInterfacePatchSchema::DHCPv4Schema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "ethernet_interface_dhcpv4_patch",
        jsonrpc::PARAMS_BY_NAME,
        constants::NetworkInterface::IPv4_DHCP_ENABLED, VALID_OPTIONAL(VALID_JSON_BOOLEAN),
        constants::NetworkInterface::IPv4_DHCP_USE_DNS_SERVERS, VALID_OPTIONAL(VALID_JSON_BOOLEAN),
        nullptr
    };
    return procedure;
}

const jsonrpc::ProcedureValidator& EthernetInterfacePatchSchema::STATELESS_ADDRESS_AUTO_Schema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "ethernet_interface_stateless_auto_patch",
        jsonrpc::PARAMS_BY_NAME,
        constants::NetworkInterface::IPv6_AUTO_CONFIG_ENABLED, VALID_OPTIONAL(VALID_JSON_BOOLEAN),
        nullptr
    };
    return procedure;
}

const jsonrpc::ProcedureValidator& EthernetInterfacePatchSchema::IPv4StaticNameServerSchema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "ethernet_interface_ipv4_name_server_patch",
        jsonrpc::PARAMS_BY_NAME,
        constants::IpAddress::ADDRESS, VALID_OPTIONAL(VALID_JSON_STRING),
        nullptr
    };
    return procedure;
}

const jsonrpc::ProcedureValidator& EthernetInterfacePatchSchema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "ethernet_interface_patch",
        jsonrpc::PARAMS_BY_NAME,
        constants::NetworkInterface::HOST_NAME, VALID_OPTIONAL(VALID_JSON_STRING),
        constants::NetworkInterface::FQDN, VALID_OPTIONAL(VALID_JSON_STRING),       

        constants::NetworkInterface::IPv6_DHCP, VALID_OPTIONAL(VALID_ATTRIBUTE(DHCPv6Schema)),
        constants::NetworkInterface::IPv6_AUTO_CONFIG_ENABLED, VALID_OPTIONAL(VALID_JSON_BOOLEAN),      
        constants::NetworkInterface::IPv6_DEFAULT_GATEWAY, VALID_OPTIONAL(VALID_JSON_STRING),       
        constants::NetworkInterface::IPv6_ADDRESSES, VALID_OPTIONAL(VALID_ARRAY_OF(VALID_ATTRIBUTE(IPv6Schema))),        
        constants::NetworkInterface::IPv6_Static_DEFAULT_GATEWAY, VALID_OPTIONAL(VALID_JSON_STRING),        
        constants::NetworkInterface::STATIC_NAME_SERVERS, VALID_OPTIONAL(VALID_ARRAY_OF(VALID_ATTRIBUTE(IPv4StaticNameServerSchema))),        
        constants::NetworkInterface::IPv4_DHCP, VALID_OPTIONAL(VALID_ATTRIBUTE(DHCPv4Schema)),
        constants::NetworkInterface::STATELESS_ADDRESS_AUTO_CONFIG, VALID_OPTIONAL(VALID_ATTRIBUTE(STATELESS_ADDRESS_AUTO_Schema)),
        constants::NetworkInterface::IPv4_ADDRESSES, VALID_OPTIONAL(VALID_ARRAY_OF(VALID_ATTRIBUTE(IPv4Schema))),        
        nullptr
    };
    return procedure;
}
