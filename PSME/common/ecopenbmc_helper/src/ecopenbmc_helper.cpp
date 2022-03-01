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
#include "ecopenbmc_helper/ecopenbmc_helper.hpp"
#include "ecsys_helper/ecsys_helper.hpp"
#include "eclog_helper/eclog_helper.hpp"
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <queue>
#include <unistd.h>	
#include <fcntl.h>	
#include <pthread.h>

using namespace ecopenbmc_helper;
using namespace ecsys_helper;
using namespace eclog_helper;

static ecOpenBmc_helper *g_ecOpenBmc_helper = NULL;
auto &gADBG = ADbg::get_instance();

#define REMOVE_RETURN_CHAR(result) result.erase(std::remove(result.begin(), result.end(), '\n'), result.end())
bool ecOpenBmc_helper::get_status()
{
    std::string result = {};
    char cmd[HT_BUFFER_LEN] = {};
    sprintf(cmd, "%s", "ipmitool mc info| grep 'Device ID'");
    HelperTools::exec_shell_(cmd, result, 3);
    if (result.find("Device ID") != std::string::npos)
    {
        m_enabled = true;
        return m_enabled;
    }
    else
    {
        m_enabled = false;
        return m_enabled;
    }
}

ecOpenBmc_helper::ecOpenBmc_helper()
{
    if (get_status())
    {
        std::string result = {};
        char cmd[HT_BUFFER_LEN] = {};
        sprintf(cmd, "%s", "ipmitool mc info| grep 'Firmware Revision'");
        HelperTools::exec_shell_(cmd, result, 3);
        if (result.find("Firmware Revision") != std::string::npos)
        {
            std::size_t pos = result.find_last_of("Firmware Revision         : ");
            m_fw_version = result.substr(pos + 1);
            REMOVE_RETURN_CHAR(m_fw_version);
        }
    }
    return;
}

ecOpenBmc_helper::~ecOpenBmc_helper()
{
    return;
}

ecOpenBmc_helper&ecOpenBmc_helper::get_instance()
{
    if (NULL == g_ecOpenBmc_helper)
    {
        g_ecOpenBmc_helper= new ecOpenBmc_helper();
    }
    return *g_ecOpenBmc_helper;
}

std::string get_str_between_two_str( std::string &oStr, std::string sStr1, std::string sStr2);
std::string get_str_between_two_str( std::string &oStr, std::string sStr1, std::string sStr2)
{
    std::string rStr;
    int start = oStr.find(sStr1);
    if (start >= 0)
    {
        string tstr = oStr.substr(start + sStr1.length());
        int stop = tstr.find(sStr2);
        if (stop > 1)
            rStr = oStr.substr(start + sStr1.length(), stop);
        else
            rStr = "Error";
    }
    else
        rStr = "Error";
    return rStr;
}

bool ecOpenBmc_helper::get_ipv4_info(ipv4_info_t &ipv4info)
{
    if(m_enabled)
    {
        std::string result = {};
        char cmd[HT_BUFFER_LEN] = {};
        sprintf(cmd, "%s", "ipmitool lan print");
        HelperTools::exec_shell_(cmd, result, 3);
        if (result.find("IP Address") != std::string::npos)
        {
            std::string type = get_str_between_two_str(result, "IP Address Source       : ", "\n");
            if (type == "DHCP Address")
                m_ipv4_info.m_ipv4_type = "DHCP";
            else
                m_ipv4_info.m_ipv4_type = "Static";
            m_ipv4_info.m_ipv4_address = get_str_between_two_str(result, "IP Address              : ", "\n");
            m_ipv4_info.m_ipv4_subnetmask = get_str_between_two_str(result, "Subnet Mask             : ", "\n");
            m_ipv4_info.m_mac_address = get_str_between_two_str(result, "MAC Address             : ", "\n");
            m_ipv4_info.m_default_gateway = get_str_between_two_str(result, "Default Gateway IP      : ", "\n");

            gADBG.acc_printf(LEVEL_INFO, "[OpenBMC Helper] ip type[%s]", m_ipv4_info.m_ipv4_type.c_str());
            gADBG.acc_printf(LEVEL_INFO, "[OpenBMC Helper] ipaddr[%s]", m_ipv4_info.m_ipv4_address.c_str());
            gADBG.acc_printf(LEVEL_INFO, "[OpenBMC Helper] subnetmask[%s]", m_ipv4_info.m_ipv4_subnetmask.c_str());
            gADBG.acc_printf(LEVEL_INFO, "[OpenBMC Helper] mac_addr[%s]", m_ipv4_info.m_mac_address.c_str());
            gADBG.acc_printf(LEVEL_INFO, "[OpenBMC Helper] def_gw_ipaddr[%s]", m_ipv4_info.m_default_gateway.c_str());

            ipv4info = m_ipv4_info;
        }
        sprintf(cmd, "%s", "ipmitool mc getsysinfo system_name");
        HelperTools::exec_shell_(cmd, result, 3);
        REMOVE_RETURN_CHAR(result); 
        m_hostname = result;
        return true;
    }
    else
    {
        return false;
    }
}
