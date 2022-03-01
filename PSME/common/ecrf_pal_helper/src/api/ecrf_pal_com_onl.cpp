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

#include "ecrf_pal_helper/asxvolt16.hpp"
#include "ecrf_pal_helper/asgvolt64.hpp"
#include "ecrf_pal_helper/as5916_54xks.hpp"
#include "ecrf_pal_helper/as5916_54xk.hpp"
#include "ecrf_pal_helper/api/ecrf_pal_com.hpp"
#include "eclog_helper/eclog_helper.hpp"

#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <dirent.h>
#include <string.h>
#include <chrono>
#include <ctime>

using namespace eclog_helper;
auto &Dbg = ADbg::get_instance();
/*Fon onlp*/
#ifdef __cplusplus
extern "C"
{
#include <onlp/onlp.h>
#include <onlp/oids.h>
#include <unistd.h>
#include <onlp/sys.h>
#include <AIM/aim_log_handler.h>
#include <syslog.h>
#include <onlp/platformi/sysi.h>
#include <onlp/platformi/sfpi.h>
#include <onlp/sfp.h>
#include <onlp/fan.h>
#include <onlp/thermal.h>
#include <onlp/sys.h>
#include <onlp/psu.h>
#include <onlplib/mmap.h>
#include <AIM/aim.h>
}
#endif

#define UNUSED(x) (void)(x)

using namespace ecrf_pal_com_s;

int ecrf_pal_com_onl::m_com_onl_thermal_sen_max_num = 0;
int ecrf_pal_com_onl::m_com_onl_psu_max_num = 0;
int ecrf_pal_com_onl::m_com_onl_fan_max_num = 0;
int ecrf_pal_com_onl::m_com_onl_port_max_num = 0;

int (*d_onlp_init)() = NULL;
int (*d_onlp_sys_info_get)(onlp_sys_info_t *rv) = NULL;
void (*d_onlp_sys_info_free)(onlp_sys_info_t* info) = NULL;
int (*d_onlp_fan_info_get)(onlp_oid_t id, onlp_fan_info_t* rv) = NULL;
int (*d_onlp_sfpi_is_present)(int port) = NULL;
int (*d_onlp_thermal_info_get)(onlp_oid_t id, onlp_thermal_info_t* rv) = NULL;
int (*d_onlp_psu_info_get)(onlp_oid_t id, onlp_psu_info_t* rv) = NULL;
int (*d_onlp_oid_iterate)(onlp_oid_t oid, onlp_oid_type_t type, onlp_oid_iterate_f itf, void* cookie) = NULL;
int (*d_onlp_sfp_control_set)(int port, onlp_sfp_control_t control, int value) = NULL;
int (*d_onlp_sfp_control_get)(int port, onlp_sfp_control_t control, int * value) = NULL;

static int iterate_oids_callback__(onlp_oid_t oid, void *cookie)
{
    try
    {
        int type = ONLP_OID_TYPE_GET(oid);

        UNUSED(cookie);

        switch (type)
        {
        case ONLP_OID_TYPE_THERMAL:
            ecrf_pal_com_onl::increase_thermal_num();
            break;
        case ONLP_OID_TYPE_FAN:
            ecrf_pal_com_onl::increase_fan_num();
            break;
        case ONLP_OID_TYPE_PSU:
            ecrf_pal_com_onl::increase_psu_num();
            break;
        default:
            printf("NA[%d] \r\n", type);
            break;
        }
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cout << "iterate_oids_callback__ - exception : " << e.what() << std::endl;
        return ECRF_PAL_STATUS_E_GENERIC;
    }

}

int ecrf_pal_com_onl::rfpal_init()
{
    return ECRF_PAL_STATUS_OK;
}

ecrf_pal_com_onl::ecrf_pal_com_onl()
{
    try
    {
        if (rfpal_init() != ECRF_PAL_STATUS_OK)
            std::cout << "onl rfpal_init error!!" << std::endl;

        fHandle_onlp = dlopen(m_onlp_lib_path.c_str(), RTLD_LAZY);

        if (fHandle_onlp)
        {
            std::cout << "Using dynamic loading function!!" << std::endl;
            d_onlp_init = (int (*)())dlsym(fHandle_onlp, "onlp_init");
            d_onlp_sys_info_get = (int (*)(onlp_sys_info_t * rv)) dlsym(fHandle_onlp, "onlp_sys_info_get");
            d_onlp_sys_info_free = (void (*)(onlp_sys_info_t * info)) dlsym(fHandle_onlp, "onlp_sys_info_free");
            d_onlp_fan_info_get = (int (*)(onlp_oid_t id, onlp_fan_info_t * rv)) dlsym(fHandle_onlp, "onlp_fan_info_get");
            d_onlp_sfpi_is_present = (int (*)(int port))dlsym(fHandle_onlp, "onlp_sfpi_is_present");
            d_onlp_thermal_info_get = (int (*)(onlp_oid_t id, onlp_thermal_info_t * rv)) dlsym(fHandle_onlp, "onlp_thermal_info_get");
            d_onlp_psu_info_get = (int (*)(onlp_oid_t id, onlp_psu_info_t * rv)) dlsym(fHandle_onlp, "onlp_psu_info_get");
            d_onlp_oid_iterate = (int (*)(onlp_oid_t oid, onlp_oid_type_t type, onlp_oid_iterate_f itf, void *cookie))dlsym(fHandle_onlp, "onlp_oid_iterate");
            d_onlp_sfp_control_set = (int (*)(int port, onlp_sfp_control_t control, int value))dlsym(fHandle_onlp, "onlp_sfp_control_set");
            d_onlp_sfp_control_get = (int (*)(int port, onlp_sfp_control_t control, int *value))dlsym(fHandle_onlp, "onlp_sfp_control_get");
            m_onlp_lib_init = true;
        }
        else
            std::cout << "Cannot find *****.so in /usr/local/lib/" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "ecrf_pal_com_onl - exception : " << e.what() << std::endl;
    }
    return;
}

int ecrf_pal_com_onl::rfpal_sys_info_com_get(rfpal_sys_info *p_sys_info)
{
    try
    {
        int rv;
        onlp_sys_info_t si;

        if (!rfpal_is_onlp_com_get())
            return ECRF_PAL_STATUS_E_GENERIC;

        if (d_onlp_init)
            d_onlp_init();
        else
        {
            std::cout << "d_onlp_init error!!" << std::endl;
            return ECRF_PAL_STATUS_E_GENERIC;
        }

        //Collect PSU/THERMAL/FAN number
        if (d_onlp_oid_iterate)
        {
            d_onlp_oid_iterate(ONLP_OID_SYS, ONLP_OID_TYPE_PSU, iterate_oids_callback__, NULL);
            d_onlp_oid_iterate(ONLP_OID_SYS, ONLP_OID_TYPE_THERMAL, iterate_oids_callback__, NULL);
            d_onlp_oid_iterate(ONLP_OID_SYS, ONLP_OID_TYPE_FAN, iterate_oids_callback__, NULL);
        }

        if (d_onlp_sys_info_get)
            rv = d_onlp_sys_info_get(&si);
        else
            return ECRF_PAL_STATUS_E_GENERIC;

        if (rv < 0)
        {
            return ECRF_PAL_STATUS_E_GENERIC;
        }
        else
        {
            onlp_onie_info_t *info = &si.onie_info;

            if (info->product_name && p_sys_info->product_name && strlen(info->product_name) < ECRF_PAL_CONST_256 )
                memcpy(p_sys_info->product_name, info->product_name, strlen(info->product_name));

            if (info->part_number && p_sys_info->part_number && strlen(info->part_number) < ECRF_PAL_CONST_128)
                memcpy(p_sys_info->part_number, info->part_number, strlen(info->part_number));

            if (info->serial_number && p_sys_info->serial_number && strlen(info->serial_number) < ECRF_PAL_CONST_128)
                memcpy(p_sys_info->serial_number, info->serial_number, strlen(info->serial_number));

            if (info->mac && p_sys_info->mac)
                memcpy(p_sys_info->mac, info->mac, ECRF_PAL_CONST_6);

            if (info->mac_range && p_sys_info->mac_range)
                p_sys_info->mac_range = info->mac_range;

            if (info->manufacturer && p_sys_info->manufacturer && strlen(info->manufacturer) < ECRF_PAL_CONST_128)
                memcpy(p_sys_info->manufacturer, info->manufacturer, strlen(info->manufacturer));

            if (info->manufacture_date && p_sys_info->manufacture_date && strlen(info->manufacture_date) < ECRF_PAL_CONST_32)
                memcpy(p_sys_info->manufacture_date, info->manufacture_date, strlen(info->manufacture_date));

            if (info->vendor && p_sys_info->vendor && strlen(info->vendor) < ECRF_PAL_CONST_256)
                memcpy(p_sys_info->vendor, info->vendor, strlen(info->vendor));

            if (info->platform_name && p_sys_info->platform_name && strlen(info->platform_name) < ECRF_PAL_CONST_256)
                memcpy(p_sys_info->platform_name, info->platform_name, strlen(info->platform_name));

            if (info->device_version && p_sys_info->device_version)
                p_sys_info->device_version = info->device_version;

            if (info->label_revision && p_sys_info->label_revision && strlen(info->label_revision) < ECRF_PAL_CONST_32)
                memcpy(p_sys_info->label_revision, info->label_revision, strlen(info->label_revision));

            if (info->country_code && p_sys_info->country_code && strlen(info->country_code) < ECRF_PAL_CONST_32)
                memcpy(p_sys_info->country_code, info->country_code, strlen(info->country_code));

            if (info->diag_version && p_sys_info->diag_version && strlen(info->diag_version) < ECRF_PAL_CONST_32)
                memcpy(p_sys_info->diag_version, info->diag_version, strlen(info->diag_version));

            if (info->service_tag && p_sys_info->service_tag && strlen(info->service_tag) < ECRF_PAL_CONST_32)
                memcpy(p_sys_info->service_tag, info->service_tag, strlen(info->service_tag));

            if (info->onie_version && p_sys_info->onie_version && strlen(info->onie_version) < ECRF_PAL_CONST_32)
                memcpy(p_sys_info->onie_version, info->onie_version, strlen(info->onie_version));

            //Todo: Other SW version info.
            if (d_onlp_sys_info_free)
                d_onlp_sys_info_free(&si);
            else
                return ECRF_PAL_STATUS_E_GENERIC;
            return ECRF_PAL_STATUS_OK;
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "rfpal_sys_info_com_get - exception : " << e.what() << std::endl;
        return ECRF_PAL_STATUS_E_GENERIC;
    }

}

int ecrf_pal_com_onl::rfpal_fan_num_com_get()
{
    if (!rfpal_is_onlp_com_get())
        return ECRF_PAL_STATUS_E_GENERIC;

    return m_com_onl_fan_max_num + m_com_onl_psu_max_num;
}

int ecrf_pal_com_onl::rfpal_fan_info_com_get(unsigned id, rfpal_fan_info *p_fan_info)
{
    try
    {
        int rv;
        onlp_fan_info_t fv;

        if (!rfpal_is_onlp_com_get())
            return ECRF_PAL_STATUS_E_GENERIC;

        int ii = id;

        unsigned int id_in = ONLP_FAN_ID_CREATE(ii);

        if (d_onlp_fan_info_get)
            rv = d_onlp_fan_info_get(id_in, &fv);
        else
            return ECRF_PAL_STATUS_E_GENERIC;

        if (rv < 0)
        {
            std::cout << "rfpal_fan_info_com_get error!!" << std::endl;
            return ECRF_PAL_STATUS_E_GENERIC;
        }
        else
        {
            p_fan_info->id = id;
            if (fv.status & 1)
            {
                p_fan_info->id = id;
                p_fan_info->status = 1;
                p_fan_info->front_percentage = fv.percentage;
                p_fan_info->rear_percentage = fv.percentage; //use the same value //
                p_fan_info->front_rpm = fv.rpm;
                p_fan_info->rear_rpm = fv.rpm;
                memcpy(p_fan_info->serial, fv.serial, strlen(fv.serial));
                memcpy(p_fan_info->model, fv.model, strlen(fv.model));

                //Todo threshold value from file //
                //p_fan_info->m_threshold.warning = 100;
                //p_fan_info->m_threshold.error = 200;
                //p_fan_info->m_threshold.shutdown = 300;
            }
            else
            {
                p_fan_info->status = 0;
            }
            return ECRF_PAL_STATUS_OK;
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "rfpal_fan_info_com_get - exception : " << e.what() << std::endl;
        return ECRF_PAL_STATUS_E_GENERIC;
    }
}

int ecrf_pal_com_onl::rfpal_port_num_com_get()
{
    // ONLP not support get all port number //
    return ECRF_PAL_STATUS_E_GENERIC;  
}

int ecrf_pal_com_onl::rfpal_port_com_trans_is_present(int id)
{
    try
    {
        if (!rfpal_is_onlp_com_get())
            return ECRF_PAL_STATUS_E_GENERIC;

        int b = 0;
        int rindex = id - 1; //Need start from 0//

        if (d_onlp_sfpi_is_present)
            b = d_onlp_sfpi_is_present(rindex);
        else
            return ECRF_PAL_STATUS_E_GENERIC;

        if (b == 1)
            return ECRF_PAL_STATUS_OK;
        else if (b == 0)
            return 0;
        else
            return ECRF_PAL_STATUS_E_GENERIC;
    }
    catch (const std::exception &e)
    {
        std::cout << "rfpal_port_com_trans_is_present - exception : " << e.what() << std::endl;
        return ECRF_PAL_STATUS_E_GENERIC;
    }
}

int ecrf_pal_com_onl::rfpal_sfp_tx_disable_com_set(unsigned id, int value)
{
    try
    {
        if (!rfpal_is_onlp_com_get())
            return ECRF_PAL_STATUS_E_GENERIC;

        int b = 0;
        int rindex = id - 1; //Need start from 0//

        if (d_onlp_sfp_control_set)
            b = d_onlp_sfp_control_set(rindex, ONLP_SFP_CONTROL_TX_DISABLE, value);
        else
            return ECRF_PAL_STATUS_E_GENERIC;

        if (b >= 0)
            return ECRF_PAL_STATUS_OK;
        else
            return ECRF_PAL_STATUS_E_GENERIC;
    }
    catch (const std::exception &e)
    {
        std::cout << "rfpal_sfp_tx_disable_com_set - exception : " << e.what() << std::endl;
        return ECRF_PAL_STATUS_E_GENERIC;
    }
}

int ecrf_pal_com_onl::rfpal_sfp_tx_disable_com_get(unsigned id)
{
    try
    {
        if (!rfpal_is_onlp_com_get())
            return ECRF_PAL_STATUS_E_GENERIC;

        int b = 0;
        int rindex = id - 1; //Need start from 0//
        int value = 0;
        if (d_onlp_sfp_control_set)
            b = d_onlp_sfp_control_get(rindex, ONLP_SFP_CONTROL_TX_DISABLE, &value);
        else
            return ECRF_PAL_STATUS_E_GENERIC;

        if (b >= 0)
            return value;
        else
            return ECRF_PAL_STATUS_E_GENERIC;
    }
    catch (const std::exception &e)
    {
        std::cout << "rfpal_sfp_tx_disable_com_get - exception : " << e.what() << std::endl;
        return ECRF_PAL_STATUS_E_GENERIC;
    }
}

int ecrf_pal_com_onl::rfpal_sfp_tx_eeprom_com_get(unsigned int  id, char * p_eeprom, int size)
{
    //Todo: 
    UNUSED(id);
    UNUSED(p_eeprom);
    UNUSED(size);
    return ECRF_PAL_STATUS_E_GENERIC;
}

int ecrf_pal_com_onl::rfpal_thermal_num_com_get()
{
    return m_com_onl_thermal_sen_max_num + m_com_onl_psu_max_num;
}

int ecrf_pal_com_onl:: rfpal_thermal_info_com_get(unsigned id_in, rfpal_thermal_info *p_thermal_info)
{
    lock_guard<mutex> lock{m_mutex};
    try
    {
        int rv;
        onlp_thermal_info_t fv;

        if (!rfpal_is_onlp_com_get())
            return ECRF_PAL_STATUS_E_GENERIC;

        unsigned int id = ONLP_THERMAL_ID_CREATE(id_in);

        if (d_onlp_thermal_info_get)
            rv = d_onlp_thermal_info_get(id, &fv);
        else
            return ECRF_PAL_STATUS_E_GENERIC;

        if (rv < 0)
        {
            p_thermal_info->status = 0;
            return ECRF_PAL_STATUS_E_GENERIC;
        }
        else
        {
            if (fv.status & 1)
            {
                p_thermal_info->status = 1;
                p_thermal_info->id = id_in;
                p_thermal_info->mcelsius = fv.mcelsius;

                // if not have files , use data from onlp
                // Only warning upper can be use . error and shutdown value need provide by HW default //
                Json::Value th_s;
                Json::Reader j_reader = {};

                std::ifstream th_file(m_thermal_threshold_file_path);

                if (th_file.good())
                {
                    bool isJson = (j_reader.parse(th_file, th_s));
                    if (isJson)
                    {
                        if(th_s[std::to_string(id_in).c_str()]["warning_upper"].asInt())
                        {
                            p_thermal_info->m_threshold.warning_upper = th_s[std::to_string(id_in).c_str()]["warning_upper"].asInt();
                            p_thermal_info->m_threshold.warning_lower = th_s[std::to_string(id_in).c_str()]["warning_lower"].asInt();
                            th_file.close();
                    }
                        else
                        {
                    th_file.close();
                            std::ofstream th_o_file(m_thermal_threshold_file_path);
                            Json::StyledWriter styledWriter;
                            std::string t_id = std::to_string(id_in);
                            th_s[t_id.c_str()]["warning_lower"] = 0;
                            th_s[t_id.c_str()]["warning_upper"] = fv.thresholds.warning;
                            th_s[t_id.c_str()]["error"] = fv.thresholds.error;
                            th_s[t_id.c_str()]["shutdown"] = fv.thresholds.shutdown;
                            th_o_file << styledWriter.write(th_s);
                            th_o_file.close();
                            p_thermal_info->m_threshold.warning_upper = fv.thresholds.warning;
                            p_thermal_info->m_threshold.warning_lower = 0;
                        }
                   }
                }
                else
                {
                    std::ofstream th_o_file(m_thermal_threshold_file_path);
                    Json::StyledWriter styledWriter;
                    std::string t_id = std::to_string(id_in);
                    Json::Value th_os;
                    th_os[t_id.c_str()]["warning_lower"] = 0;
                    th_os[t_id.c_str()]["warning_upper"] = fv.thresholds.warning;
                    th_os[t_id.c_str()]["error"] = fv.thresholds.error;
                    th_os[t_id.c_str()]["shutdown"] = fv.thresholds.shutdown;
                    th_o_file << styledWriter.write(th_os);
                    th_o_file.close();
                    p_thermal_info->m_threshold.warning_upper = fv.thresholds.warning;
                    p_thermal_info->m_threshold.warning_lower = 0;
                }

                p_thermal_info->m_threshold.error = fv.thresholds.error;
                p_thermal_info->m_threshold.shutdown = fv.thresholds.shutdown;
            }
            else
                p_thermal_info->status = 0;
        }
        return ECRF_PAL_STATUS_OK;
    }
    catch (const std::exception &e)
    {
        std::cout << "rfpal_thermal_info_com_get - exception : " << e.what() << std::endl;
        return ECRF_PAL_STATUS_E_GENERIC;
    }
}

int ecrf_pal_com_onl::rfpal_psu_num_com_get()
{
    if (!rfpal_is_onlp_com_get())
        return ECRF_PAL_STATUS_E_GENERIC;

    return m_com_onl_psu_max_num;
}

int ecrf_pal_com_onl::rfpal_psu_info_com_get(unsigned id_in, rfpal_psu_info *p_psu_info)
{
    try
    {
        if (!rfpal_is_onlp_com_get())
            return ECRF_PAL_STATUS_E_GENERIC;

        int rv;
        onlp_psu_info_t fv;
        int ii = id_in;

        unsigned int id = ONLP_PSU_ID_CREATE(ii);
        if (d_onlp_psu_info_get)
            rv = d_onlp_psu_info_get(id, &fv);
        else
            return ECRF_PAL_STATUS_E_GENERIC;

        p_psu_info->id = id_in;
        if (rv < 0)
        {
            p_psu_info->status = 0;
            return ECRF_PAL_STATUS_E_GENERIC;
        }
        else
        {
            if (fv.status & 1)
            {
                p_psu_info->status = 1;

                memcpy(p_psu_info->model, fv.model, strlen(fv.model));
                memcpy(p_psu_info->serial, fv.serial, strlen(fv.serial));

                if (fv.caps & ecrf_pal_com_onl::ECRF_PAL_PSU_ONLP_CAPS_AC)
                    p_psu_info->psu_type = ecrf_pal_com_s::RFPAL_PSU_TYPE_AC;
                else if (fv.caps & ecrf_pal_com_onl::ECRF_PAL_PSU_ONLP_CAPS_DC12)
                    p_psu_info->psu_type = ecrf_pal_com_s::RFPAL_PSU_TYPE_DC12;
                else if (fv.caps & ecrf_pal_com_onl::ECRF_PAL_PSU_ONLP_CAPS_DC48)
                    p_psu_info->psu_type = ecrf_pal_com_s::RFPAL_PSU_TYPE_DC48;
                else
                    p_psu_info->psu_type = ecrf_pal_com_s::RFPAL_PSU_TYPE_INVALID;


                std::cout << "psu info fv.caps[ " << std::hex << "]" << std::endl;

                p_psu_info->mvin = fv.mvin;
                p_psu_info->mvout = fv.mvout;
                p_psu_info->miin = fv.miout;
                p_psu_info->mpin = fv.mpin;
                p_psu_info->mpout = fv.mpout;
            }
            else
            {
                p_psu_info->status = 0;
            }
        }
        return ECRF_PAL_STATUS_OK;
    }
    catch (const std::exception &e)
    {
        std::cout << "rfpal_psu_info_com_get - exception : " << e.what() << std::endl;
        return ECRF_PAL_STATUS_E_GENERIC;
    }
}
