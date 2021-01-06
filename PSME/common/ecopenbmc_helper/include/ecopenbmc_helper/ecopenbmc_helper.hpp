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

#ifndef EC_OPENBMC_HELPER_HPP
#define EC_OPENBMC_HELPER_HPP

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
#include <sys/ipc.h> 
#include <sys/msg.h> 

namespace ecopenbmc_helper
{

using namespace std;
typedef struct ipv4_info
{
    std::string m_ipv4_address = "";
    std::string m_ipv4_subnetmask = "";
    std::string m_ipv4_type = "";
    std::string m_mac_address = "";
    std::string m_default_gateway = "";
} ipv4_info_t;

class ecOpenBmc_helper
{
public:
    static ecOpenBmc_helper &get_instance();
    ecOpenBmc_helper();
    ~ecOpenBmc_helper();
    bool get_status();
    std::string get_fw_version() { return m_fw_version; };
    std::string get_hostname() { return m_hostname; };
    std::string get_ip_address() { return m_ipv4_info.m_ipv4_address; };
    bool get_ipv4_info(ipv4_info_t &ipv4info);

private:
    bool m_enabled = false;
    std::string m_fw_version = "";
    std::string m_hostname = "";
    ipv4_info_t m_ipv4_info={};
};
}
#endif
