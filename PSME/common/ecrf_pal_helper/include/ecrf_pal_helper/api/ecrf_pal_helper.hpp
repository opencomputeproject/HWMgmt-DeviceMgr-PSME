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

#ifndef EC_RF_PAL_HELPER_HPP
#define EC_RF_PAL_HELPER_HPP

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
#include <json/json.hpp>
#include <mutex>
#include <eclog_helper/eclog_helper.hpp>
#include <ecnet_helper/ecnet_helper.hpp>
#include <ecsys_helper/ecsys_helper.hpp>
#include <ecopenbmc_helper/ecopenbmc_helper.hpp>

#define FF3(readin) float(int(readin * 1000))
namespace ecrf_pal_helper 
{

using namespace std;
using namespace ecnet_helper;
using namespace ecsys_helper;
using namespace eclog_helper;
using namespace ecopenbmc_helper;

typedef struct EventMsgContent
{
    std::string message_content;
    std::string message_id;
    std::string origin_of_condition;
    vector <std::string> args;
    Json::Value oem;
} EventMsgContent_T;

static constexpr const int SIZE_EEPROM = 600;
static constexpr const int COUNT_TIMES = 10;
static constexpr const char STD_SEC_PATH[] = "/usr/local/bin/mod_conf/";
static constexpr const char PORT_MAP_PATH[] = "/usr/local/bin/mod_conf/map/";
class e_oom
{
    bool get_conf();
    void set_proto();
    bool m_support = false;
    unsigned m_belong_to_port = {0};
    std::string m_eeprom_path = {};
    unsigned char m_eeprom[SIZE_EEPROM];

    float get_value(std::string attri_name);
    float get_value_u(std::string attri_name);
    std::string get_value_s(std::string attri_name);

    // Std. is using //
    Json::Value m_std = {};

    json::Value m_current_status = {};

    Json::Value get_attri_by_name(std::string att_name, Json::Value in);

    // Collection of support Std. New Transceiver Type need Added here begin//
    std::map<std::pair<int, int>, Json::Value> m_8077i = {};
    std::map<std::pair<int, int>, Json::Value> m_8472 = {};
    std::map<std::pair<int, int>, Json::Value> m_8438i = {};
    std::map<std::pair<int, int>, Json::Value> m_8436i = {};
    // Collection of support Std. New Transceiver Type need Added here end//
    void refresh_vendor_info();
    void refresh_temp();
    void refresh_voltage();
    void refresh_bias();
    void refresh_tx_pwr();
    void refresh_rx_pwr();

    void default_vendor_info();
    void default_temp();
    void default_voltage();
    void default_bias();
    void default_tx_pwr();
    void default_rx_pwr();

public:
    e_oom()
    {
        get_conf();
        set_proto();
        status_default();
    };
    ~e_oom(){};

    bool get_support() { return m_support; };
    void set_support(bool support)
    {
        m_support = support;

        if (!support)
        {
            set_proto();
            status_default();
        }
    };

    void set_belong_to_port(unsigned belong_to_port)
    {
        m_belong_to_port = belong_to_port;
    };

    unsigned get_belong_to_port()
    {
        return m_belong_to_port;
    };

    void set_eeprom_path(std::string in)
    {
        m_eeprom_path = in;
    };

    bool store_eeprom(char *in_eeprom);
    bool refresh_status();
    bool status_default();

    json::Value get_current_status() { return m_current_status; };

    bool get_eeprom_raw();
    bool get_eeprom_raw(int rindex);
};

class Dev_Info
{
public:
    std::string m_Status_Health = {};
    std::string m_Status_State = {};
    std::string m_Model = {};
    std::string m_SN = {};
    bool m_Present = false;
    int m_fan_alert = COUNT_TIMES;
    int m_ID = 0;
    int m_Type = 0;
    int m_thermal_alert  = COUNT_TIMES;
    int m_sys_thermal_alert = 0;
    int m_psu_thermal_alert = 0;

    double m_Warning_upper = 0;
    double m_Warning_lower = 0;
    double m_Error = 0;
    double m_Shutdown = 0;
 
    static std::vector<EventMsgContent_T> m_Event_Resource_Alert;
    static std::vector<EventMsgContent_T> m_Event_Port_Resource_Alert;
    static std::vector<EventMsgContent_T> get_Event_Resource_Alert();
    static std::vector<EventMsgContent_T> get_Event_Port_Resource_Alert();
    static void Clear_Event_Resource_Alert();
    static void Clear_Event_Port_Resource_Alert();
};

class Thermal_Info : public Dev_Info
{
public:
    enum Thermal_Type
    {
        CPU_Sensor = 1,
        SYSTEM_Sensor = 2,
        PSU_Sensor = 3
    };

    double m_Current_Temperature = 0;
    void set_info(int ID, double Current_Temperature, double Warning_Upper, double Warning_Lower, double Error, double Shutdown, bool present);
    virtual void set_thermal_threshold_value(int Warning_Upper, int Warning_Lower, int Error, int Shutdown) = 0;
    void get_thermal_threshold_value();
    virtual ~Thermal_Info(){};
};

class CPU_Thermal : public Thermal_Info
{
public:
    void set_thermal_threshold_value(int Warning_Upper, int Warning_Lower, int Error, int Shutdown);
};

class SYS_Thermal : public Thermal_Info
{
public:
    void set_thermal_threshold_value(int Warning_Upper, int Warning_Lower, int Error, int Shutdown);
};

class PSU_Thermal : public Thermal_Info
{
public:
    void set_thermal_threshold_value(int Warning_Upper, int Warning_Lower, int Error, int Shutdown);
};

class Psu_Info : public Dev_Info
{
public:
    typedef enum ecrf_pal_psu_type
    {
        ECRF_PAL_PSU_TYPE_AC,
        ECRF_PAL_PSU_TYPE_DC12,
        ECRF_PAL_PSU_TYPE_DC48,
        ECRF_PAL_PSU_TYPE_LAST = ECRF_PAL_PSU_TYPE_DC48,
        ECRF_PAL_PSU_TYPE_COUNT,
        ECRF_PAL_PSU_TYPE_INVALID = -1,
    } ecrf_pal_psu_type_t;

    enum Psu_Type
    {
        SYSTEM = 1
    };

    int m_Vin = 0;
    int m_Vout = 0;
    int m_Iin = 0;
    int m_Iout = 0;
    int m_Pin = 0;
    int m_Pout = 0;
    ecrf_pal_psu_type_t m_Psu_Type = ecrf_pal_helper::Psu_Info::ECRF_PAL_PSU_TYPE_INVALID;
    void set_info(int ID, std::string Model, std::string SN, int Vin, int Vout, int Iin, int Iout, int Pin, int Pout, ecrf_pal_psu_type_t type, bool present);
};

class Fan_Info : public Dev_Info
{
public:
    enum Fan_Type
    {
        PSU_Fan = 1,
        SYSTEM_Fan = 2
    };
    int m_RPM = 0;
    int m_Per = 0;
    void set_info(int ID, std::string Model, std::string SN, int RPM, int Per, bool present);
    void get_fan_speed_threshold_value();
};

class SYS_Fan : public Fan_Info
{
public:
};

class PSU_Fan : public Fan_Info
{
public:
};

class Port_Info : public Dev_Info
{
    unsigned char m_EEPROM[SIZE_EEPROM] = {0};

public:
    enum Port_Type
    {
        Ether_Port = 1,
        SFP_Port = 2,
        PON_Port = 3,
        QSFP_Port = 4

    };
    bool m_Present_Status = 0;
    void set_info(int ID, int Type, int Present_Status, bool present);

    e_oom m_port_oom;
    std::string m_sysfile_path;

    bool get_eeprom_raw()
    {
        return m_port_oom.get_eeprom_raw();
    };

    bool get_eeprom_raw(int rindex)
    {
        return m_port_oom.get_eeprom_raw(rindex);
    };

    json::Value get_trans_status()
    {
        return m_port_oom.get_current_status();
    };

    void clean_trans_data()
    {
        m_port_oom.set_support(false);
    };

    void set_sysfile_path(std::string in) 
    { 
        m_sysfile_path = in; 
    };

    std::string get_sysfile_path()
    {
        return m_sysfile_path;
    }

    void set_eeprom_path(std::string in)
    {
        m_port_oom.set_eeprom_path(in);
        set_sysfile_path(in.substr(0, in.rfind('/')));
    };
    void set_tx_disable(bool status, std::string in_tx_sysfs);
    int get_tx_disable_status(std::string in_tx_sysfs);
};

static constexpr const int SAMPLE_NUM = 4;

class SysResource: public Dev_Info
{
public:
    void mon_memory_usage_info();
    void mon_cpu_usage_info();
    void mon_storage_usage_info();
    void mon_openbmc_status();
    int mem_sample_number = SAMPLE_NUM;
    int cpu_sample_number = SAMPLE_NUM;
    int storage_sample_number = SAMPLE_NUM;
    double free_mem_percentage = 0.0;
    double cpu_usage_percentage = 0.0;
    double storage_usage_percentage = 0.0;
    DiskInfo m_storage={};
    bool openbmc_status = false;
};

class Switch
{
public:
    Switch()
    {
        get_basic_info();
        get_board_info();
    }
    static std::vector<EventMsgContent_T> m_Event_Resource_Add;
    static std::vector<EventMsgContent_T> m_Event_Resource_Remove;
    static std::vector<EventMsgContent_T> m_Event_Port_Resource_Add;
    static std::vector<EventMsgContent_T> m_Event_Port_Resource_Remove;
    static std::vector<EventMsgContent_T> get_Event_Resource_Add();
    static std::vector<EventMsgContent_T> get_Event_Port_Resource_Add();
    static std::vector<EventMsgContent_T> get_Event_Resource_Remove();
    static std::vector<EventMsgContent_T> get_Event_Port_Resource_Remove();
    static std::vector<EventMsgContent_T> get_Event_Resource_Alert();
    static std::vector<EventMsgContent_T> get_Event_Port_Resource_Alert();

    static void clean_Event_Resource_Event();
    static void clean_Event_Port_Resource_Event();

    SysResource m_SysResource={};

    ~Switch();

    enum Fan_Content
    {
        RPM = 1,
        Per = 2,
        Type = 3,
        Fan_Present = 4
    };

    enum Psu_Content
    {
        Vin = 1,
        Vout = 2,
        Iin = 3,
        Iout = 4,
        Pin = 5,
        Pout = 6,
        Psu_Present = 7,
        Psu_type = 8
    };

    enum Thermal_Content
    {
        Current_Temperature = 1,
        Warning_uppper = 2,
        Warning_lower = 3,
        Error = 4,
        Shutdown = 5,
        Thermal_Type = 6,
        Thermal_Present = 7
    };

    enum Port_Content
    {
        Port_Present = 1,
        Port_Type = 2
    };

    void monitor_sys_resource();
    void get_fan_info();
    void get_psu_info();
    void get_thermal_info();
    void get_port_oom_info();
    void get_per_port_sys_file();
    void get_port_present_info();
    void get_pon_port_oom_info();
    void update_fan_present_event();
    void update_psu_present_event();
    void update_port_present_event();
    void update_thermal_present_event();
    void update_transceivers_oom_event();

    int get_fan_info_by_(int fanid, Fan_Content id);
    int get_fan_num() { return m_fan_max_num; };
    int get_psu_num() { return m_psu_max_num; };
    double get_thermal_info_by_(int thermalid, Thermal_Content id);
    int get_thermal_num() { return m_thermal_sen_max_num; };
    int get_port_info_by_(int portid, Port_Content id);
    int get_port_num() { return m_port_max_num; };


    static Switch &get_instance();


    std::string get_fan_info_by_(int fanid, std::string type);
    std::string get_thermal_info_by_(int thermalid, std::string type);
    std::string get_product_name() { return m_Product_Name; };
    std::string get_platform_name() { return m_Platform_Name; };
    std::string get_manufacturer() { return m_Manufacturer; };
    std::string get_serial_number() { return m_Serial_Number; };
    std::string get_part_number() { return m_Part_Number; };
    std::string get_manufacture_date() { return m_Manu_Date; };
    std::string get_service_tag() { return m_Service_Tag; };
    std::string get_nos_type() { return m_nos_type; };

    json::Value get_port_trans_info_by_(int portid);

    virtual void set_port_tx_disable_status(int port, bool tx_status);
    virtual int get_port_tx_disable_status(int port);
    virtual int get_psu_info_by_(int psuid, Psu_Content id);
    std::string get_psu_info_by_(int psuid, std::string type);

protected:
    unsigned char m_MAC[6] = {};
    std::string m_Product_Name = {};
    std::string m_Part_Number = {};
    std::string m_Serial_Number = {};
    std::string m_Manufacturer = {};
    std::string m_Manu_Date = {};
    std::string m_Vendor = {};
    std::string m_Platform_Name = {};
    std::string m_Label_Revision = {};
    std::string m_Country_Code = {};
    std::string m_Diag_Version = {};
    std::string m_Service_Tag = {};
    std::string m_Device_Version = {};
    std::string m_ONIE_Version = {};
    std::string m_NOS_type = {};
    std::string m_NOS_version= {};
    std::string m_Cpld_version = {};
    std::string m_Fpga_version = {};
    std::string m_BIOS_version = {};
    std::string m_BAL_version = {};
    std::string m_BAL_BSP_version = {};
    std::string m_cpu_manu = {};
    std::string m_cpu_vid = {};
    std::string m_cpu_model_name = {};
    std::string m_platinfo_conf_path = {"/etc/psme/platform_info.conf"};
    std::string m_platinfo_name = {"na"};
    std::string m_nos_type = {"onl"};
    std::string m_sys_tx_name={"/sfp_tx_disable"};

    Json::Reader m_eeprom_j_reader = {};

    int m_MAC_Range = 0;
    int m_fan_max_num = 0;
    int m_port_max_num = 0;
    int m_thermal_sen_max_num = 0;
    int m_psu_max_num = 0;
    int m_cpu_stepping = 0;
    int m_cpu_max_speed = 0;
    int m_cpu_total_core = 0;
    int m_cpu_total_thread = 0;
    int m_memory_total_count = 0;
    int m_drv_total_count = 0;

    // method //
    void get_basic_info();
    void get_board_info();
    void get_fan_thermal_threshold_info(); //FAN and Thermal//

    mutable std::mutex m_data_mutex{};

    void set_port_present(int ID, bool status)
    {
        // bitwise mapping : 0x1 means port 1 , 0x2 means port2, 0x3 means port 1 port2 //
        if (((ID - 1) >= 0) && ((ID - 1) < 64))
        {
            if (status)
            {
                m_Port_Present |= ((1ULL << (ID - 1)));
            }
            else
            {
                m_Port_Present &= (~(1ULL << (ID - 1)));
            }
        }
        else if (((ID - 1) >= 0) && ((ID - 1) < 128) && (ID - 1 >= 64))
        {
            if (status)
            {
                m_Port_Present_A64 |= ((1ULL << (ID - 1)));
            }
            else
            {
                m_Port_Present_A64 &= (~(1ULL << (ID - 1)));
            }
        }
    };

    void set_thermal_present(int ID, bool status)
    {
        // bitwise mapping : 0x1 means port 1 , 0x2 means port2, 0x3 means port 1 port2 //
        if (ID - 1 >= 0)
        {
            if (status)
                m_Thermal_Present |= ((1 << (ID - 1)));
            else
                m_Thermal_Present &= (~(1 << (ID - 1)));
        }
        m_pre_Thermal_Present = m_Thermal_Present;
    };

    unsigned int get_thermal_present() { return m_Thermal_Present; };

    void set_fan_present(int ID, bool status)
    {
        // bitwise mapping : 0x1 means port 1 , 0x2 means port2, 0x3 meane port 1 port2 //
        if (ID - 1 >= 0)
        {
            if (status)
                m_Fan_Present |= ((1 << (ID - 1)));
            else
                m_Fan_Present &= (~(1 << (ID - 1)));
        }
    };

    unsigned int get_fan_present() { return m_Fan_Present; };

    void set_psu_present(int ID, bool status)
    {
        // bitwise mapping : 0x1 means port 1 , 0x2 means port2, 0x3 means port 1 port2 //
        if (ID - 1 >= 0)
        {
            if (status)
                m_Psu_Present |= ((1 << (ID - 1)));
            else
                m_Psu_Present &= (~(1 << (ID - 1)));
        }
    };

    unsigned int get_psu_present() { return m_Psu_Present; };

    vector<Thermal_Info *> m_vec_Thermal_Info = {};
    vector<Port_Info *> m_vec_Port_Info = {};
    vector<Psu_Info *> m_vec_Psu_Info = {};
    vector<Fan_Info *> m_vec_Fan_Info = {};

    unsigned int m_Thermal_Present = 0;
    unsigned int m_Psu_Present = 0;
    unsigned int m_Fan_Present = 0;
    unsigned int m_pre_Thermal_Present = 0;
    unsigned int m_pre_Psu_Present = 0;
    unsigned int m_pre_Fan_Present = 0;

    unsigned long long m_pre_Port_Present = 0;
    unsigned long long m_pre_Port_Present_A64 = 0;
    unsigned long long m_Port_Present = 0;
    unsigned long long m_Port_Present_A64 = 0;
};
} // namespace ecrf_pal_helper 

#endif
