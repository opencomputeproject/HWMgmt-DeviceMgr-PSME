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

#include "ecrf_pal_helper/api/ecrf_pal_com.hpp"
#include "eclog_helper/eclog_helper.hpp"
#include "ecsys_helper/ecsys_helper.hpp"
#include <cstring>
#include <atomic>
using namespace ecsys_helper;
using namespace eclog_helper;

namespace ecrf_pal_com_s
{
auto &gADbg = ADbg::get_instance();

static ecrf_pal_com *g_ecrf_pal_com = NULL;
bool ecrf_pal_com::m_onlp_lib_init = false;

ecrf_pal_com &ecrf_pal_com::get_instance()
{
    try
    {
        if (NULL == g_ecrf_pal_com)
        {
            ifstream onl_f("/etc/onl/platform");
            ifstream sonic_d("/etc/sonic");
            if (onl_f.good())
            {
                gADbg.acc_printf(LEVEL_INFO,"ONLP NOS env.!!!!!");
                g_ecrf_pal_com = new ecrf_pal_com_onl();
            }
            else if(sonic_d.good())
            {
                gADbg.acc_printf(LEVEL_INFO,"SONiC NOS env.!!!!");
                g_ecrf_pal_com = new ecrf_pal_com_sonic();
            }
            else
                gADbg.acc_printf(LEVEL_WARN,"CURRENT NOS not SUPPORTED!!!!");
        }
        return *g_ecrf_pal_com;
    }
    catch (const std::exception &e)
    {
        std::cout << "ecrf_pal_com get_instance- exception : " << e.what() << std::endl;
        return *g_ecrf_pal_com;
    }
}

int ecrf_pal_com::rfpal_fan_speed_thresholds_com_set(unsigned id, rfpal_thresholds threshold_info)
{
    Json::Value th_s;
    Json::Reader j_reader = {};

    std::ifstream th_file(m_fan_threshold_file_path);

    if(th_file.good())
    {
        bool isJson = (j_reader.parse(th_file, th_s));
        if (isJson)
        {
            th_file.close();
            std::ofstream th_o_file(m_fan_threshold_file_path);
            Json::StyledWriter styledWriter;
            std::string t_id = std::to_string(id);
            th_s[t_id.c_str()]["warning"] = threshold_info.warning_upper;
            th_s[t_id.c_str()]["error"] = threshold_info.error;
            th_s[t_id.c_str()]["shutdown"] = threshold_info.shutdown;

            th_o_file << styledWriter.write(th_s);
            th_o_file.close();
        }
        else
            gADbg.acc_printf(LEVEL_WARN, "get fan threshold id[%d] json file format error!!", id);
    }
    else
    {   //No file exist , create one //
        Json::Value th_s_r;
        std::ofstream th_o_file(m_fan_threshold_file_path);
        Json::StyledWriter styledWriter;
        std::string t_id = std::to_string(id);
        th_s_r[t_id.c_str()]["warning"] = threshold_info.warning_upper;
        th_s_r[t_id.c_str()]["error"] = threshold_info.error;
        th_s_r[t_id.c_str()]["shutdown"] = threshold_info.shutdown;
        th_o_file << styledWriter.write(th_s_r);
        th_o_file.close();
    }
    return ECRF_PAL_STATUS_E_GENERIC;
}
#define IMISED(x) ()
int ecrf_pal_com::rfpal_fan_speed_thresholds_com_get(unsigned id, rfpal_thresholds *threshold_info)
{
    //Todo
    (void) (id);
    (void) (threshold_info);
    return ECRF_PAL_STATUS_E_GENERIC;
}

int ecrf_pal_com::rfpal_thermal_thresholds_com_set(unsigned id, rfpal_thresholds threshold_info)
{
    lock_guard<mutex> lock{m_mutex};
    Json::Value th_s;
    Json::Reader j_reader = {};
    std::ifstream th_file(m_thermal_threshold_file_path);
    gADbg.acc_printf(LEVEL_INFO, "rfpal_thermal_thresholds_com_set");

    if(th_file.good())
    {
        bool isJson = (j_reader.parse(th_file, th_s));
        if (isJson)
        {
            th_file.close();
            std::ofstream th_o_file(m_thermal_threshold_file_path);
            Json::StyledWriter styledWriter;
            std::string t_id = std::to_string(id);
            th_s[t_id.c_str()]["warning_upper"] = threshold_info.warning_upper * 1000;
            th_s[t_id.c_str()]["warning_lower"] = threshold_info.warning_lower * 1000;
            th_o_file << styledWriter.write(th_s);
            th_o_file.close();
            gADbg.acc_printf(LEVEL_INFO, "rfpal_thermal_thresholds_com_set and update[%d] ok!", id);
        }
        else
        {
            gADbg.acc_printf(LEVEL_WARN,"get thermal threshold id[%d] json file format error!!", id);
            return ECRF_PAL_STATUS_E_GENERIC;
        }
    }
    else
    {   //No file exist , create one //
        Json::Value th_s_r;
        std::ofstream th_o_file(m_thermal_threshold_file_path);
        Json::StyledWriter styledWriter;
        std::string t_id = std::to_string(id);
        th_s_r[t_id.c_str()]["warning_upper"] = threshold_info.warning_upper;
        th_s_r[t_id.c_str()]["warning_lower"] = threshold_info.warning_lower;
        th_s_r[t_id.c_str()]["error"] = threshold_info.error;
        th_s_r[t_id.c_str()]["shutdown"] = threshold_info.shutdown;
        th_o_file << styledWriter.write(th_s_r);
        th_o_file.close();
    }
    return ECRF_PAL_STATUS_OK;
}
int ecrf_pal_com::rfpal_create_default_sys_thresholds()
{
    Json::Value th_s;
    std::ofstream th_o_file(m_sys_threshold_file_path);
    Json::StyledWriter styledWriter;

    th_s["CPU"]["warning_upper"] = 95000;
    th_s["CPU"]["warning_lower"] = 10000;
    th_s["MEM"]["warning_upper"] = 90000;
    th_s["MEM"]["warning_lower"] = 20000;
    th_s["STORAGE"]["warning_upper"] = 90000;
    th_s["STORAGE"]["warning_lower"] = 10000;

    th_o_file << styledWriter.write(th_s);
    th_o_file.close();
    gADbg.acc_printf(LEVEL_WARN, "rfpal_create_default_sys_thresholds!!");
    return ECRF_PAL_STATUS_OK;
}

int ecrf_pal_com::rfpal_sys_thresholds_com_set(ecrf_sys_threshold_info_t type, rfpal_thresholds threshold_info)
{
    lock_guard<mutex> lock{m_mutex_sys};
    Json::Value th_s;
    Json::Reader j_reader = {};
    std::ifstream th_file(m_sys_threshold_file_path);
    gADbg.acc_printf(LEVEL_INFO, "rfpal_sys_thresholds_com_set");

    if (th_file.good())
    {
        bool isJson = (j_reader.parse(th_file, th_s));
        if (isJson)
        {
            th_file.close();
            std::ofstream th_o_file(m_sys_threshold_file_path);
            Json::StyledWriter styledWriter;

            switch (type)
            {
            case ECRF_PAL_SYS_CPU:
                if(threshold_info.warning_upper !=0)
                {
                    gADbg.acc_printf(LEVEL_INFO, "rfpal_sys_thresholds_com_set upper[%f] ", threshold_info.warning_upper);
                    th_s["CPU"]["warning_upper"] = threshold_info.warning_upper * 1000;
                }

                if(threshold_info.warning_lower !=0)
                {
                    gADbg.acc_printf(LEVEL_INFO, "rfpal_sys_thresholds_com_set lower[%f] ", threshold_info.warning_lower);
                    th_s["CPU"]["warning_lower"] = threshold_info.warning_lower * 1000;
                }
                break;
            case ECRF_PAL_SYS_MEM:
                th_s["MEM"]["warning_upper"] = threshold_info.warning_upper * 1000;
                th_s["MEM"]["warning_lower"] = threshold_info.warning_lower * 1000;
                break;
            case ECRF_PAL_SYS_STORAGE:
                th_s["STORAGE"]["warning_upper"] = threshold_info.warning_upper * 1000;
                th_s["STORAGE"]["warning_lower"] = threshold_info.warning_lower * 1000;
                break;
            default:
                break;
            }

            th_o_file << styledWriter.write(th_s);
            th_o_file.close();
            gADbg.acc_printf(LEVEL_INFO, "rfpal_sys_thresholds_com_set ok!");
        }
        else
        {
            gADbg.acc_printf(LEVEL_WARN, "set sys threshold json file format error!!");
            return ECRF_PAL_STATUS_E_GENERIC;
        }
    }
    else
    {
        rfpal_create_default_sys_thresholds();
        gADbg.acc_printf(LEVEL_WARN, "set sys threshold json file format error!!");
        return ECRF_PAL_STATUS_E_GENERIC;
    }
    return ECRF_PAL_STATUS_OK;
}

int ecrf_pal_com::rfpal_sys_thresholds_com_get(ecrf_sys_threshold_info_t type, rfpal_thresholds *threshold_info)
{
    lock_guard<mutex> lock{m_mutex_sys};
    try
    {
        Json::Value th_s;
        Json::Reader j_reader = {};

        std::ifstream th_file(m_sys_threshold_file_path);

        if (th_file.good())
        {
            bool isJson = (j_reader.parse(th_file, th_s));

            if (isJson)
            {
                switch (type)
                {
                case ECRF_PAL_SYS_CPU:
                    threshold_info->warning_upper = th_s["CPU"]["warning_upper"].asFloat();
                    threshold_info->warning_lower = th_s["CPU"]["warning_lower"].asFloat();
                    break;
                case ECRF_PAL_SYS_MEM:
                    threshold_info->warning_upper = th_s["MEM"]["warning_upper"].asFloat();
                    threshold_info->warning_lower = th_s["MEM"]["warning_lower"].asFloat();
                    break;
                case ECRF_PAL_SYS_STORAGE:
                    threshold_info->warning_upper = th_s["STORAGE"]["warning_upper"].asFloat();
                    threshold_info->warning_lower = th_s["STORAGE"]["warning_lower"].asFloat();
                    break;
                default:
                    break;
                }
            }
            th_file.close();
        }
        else
        {
            gADbg.acc_printf(LEVEL_WARN, "Get SYS threshold error!! file not found");
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "ecrf_pal_com  () - exception : " << e.what() << std::endl;
        return ECRF_PAL_STATUS_E_GENERIC;
    }
    return ECRF_PAL_STATUS_OK;
}

int ecrf_pal_com::rfpal_thermal_thresholds_com_get(unsigned id, rfpal_thresholds *threshold_info)
{
    lock_guard<mutex> lock{m_mutex};
    try
    {
        Json::Value th_s;
        Json::Reader j_reader = {};

        std::ifstream th_file(m_thermal_threshold_file_path);

        if(th_file.good())
        {
            bool isJson = (j_reader.parse(th_file, th_s));

            if (isJson)
            {
                std::string t_id = std::to_string(id);
                threshold_info->warning_upper = th_s[t_id.c_str()]["warning_upper"].asFloat();
                threshold_info->warning_lower = th_s[t_id.c_str()]["warning_lower"].asFloat();
                threshold_info->error = th_s[t_id.c_str()]["error"].asFloat();
                threshold_info->shutdown = th_s[t_id.c_str()]["shutdown"].asFloat();
            }
            th_file.close();
        }
        else
        {
            threshold_info->warning_lower = ECRF_PAL_CONST_NOT_SUPPORTED;
            threshold_info->warning_upper = ECRF_PAL_CONST_NOT_SUPPORTED;
            threshold_info->error = ECRF_PAL_CONST_NOT_SUPPORTED;
            threshold_info->shutdown = ECRF_PAL_CONST_NOT_SUPPORTED;
            gADbg.acc_printf(LEVEL_WARN,"Get thermal threshold id[%d] error!! file not found", id);
            return ECRF_PAL_STATUS_E_GENERIC;
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "ecrf_pal_com  () - exception : " << e.what() << std::endl;
        return ECRF_PAL_STATUS_E_GENERIC;
    }

    return ECRF_PAL_STATUS_OK;
}

int ecrf_pal_com::rfpal_sysfs_node_get(const char* sysfsnode_path, char* sysfsnode_value)
{
    FILE *node_read=NULL;		
    if ((node_read = fopen(sysfsnode_path, "r")) == NULL)
    {
        return ECRF_PAL_STATUS_E_INVALID;
    }
    if ((fgets(sysfsnode_value, 64, node_read)) == NULL)
    {
        return ECRF_PAL_STATUS_E_INVALID;
    }
    fclose(node_read);
    return ECRF_PAL_STATUS_OK;
}

int ecrf_pal_com::rfpal_sysfs_node_set(const char* sysfsnode_path, char* sysfsnode_value)
{
    FILE *node_write=NULL;		
		
    if ((node_write = fopen(sysfsnode_path, "w+")) == NULL)
    {
        return ECRF_PAL_STATUS_E_INVALID;
    }
    
    if (fputs(sysfsnode_value, node_write) <= 0)
    {
        fclose(node_write);
        return ECRF_PAL_STATUS_E_INVALID;
    }
        
    fclose(node_write);
    return ECRF_PAL_STATUS_OK;
}

int ecrf_pal_com::rfpal_system_cmd_set(const char* system_cmd, char* result_value)
{
    FILE *fp = NULL;

    if ((fp = popen(system_cmd, "r")) == NULL)
    {
        printf("%s error\n", system_cmd);
        return ECRF_PAL_STATUS_E_INVALID;
    }

    if (fgets(result_value, 64, fp) != NULL)
    {
    }
    pclose(fp);
 
    result_value[strlen(result_value)-1] = 0;
    return ECRF_PAL_STATUS_OK;
}
} // namespace ecrf_pal_com_s
