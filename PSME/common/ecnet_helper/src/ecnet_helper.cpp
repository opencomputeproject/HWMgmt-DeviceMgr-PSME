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

#include "ecnet_helper/ecnet_helper.hpp"
#include "ecsys_helper/ecsys_helper.hpp"

using namespace ecnet_helper;
using namespace ecsys_helper;

int HostnameConfig::Get_Hostname(std::string &hostname)
{
    struct utsname u;
    char name[256] = {};
    int status = uname(&u);
    if (-1 != status)
    {
        strncpy(name, u.nodename, sizeof(u.nodename));
        hostname = name;
    }
    return (status);
}

int HostnameConfig::Set_Hostname(std::string &new_hostname)
{
    int result = 1;
    std::string RLINE;
    std::string old_hostname;
    Get_Hostname(old_hostname);

    Open();

    while (!m_source_hosts.eof()) // To get you all the lines.
    {
        getline(m_source_hosts, RLINE); // Saves the line in .RLINE
        string sout = std::regex_replace(RLINE, std::regex(old_hostname), new_hostname);
        m_dest_hosts << sout; // Prints out.
        m_dest_hosts << '\n'; // Prints out.
    }

    while (!m_source_hostname.eof()) // To get you all the lines.
    {
        getline(m_source_hostname, RLINE); // Saves the line in .RLINE
        string sout = std::regex_replace(RLINE, std::regex(old_hostname), new_hostname);
        m_dest_hostname << sout; // Prints out.
        m_dest_hostname << '\n'; // Prints out.
    }

    remove(m_file_hosts.c_str());
    rename(m_tmp_file_name_1.c_str(), m_file_hosts.c_str());

    remove(m_file_hostname.c_str());
    rename(m_tmp_file_name_2.c_str(), m_file_hostname.c_str());

    result = sethostname(new_hostname.c_str(), strlen(new_hostname.c_str()));

    Close();
    return result;
}
void HostnameConfig::Open()
{
    m_source_hosts.open(m_file_hosts);
    m_source_hostname.open(m_file_hostname);
    m_dest_hosts.open(m_tmp_file_name_1);
    m_dest_hostname.open(m_tmp_file_name_2);
}

void HostnameConfig::Close()
{
    m_source_hosts.close();
    m_source_hostname.close();
    m_dest_hosts.close();
    m_dest_hostname.close();
}

void HostnameConfig::Restart()
{
    return;
}

int InterfaceConfig::Get_Interface_Ip_Info(std::string &inf_name)
{
    int fd = 0;
    struct ifreq ifr;
    int sock = 0;
    struct ethtool_cmd edata;

    int rc = 0;
    int result = 1;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;

    strncpy(ifr.ifr_name, inf_name.c_str(), IFNAMSIZ - 1);

    if (ioctl(fd, SIOCGIFADDR, &ifr) < 0)
    {
        printf("InterfaceConfig::Get_Interface_info ip error");
        result = -1;
    }
    else
        m_ipaddr = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);

    if (ioctl(fd, SIOCGIFNETMASK, &ifr) < 0)
    {
        printf("InterfaceConfig::Get_Interface_info mask error");
        result = -1;
    }
    else
        m_ipaddrmask = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);

    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0)
    {
        printf("InterfaceConfig::Get_Interface_info mac error");
        result = -1;
    }
    else
        memcpy(m_mac, ifr.ifr_ifru.ifru_hwaddr.sa_data, ETH_ALEN);

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    if (sock < 0)
    {
        printf("InterfaceConfig::Get_Interface_info speed error ");
        result = -1;
    }

    if (ioctl(fd, SIOCGIFMTU, &ifr) < 0)
    {
        printf("InterfaceConfig::Get_Interface_info mtu error");
        result = -1;
    }
    else
        m_mtu = ifr.ifr_mtu;

    if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0)
    {
        printf("InterfaceConfig::Get_Interface_info linkup down error");
        result = -1;
    }
    else
        m_linkup = ifr.ifr_flags;

    strncpy(ifr.ifr_name, inf_name.c_str(), sizeof(ifr.ifr_name));
    ifr.ifr_data = (caddr_t)&edata;

    edata.cmd = ETHTOOL_GSET;

    rc = ioctl(sock, SIOCETHTOOL, &ifr);
    if (rc < 0)
    {
        printf("InterfaceConfig::Get_Interface_info speed ioctl error ");
        result = -1;
    }
    switch (ethtool_cmd_speed(&edata))
    {
    case SPEED_10:
        m_speed = 10;
        break;
    case SPEED_100:
        m_speed = 100;
        break;
    case SPEED_1000:
        m_speed = 1000;
        break;
    case SPEED_2500:
        m_speed = 2500;
        break;
    case SPEED_10000:
        m_speed = 10000;
        break;
    default:
        m_speed = 1000;
    }

    switch (edata.duplex)
    {
    case DUPLEX_FULL:
        m_full = "full";
        break;
    case DUPLEX_HALF:
        m_full = "half";
        break;
    default:
        m_full = "full";
        break;
    }

    switch (edata.autoneg)
    {
    case AUTONEG_DISABLE:
        m_auto = false;
        break;
    case AUTONEG_ENABLE:
        m_auto = true;
        break;
    default:
        m_auto = true;
        break;
    }

    close(fd);
    close(sock);
    return result;
}

int InterfaceConfig::Get_Interface_DHCP_Enable(std::string &inf_name)
{
    int result = 1;
    std::string RLINE = {};
    std::size_t if_found = 0;
    std::size_t name_found = 0;
    std::size_t dhcp_found = 0;

    std::string type4("v4");

    Open(type4);
    //For ipv4
    if (m_source_v4_interfaces)
    {
        while (getline(m_source_v4_interfaces, RLINE))
        {
            if_found = RLINE.find("iface");
            name_found = RLINE.find(inf_name);

            if (if_found != std::string::npos && name_found != std::string::npos)
            {
                dhcp_found = RLINE.find("dhcp");

                if (dhcp_found != std::string::npos)
                    m_dhcpv4_enable = true;
                else
                    m_dhcpv4_enable = false;

                break;
            }
        }
    }
    Close(type4);

    std::string type6("v6");
    Open(type6);
    //For ipv6
    if (m_source_v6_interfaces)
    {
        while (getline(m_source_v6_interfaces, RLINE))
        {
            if_found = RLINE.find("iface");
            name_found = RLINE.find(inf_name);

            if (if_found != std::string::npos && name_found != std::string::npos)
            {
                dhcp_found = RLINE.find("inet6 auto");

                if (dhcp_found != std::string::npos)
                    m_Ipv6auto_enable = true;
                else
                    m_Ipv6auto_enable = false;

                dhcp_found = RLINE.find("inet6 dhcp");

                if (dhcp_found != std::string::npos)
                    m_dhcpv6_enable = true;
                else
                    m_dhcpv6_enable = false;

                break;
            }
        }
    }
    Close(type6);

    return result;
}

const std::string InterfaceConfig::Get_Static_DNS_Name_Server(std::string &inf_name)
{
    std::string zero("0.0.0.0");
    std::string RLINE = {};
    std::size_t if_found = 0;
    std::size_t dnsname_found = 0;

    std::string type("v4");

    Open(type);
    if (m_source_v4_interfaces)
    {
        while (!m_source_v4_interfaces.eof()) // To get you all the lines.
        {
            getline(m_source_v4_interfaces, RLINE);
            if_found = RLINE.find(inf_name);
            if (if_found != std::string::npos)
            {
                while (!m_source_v4_interfaces.eof()) // To get you all the lines.
                {
                    getline(m_source_v4_interfaces, RLINE);
                    dnsname_found = RLINE.find("dns-nameservers");

                    if (dnsname_found != std::string::npos)
                        return RLINE.substr(RLINE.find(" ") + 1);
                }
            }
        }
        Close(type);
    }
    return zero;
}

int InterfaceConfig::Set_Interface_DHCPv4_Enable(std::string &inf_name)
{
    std::string type("v4");
    remove(m_file_network_interfaces.c_str());
    remove(m_tmp_file_name_1.c_str());

    Open(type);

    m_dest_v4_interfaces << "auto " << inf_name << endl;
    m_dest_v4_interfaces << "iface " << inf_name << " inet dhcp" << endl;
    m_dest_v4_interfaces << "source " << m_file_network_dhcpv6 << endl;
    Close(type);
    rename(m_tmp_file_name_1.c_str(), m_file_network_interfaces.c_str());

    return 1;
}

int InterfaceConfig::Set_Interface_IPv6_SLAAC_Enable(std::string &inf_name)
{
    std::string type("v6");
    remove(m_file_network_dhcpv6.c_str());
    remove(m_tmp_file_name_2.c_str());

    Open(type);

    m_dest_v6_interfaces << "auto " << inf_name << endl;
    m_dest_v6_interfaces << "iface " << inf_name << " inet6 auto" << endl;
    m_dest_v6_interfaces << "dhclient -1 -6  -lf /var/lib/dhcp/dhclient6." << inf_name << ".leases -v " << inf_name << "|| true" << endl;

    Close(type);
    rename(m_tmp_file_name_2.c_str(), m_file_network_dhcpv6.c_str());

    return 1;
}

int InterfaceConfig::Set_Interface_IPv6_DHCP_Enable(std::string &inf_name)
{
    std::string type("v6");
    remove(m_file_network_dhcpv6.c_str());
    remove(m_tmp_file_name_2.c_str());

    Open(type);

    m_dest_v6_interfaces << "auto " << inf_name << endl;
    m_dest_v6_interfaces << "iface " << inf_name << " inet6 dhcp" << endl;
    m_dest_v6_interfaces << "dhclient -1 -6  -lf /var/lib/dhcp/dhclient6." << inf_name << ".leases -v " << inf_name << "|| true" << endl;

    Close(type);
    rename(m_tmp_file_name_2.c_str(), m_file_network_dhcpv6.c_str());

    return 1;
}

int InterfaceConfig::Set_Interface_IPv6_Disable(std::string &inf_name)
{
    char result[HT_BUFFER_LEN] = {};
    char cmd[HT_BUFFER_LEN] = {};
    sprintf(cmd, "%s %s", " ip -6 addr flush", inf_name.c_str());
    HelperTools::exec_shell(cmd, result, 0); 

    remove(m_file_network_dhcpv6.c_str());
    return 1;
}

int InterfaceConfig::Set_Interface_Ipv4_Info(std::string &inf_name, std::string &ifr_ip, std::string &ifr_ipmask, std::string &ifr_gateway, std::string &ifr_nameserver)
{
    std::string type("v4");
    struct ifreq ifr;
    const char *name = inf_name.c_str();
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    strncpy(ifr.ifr_name, name, IFNAMSIZ);

    ifr.ifr_addr.sa_family = AF_INET;
    inet_pton(AF_INET, ifr_ip.c_str(), ifr.ifr_addr.sa_data + 2);
    ioctl(fd, SIOCSIFADDR, &ifr);

    inet_pton(AF_INET, ifr_ipmask.c_str(), ifr.ifr_addr.sa_data + 2);
    ioctl(fd, SIOCSIFNETMASK, &ifr);

    remove(m_file_network_interfaces.c_str());
    remove(m_tmp_file_name_1.c_str());

    Open(type);

    m_dest_v4_interfaces << "auto " << inf_name << endl;
    m_dest_v4_interfaces << "iface " << inf_name << " inet static" << endl;

    if (ifr_ip.length() > 0)
        m_dest_v4_interfaces << "address " << ifr_ip << endl;

    if (ifr_gateway.length() > 0)
        m_dest_v4_interfaces << "gateway " << ifr_gateway << endl;

    if (ifr_nameserver.length() > 0)
        m_dest_v4_interfaces << "dns-nameservers " << ifr_nameserver << endl;

    if (ifr_ipmask.length() > 0)
        m_dest_v4_interfaces << "netmask " << ifr_ipmask << endl;

    m_dest_v4_interfaces << "source " << m_file_network_dhcpv6 << endl;

    Close(type);
    rename(m_tmp_file_name_1.c_str(), m_file_network_interfaces.c_str());

    return 1;
}

int InterfaceConfig::Set_Interface_Ipv6_Info(std::string &inf_name, std::string &ifr_ip, std::string &ifr_ipmask, std::string &ifr_gateway, std::string &ifr_nameserver)
{
    std::string type("v6");

    remove(m_file_network_dhcpv6.c_str()); // remove /etc/network/dhcpv6
    remove(m_tmp_file_name_2.c_str());     // remove tmp file //

    Open(type);

    m_dest_v6_interfaces << "auto " << inf_name << endl;
    m_dest_v6_interfaces << "iface " << inf_name << " inet6 static" << endl;

    if (ifr_ip.length() > 0)
        m_dest_v6_interfaces << "address " << ifr_ip << endl;

    if (ifr_gateway.length() > 0)
        m_dest_v6_interfaces << "gateway " << ifr_gateway << endl;

    if (ifr_nameserver.length() > 0)
        m_dest_v6_interfaces << "dns-nameservers " << ifr_nameserver << endl;

    if (ifr_ipmask.length() > 0)
        m_dest_v6_interfaces << "netmask " << ifr_ipmask << endl;

    Close(type);
    rename(m_tmp_file_name_2.c_str(), m_file_network_dhcpv6.c_str());

    return 1;
}

void InterfaceConfig::Open(std::string &type)
{
    if (type == "v4")
    {
        if (std::ifstream(m_file_network_interfaces.c_str()))
            m_source_v4_interfaces.open(m_file_network_interfaces);
        else
            m_source_v4_interfaces.open(m_file_network_interfaces, std::ios::out);

        if (std::ifstream(m_tmp_file_name_1.c_str()))
            m_dest_v4_interfaces.open(m_tmp_file_name_1);
        else
            m_dest_v4_interfaces.open(m_tmp_file_name_1, std::ios::out);
    }

    if (type == "v6")
    {
        if (std::ifstream(m_file_network_dhcpv6.c_str()))
            m_source_v6_interfaces.open(m_file_network_dhcpv6);
        else
            m_source_v6_interfaces.open(m_file_network_dhcpv6, std::ios::out);

        if (std::ifstream(m_tmp_file_name_2.c_str()))
            m_dest_v6_interfaces.open(m_tmp_file_name_2);
        else
            m_dest_v6_interfaces.open(m_tmp_file_name_2, std::ios::out);
    }
}

void InterfaceConfig::Close(std::string &type)
{
    if (type == "v4")
    {
        m_source_v4_interfaces.close();
        m_dest_v4_interfaces.close();
    }
    if (type == "v6")
    {
        m_source_v6_interfaces.close();
        m_dest_v6_interfaces.close();
    }
    return;
}

void InterfaceConfig::Restart()
{
    if (std::ifstream(m_file_network_interfaces.c_str()))
    {
        char result[HT_BUFFER_LEN] = {};
        char cmd[HT_BUFFER_LEN] = {};
        sprintf(cmd, "ifconfig %s down;service networking stop;ifconfig %s 0.0.0.0;sleep 5 ;service networking start ", m_ifname.c_str(), m_ifname.c_str());
        HelperTools::exec_shell(cmd, result, 0);
    }
    return;
}
