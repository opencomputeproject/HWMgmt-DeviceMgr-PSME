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

#ifndef ECRF_PAL_COM_HPP
#define ECRF_PAL_COM_HPP

#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <ostream>
#include <string>
#include <limits.h>
#include <stdio.h>
#include <dlfcn.h>
#include <json/json.h>
#include <json/json.hpp>
#include <mutex>


using namespace std;

#define UNUSED(x) (void)(x)

namespace ecrf_pal_com_s
{

typedef enum ecrf_pal_const_num_e
{
    ECRF_PAL_CONST_6 = 6,
    ECRF_PAL_CONST_32 = 32,
    ECRF_PAL_CONST_64 = 64,
    ECRF_PAL_CONST_128 = 128,
    ECRF_PAL_CONST_256 = 256,
    ECRF_PAL_CONST_NOT_SUPPORTED = -99000
} ecrf_pal_const_num_t;

typedef struct rfpal_thresholds
{
    double warning_lower;  //warning value mapping to "LowerThresholdNonCritical"//
    double warning_upper;  //warning value mapping to "UpperThresholdNonCritical"//
    double error;    //error value mapping to "UpperThresholdCritical"//
    double shutdown; //shutdown value mapping to "UpperThresholdFatal"//
} rfpal_thresholds_t;

typedef struct rfpal_fan_info
{
    int id;                         //fan id
    int status;                     //present 1, absent 0
    char model[ECRF_PAL_CONST_64];  //FAN module name
    char serial[ECRF_PAL_CONST_64]; //FAN serial number
    signed int front_rpm;           //Front FAN rpm value
    signed int rear_rpm;            //Rear FAN rpm value
    signed int front_percentage;    //Front FAN speed percentate
    signed int rear_percentage;     //Rear FAN speed percentate
    rfpal_thresholds m_threshold;   //Speed warning/erro/shutdown threshold value
} rfpal_fan_info_t;

typedef struct rfpal_thermal_info
{
    int status; //present 1, absent 0//
    int id;     //port id

    //Thermal sensor reading temperature value//
    //ex. 45 degrees Celsius will return 45000
    int mcelsius;
    rfpal_thresholds m_threshold; //thresholds
} rfpal_thermal_info_t;

typedef enum rfpal_psu_type
{
    RFPAL_PSU_TYPE_AC,
    RFPAL_PSU_TYPE_DC12,
    RFPAL_PSU_TYPE_DC48,
    RFPAL_PSU_TYPE_LAST = RFPAL_PSU_TYPE_DC48,
    RFPAL_PSU_TYPE_COUNT,
    RFPAL_PSU_TYPE_INVALID = -1,
} rfpal_psu_type_t;

typedef struct rfpal_psu_info
{

    int status;                     //present 1, absent 0
    int id;                         //PSU id
    char model[ECRF_PAL_CONST_64];  //PSU module name
    char serial[ECRF_PAL_CONST_64]; //PSU serial number
    unsigned int mvin;              //PSU Voltage in value  (Voltage)
    unsigned int mvout;             //PSU Voltage out value (Voltage)
    unsigned int miin;              //PSU Current in value  (Am)
    unsigned int miout;             //PSU Current out value (Am)
    unsigned int mpin;              //PSU Power in value    (Watt)
    unsigned int mpout;             //PSU Power out value   (Watt)
    rfpal_psu_type psu_type;        //PSU Type
} rfpal_psu_info_t;

typedef struct rfpal_sys_info
{
    char platform_name[ECRF_PAL_CONST_256];   //platform_name
    char product_name[ECRF_PAL_CONST_256];    //system product name
    char part_number[ECRF_PAL_CONST_128];     //system part number
    char serial_number[ECRF_PAL_CONST_128];   //system serial number
    char mac[ECRF_PAL_CONST_6];               //1st MAC address
    char manufacturer[ECRF_PAL_CONST_128];    //system manufacturer
    char manufacture_date[ECRF_PAL_CONST_32]; //system manufacture date
    char vendor[ECRF_PAL_CONST_256];          //system vendor
    int device_version;                       //system hardware device version (R0A,R01...)
    char label_revision[ECRF_PAL_CONST_32];   //system hardware label revision
    char country_code[ECRF_PAL_CONST_32];     //system hardware country_code
    char diag_version[ECRF_PAL_CONST_32];     //system diag_version
    char service_tag[ECRF_PAL_CONST_32];      //system service_tag
    int mac_range;                            //mac range

    char onie_version[ECRF_PAL_CONST_32];        //system onie version
    char cpld_version[ECRF_PAL_CONST_32];        //system cpld version
    char fpga_version[ECRF_PAL_CONST_32];        //system fpga version
    char bios_version[ECRF_PAL_CONST_32];        //system bios version
    char nos_type[ECRF_PAL_CONST_32];            //network OS type (ONL/SONiC)
    char nos_version[ECRF_PAL_CONST_32];         //network OS version
    char bal_version[ECRF_PAL_CONST_32];         //bal sdk version
    char acc_bal_bsp_version[ECRF_PAL_CONST_32]; //accton bsp version
} rfpal_sys_info_t;

typedef enum ecrf_pal_status_e
{
    ECRF_PAL_STATUS_OK = 1,
    ECRF_PAL_STATUS_E_GENERIC = -1,
    ECRF_PAL_STATUS_E_UNSUPPORTED = -10,
    ECRF_PAL_STATUS_E_MISSING = -11,
    ECRF_PAL_STATUS_E_INVALID = -12,
    ECRF_PAL_STATUS_E_INTERNAL = -13,
    ECRF_PAL_STATUS_E_PARAM = -14,
} ecrf_pal_status_t;

typedef enum ecrf_sys_threshold_info_e
{
    ECRF_PAL_SYS_CPU = 1,
    ECRF_PAL_SYS_MEM = 2,
    ECRF_PAL_SYS_STORAGE = 3,
} ecrf_sys_threshold_info_t;


class ecrf_pal_com
{
public:
    ecrf_pal_com() { ; };
    ~ecrf_pal_com() { ; };

    static bool m_onlp_lib_init;

    //common used function
    int rfpal_fan_speed_thresholds_com_set(unsigned id, rfpal_thresholds threshold_info);
    int rfpal_fan_speed_thresholds_com_get(unsigned id, rfpal_thresholds *threshold_info);
    int rfpal_thermal_thresholds_com_set(unsigned id, rfpal_thresholds threshold_info);
    int rfpal_thermal_thresholds_com_get(unsigned id, rfpal_thresholds *threshold_info);
    int rfpal_create_default_sys_thresholds();
    int rfpal_sys_thresholds_com_set(ecrf_sys_threshold_info_t type,  rfpal_thresholds threshold_info);
    int rfpal_sys_thresholds_com_get(ecrf_sys_threshold_info_t type,  rfpal_thresholds *threshold_info);
    int rfpal_sysfs_node_get(const char* sysfsnode_path, char* sysfsnode_value);
    int rfpal_sysfs_node_set(const char* sysfsnode_path, char* sysfsnode_value);    
    int rfpal_system_cmd_set(const char* system_cmd, char* result_value);
    virtual int rfpal_init() = 0;
    virtual int rfpal_sys_info_com_get(rfpal_sys_info *p_sys_info) = 0;
    virtual int rfpal_fan_num_com_get() = 0;
    virtual int rfpal_fan_info_com_get(unsigned id, rfpal_fan_info *p_fan_info) = 0;
    virtual int rfpal_port_num_com_get() = 0;
    virtual int rfpal_port_com_trans_is_present(int id) = 0;
    virtual int rfpal_sfp_tx_disable_com_set(unsigned id, int value) = 0;
    virtual int rfpal_sfp_tx_disable_com_get(unsigned id) = 0;
    virtual int rfpal_sfp_tx_eeprom_com_get(unsigned int  id, char * p_eeprom, int size) = 0;
    virtual int rfpal_thermal_num_com_get() = 0;
    virtual int rfpal_thermal_info_com_get(unsigned id, rfpal_thermal_info *p_thermal_info) = 0;

    virtual int rfpal_psu_num_com_get() = 0;
    virtual int rfpal_psu_info_com_get(unsigned id, rfpal_psu_info *p_psu_info) = 0;

    static int rfpal_is_onlp_com_get() { return m_onlp_lib_init; };
    static ecrf_pal_com &get_instance();
    std::string m_thermal_threshold_update_file_path = "/etc/psme/thermal_threshold_update";
    std::string m_thermal_threshold_file_path = "/etc/psme/thermal_threshold";
    std::string m_sys_threshold_file_path = "/etc/psme/sys_threshold";
    rfpal_thermal_info thermal_info_cache={};

    std::mutex m_mutex={};	
    std::mutex m_mutex_sys={};	
    private:

    std::string m_fan_threshold_file_path= "/etc/psme/fan_threshold";
};

class ecrf_pal_com_onl : virtual public ecrf_pal_com
{
    typedef enum ecrf_pal_psu_caps_e
    {
        ECRF_PAL_PSU_ONLP_CAPS_AC = (1 << 0),
        ECRF_PAL_PSU_ONLP_CAPS_DC12 = (1 << 1),
        ECRF_PAL_PSU_ONLP_CAPS_DC48 = (1 << 2),
        ECRF_PAL_PSU_ONLP_CAPS_VIN = (1 << 3),
        ECRF_PAL_PSU_ONLP_CAPS_VOUT = (1 << 4),
        ECRF_PAL_PSU_ONLP_CAPS_IIN = (1 << 5),
        ECRF_PAL_PSU_ONLP_CAPS_IOUT = (1 << 6),
        ECRF_PAL_PSU_ONLP_CAPS_PIN = (1 << 7),
        ECRF_PAL_PSU_ONLP_CAPS_POUT = (1 << 8),
    } ecrf_pal_psu_caps_t;
public:

    int rfpal_init();
    int rfpal_sys_info_com_get(rfpal_sys_info *p_sys_info);
    int rfpal_fan_num_com_get();
    int rfpal_fan_info_com_get(unsigned id, rfpal_fan_info *p_fan_info);
    int rfpal_port_num_com_get();
    int rfpal_port_com_trans_is_present(int id);
    int rfpal_sfp_tx_disable_com_set(unsigned id, int value);
    int rfpal_sfp_tx_disable_com_get(unsigned id);
    int rfpal_sfp_tx_eeprom_com_get(unsigned int id, char *p_eeprom, int size);
    int rfpal_thermal_num_com_get();
    int rfpal_thermal_info_com_get(unsigned id, rfpal_thermal_info *p_thermal_info);
    int rfpal_psu_num_com_get();
    int rfpal_psu_info_com_get(unsigned id, rfpal_psu_info *p_psu_info);

    // ONL Dynamic load
    void *fHandle_onlp = 0;

    ecrf_pal_com_onl();
    ~ecrf_pal_com_onl()
    {
        if (fHandle_onlp)
            dlclose(fHandle_onlp);
    };
    static int m_com_onl_thermal_sen_max_num;
    static int m_com_onl_psu_max_num;
    static int m_com_onl_fan_max_num;
    static int m_com_onl_port_max_num;
    static void increase_thermal_num() { m_com_onl_thermal_sen_max_num++; return; };
    static void increase_psu_num() { m_com_onl_psu_max_num++; return; };
    static void increase_fan_num() { m_com_onl_fan_max_num++; return; };
    static void increase_port_num() { m_com_onl_port_max_num++; return; };

    //Because of pointer in class //
    ecrf_pal_com_onl& operator=(ecrf_pal_com_onl&) = delete;
    ecrf_pal_com_onl(const ecrf_pal_com_onl&) = delete;
    ecrf_pal_com_onl(ecrf_pal_com_onl&&) = delete;
    ecrf_pal_com_onl& operator=(ecrf_pal_com_onl&&) = delete;

private:
    std::string m_onlp_lib_path = "/lib/x86_64-linux-gnu/libonlp.so";
};

class ecrf_pal_com_sonic : virtual public ecrf_pal_com
{
public:
    int rfpal_init();
    int rfpal_sys_info_com_get(rfpal_sys_info *p_sys_info);
    int rfpal_fan_num_com_get();
    int rfpal_fan_info_com_get(unsigned id, rfpal_fan_info *p_fan_info);
    int rfpal_port_num_com_get();
    int rfpal_port_com_trans_is_present(int id);
    int rfpal_sfp_tx_disable_com_set(unsigned id ,int value);
    int rfpal_sfp_tx_disable_com_get(unsigned id);        
    int rfpal_sfp_tx_eeprom_com_get(unsigned int  id, char * p_eeprom, int size);
    int rfpal_thermal_num_com_get();
    int rfpal_thermal_info_com_get(unsigned id, rfpal_thermal_info *p_thermal_info);
    int rfpal_psu_num_com_get();
    int rfpal_psu_info_com_get(unsigned id, rfpal_psu_info *p_psu_info);

    ecrf_pal_com_sonic();
    ~ecrf_pal_com_sonic(){};
};
} // namespace ecrf_pal_com_s
#endif
