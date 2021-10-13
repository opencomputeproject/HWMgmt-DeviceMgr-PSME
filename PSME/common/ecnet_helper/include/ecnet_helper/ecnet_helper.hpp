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
 * http://www.apache.org/licenses/LICENSE-2.0
 *
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
* KIND, either express or implied.  See the License for the
* specific language governing permissions and limitations
* under the License.
 */

#ifndef EC_NET_HELPER_HPP
#define EC_NET_HELPER_HPP

#include <syslog.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <ostream>
#include <string>
#include <limits.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <algorithm>
#include <regex>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <unistd.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>
#include <json/json.h>
#include <mutex>

extern char *__progname;
namespace ecnet_helper
{

using namespace std;

constexpr const size_t MAC_ADDRESS_LENGTH = 18;
constexpr const size_t BUFFER_SIZE = 512;

////////////////////////////////////////////////////////////////////
// To replace /etc/hosts /etc/hostname old hostname to new hostname
////////////////////////////////////////////////////////////////////
class ConfigMody
{
public:
    void virtual Restart(){};
    std::string m_tmp_file_name_1 = {};
    std::string m_tmp_file_name_2 = {};
};

class HostnameConfig : public ConfigMody
{
public:
    std::string m_file_hosts = {};
    std::string m_file_hostname = {};
    ifstream m_source_hosts = {};
    ifstream m_source_hostname = {};
    ofstream m_dest_hosts = {};
    ofstream m_dest_hostname = {};
    explicit HostnameConfig()
    {
        m_file_hosts = "/etc/hosts";
        m_file_hostname = "/etc/hostname";
        m_tmp_file_name_1 = "/tmp/tmp_hostname";
        m_tmp_file_name_2 = "/tmp/tmp_hostname_";
    }
    int Get_Hostname(std::string &hostname);
    int Set_Hostname(std::string &hostname);
    void Restart();

private:
    void Open();
    void Close();
};

////////////////////////////////////////////////////////////////////
// To modify /etc/network/interfaces dhcp/static content
////////////////////////////////////////////////////////////////////
class InterfaceConfig : public ConfigMody
{
public:
    std::string m_ifname = {};
    std::string m_file_network_interfaces = {};
    std::string m_file_network_dhcpv6 = {};
    std::string m_ipaddrmask = {};
    unsigned char m_mac[6];
    ifstream m_source_v4_interfaces = {};
    ofstream m_dest_v4_interfaces = {};

    ifstream m_source_v6_interfaces = {};
    ofstream m_dest_v6_interfaces = {};

    std::string m_psme_config= "/etc/psme/psme.conf";
    ifstream m_config = {};
    Json::Value m_psme_config_content = {};

    explicit InterfaceConfig() : m_ifname("ma1")
    {
        m_config.open(m_psme_config);

        if (m_config.good())
        {
            Json::Reader reader;
            bool isJsonOK = (reader.parse(m_config, m_psme_config_content));

            if (isJsonOK)
                m_ifname = m_psme_config_content["server"]["network-interface-name"].asString();
            m_config.close();
        }

        m_file_network_interfaces = "/etc/network/interfaces.d/" + m_ifname;
        m_tmp_file_name_1 = "/tmp/tmp_interfaces";
        m_dhcpv4_enable = false;

        m_file_network_dhcpv6 = "/etc/network/dhcpv6" + m_ifname; //Can't put it in interfaces.d or STATIC IPv4/v6 will have problem.
        m_tmp_file_name_2 = "/tmp/tmp_dhcpv6";
        m_dhcpv6_enable = false;
        m_Ipv6auto_enable = false;
        Get_Interface_DHCP_Enable(m_ifname);
    }

    int Get_Interface_Ip_Info(std::string &inf_name);
    int Set_Interface_Ipv4_Info(std::string &inf_name, std::string &ifr_ip, std::string &ifr_ipmask, std::string &ifr_gateway, std::string &ifr_nameserver);
    int Set_Interface_Ipv6_Info(std::string &inf_name, std::string &ifr_ip, std::string &ifr_ipmask, std::string &ifr_gateway, std::string &ifr_nameserver);
    int Get_Interface_DHCP_Enable(std::string &inf_name);
    int Set_Interface_DHCPv4_Enable(std::string &inf_name);
    int Set_Interface_IPv6_SLAAC_Enable(std::string &inf_name);
    int Set_Interface_IPv6_DHCP_Enable(std::string &inf_name);
    int Set_Interface_IPv6_Disable(std::string &inf_name);

    void Set_Interface_name(std::string &name)
    {
        m_ifname = name;
    }

    const std::string Get_Static_DNS_Name_Server(std::string &inf_name);

    const std::string &get_ip_address() const
    {
        return m_ipaddr;
    }

    const std::string &get_full_hal_status() const
    {
        return m_full;
    }

    bool get_auto_nego_status() const
    {
        return m_auto;
    }

    bool get_dhcpv4_enable() const
    {
        return m_dhcpv4_enable;
    }

    bool get_ipv6_auto_enable() const
    {
        return m_Ipv6auto_enable;
    }

    bool get_dhcpv6_enable() const
    {
        return m_dhcpv6_enable;
    }

    int get_link_speed() const
    {
        return m_speed;
    }

    int get_mtu() const
    {
        return m_mtu;
    }

    int get_link_up_down() const
    {
        return m_linkup;
    }

    void Restart();

private:
    std::string m_ipaddr = {};
    std::string m_full = {};
    bool m_auto = {};
    int m_speed = {};
    int m_mtu = {};
    int m_linkup = {};
    bool m_dhcpv4_enable = {};
    bool m_dhcpv6_enable = {};
    bool m_Ipv6auto_enable = {};

    void Open(std::string &type);
    void Close(std::string &type);
};
} // namespace ecnet_helper
#endif
