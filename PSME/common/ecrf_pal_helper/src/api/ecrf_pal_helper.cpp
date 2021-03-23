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
#include "ecrf_pal_helper/api/ecrf_pal_helper.hpp"
#include "ecrf_pal_helper/api/ecrf_pal_helper_api.hpp"
#include "ecrf_pal_helper/asxvolt16.hpp"
#include "ecrf_pal_helper/asgvolt64.hpp"
#include "ecrf_pal_helper/as5916_54xks.hpp"
#include "ecrf_pal_helper/as5916_54xk.hpp"
#include <eclog_helper/eclog_helper.hpp>
#include <ecsys_helper/ecsys_helper.hpp>
#include <sys/sysinfo.h>
using namespace eclog_helper;
using namespace ecsys_helper;
using namespace ecrf_pal_helper;
using namespace ecrf_pal_com_s;
using namespace ecrf_pal_com_api;

#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <dirent.h>
#include <string.h>
#include <chrono>
#include <ctime>

#define UNUSED(x) (void)(x)

namespace ecrf_pal_helper
{
signed long get_value_from_pointer(signed char *ptr, int size);
unsigned long get_value_from_pointer_u(unsigned char *ptr, int size);
auto &gADbg = ADbg::get_instance();
signed long get_value_from_pointer(signed char *ptr, int size)
{
    signed long sum = 0;
    signed char i;

    if (size > 4)
        return (sum);

    for (i = 0; i < size; ++i)
    {
        sum = sum * 256 + (*(ptr + i));
    }

    return (sum);
}

unsigned long get_value_from_pointer_u(unsigned char *ptr, int size)
{
    try
    {
        unsigned long sum = 0;
        unsigned char i;

        if (size > 4)
            return (sum);

        for (i = 0; i < size; ++i)
        {
            sum = sum * 256 + (*(ptr + i));
        }

        return (sum);
    }
    catch (const exception &e)
    {
        printf("get_value_from_pointer_u error\r\n");
        return 0;
    }
}

float e_oom::get_value(std::string attri_name)
{
    try
    {
        Json::Value Att_Json = m_std["Attributes"];
        Json::Value _Json = get_attri_by_name(attri_name, Att_Json);

        float tmp_unit = 0;
        int tmp_base = _Json[attri_name]["Byte_Address"].asInt();
        //printf("get_status [%s] base offset[%d]\r\n",attri_name.c_str(), tmp_base);
        int tmp_size = _Json[attri_name]["Size"].asInt();
        //printf("get_status  Size[%d]\r\n", tmp_size);
        tmp_unit = _Json[attri_name]["unit"].asFloat();
        //printf("get_status  tmp_unit[%f]\r\n", tmp_unit);

        signed char tmp_PN[4] = {0};
        memcpy(tmp_PN, m_eeprom + tmp_base, tmp_size);
        signed long e_value = get_value_from_pointer(tmp_PN, tmp_size);

        //printf("get_value [%d] \r\n", e_value);
        //printf("get_value return [%f]\r\n", e_value * tmp_unit);

        return (e_value * tmp_unit);
    }
    catch (const exception &e)
    {
        printf("get_value error\r\n");
        return 0;
    }
}

float e_oom::get_value_u(std::string attri_name)
{
    try
    {
        Json::Value Att_Json = m_std["Attributes"];
        Json::Value _Json = get_attri_by_name(attri_name, Att_Json);

        float tmp_unit = 0;
        int tmp_base = _Json[attri_name]["Byte_Address"].asInt();
        //printf("get_status_u [%s] base offset[%d]\r\n",attri_name.c_str(), tmp_base);
        int tmp_size = _Json[attri_name]["Size"].asInt();
        //printf("get_status_u  Size[%d]\r\n", tmp_size);
        tmp_unit = _Json[attri_name]["unit"].asFloat();
        //printf("get_status_u  tmp_unit[%f]\r\n", tmp_unit);

        unsigned char tmp_PN[4] = {0};
        memcpy(tmp_PN, m_eeprom + tmp_base, tmp_size);
        unsigned long e_value = get_value_from_pointer_u(tmp_PN, tmp_size);

        //printf("get_value_u [%u] \r\n", e_value);
        //printf("get_value_u return [%f] \r\n", e_value * tmp_unit);

        return (e_value * tmp_unit);
    }
    catch (const exception &e)
    {
        printf("get_value_u error\r\n");
        return 0;
    }
}

std::string e_oom::get_value_s(std::string attri_name)
{
    try
    {
        std::string ret_attri = {};
        unsigned char tmp_value[32] = {0};

        Json::Value Att_Json = m_std["Attributes"];
        Json::Value _Json = get_attri_by_name(attri_name, Att_Json);

        int tmp_base = _Json[attri_name]["Byte_Address"].asInt();
        //printf("get_status_s [%s] base offset[%d]\r\n",attri_name.c_str(), tmp_base);
        int tmp_size = _Json[attri_name]["Size"].asInt();
        //printf("get_status_s  Size[%d]\r\n", tmp_size);

        if (tmp_size < (int)sizeof(tmp_value))
        {
            memcpy(tmp_value, m_eeprom + tmp_base, tmp_size);
            ret_attri = (char *)tmp_value;
            ret_attri.erase(remove(ret_attri.begin(), ret_attri.end(), ' '), ret_attri.end());
        }

        return ret_attri;
    }
    catch (const exception &e)
    {
        printf("get_value_s error\r\n");
        return 0;
    }
}

void e_oom::refresh_vendor_info()
{
    try
    {
        /*vendor_info Begin:*/
        std::string ff;
        /* get SFP Vendor Name */
        if (m_current_status["SFP Vendor Name"] == "NA")
        {
            ff = get_value_s("Vendor_name");
            m_current_status["SFP Vendor Name"] = ff;
        }

        /* get Part Number */
        if (m_current_status["Part Number"] == "NA")
        {
            ff = get_value_s("Vendor_PN");
            m_current_status["Part Number"] = ff;
        }

        /* get Serial Number */
        if (m_current_status["Serial Number"] == "NA")
        {
            ff = get_value_s("Vendor_SN");
            m_current_status["Serial Number"] = ff;
        }

        /* get Manufacture Date */
        if (m_current_status["Manufacture Date"] == "NA")
        {
            ff = get_value_s("Data_Code");
            m_current_status["Manufacture Date"] = ff;
        }
        /*vendor_info End  :*/
    }
    catch (const exception &e)
    {
        printf("refresh_vendor_info error\r\n");
        return;
    }
}

void e_oom::default_vendor_info()
{
    /*vendor_info Begin:*/
    m_current_status["SFP Vendor Name"] = "NA";
    m_current_status["Part Number"] = "NA";
    m_current_status["Serial Number"] = "NA";
    m_current_status["Manufacture Date"] = "NA";
    /*vendor_info End  :*/
}

void e_oom::refresh_temp()
{
    try
    {
        /*Temperature Begin:*/
        float ff;
        /* get Temp_High_Alarm */
        if (m_current_status["Temperature"]["UpperThresholdFatal"] == 0)
        {
            ff = get_value("Temp_High_Alarm");
            m_current_status["Temperature"]["UpperThresholdFatal"] = FF3(ff);
        }

        /* get Temp_Low_Alarm */
        if (m_current_status["Temperature"]["LowerThresholdFatal"] == 0)
        {
            ff = get_value("Temp_Low_Alarm");
            m_current_status["Temperature"]["LowerThresholdFatal"] = FF3(ff);
        }

        /* get Temp_High_Warning */
        if (m_current_status["Temperature"]["UpperThresholdCritical"] == 0)
        {
            ff = get_value("Temp_High_Warning");
            m_current_status["Temperature"]["UpperThresholdCritical"] = FF3(ff);
        }

        /* get Temp_Low_Warning */
        if (m_current_status["Temperature"]["LowerThresholdCritical"] == 0)
        {
            ff = get_value("Temp_Low_Warning");
            m_current_status["Temperature"]["LowerThresholdCritical"] = FF3(ff);
        }

        /* get Temperature */
        ff = get_value("Temperature");
        m_current_status["Temperature"]["Reading"] = FF3(ff);

        /*
    
               Critical
    
           UpperThresholdFatal >=
    
               Warning
    
           UpperThresholdCritical >=
    
               OK
    
           LowerrThresholdCritical >=
          
               Warning
    
           LowerThresholdFatal >=
    
               Critical
    */
        if (((FF3(ff) >= m_current_status["Temperature"]["UpperThresholdCritical"]) && (FF3(ff) < m_current_status["Temperature"]["UpperThresholdFatal"])) || ((FF3(ff) >= m_current_status["Temperature"]["LowerThresholdFatal"]) && (FF3(ff) < m_current_status["Temperature"]["LowerThresholdCritical"])))
        {
            m_current_status["Temperature"]["Status"]["Health"] = "Warning";
            m_current_status["Temperature"]["Status"]["State"] = "Enabled";
        }
        else if ((FF3(ff) >= m_current_status["Temperature"]["UpperThresholdFatal"]) || (FF3(ff) < m_current_status["Temperature"]["LowerThresholdFatal"]))
        {
            m_current_status["Temperature"]["Status"]["Health"] = "Critical";
            m_current_status["Temperature"]["Status"]["State"] = "Enabled";
        }
        else
        {
            m_current_status["Temperature"]["Status"]["Health"] = "OK";
            m_current_status["Temperature"]["Status"]["State"] = "Enabled";
        }
        /*Temperature End  :*/
    }
    catch (const exception &e)
    {
        printf("refresh_temp error\r\n");
        return;
    }
}

void e_oom::default_temp()
{
    /*Temperature Begin:*/
    m_current_status["Temperature"]["UpperThresholdFatal"] = 0;
    m_current_status["Temperature"]["LowerThresholdFatal"] = 0;
    m_current_status["Temperature"]["UpperThresholdCritical"] = 0;
    m_current_status["Temperature"]["LowerThresholdCritical"] = 0;
    m_current_status["Temperature"]["Reading"] = 0;
    m_current_status["Temperature"]["Status"]["Health"] = json::Value::Type::NIL;
    m_current_status["Temperature"]["Status"]["State"] = json::Value::Type::NIL;
    /*Temperature End  :*/
}

void e_oom::refresh_voltage()
{
    try
    {
        /*Voltage Begin:*/
        double ff;
        /* get Voltage_High_Alarm */
        if (m_current_status["Voltage"]["UpperThresholdFatal"] == 0)
        {
            ff = get_value_u("Voltage_High_Alarm");
            m_current_status["Voltage"]["UpperThresholdFatal"] = FF3(ff);
        }

        /* get Voltage_Low_Alarm */
        if (m_current_status["Voltage"]["LowerThresholdFatal"] == 0)
        {
            ff = get_value_u("Voltage_Low_Alarm");
            m_current_status["Voltage"]["LowerThresholdFatal"] = FF3(ff);
        }

        /* get Voltage_High_Warning */
        if (m_current_status["Voltage"]["UpperThresholdCritical"] == 0)
        {
            ff = get_value_u("Voltage_High_Warning");
            m_current_status["Voltage"]["UpperThresholdCritical"] = FF3(ff);
        }

        /* get Voltage_Low_Warning */
        if (m_current_status["Voltage"]["LowerThresholdCritical"] == 0)
        {
            ff = get_value_u("Voltage_Low_Warning");
            m_current_status["Voltage"]["LowerThresholdCritical"] = FF3(ff);
        }

        /* get Voltage */
        ff = get_value_u("Voltage");
        m_current_status["Voltage"]["Reading"] = FF3(ff);

        /*
    
               Critical
    
           UpperThresholdFatal 
    
               Warning
    
           UpperThresholdCritical 
    
               OK
    
           LowerThresholdCritical 
          
               Warning
    
           LowerThresholdFatal 
    
               Critical
    */

        if (((FF3(ff) >= m_current_status["Voltage"]["UpperThresholdCritical"]) && (FF3(ff) < m_current_status["Voltage"]["UpperThresholdFatal"])) || ((FF3(ff) >= m_current_status["Temperature"]["LowerThresholdFatal"]) && (FF3(ff) < m_current_status["Temperature"]["LowerThresholdCritical"])))
        {
            m_current_status["Voltage"]["Status"]["Health"] = "Warning";
            m_current_status["Voltage"]["Status"]["State"] = "Enabled";
        }
        else if ((FF3(ff) >= m_current_status["Voltage"]["UpperThresholdFatal"]) || (FF3(ff) < m_current_status["Voltage"]["LowerThresholdFatal"]))
        {
            m_current_status["Voltage"]["Status"]["Health"] = "Critical";
            m_current_status["Voltage"]["Status"]["State"] = "Enabled";
        }
        else
        {
            m_current_status["Voltage"]["Status"]["Health"] = "OK";
            m_current_status["Voltage"]["Status"]["State"] = "Enabled";
        }
        /*Voltage End  :*/
    }
    catch (const exception &e)
    {
        printf("refresh_voltage error\r\n");
        return;
    }
}

void e_oom::default_voltage()
{
    /*Voltage Begin:*/
    m_current_status["Voltage"]["UpperThresholdFatal"] = 0;
    m_current_status["Voltage"]["LowerThresholdFatal"] = 0;
    m_current_status["Voltage"]["UpperThresholdCritical"] = 0;
    m_current_status["Voltage"]["LowerThresholdCritical"] = 0;
    m_current_status["Voltage"]["Reading"] = 0;
    m_current_status["Voltage"]["Status"]["Health"] = json::Value::Type::NIL;
    m_current_status["Voltage"]["Status"]["State"] = json::Value::Type::NIL;
    /*Voltage End  :*/
}

void e_oom::refresh_bias()
{
    try
    {
        float ff;
        /*BiasCurrent Begin  :*/
        /* get Bias_High_Alarm */
        if (m_current_status["BiasCurrent"]["UpperThresholdFatal"] == 0)
        {
            ff = get_value_u("Bias_High_Alarm");
            m_current_status["BiasCurrent"]["UpperThresholdFatal"] = FF3(ff);
        }

        /* get Bias_Low_Alarm */
        if (m_current_status["BiasCurrent"]["LowerThresholdFatal"] == 0)
        {
            ff = get_value_u("Bias_Low_Alarm");
            m_current_status["BiasCurrent"]["LowerThresholdFatal"] = FF3(ff);
        }

        /* get Bias_High_Warning */
        if (m_current_status["BiasCurrent"]["UpperThresholdCritical"] == 0)
        {
            ff = get_value_u("Bias_High_Warning");
            m_current_status["BiasCurrent"]["UpperThresholdCritical"] = FF3(ff);
        }

        /* get Bias_Low_Warning */
        if (m_current_status["BiasCurrent"]["LowerThresholdCritical"] == 0)
        {
            ff = get_value_u("Bias_Low_Warning");
            m_current_status["BiasCurrent"]["LowerThresholdCritical"] = FF3(ff);
        }

        /* get Bias */
        ff = get_value_u("Tx_Bias");
        m_current_status["BiasCurrent"]["Reading"] = FF3(ff);
        /*
    
               Critical
    
           UpperThresholdFatal >=
    
               Warning
    
           UpperThresholdCritical >=
    
               OK
    
           LowerThresholdCritical >=
          
               Warning
    
           LowerThresholdFatal >=
    
               Critical
    */
        if (((FF3(ff) >= m_current_status["BiasCurrent"]["UpperThresholdCritical"]) && (FF3(ff) < m_current_status["BiasCurrent"]["UpperThresholdFatal"])) || ((FF3(ff) >= m_current_status["BiasCurrent"]["LowerThresholdFatal"]) && (FF3(ff) < m_current_status["BiasCurrent"]["LowerThresholdCritical"])))
        {
            m_current_status["BiasCurrent"]["Status"]["Health"] = "Warning";
            m_current_status["BiasCurrent"]["Status"]["State"] = "Enabled";
        }
        else if ((FF3(ff) >= m_current_status["BiasCurrent"]["UpperThresholdFatal"]) || (FF3(ff) < m_current_status["BiasCurrent"]["LowerThresholdFatal"]))
        {
            m_current_status["BiasCurrent"]["Status"]["Health"] = "Critical";
            m_current_status["BiasCurrent"]["Status"]["State"] = "Enabled";
        }
        else
        {
            m_current_status["BiasCurrent"]["Status"]["Health"] = "OK";
            m_current_status["BiasCurrent"]["Status"]["State"] = "Enabled";
        }
        /*BiasCurrent End    :*/
    }
    catch (const exception &e)
    {
        printf("refresh_bias error\r\n");
        return;
    }
}

void e_oom::default_bias()
{
    /*BiasCurrent Begin  :*/
    m_current_status["BiasCurrent"]["UpperThresholdFatal"] = 0;
    m_current_status["BiasCurrent"]["LowerThresholdFatal"] = 0;
    m_current_status["BiasCurrent"]["UpperThresholdCritical"] = 0;
    m_current_status["BiasCurrent"]["LowerThresholdCritical"] = 0;
    m_current_status["BiasCurrent"]["Reading"] = 0;
    m_current_status["BiasCurrent"]["Status"]["Health"] = json::Value::Type::NIL;
    m_current_status["BiasCurrent"]["Status"]["State"] = json::Value::Type::NIL;

    /*BiasCurrent End    :*/
}

void e_oom::refresh_tx_pwr()
{
    try
    {
        /*TxPower Current Begin    :*/
        float ff;
        /* get TX_Power_High_Alarm */

        if (m_current_status["TxPower"]["UpperThresholdFatal"] == 0)
        {
            ff = get_value_u("TX_Power_High_Alarm");
            m_current_status["TxPower"]["UpperThresholdFatal"] = FF3(ff);
        }

        /* get TX_Power_Low_Alarm */
        if (m_current_status["TxPower"]["LowerThresholdFatal"] == 0)
        {
            ff = get_value_u("TX_Power_Low_Alarm");
            m_current_status["TxPower"]["LowerThresholdFatal"] = FF3(ff);
        }

        /* get TX_Power_High_Warning */
        if (m_current_status["TxPower"]["UpperThresholdCritical"] == 0)
        {
            ff = get_value_u("TX_Power_High_Warning");
            m_current_status["TxPower"]["UpperThresholdCritical"] = FF3(ff);
        }

        /* get TX_Power_Low_Warning */
        if (m_current_status["TxPower"]["LowerThresholdCritical"] == 0)
        {
            ff = get_value_u("TX_Power_Low_Warning");
            m_current_status["TxPower"]["LowerThresholdCritical"] = FF3(ff);
        }

        /* get Tx_Power */
        ff = get_value_u("Tx_Power");
        m_current_status["TxPower"]["Reading"] = FF3(ff);

        /*
    
               Critical
    
           UpperThresholdFatal 
    
               Warning
    
           UpperThresholdCritical 
    
               OK
    
           LowerThresholdCritical 
          
               Warning
    
           LowerThresholdFatal 
    
               Critical
    */

        if (((FF3(ff) >= m_current_status["TxPower"]["UpperThresholdCritical"]) && (FF3(ff) < m_current_status["TxPower"]["UpperThresholdFatal"])) || ((FF3(ff) >= m_current_status["TxPower"]["LowerThresholdFatal"]) && (FF3(ff) < m_current_status["TxPower"]["LowerThresholdCritical"])))
        {
            m_current_status["TxPower"]["Status"]["Health"] = "Warning";
            m_current_status["TxPower"]["Status"]["State"] = "Enabled";
        }
        else if ((FF3(ff) >= m_current_status["TxPower"]["UpperThresholdFatal"]) || (FF3(ff) < m_current_status["TxPower"]["LowerThresholdFatal"]))
        {
            m_current_status["TxPower"]["Status"]["Health"] = "Critical";
            m_current_status["TxPower"]["Status"]["State"] = "Enabled";
        }
        else
        {
            m_current_status["TxPower"]["Status"]["Health"] = "OK";
            m_current_status["TxPower"]["Status"]["State"] = "Enabled";
        }
        /*TxPower Current End      :*/
    }
    catch (const exception &e)
    {
        printf("refresh_tx_pwr error\r\n");
        return;
    }
}

void e_oom::default_tx_pwr()
{
    /*TxPower Current Begin    :*/
    m_current_status["TxPower"]["UpperThresholdFatal"] = 0;
    m_current_status["TxPower"]["LowerThresholdFatal"] = 0;
    m_current_status["TxPower"]["UpperThresholdCritical"] = 0;
    m_current_status["TxPower"]["LowerThresholdCritical"] = 0;
    m_current_status["TxPower"]["Reading"] = 0;
    m_current_status["TxPower"]["Status"]["Health"] = json::Value::Type::NIL;
    m_current_status["TxPower"]["Status"]["State"] = json::Value::Type::NIL;
    /*TxPower Current End      :*/
}

void e_oom::refresh_rx_pwr()
{
    try
    {
        /*RxPower Current Begin      :*/
        float ff;
        /* get RX_Power_High_Alarm */
        if (m_current_status["RxPower"]["UpperThresholdFatal"] == 0)
        {
            ff = get_value("RX_Power_High_Alarm");
            m_current_status["RxPower"]["UpperThresholdFatal"] = FF3(ff);
        }

        /* get RX_Power_Low_Alarm */
        if (m_current_status["RxPower"]["LowerThresholdFatal"] == 0)
        {
            ff = get_value("RX_Power_Low_Alarm");
            m_current_status["RxPower"]["LowerThresholdFatal"] = FF3(ff);
        }

        /* get RX_Power_High_Warning */
        if (m_current_status["RxPower"]["UpperThresholdCritical"] == 0)
        {
            ff = get_value("RX_Power_High_Warning");
            m_current_status["RxPower"]["UpperThresholdCritical"] = FF3(ff);
        }

        /* get RX_Power_Low_Warning */
        if (m_current_status["RxPower"]["LowerThresholdCritical"] == 0)
        {
            ff = get_value("RX_Power_Low_Warning");
            m_current_status["RxPower"]["LowerThresholdCritical"] = FF3(ff);
        }

        /* get Rx_Power */
        ff = get_value("Rx_Power");
        m_current_status["RxPower"]["Reading"] = FF3(ff);

        /*
    
               Critical
    
           UpperThresholdFatal >=
    
               Warning
    
           UpperThresholdCritical >=
    
               OK
    
           LowerThresholdCritical >=
          
               Warning
    
           LowerThresholdFatal >=
    
               Critical
    */
        if (((FF3(ff) >= m_current_status["RxPower"]["UpperThresholdCritical"]) && (FF3(ff) < m_current_status["RxPower"]["UpperThresholdFatal"])) || ((FF3(ff) >= m_current_status["RxPower"]["LowerThresholdFatal"]) && (FF3(ff) < m_current_status["RxPower"]["LowerThresholdCritical"])))
        {
            m_current_status["RxPower"]["Status"]["Health"] = "Warning";
            m_current_status["RxPower"]["Status"]["State"] = "Enabled";
        }
        else if ((FF3(ff) >= m_current_status["RxPower"]["UpperThresholdFatal"]) || (FF3(ff) < m_current_status["RxPower"]["LowerThresholdFatal"]))
        {
            m_current_status["RxPower"]["Status"]["Health"] = "Critical";
            m_current_status["RxPower"]["Status"]["State"] = "Enabled";
        }
        else
        {
            m_current_status["RxPower"]["Status"]["Health"] = "OK";
            m_current_status["RxPower"]["Status"]["State"] = "Enabled";
        }
        /*RxPower Current End      :*/
    }
    catch (const exception &e)
    {
        printf("refresh_rx_pwr error\r\n");
        return;
    }
}

void e_oom::default_rx_pwr()
{
    /*RxPower Current Begin      :*/
    m_current_status["RxPower"]["UpperThresholdFatal"] = 0;
    m_current_status["RxPower"]["LowerThresholdFatal"] = 0;
    m_current_status["RxPower"]["UpperThresholdCritical"] = 0;
    m_current_status["RxPower"]["LowerThresholdCritical"] = 0;
    m_current_status["RxPower"]["Reading"] = 0;
    m_current_status["RxPower"]["Status"]["Health"] = json::Value::Type::NIL;
    m_current_status["RxPower"]["Status"]["State"] = json::Value::Type::NIL;
    /*RxPower Current End      :*/
}

bool e_oom::refresh_status()
{
    if (get_support())
    {
        /*Threshold value / Vendor info / olny get once until un-plug or restart service*/
        refresh_vendor_info();
        refresh_temp();
        refresh_voltage();
        refresh_bias();
        refresh_tx_pwr();
        refresh_rx_pwr();
    }
    return true;
}

bool e_oom::status_default()
{
    default_vendor_info();
    default_temp();
    default_voltage();
    default_bias();
    default_tx_pwr();
    default_rx_pwr();
    return true;
}

bool e_oom::get_eeprom_raw(int port_id)
{
    try
    {
        char buffer[SIZE_EEPROM] = {0};
        if (rfpal_sfp_tx_eeprom_get(port_id, buffer, SIZE_EEPROM) != 1)
        {
            //Todo : remove following function after use the same API
            std::ifstream is;
            is.rdbuf()->pubsetbuf(0, 0); // do not buffer //

            if (is)
            {
                is.seekg(0, is.beg);
#if 1
                is.open(m_eeprom_path);
#else
                //Read raw data file for testing //
                is.open("/outputfile.bin");
#endif
                if (is)
                {
                    is.read(buffer, SIZE_EEPROM);
                    if (store_eeprom(buffer))
                    {
#if 0 //for debug usage            
                    int i = 0; 
                    while(i < SIZE_EEPROM)
                    {
                        printf("%02x ", (unsigned char)buffer[i]);
                        i++;
                        if(i % 8 == 0)
                            printf("\r\n");
                    }
#endif
                        refresh_status();
                    }
                    else
                        status_default();

                    is.close();
                }
                else
                    std::cout << "Open" << m_eeprom_path << " error!!" << std::endl;
            }
            else
            {
                std::cout << "Port[" << m_eeprom_path << "] reading file error!!" << std::endl;
                status_default();
                return false;
            }
        }
        else
        {
            if (store_eeprom(buffer))
            {
#if 0 //for debug usage            
                    int i = 0; 
                    while(i < SIZE_EEPROM)
                    {
                        printf("%02x ", (unsigned char)buffer[i]);
                        i++;
                        if(i % 8 == 0)
                            printf("\r\n");
                    }
#endif
                refresh_status();
            }
            else
                status_default();
        }
        return true;
    }
    catch (const std::exception &e)
    {
        std::cout << "get_eeprom_raw() - exception : " << e.what() << std::endl;
        return false;
    }
}

Json::Value e_oom::get_attri_by_name(std::string att_name, Json::Value in)
{
    try
    {
        Json::Value PN_Json;
        int size = in.size();

        for (int i = 0; i < size; i++)
        {
            if (in[i].isMember(att_name))
            {
                //printf("get_attri_by_name return [%d]\r\n", i);
                return in[i];
            }
        }
        return PN_Json;
    }
    catch (const std::exception &e)
    {
        Json::Value PN_Json;

        std::cout << "get_attri_by_name() - exception : " << e.what() << std::endl;
        return PN_Json;
    }
}

// ------------------------------------------------------------------
//  New Transceiver Type need Added here
//  1. Decide by identify ID,
//     03h : SFP+  (10GPON) ,  8472i
//     06h : XFP   (XGSPON) ,  8077i
//     0dh : QSFP+ (40G)    ,  8436i
//     11h : QSFP28(100G)   ,  8438i
//  2. Identify which Std. should be used.
//  3. Store raw data.
// ------------------------------------------------------------------
bool e_oom::store_eeprom(char *in_eeprom)
{
    try
    {
        int id = in_eeprom[0];

        if (!get_support())
        {
            if (id == 0x6)
            {
                int size = m_8077i.size();
                Json::Value tmp_std;

                for (int i = 1; i <= size; i++)
                {
                    tmp_std = m_8077i[std::make_pair(id, i)];
                    std::string C_PN = tmp_std["C_PN"].asString();

                    gADbg.acc_printf(LEVEL_INFO,"8077 PART_NUMBER[%s]", C_PN.c_str());

                    Json::Value Att_Json = tmp_std["Attributes"];
                    Json::Value PN_Json = get_attri_by_name("Vendor_PN", Att_Json);

                    int PN_Base = PN_Json["Vendor_PN"]["Byte_Address"].asInt();
                    unsigned int PN_Size = PN_Json["Vendor_PN"]["Size"].asInt();

                    if (C_PN.size() < PN_Size)
                    {
                        char tmp_PN[64] = {0};
                        memcpy(tmp_PN, in_eeprom + PN_Base, C_PN.size());
                        gADbg.acc_printf(LEVEL_INFO,"tmp_PN[%s]", tmp_PN);
                        std::string tstring = tmp_PN;

                        if (tstring == C_PN)
                        {
                            m_std = tmp_std;
                            gADbg.acc_printf(LEVEL_INFO,"8077i Use C_PN[%s] !!!!", C_PN.c_str());
                            set_support(true);
                            memcpy(m_eeprom, in_eeprom, SIZE_EEPROM);
                            return true;
                        }
                    }
                }

                // Use 8077i as default //
                std::string STD_PN = "8077i";
                m_std = tmp_std;
                gADbg.acc_printf(LEVEL_WARN,"8077i Cannot get customer's define. Use Std[%s] one.", STD_PN.c_str());
                set_support(true);

                memcpy(m_eeprom, in_eeprom, SIZE_EEPROM);

                return true;
            }
            else if (id == 0x3)
            {
                int size = m_8472.size();
                Json::Value tmp_std;

                for (int i = 1; i <= size; i++)
                {
                    tmp_std = m_8472[std::make_pair(id, i)];
                    std::string C_PN = tmp_std["C_PN"].asString();
                    std::string STD_PN = "8472";

                    gADbg.acc_printf(LEVEL_INFO,"8472 C_PN[%s]", C_PN.c_str());

                    Json::Value Att_Json = tmp_std["Attributes"];

                    Json::Value PN_Json = get_attri_by_name("Vendor_PN", Att_Json);

                    int PN_Base = PN_Json["Vendor_PN"]["Byte_Address"].asInt();
                    gADbg.acc_printf(LEVEL_INFO,"PN_Base offset[%d]", PN_Base);

                    unsigned int PN_Size = PN_Json["Vendor_PN"]["Size"].asInt();
                    gADbg.acc_printf(LEVEL_INFO,"PN_Base Size[%d]", PN_Size);

                    if (C_PN.size() < PN_Size)
                    {
                        char tmp_PN[64] = {0};
                        memcpy(tmp_PN, in_eeprom + PN_Base, C_PN.size());
                        gADbg.acc_printf(LEVEL_INFO,"8472 tmp_PN[%s]", tmp_PN);
                        std::string tstring = tmp_PN;

                        if (tstring == C_PN)
                        {
                            m_std = tmp_std;
                            gADbg.acc_printf(LEVEL_INFO,"8472 Use C_PN[%s] !!!!", C_PN.c_str());
                            set_support(true);
                            memcpy(m_eeprom, in_eeprom, SIZE_EEPROM);
                            return true;
                        }
                    }
                }

                // Use 8472 as default //
                std::string STD_PN = "8472";
                m_std = tmp_std;
                gADbg.acc_printf(LEVEL_WARN,"Cannot get customer's define. Use Std[%s] one.", STD_PN.c_str());
                set_support(true);
                memcpy(m_eeprom, in_eeprom, SIZE_EEPROM);

                return true;
            }
            else if (id == 0x11)
            {
                int size = m_8438i.size();
                Json::Value tmp_std;

                for (int i = 1; i <= size; i++)
                {
                    tmp_std = m_8438i[std::make_pair(0x11, i)];
                    std::string C_PN = tmp_std["C_PN"].asString();
                    std::string STD_PN = "8438";

                    gADbg.acc_printf(LEVEL_INFO,"8438 C_PN[%s]", C_PN.c_str());

                    Json::Value Att_Json = tmp_std["Attributes"];

                    Json::Value PN_Json = get_attri_by_name("Vendor_PN", Att_Json);

                    int PN_Base = PN_Json["Vendor_PN"]["Byte_Address"].asInt();
                    gADbg.acc_printf(LEVEL_INFO,"PN_Base offset[%d]", PN_Base);

                    unsigned int PN_Size = PN_Json["Vendor_PN"]["Size"].asInt();
                    gADbg.acc_printf(LEVEL_INFO,"PN_Base Size[%d]", PN_Size);

                    if (C_PN.size() < PN_Size)
                    {
                        char tmp_PN[64] = {0};
                        memcpy(tmp_PN, in_eeprom + PN_Base, C_PN.size());
                        gADbg.acc_printf(LEVEL_INFO,"8438 tmp_PN[%s]", tmp_PN);
                        std::string tstring = tmp_PN;

                        if (tstring == C_PN)
                        {
                            m_std = tmp_std;
                            gADbg.acc_printf(LEVEL_INFO,"8438 Use C_PN[%s] !!!!", C_PN.c_str());
                            set_support(true);
                            memcpy(m_eeprom, in_eeprom, SIZE_EEPROM);
                            return true;
                        }
                    }
                }

                // Use 8438 as default //
                std::string STD_PN = "8438";
                m_std = tmp_std;
                gADbg.acc_printf(LEVEL_WARN,"Cannot get customer's define. Use Std[%s] one.", STD_PN.c_str());
                set_support(true);
                memcpy(m_eeprom, in_eeprom, SIZE_EEPROM);

                return true;
            }
            else if (id == 0x0d)
            {
                int size = m_8436i.size();
                Json::Value tmp_std;

                for (int i = 1; i <= size; i++)
                {
                    tmp_std = m_8436i[std::make_pair(0x0d, i)];
                    std::string C_PN = tmp_std["C_PN"].asString();
                    std::string STD_PN = "8436";

                    gADbg.acc_printf(LEVEL_INFO,"8436 C_PN[%s]", C_PN.c_str());

                    Json::Value Att_Json = tmp_std["Attributes"];

                    Json::Value PN_Json = get_attri_by_name("Vendor_PN", Att_Json);

                    int PN_Base = PN_Json["Vendor_PN"]["Byte_Address"].asInt();
                    gADbg.acc_printf(LEVEL_INFO,"PN_Base offset[%d]", PN_Base);

                    unsigned int PN_Size = PN_Json["Vendor_PN"]["Size"].asInt();
                    gADbg.acc_printf(LEVEL_INFO,"PN_Base Size[%d]", PN_Size);

                    if (C_PN.size() < PN_Size)
                    {
                        char tmp_PN[64] = {0};
                        memcpy(tmp_PN, in_eeprom + PN_Base, C_PN.size());
                        gADbg.acc_printf(LEVEL_INFO,"8436 tmp_PN[%s]", tmp_PN);
                        std::string tstring = tmp_PN;

                        if (tstring == C_PN)
                        {
                            m_std = tmp_std;
                            gADbg.acc_printf(LEVEL_INFO,"8436 Use C_PN[%s] !!!!", C_PN.c_str());
                            set_support(true);
                            memcpy(m_eeprom, in_eeprom, SIZE_EEPROM);
                            return true;
                        }
                    }
                }

                // Use 8436 as default //
                std::string STD_PN = "8436";
                m_std = tmp_std;
                gADbg.acc_printf(LEVEL_WARN,"Cannot get customer's define. Use Std[%s] one.", STD_PN.c_str());
                set_support(true);
                memcpy(m_eeprom, in_eeprom, SIZE_EEPROM);

                return true;
            }
            else
            {
                gADbg.acc_printf(LEVEL_WARN,"Not support Std.");
                return false;
            }
        }
        else
            memcpy(m_eeprom, in_eeprom, SIZE_EEPROM);

        return true;
    }
    catch (const std::exception &e)
    {
        std::cout << "store_eeprom() - exception : " << e.what() << std::endl;
        return false;
    }
}

void e_oom::set_proto()
{
    m_current_status["SFP Vendor Name"] = json::Value::Type::NIL;
    m_current_status["Part Number"] = json::Value::Type::NIL;
    m_current_status["Serial Number"] = json::Value::Type::NIL;
    m_current_status["Manufacture Date"] = json::Value::Type::NIL;

    json::Value Temperatures(json::Value::Type::OBJECT);

    Temperatures["Reading"] = json::Value::Type::NIL;
    Temperatures["UpperThresholdFatal"] = json::Value::Type::NIL;
    Temperatures["UpperThresholdCritical"] = json::Value::Type::NIL;
    Temperatures["UpperThresholdNonCritical"] = json::Value::Type::NIL;
    Temperatures["LowerThresholdNonCritical"] = json::Value::Type::NIL;
    Temperatures["LowerThresholdCritical"] = json::Value::Type::NIL;
    Temperatures["LowerThresholdFatal"] = json::Value::Type::NIL;

    Temperatures["Status"]["HealthRollup"] = json::Value::Type::NIL;
    Temperatures["Status"]["Health"] = json::Value::Type::NIL;
    Temperatures["Status"]["State"] = json::Value::Type::NIL;

    m_current_status["Temperature"] = std::move(Temperatures);

    json::Value Bias(json::Value::Type::OBJECT);

    Bias["Reading"] = json::Value::Type::NIL;
    Bias["UpperThresholdFatal"] = json::Value::Type::NIL;
    Bias["UpperThresholdCritical"] = json::Value::Type::NIL;
    Bias["UpperThresholdNonCritical"] = json::Value::Type::NIL;
    Bias["LowerThresholdNonCritical"] = json::Value::Type::NIL;
    Bias["LowerThresholdCritical"] = json::Value::Type::NIL;
    Bias["LowerThresholdFatal"] = json::Value::Type::NIL;

    Bias["Status"]["HealthRollup"] = json::Value::Type::NIL;
    Bias["Status"]["Health"] = json::Value::Type::NIL;
    Bias["Status"]["State"] = json::Value::Type::NIL;

    m_current_status["BiasCurrent"] = std::move(Bias);

    json::Value Voltage(json::Value::Type::OBJECT);

    Voltage["Reading"] = json::Value::Type::NIL;
    Voltage["UpperThresholdFatal"] = json::Value::Type::NIL;
    Voltage["UpperThresholdCritical"] = json::Value::Type::NIL;
    Voltage["UpperThresholdNonCritical"] = json::Value::Type::NIL;
    Voltage["LowerThresholdNonCritical"] = json::Value::Type::NIL;
    Voltage["LowerThresholdCritical"] = json::Value::Type::NIL;
    Voltage["LowerThresholdFatal"] = json::Value::Type::NIL;

    Voltage["Status"]["HealthRollup"] = json::Value::Type::NIL;
    Voltage["Status"]["Health"] = json::Value::Type::NIL;
    Voltage["Status"]["State"] = json::Value::Type::NIL;

    m_current_status["Voltage"] = std::move(Voltage);

    json::Value TxPwr(json::Value::Type::OBJECT);

    TxPwr["Reading"] = json::Value::Type::NIL;
    TxPwr["UpperThresholdFatal"] = json::Value::Type::NIL;
    TxPwr["UpperThresholdCritical"] = json::Value::Type::NIL;
    TxPwr["UpperThresholdNonCritical"] = json::Value::Type::NIL;
    TxPwr["LowerThresholdNonCritical"] = json::Value::Type::NIL;
    TxPwr["LowerThresholdCritical"] = json::Value::Type::NIL;
    TxPwr["LowerThresholdFatal"] = json::Value::Type::NIL;

    TxPwr["Status"]["HealthRollup"] = json::Value::Type::NIL;
    TxPwr["Status"]["Health"] = json::Value::Type::NIL;
    TxPwr["Status"]["State"] = json::Value::Type::NIL;

    m_current_status["TxPower"] = std::move(TxPwr);

    json::Value RxPwr(json::Value::Type::OBJECT);

    RxPwr["Reading"] = json::Value::Type::NIL;
    RxPwr["UpperThresholdFatal"] = json::Value::Type::NIL;
    RxPwr["UpperThresholdCritical"] = json::Value::Type::NIL;
    RxPwr["UpperThresholdNonCritical"] = json::Value::Type::NIL;
    RxPwr["LowerThresholdNonCritical"] = json::Value::Type::NIL;
    RxPwr["LowerThresholdCritical"] = json::Value::Type::NIL;
    RxPwr["LowerThresholdFatal"] = json::Value::Type::NIL;

    RxPwr["Status"]["HealthRollup"] = json::Value::Type::NIL;
    RxPwr["Status"]["Health"] = json::Value::Type::NIL;
    RxPwr["Status"]["State"] = json::Value::Type::NIL;

    m_current_status["RxPower"] = std::move(RxPwr);
}

// ------------------------------------------------------------------
//  1. Decide by identify ID,
//     03h : SFP+  (10GPON) ,  8472i
//     06h : XFP   (XGSPON) ,  8077i
//     0dh : QSFP+ (40G)    ,  8436i
//     11h : QSFP28(100G)   ,  8438i
//  2. Take customer PN to compare first.
//  3. If not find PN in customer PN, use std instead
// ------------------------------------------------------------------

bool e_oom::get_conf()
{
    try
    {
        //New Transceiver Type need Added here begin//
        int count_8077i = 0;
        int count_8472 = 0;
        int count_8438i = 0;
        int count_8436i = 0;
        //New Transceiver Type need Added here end//

        std::string ME_S_DIR(STD_SEC_PATH);
        struct dirent *entry;

        if (m_support == true)
            return true;

        DIR *dir = opendir(ME_S_DIR.c_str());

        if (dir == NULL)
        {
            gADbg.acc_printf(LEVEL_WARN,"e_oomm::get_conf no such path exist!!");
            return false;
        }

        while ((entry = readdir(dir)) != NULL)
        {
            std::string dot(".");
            std::string ddot("..");
            std::string maps("map");

            std::string tmp_me_name = entry->d_name;

            if ((dot == tmp_me_name) || (ddot == tmp_me_name) || (maps == tmp_me_name))
                continue;

            std::string m_config_file_path = ME_S_DIR + tmp_me_name;

            std::ifstream m_source_files = {};

            m_source_files.open(m_config_file_path);

            if (m_source_files.good())
            {
                Json::Value std_s;
                Json::Reader reader;
                bool isJsonOK = (reader.parse(m_source_files, std_s));
                //New Transceiver Type need Added here//
                if (isJsonOK)
                {
                    //New Transceiver Type need Added here begin//
                    int id = std::stoi(std_s["Identifer"].asString(), 0, 16);
                    if (id == 0x06)
                    {
                        count_8077i++;
                        gADbg.acc_printf(LEVEL_INFO,"0x06 Identifer[%s]",std_s["Identifer"].asString().c_str());
                        gADbg.acc_printf(LEVEL_INFO,"Set id[%d] count_std[%d] TRANS_TYPE_8077I", id, count_8077i);
                        m_8077i[std::make_pair(id, count_8077i)] = std_s;
                    }
                    else if (id == 0x03)
                    {
                        count_8472++;
                        gADbg.acc_printf(LEVEL_INFO,"0x03 Identifer[%s]",std_s["Identifer"].asString().c_str());
                        gADbg.acc_printf(LEVEL_INFO,"Set id[%d] count_std[%d] TRANS_TYPE_8472", id, count_8472);
                        m_8472[std::make_pair(id, count_8472)] = std_s;
                    }
                    else if (id == 0x11)
                    {
                        count_8438i++;
                        gADbg.acc_printf(LEVEL_INFO,"0x0B Identifer[%s]",std_s["Identifer"].asString().c_str());
                        gADbg.acc_printf(LEVEL_INFO,"Set id[%d] count_std[%d] TRANS_TYPE_8438", id, count_8438i);
                        m_8438i[std::make_pair(id, count_8438i)] = std_s;
                    }
                    else if (id == 0x0d)
                    {
                        count_8436i++;
                        gADbg.acc_printf(LEVEL_INFO,"0x0d Identifer[%s]", std_s["Identifer"].asString().c_str());
                        gADbg.acc_printf(LEVEL_INFO,"Set id[%d] count_std[%d] TRANS_TYPE_8436", id, count_8436i);
                        m_8436i[std::make_pair(id, count_8436i)] = std_s;
                    }
                    else
                        gADbg.acc_printf(LEVEL_WARN,"Can't identifiy!!");
                    //New Transceiver Type need Added here end//
                }
                else
                {
                    gADbg.acc_printf(LEVEL_WARN,"Get conf_s ng");
                    closedir(dir);
                    m_source_files.close();
                    return false;
                }
            }
            else
            {
                gADbg.acc_printf(LEVEL_WARN,"Open file NG");
                m_source_files.close();
                closedir(dir);
                return false;
            }
        }
        closedir(dir);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cout << "get_conf() - exception : " << e.what() << std::endl;
        return false;
    }
}

static Switch *g_Switch = NULL;

void Switch::get_board_info()
{
    std::lock_guard<std::mutex> lock{m_data_mutex};
    try
    {
        std::ifstream m_ifile(m_platinfo_conf_path);
        Json::Value content;

        if (m_ifile.good())
        {
            Json::Reader reader;
            bool isJsonOK = (m_ifile != NULL && reader.parse(m_ifile, content));

            if (isJsonOK)
            {
                const Json::Value defValue; //used for default reference
                const Json::Value s = content.get("managers", defValue);

                m_platinfo_name =  s[0]["ecrf_pal"]["plt_type"].asString();
                m_nos_type      =  s[0]["ecrf_pal"]["nos_type"].asString();

                int t_rv = rfpal_fan_num_get();

                if (t_rv != -1)
                {
                    m_fan_max_num = t_rv;
                }
                else
                    m_fan_max_num = s[0]["ecrf_pal"]["fan_max_num"].asInt();

                if (get_fan_num() > 0)
                {
                    int i = 0;

                    for (i = 1; i <= get_fan_num(); i++)
                    {
                        Fan_Info *p = NULL;
                        std::string findex("fan");

                        if (s[0]["ecrf_pal"]["fans"][findex + std::to_string(i)].asString() == "System")
                        {
                            p = new SYS_Fan();
                            p->m_ID = i;
                            p->m_Type = Fan_Info::SYSTEM_Fan;
                        }
                        else if (s[0]["ecrf_pal"]["fans"][findex + std::to_string(i)].asString() == "Psu")
                        {
                            p = new SYS_Fan();
                            p->m_ID = i;
                            p->m_Type = Fan_Info::PSU_Fan;
                        }
                        else
                        {
                            p = new SYS_Fan();
                            p->m_ID = i;
                            p->m_Type = Fan_Info::SYSTEM_Fan;
                        }
                        m_vec_Fan_Info.push_back(p);
                    }
                }

                t_rv = rfpal_port_num_get();

                if (t_rv != -1)
                {
                    m_port_max_num = t_rv;
                }
                else
                    m_port_max_num = s[0]["ecrf_pal"]["port_max_num"].asInt();

                if (m_port_max_num > 0)
                {
                    /*Get mapping json file*/
                    Json::Value mapping_s;
                    gADbg.acc_printf(LEVEL_INFO,"m_platinfo_name[%s]", m_platinfo_name.c_str());

                    std::string mapping_file_path = PORT_MAP_PATH + m_platinfo_name;
                    gADbg.acc_printf(LEVEL_INFO,"mapping_file_path[%s]", mapping_file_path.c_str());

                    std::ifstream map_files(mapping_file_path);

                    bool isJson = (m_eeprom_j_reader.parse(map_files, mapping_s));

                    if (isJson)
                    {
                        gADbg.acc_printf(LEVEL_INFO,"Get eeprom mapping file ok!!");
                    }
                    else
                        gADbg.acc_printf(LEVEL_WARN,"Get eeprom mapping file error!!");

                    int i = 0;
                    for (i = 1; i <= m_port_max_num; i++)
                    {
                        Port_Info *p = NULL;
                        p = new Port_Info();
                        p->m_ID = i;

                        std::string findex("port");
                        if (s[0]["ecrf_pal"]["ports"][findex + std::to_string(i)].asString() == "Ether")
                            p->m_Type = Port_Info::Ether_Port;
                        else if (s[0]["ecrf_pal"]["ports"][findex + std::to_string(i)].asString() == "SFP")
                        {
                            p->m_Type = Port_Info::SFP_Port;
                            /*Set SFP eeprom mapping path*/
                            gADbg.acc_printf(LEVEL_INFO,"set_eeprom_path port [%d] [%s]!! ", i, mapping_s[std::to_string(i)].asString().c_str());
                            p->set_eeprom_path(mapping_s[std::to_string(i)].asString());
                        }
                        else if (s[0]["ecrf_pal"]["ports"][findex + std::to_string(i)].asString() == "QSFP")
                        {
                            p->m_Type = Port_Info::QSFP_Port;
                            /*Set SFP eeprom mapping path*/
                            gADbg.acc_printf(LEVEL_INFO,"set_eeprom_path port [%d] [%s]!! ", i, mapping_s[std::to_string(i)].asString().c_str());
                            p->set_eeprom_path(mapping_s[std::to_string(i)].asString());
                        }
                        else if (s[0]["ecrf_pal"]["ports"][findex + std::to_string(i)].asString() == "PON")
                        {
                            p->m_Type = Port_Info::PON_Port;
                            /*Set PON eeprom mapping path*/
                            gADbg.acc_printf(LEVEL_INFO,"set_eeprom_path port [%d] [%s]!! ", i, mapping_s[std::to_string(i)].asString().c_str());
                            p->set_eeprom_path(mapping_s[std::to_string(i)].asString());
                        }
                        else
                            p->m_Type = Port_Info::Ether_Port;

                        m_vec_Port_Info.push_back(p);
                    }
                }

                t_rv = rfpal_thermal_num_get();

                if (t_rv != -1)
                {
                    m_thermal_sen_max_num = t_rv;
                }
                else
                    m_thermal_sen_max_num = s[0]["ecrf_pal"]["thermal_sen_max_num"].asInt();

                if (get_thermal_num() > 0)
                {
                    int i = 0;
                    for (i = 1; i <= get_thermal_num(); i++)
                    {
                        Thermal_Info *p = NULL;

                        std::string findex("thermal");

                        if (s[0]["ecrf_pal"]["thermals"][findex + std::to_string(i)].asString() == "CPU")
                        {
                            p = new CPU_Thermal();
                            p->m_ID = i;
                            p->m_Type = Thermal_Info::CPU_Sensor;
                            m_vec_Thermal_Info.push_back(p);
                        }
                        else if (s[0]["ecrf_pal"]["thermals"][findex + std::to_string(i)].asString() == "System")
                        {
                            p = new SYS_Thermal();
                            p->m_ID = i;
                            p->m_Type = Thermal_Info::SYSTEM_Sensor;
                            m_vec_Thermal_Info.push_back(p);
                        }
                        else if (s[0]["ecrf_pal"]["thermals"][findex + std::to_string(i)].asString() == "Psu")
                        {
                            p = new PSU_Thermal();
                            p->m_ID = i;
                            p->m_Type = Thermal_Info::PSU_Sensor;
                            m_vec_Thermal_Info.push_back(p);
                        }
                        else
                            gADbg.acc_printf(LEVEL_WARN,"THERMLA create ERROR!!");
                    }
                }

                t_rv = rfpal_psu_num_get();

                if (t_rv != -1)
                {
                    m_psu_max_num = t_rv;
                }
                else
                    m_psu_max_num = s[0]["ecrf_pal"]["psu_max_num"].asInt();

                if (get_psu_num() > 0)
                {
                    int i = 0;
                    for (i = 1; i <= get_psu_num(); i++)
                    {
                        Psu_Info *p = NULL;
                        p = new Psu_Info();
                        p->m_ID = i;

                        std::string findex("psu");

                        if (s[0]["ecrf_pal"]["psus"][findex + std::to_string(i)].asString() == "System")
                            p->m_Type = Psu_Info::SYSTEM;
                        else
                            p->m_Type = Psu_Info::SYSTEM;

                        m_vec_Psu_Info.push_back(p);
                    }
                }

                m_cpu_stepping = s[0]["ecrf_pal"]["cpu_stepping"].asInt();
                m_cpu_max_speed = s[0]["ecrf_pal"]["cpu_max_speed"].asInt();
                m_cpu_total_core = s[0]["ecrf_pal"]["cpu_total_core"].asInt();
                m_cpu_total_thread = s[0]["ecrf_pal"]["cpu_total_thread"].asInt();
                m_memory_total_count = s[0]["ecrf_pal"]["memory_total_count"].asInt();
                m_drv_total_count = s[0]["ecrf_pal"]["drv_total_count"].asInt();
                m_cpu_manu = s[0]["ecrf_pal"]["cpu_manu"].asString();
                m_cpu_vid = s[0]["ecrf_pal"]["cpu_vid"].asString();
                m_cpu_model_name = s[0]["ecrf_pal"]["cpu_model_name"].asString();

                std::string log_msg;
                /**/
                m_ifile.close();
            }
            else
                gADbg.acc_printf(LEVEL_WARN,"get_board_info error");
        }
        else
            gADbg.acc_printf(LEVEL_WARN,"[%s] file not exist", m_platinfo_conf_path.c_str());

    }
    catch (const std::exception &ex)
    {
        std::cout << "get_board_info() - exception : " << ex.what() << std::endl;
        return;
    }
}

void Switch::get_port_present_info()
{
    try
    {
        std::lock_guard<std::mutex> lock{m_data_mutex};

        for (int ii = 1; ii <= m_port_max_num; ii++)
        {
            for (vector<Port_Info *>::iterator pObj = m_vec_Port_Info.begin(); pObj != m_vec_Port_Info.end(); ++pObj)
            {
                if ((*pObj)->m_ID == ii)
                {
                    if ((*pObj)->m_Type == Port_Info::Ether_Port)
                    {
                        set_port_present(ii, true);
                        (*pObj)->set_info(ii, Port_Info::Ether_Port, 1, true); // Type 1: XFP // Type 2 : ETHER
                    }
                    else
                    {
                        int rv = rfpal_port_trans_is_present(ii);
                        if (rv == 1)
                        {
                            set_port_present(ii, true);
                            (*pObj)->set_info(ii, (*pObj)->m_Type, 1, true);
                        }
                        else
                        {
                            set_port_present(ii, false);
                            (*pObj)->set_info(ii, (*pObj)->m_Type, 0, false);
                            (*pObj)->clean_trans_data();
                        }
                    }
                }
            }
        }
        return;
    }
    catch (const std::exception &e)
    {
        std::cout << "get_port_present_info() - exception : " << e.what() << std::endl;
        return;
    }
}

void Switch::get_port_oom_info()
{
    try
    {
        std::lock_guard<std::mutex> lock{m_data_mutex};

        for (int ii = 1; ii <= m_port_max_num; ii++)
        {
            for (vector<Port_Info *>::iterator pObj = m_vec_Port_Info.begin(); pObj != m_vec_Port_Info.end(); ++pObj)
            {
                if ((*pObj)->m_ID == ii)
                {
                    if ((*pObj)->m_Type == Port_Info::Ether_Port)
                    {
                        set_port_present(ii, true);
                        (*pObj)->set_info(ii, Port_Info::Ether_Port, 1, true);
                    }
                    else
                    {
                        int rv = rfpal_port_trans_is_present(ii);
                        if (rv)
                        {

                            if (!(*pObj)->get_eeprom_raw(ii))
                            {
                                gADbg.acc_printf(LEVEL_WARN,"catch eeprom 8472 data error");
                            }
                        }
                        else
                        {
                            (*pObj)->clean_trans_data();
                        }
                    }
                }
            }
        }
        return;
    }
    catch (const std::exception &e)
    {
        std::cout << "get_port_oom_info() - exception : " << e.what() << std::endl;
        return;
    }
}

void Switch::get_thermal_info()
{
    try
    {
        for (int ii = 1; ii <= get_thermal_num(); ii++)
        {
            int rv;
            rfpal_thermal_info fv;
            memset(&fv, 0, sizeof(rfpal_thermal_info));
            rv = rfpal_thermal_info_get(ii, &fv);

            for (vector<Thermal_Info *>::iterator pObj = m_vec_Thermal_Info.begin(); pObj != m_vec_Thermal_Info.end(); ++pObj)
            {
                if ((*pObj)->m_ID == ii)
                {
                    if (rv < 0 && (*pObj)->m_ID == ii)
                    {
                        set_thermal_present(ii, false);
                        (*pObj)->set_info(ii, 0, 0, 0, 0, 0, false);
                        gADbg.acc_printf(LEVEL_WARN,"get_thermal_info [%d] error", ii);
                    }
                    else
                    {
                        if (fv.status & 1)
                        {
                            set_thermal_present(ii, true);
                            //gADbg.acc_printf(LEVEL_INFO, "get_thermal_info reading temp[%d] warning upper[%d] warning lower[%d] error[%d] shutdown[%d]",
                            //fv.mcelsius, fv.m_threshold.warning_upper, fv.m_threshold.warning_lower, fv.m_threshold.error, fv.m_threshold.shutdown);
                            (*pObj)->set_info(ii, fv.mcelsius, fv.m_threshold.warning_upper, fv.m_threshold.warning_lower, fv.m_threshold.error, fv.m_threshold.shutdown, true);
                        }
                        else
                        {
                            set_thermal_present(ii, false);
                            (*pObj)->set_info(ii, 0, fv.m_threshold.warning_upper, fv.m_threshold.warning_lower, fv.m_threshold.error, fv.m_threshold.shutdown, false);
                        }
                    }
                }
            }
        }
        return;
    }
    catch (const std::exception &e)
    {
        std::cout << "get_thermal_info() - exception : " << e.what() << std::endl;
        return;
    }
}

int Switch::get_port_tx_disable_status(int port)
{
    try
    {
        std::lock_guard<std::mutex> lock{m_data_mutex};

        for (int ii = 1; ii <= m_port_max_num; ii++)
        {
            for (vector<Port_Info *>::iterator pObj = m_vec_Port_Info.begin(); pObj != m_vec_Port_Info.end(); ++pObj)
            {
                if ((*pObj)->m_ID == port)
                {
                    std::string sysfs_path = (*pObj)->get_sysfile_path();
                    return (*pObj)->get_tx_disable_status(sysfs_path + m_sys_tx_name);
                }
            }
        }
        return -1;
    }
    catch (const std::exception &e)
    {
        std::cout << "get_port_tx_disable_status() - exception : " << e.what() << std::endl;
        return -1;
    }
}

void Switch::set_port_tx_disable_status(int port, bool tx_status)
{
    printf("Switch::set_port_tx_disable_status port[%d] tx_status[%d]\r\n", port, tx_status);
    try
    {
        std::lock_guard<std::mutex> lock{m_data_mutex};

        for (int ii = 1; ii <= m_port_max_num; ii++)
        {
            for (vector<Port_Info *>::iterator pObj = m_vec_Port_Info.begin(); pObj != m_vec_Port_Info.end(); ++pObj)
            {
                if ((*pObj)->m_ID == port)
                {
                    std::string sysfs_path = (*pObj)->get_sysfile_path();
                    (*pObj)->set_tx_disable(tx_status, sysfs_path + m_sys_tx_name);
                    return;
                }
            }
        }
        return;
    }
    catch (const std::exception &e)
    {
        std::cout << "set_port_tx_disable_status() - exception : " << e.what() << std::endl;
        return;
    }
}

void Psu_Info::set_info(int ID, std::string Model, std::string SN, int Vin, int Vout, int Iin, int Iout, int Pin, int Pout, ecrf_pal_psu_type_t type, bool present)
{
    try
    {
        auto & Entry = RFLogEntry::get_instance();
        m_ID = ID;
        if (!Model.empty() && Model[Model.size() - 1] == '\n')
            Model.erase(Model.size() - 1);
        m_Model = Model;
        m_SN = SN;
        m_Vin = Vin;
        m_Vout = Vout;
        m_Iin = Iin;
        m_Iout = Iout;
        m_Pin = Pin;
        m_Pout = Pout;
        m_Present = present;
        m_Psu_Type = type;

        if (m_Type == SYSTEM)
        {
            if (present && (Pout == 0))
            {
                //PSU don't plugin power cord
                m_Status_Health = "OK";
                m_Status_State = "UnavailableOffline";

                std::string event("Event");
                std::string servrity("Warning");
                std::string sensor_type("Power Supply / Converter");
                std::string message("PSU UnavailableOffline");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message, ID);
            }
            else if (present && (Pout > 0))
            {
                //PSU working normal
                m_Status_Health = "OK";
                m_Status_State = "Enabled";
            }
            else if (!present)
            {
                //PSU un-plug //
                m_Status_Health = "Warning";
                m_Status_State = "Absent";

                std::string event("Event");
                std::string servrity("Warning");
                std::string sensor_type("Power Supply / Converter");
                std::string message("PSU absent");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message, ID);
            }
            else
            {
                m_Status_Health = "Warning";
                m_Status_State = "Absent";

                std::string event("Event");
                std::string servrity("Warning");
                std::string sensor_type("Power Supply / Converter");
                std::string message("PSU absent");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message, ID);
            }
        }

        return;
    }
    catch (const std::exception &e)
    {
        std::cout << "Psu_Info set_info() - exception : " << e.what() << std::endl;
        return;
    }
}

void Fan_Info::set_info(int ID, std::string Model, std::string SN, int RPM, int Per, bool present)
{
    try
    {
        auto & Entry = RFLogEntry::get_instance();
        m_ID = ID;
        m_Model = Model;
        m_SN = SN;
        m_RPM = RPM;
        m_Per = Per;
        m_Present = present;

        if (m_Type == SYSTEM_Fan)
        {
            if (present && (RPM == 0))
            {
                //Fan plug in but not SPIN
                m_Status_Health = "Warning";
                m_Status_State = "Enabled";

                std::string orig_of_condition ="/redfish/v1/Chassis/1/Thermal/Fans/" + std::to_string(ID);
                std::string message_event = std::string("The resource property ") + orig_of_condition + " has detected errors of type " + std::string("[RPM is 0].");

                std::string event("Event");
                std::string servrity("Warning");
                std::string sensor_type("Fan");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message_event, ID);

               if (m_fan_alert == COUNT_TIMES)
                { 
                    EventMsgContent_T message_body;
                    message_body.message_content = message_event;
                    message_body.message_id = "ResourceEvent.1.0.3.ResourceErrorsDetected";
                    message_body.origin_of_condition = orig_of_condition;
                    m_Event_Resource_Alert.push_back(message_body);
                    m_fan_alert = 0;
                }
                m_fan_alert++;
            }
            else if (present && (RPM > 0))
            {
                //Fan working normal
                m_Status_Health = "OK";
                m_Status_State = "Enabled";
                m_fan_alert = COUNT_TIMES;
            }
            else if (!present)
            {
                //FAN un-plug //
                m_Status_Health = "Warning";
                m_Status_State = "Absent";

                std::string event("Event");
                std::string servrity("Warning");
                std::string sensor_type("Fan");
                std::string message("System fan absent");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message, ID);
            }
            else
            {
                m_Status_Health = "Warning";
                m_Status_State = "Absent";
            }
        }
        else if (m_Type == PSU_Fan)
        {
            if (present && (RPM == 0))
            {
                //Fan plug in but not SPIN
                m_Status_Health = "Warning";
                m_Status_State = "Enabled";

                std::string event("Event");
                std::string servrity("Warning");
                std::string sensor_type("Fan");
                std::string message("PSU plug in but not no power core plug in");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message, ID);
            }
            else if (present && (RPM > 0 || RPM == ECRF_PAL_CONST_NOT_SUPPORTED)) //PSU present but doesn't provide rpm info. //
            {
                //PSU working normal
                m_Status_Health = "OK";
                m_Status_State = "Enabled";
            }
            else if (!present)
            {
                //PSU un-plug //
                m_Status_Health = "Warning";
                m_Status_State = "Absent";

                std::string event("Event");
                std::string servrity("Warning");
                std::string sensor_type("Fan");
                std::string message("PSU fan absent");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message, ID);
            }
            else
            {
                m_Status_Health = "Warning";
                m_Status_State = "Absent";

                std::string event("Event");
                std::string servrity("Warning");
                std::string sensor_type("Fan");
                std::string message("PSU fan absent");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message, ID);
            }
        }
        else
        {
            m_Status_Health = "Warning";
            m_Status_State = "Absent";
        }

        return;
    }
    catch (const std::exception &e)
    {
        std::cout << "Fan_Info set_info() - exception : " << e.what() << std::endl;
        return;
    }
}

void Fan_Info::get_fan_speed_threshold_value()
{
    rfpal_thresholds in_threshold = {0};
    int rf_r = rfpal_fan_speed_thresholds_get(m_ID, &in_threshold);
    if (rf_r == ECRF_PAL_STATUS_OK)
    {
        m_Warning_upper = in_threshold.warning_upper; 
        m_Error = in_threshold.error;
        m_Shutdown = in_threshold.shutdown;
    }
    else
    {
        m_Warning_upper= 0;
        m_Error = 0;
        m_Shutdown = 0;
    }
}

void Thermal_Info::get_thermal_threshold_value()
{
    rfpal_thresholds in_threshold = {0};
    int rf_r = rfpal_thermal_thresholds_get(m_ID, &in_threshold);
    if (rf_r == ECRF_PAL_STATUS_OK)
    {
        m_Warning_lower = in_threshold.warning_lower; 
        m_Warning_upper = in_threshold.warning_upper; 
        m_Error = in_threshold.error;
        m_Shutdown = in_threshold.shutdown;
    }
    else
    {
        m_Warning_lower = 0; 
        m_Warning_upper = 0;
        m_Error = 0;
        m_Shutdown = 0;
    }
}

void CPU_Thermal::set_thermal_threshold_value(int Warning_Upper, int Warning_Lower, int Error, int Shutdown)
{
    m_Warning_upper = Warning_Upper;
    m_Warning_lower = Warning_Lower;
    auto &secrf_pal = ecrf_pal_helper::Switch::get_instance();
    if (secrf_pal.get_nos_type() == "sonic")
    {
        m_Error = Error;
        m_Shutdown = Shutdown;
    }
    else
    {
        //Todo::Wait new ONLP driver value;//
    m_Error = 93000;
    m_Shutdown = 105000;
}
    return;
}

void SYS_Thermal::set_thermal_threshold_value(int Warning_Upper, int Warning_Lower, int Error, int Shutdown)
{
    m_Warning_lower = Warning_Lower;
    m_Warning_upper = Warning_Upper;
        m_Error = Error;
        m_Shutdown = Shutdown;
    }

void PSU_Thermal::set_thermal_threshold_value(int Warning_Upper, int Warning_Lower, int Error, int Shutdown)
    {
    m_Warning_lower = Warning_Lower;
    m_Warning_upper = Warning_Upper;
        m_Error = Error;
        m_Shutdown = Shutdown;
    }

void Thermal_Info::set_info(int ID, double Current_Temperature, double Warning_Upper, double Warning_Lower, double Error, double Shutdown, bool present)
{
    try
    {
        auto &secrf_pal = ecrf_pal_helper::Switch::get_instance();

        auto &Entry = RFLogEntry::get_instance();
        m_ID = ID;
        m_Current_Temperature = Current_Temperature;
        m_Present = present;
        set_thermal_threshold_value(Warning_Upper,Warning_Lower, Error, Shutdown);

        if (secrf_pal.get_nos_type() == "sonic")
        {
            //SONiC do not support thermal Alert because driver doesn't support threshold value.//
            if(present)
            {
                if (Current_Temperature > 0 || Current_Temperature == ECRF_PAL_CONST_NOT_SUPPORTED) //PSU exist but doesn't have thermal info.
                {
                    m_Status_Health = "OK";
                    m_Status_State = "Enabled";
                }
                else
                {
                    m_Status_Health = "OK";
                    m_Status_State = "UnavailableOffline";
                }
            }
            else
            {
                m_Status_Health = "Warning";
                m_Status_State = "Absent";
            }
            return;
        }

        /*
   Area : 1 
   "Warning" "Enabled"
   
   Warning-lower th   -----
   
   Area : 2
   "OK" "Enabled"
   
   Warning-upper th   -----
   
   Area : 3
   "OK" "Warning"
   
   Error-th       -----
   
   Area : 4
   "OK" "Warning"
   
   Shutdown-th ----
   
   Area : 5
   
   */
        if ((m_Type == CPU_Sensor || m_Type == SYSTEM_Sensor || m_Type == PSU_Sensor)  && present)
        {
            //gADbg.acc_printf(LEVEL_INFO, "Thermal_Info[%d]::set_info current tmp[%d] m_Warning_upper[%d] m_Warning_lower[%d] m_Error[%d] m_Shutdown[%d]" ,ID, m_Current_Temperature, m_Warning_upper, m_Warning_lower, m_Error, m_Shutdown);
            if (m_Current_Temperature < m_Warning_lower)
            {
                //     Area : 1
                m_Status_Health = "Warning";
                m_Status_State = "Enabled";
                gADbg.acc_printf(LEVEL_WARN,"Area 1");
                std::string orig_of_condition = "/redfish/v1/Chassis/1/Thermal/Temperatures/" + std::to_string(ID);
                std::string message_event = "The resource property " + orig_of_condition + " has exceeded its warning threshold of value " + std::to_string(m_Warning_lower/1000) + ".";

                std::string event("Event");
                std::string servrity("Warning");
                std::string sensor_type("Temperature");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message_event, ID);

                if (m_thermal_alert == COUNT_TIMES)
                {
                    Json::Value oem;
                    gADbg.acc_printf(LEVEL_WARN, "Area 1 send event!!");
                    EventMsgContent_T message_body;
                    message_body.message_content = message_event;
                    message_body.message_id = "ResourceEvent.1.0.3.ResourceWarningThresholdExceeded";
                    message_body.origin_of_condition = orig_of_condition;
                    oem["CurrentValue"] = m_Current_Temperature/1000;
                    oem["State"] = "ExceededLowerThresholdNonCritical";
                    oem["ThresholdValue"] = m_Warning_lower/1000; 
                    message_body.oem = oem;
                    m_Event_Resource_Alert.push_back(message_body);
                    m_thermal_alert = 0;
                }
                m_thermal_alert ++;
            }
            else if (((m_Current_Temperature >= m_Warning_upper) && (m_Current_Temperature < m_Error)))
            {
                //     Area : 3
                m_Status_Health = "Warning";
                m_Status_State = "Enabled";
                gADbg.acc_printf(LEVEL_WARN,"Area 3");
                std::string orig_of_condition = "/redfish/v1/Chassis/1/Thermal/Temperatures/" + std::to_string(ID);
                std::string message_event = "The resource property " + orig_of_condition + " has exceeded its warning threshold of value " + std::to_string(m_Warning_upper/1000) + ".";

                std::string event("Event");
                std::string servrity("Warning");
                std::string sensor_type("Temperature");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message_event, ID);

                if (m_thermal_alert == COUNT_TIMES)
                {
                    Json::Value oem;
                    gADbg.acc_printf(LEVEL_WARN, "Area 3 send event!");
                    EventMsgContent_T message_body;
                    message_body.message_content = message_event;
                    message_body.message_id = "ResourceEvent.1.0.3.ResourceWarningThresholdExceeded";
                    message_body.origin_of_condition = orig_of_condition;
                    oem["CurrentValue"] = m_Current_Temperature/1000;
                    oem["State"] = "ExceededUpperThresholdNonCritical";
                    oem["ThresholdValue"] = m_Warning_upper/1000; 
                    message_body.oem = oem;
                    m_Event_Resource_Alert.push_back(message_body);
                    m_thermal_alert = 0;
                }
                m_thermal_alert ++;
            }
            else if (((m_Current_Temperature >= m_Error)) && (m_Current_Temperature < m_Shutdown))
            {
                //     Area : 4
                gADbg.acc_printf(LEVEL_WARN, "Area 4");
                m_Status_Health = "Warning";
                m_Status_State = "Enabled";

                std::string orig_of_condition = "/redfish/v1/Chassis/1/Thermal/Temperatures/" + std::to_string(ID);
                std::string message_event = "The resource property " + orig_of_condition + " has exceeded its error threshold of value " + std::to_string(m_Error/1000) + ".";

                std::string event("Event");
                std::string servrity("Warning");
                std::string sensor_type("Temperature");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message_event, ID);

                if (m_thermal_alert == COUNT_TIMES)
                {
                    Json::Value oem;
                    gADbg.acc_printf(LEVEL_WARN, "Area 4 send event!");
                    EventMsgContent_T message_body;
                    message_body.message_content = message_event;
                    message_body.message_id = "ResourceEvent.1.0.3.ResourceErrorThresholdExceeded";
                    message_body.origin_of_condition = orig_of_condition;
                    oem["CurrentValue"] = m_Current_Temperature/1000;
                    oem["State"] = "ExceededUpperThresholdCritical";
                    oem["ThresholdValue"] = m_Error/1000; 
                    message_body.oem = oem;
                    m_Event_Resource_Alert.push_back(message_body);
                    m_thermal_alert = 0;
                }
                m_thermal_alert ++;
            }
            else if ((m_Current_Temperature >= m_Shutdown))
            {
                //     Area : 5
                gADbg.acc_printf(LEVEL_WARN, "Area 5");
                m_Status_Health = "Critical";
                m_Status_State = "Enabled";
                std::string orig_of_condition = "/redfish/v1/Chassis/1/Thermal/Temperatures/" + std::to_string(ID);
                std::string message_event = "The resource property " + orig_of_condition + " has exceeded its error threshold of value " + std::to_string(m_Shutdown / 1000) + ".";

                std::string event("Event");
                std::string servrity("Warning");
                std::string sensor_type("Temperature");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message_event, ID);

                if (m_thermal_alert == COUNT_TIMES)
                {
                    Json::Value oem;
                    gADbg.acc_printf(LEVEL_WARN, "Area 5 send event!");
                    EventMsgContent_T message_body;
                    message_body.message_content = message_event;
                    message_body.origin_of_condition = orig_of_condition;
                    message_body.message_id = "ResourceEvent.1.0.3.ResourceErrorThresholdExceeded";
                    oem["CurrentValue"] = m_Current_Temperature/1000;
                    oem["State"] = "ExceededUpperThresholdFatal";
                    oem["ThresholdValue"] = m_Shutdown/1000;
                    message_body.oem = oem;
                    m_Event_Resource_Alert.push_back(message_body);
                    m_thermal_alert = 0;
                }
                m_thermal_alert ++;
            }
            else
            {
                //     Area : 2
                m_Status_Health = "OK";
                m_Status_State = "Enabled";
                m_thermal_alert = COUNT_TIMES;
            }
        }
        else
        {
            m_Status_Health = "Warning";
            m_Status_State = "Absent";
        }
        return;
    }
    catch (const std::exception &e)
    {
        std::cout << "Thermal_Info set_info() - exception : " << e.what() << std::endl;
        return;
    }
}

void Port_Info::set_info(int ID, int Type, int Present_Status, bool present)
{
    m_ID = ID;
    m_Type = Type;
    m_Present_Status = Present_Status;
    m_Present = present;
}

void Port_Info::set_tx_disable(bool status, std::string in_tx_sys_path)
{
#if 0
    std::string tx_sys_path = in_tx_sys_path; 
    std::ofstream os;
    printf("set_tx_disable[%s] status[%d]\r\n", tx_sys_path.c_str(), status);
    try
    {
        if (os)
        {
            os.open(tx_sys_path);
            if (status == false)
                os << 1;
            else
                os << 0;
            os.close();
        }
        else
        {
            std::cout << "sysfile open error" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "set tx exception : " << e.what() << std::endl;
    }
#else
    UNUSED(in_tx_sys_path);
    UNUSED(status);
    try
    {
        int value;
        if (status == false)
            value = 0;
        else
            value = 1;
        std::cout << "Port Id is " << m_ID << std::endl;
        rfpal_sfp_tx_disable_set(m_ID, value);
        std::cout << "TX_Disable set " << value << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "set tx exception : " << e.what() << std::endl;
    }
#endif
}

int Port_Info::get_tx_disable_status(std::string in_tx_sys_path)
{
#if 0
    std::string tx_sys_path = in_tx_sys_path;
    std::ifstream is;
    char stor[2];
    is.rdbuf()->pubsetbuf(0, 0); // do not buffer //
    try
    {
        if (is)
        {
            is.open(tx_sys_path);
            is.read((char *)stor, sizeof(stor));
            if (is.is_open() && atoi(stor) == 0)
            {
                is.close();
                return 0;
            }
            else
            {
                is.close();
                return 1;
            }
        }
        else
        {
            std::cout << "sysfile open error" << std::endl;
            return -1;
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "get tx status exception : " << e.what() << std::endl;
        return -1;
    }
#else
    UNUSED(in_tx_sys_path);
    try
    {
        int value;
        std::cout << "Port Id is " << m_ID << std::endl;
        value = rfpal_sfp_tx_disable_get(m_ID);
        std::cout << "TX_Disable get " << value << std::endl;
        return value;
    }
    catch (const std::exception &e)
    {
        std::cout << "get tx status exception : " << e.what() << std::endl;
        return -1;
    }
#endif
}

void Switch::get_psu_info()
{
    try
    {
        int rv;
        rfpal_psu_info fv;
        memset(&fv, 0, sizeof(rfpal_psu_info));

        for (int ii = 1; ii <= get_psu_num(); ii++)
        {
            rv = rfpal_psu_info_get(ii, &fv);
            for (vector<Psu_Info *>::iterator pObj = m_vec_Psu_Info.begin(); pObj != m_vec_Psu_Info.end(); ++pObj)
            {
                if (rv < 0 && (*pObj)->m_ID == ii)
                {
                    set_psu_present(ii, false);
                    gADbg.acc_printf(LEVEL_WARN,"get_psu_info [%d] error", ii);
                }
                else
                {
                    if ((*pObj)->m_ID == ii)
                    {
                        if (fv.status & 1)
                        {
                            Psu_Info::ecrf_pal_psu_type_t psu_type = ecrf_pal_helper::Psu_Info::ECRF_PAL_PSU_TYPE_INVALID;

                            if (fv.psu_type == RFPAL_PSU_TYPE_AC)
                                psu_type = ecrf_pal_helper::Psu_Info::ECRF_PAL_PSU_TYPE_AC;
                            else if (fv.psu_type == RFPAL_PSU_TYPE_DC12)
                                psu_type = ecrf_pal_helper::Psu_Info::ECRF_PAL_PSU_TYPE_DC12;
                            else if (fv.psu_type == RFPAL_PSU_TYPE_DC48)
                                psu_type = ecrf_pal_helper::Psu_Info::ECRF_PAL_PSU_TYPE_DC48;

                            set_psu_present(ii, true);
                            (*pObj)->set_info(ii, fv.model[0] ? fv.model : "NULL", fv.serial[0] ? fv.serial : "NULL", fv.mvin, fv.mvout, fv.miin, fv.miout, fv.mpin, fv.mpout, psu_type, true);
                        }
                        else
                        {
                            set_psu_present(ii, false);
                            (*pObj)->set_info(ii, "NULL", "NULL", 0, 0, 0, 0, 0, 0, ecrf_pal_helper::Psu_Info::ECRF_PAL_PSU_TYPE_INVALID, false);
                        }
                    }
                }
            }
        }
        return;
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch get_psu_info() - exception : " << e.what() << std::endl;
        return;
    }
}

int Switch::get_psu_info_by_(int psuid, Psu_Content id)
{
    try
    {
        for (int ii = 1; ii <= get_psu_num(); ii++)
        {
            for (vector<Psu_Info *>::iterator pObj = m_vec_Psu_Info.begin(); pObj != m_vec_Psu_Info.end(); ++pObj)
            {
                if ((*pObj)->m_ID == psuid)
                {
                    switch (id)
                    {
                    case Vin:
                        return (*pObj)->m_Vin;
                        break;
                    case Vout:
                        return (*pObj)->m_Vout;
                        break;
                    case Iin:
                        return (*pObj)->m_Iin;
                        break;
                    case Iout:
                        return (*pObj)->m_Iout;
                        break;
                    case Pin:
                        return (*pObj)->m_Pin;
                        break;
                    case Pout:
                        return (*pObj)->m_Pout;
                        break;
                    case Psu_type:
                        return (*pObj)->m_Psu_Type;
                        break;
                    case Psu_Present:
                        return (*pObj)->m_Present;
                        break;
                    default:
                        return 0;
                        break;
                    }
                }
            }
        }
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch get_psu_info_by_() - exception : " << e.what() << std::endl;
        return 0;
    }
}

std::string Switch::get_psu_info_by_(int psuid, std::string type)
{
    try
    {
        for (int ii = 1; ii <= get_psu_num(); ii++)
        {
            for (vector<Psu_Info *>::iterator pObj = m_vec_Psu_Info.begin(); pObj != m_vec_Psu_Info.end(); ++pObj)
            {
                if ((*pObj)->m_ID == psuid)
                {
                    if (type == "Status_Health")
                        return (*pObj)->m_Status_Health;
                    else if (type == "Status_State")
                        return (*pObj)->m_Status_State;
                    else if (type == "Model")
                        return (*pObj)->m_Model;
                    else if (type == "SN")
                        return (*pObj)->m_SN;
                    else
                        return "na";
                }
            }
        }

        return "na";
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch string get_psu_info_by_() - exception : " << e.what() << std::endl;
        return "na";
    }
}

std::string Switch::get_thermal_info_by_(int thermalid, std::string type)
{
    try
    {
        for (int ii = 1; ii <= get_thermal_num(); ii++)
        {
            for (vector<Thermal_Info *>::iterator pObj = m_vec_Thermal_Info.begin(); pObj != m_vec_Thermal_Info.end(); ++pObj)
            {
                if ((*pObj)->m_ID == thermalid)
                {
                    if (type == "Status_Health")
                        return (*pObj)->m_Status_Health;
                    else if (type == "Status_State")
                        return (*pObj)->m_Status_State;
                    else
                        return "na";
                }
            }
        }

        return "na";
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch string get_thermal_info_by_() - exception : " << e.what() << std::endl;
        return "na";
    }
}

double Switch::get_thermal_info_by_(int thermalid, Thermal_Content id)
{
    try
    {
        for (int ii = 1; ii <= get_thermal_num(); ii++)
        {
            for (vector<Thermal_Info *>::iterator pObj = m_vec_Thermal_Info.begin(); pObj != m_vec_Thermal_Info.end(); ++pObj)
            {
                if ((*pObj)->m_ID == thermalid)
                {
                    switch (id)
                    {
                    case Current_Temperature:
                        return (*pObj)->m_Current_Temperature;
                        break;
                    case Warning_uppper:
                        return (*pObj)->m_Warning_upper;
                        break;
                    case Warning_lower:
                        return (*pObj)->m_Warning_lower;
                        break;
                    case Error:
                        return (*pObj)->m_Error;
                        break;
                    case Shutdown:
                        return (*pObj)->m_Shutdown;
                        break;
                    case Thermal_Type:
                        return (*pObj)->m_Type;
                        break;
                    case Thermal_Present:
                        return (*pObj)->m_Present;
                        break;
                    default:
                        return 0;
                        break;
                    }
                }
            }
        }
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch  get_thermal_info_by_() - exception : " << e.what() << std::endl;
        return 0;
    }
}

int Switch::get_port_info_by_(int portid, Port_Content id)
{
    try
    {
        std::lock_guard<std::mutex> lock{m_data_mutex};

        for (int ii = 1; ii <= m_port_max_num; ii++)
        {
            for (vector<Port_Info *>::iterator pObj = m_vec_Port_Info.begin(); pObj != m_vec_Port_Info.end(); ++pObj)
            {
                if ((*pObj)->m_ID == portid)
                {
                    switch (id)
                    {
                    case Port_Present:
                        if ((*pObj)->m_Type == Port_Info::Ether_Port)
                            return true;
                        else
                            return (*pObj)->m_Present_Status;
                        break;
                    case Port_Type:
                        return (*pObj)->m_Type;
                        break;
                    default:
                        return 0;
                        break;
                    }
                }
            }
        }
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch get_port_info_by_() - exception : " << e.what() << std::endl;
        return 0;
    }
}

json::Value Switch::get_port_trans_info_by_(int portid)
{
    try
    {
        std::lock_guard<std::mutex> lock{m_data_mutex};

        for (int ii = 1; ii <= m_port_max_num; ii++)
        {
            for (vector<Port_Info *>::iterator pObj = m_vec_Port_Info.begin(); pObj != m_vec_Port_Info.end(); ++pObj)
            {
                if ((*pObj)->m_ID == portid)
                {
                    return (*pObj)->get_trans_status();
                }
            }
        }
        return json::Value::Type::NIL;
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch get_port_trans_info_by_() - exception : " << e.what() << std::endl;
        return json::Value::Type::NIL;
    }
}

void Switch::update_transceivers_oom_event()
{
    try
    {
        std::lock_guard<std::mutex> lock{m_data_mutex};

        for (int ii = 1; ii <= m_port_max_num; ii++)
        {
            for (vector<Port_Info *>::iterator pObj = m_vec_Port_Info.begin(); pObj != m_vec_Port_Info.end(); ++pObj)
            {
                (*pObj)->get_trans_status(); //todo //
            }
        }
        return;
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch update_transceivers_oom_event() - exception : " << e.what() << std::endl;
        return;
    }
}

void Switch::update_port_present_event()
{
    try
    {
        auto &Entry = RFLogEntry::get_instance();

        for (int id = 0; id < m_port_max_num; id++)
        {
            std::string orig_of_condition = "/redfish/v1/EthernetSwitches/1/Ports/" + std::to_string(id + 1);

            if (id < 64)
            {
                // Check port present //
                unsigned long long m_bit = (1ULL << id);
                int p_bit = ((m_pre_Port_Present & m_bit) >> id);
                int c_bit = ((m_Port_Present & m_bit) >> id);

                if ((p_bit == 1) && (c_bit == 0))
                { // port unplug
                    std::string event("Event");
                    std::string servrity("OK");
                    std::string sensor_type("Entity Presence");
                    std::string message("Port unplug.");
                    Entry.get_current_time();
                    Entry.set_log_entry(event, sensor_type, servrity, message, id + 1);

                    std::string message_event = "The resource has been removed successfully.";
                    EventMsgContent_T message_body;
                    message_body.message_id = "ResourceEvent.1.0.3.ResourceRemoved";
                    message_body.message_content = message_event;
                    message_body.origin_of_condition = orig_of_condition;
                    m_Event_Port_Resource_Remove.push_back(message_body);
                }
                else if ((p_bit == 0) && (c_bit == 1))
                { // port plug in
                    std::string event("Event");
                    std::string servrity("OK");
                    std::string sensor_type("Entity Presence");
                    std::string message("Port plug in.");
                    Entry.get_current_time();
                    Entry.set_log_entry(event, sensor_type, servrity, message, id + 1);

                    std::string message_event = "The resource has been created successfully.";
                    EventMsgContent_T message_body;
                    message_body.message_content = message_event;
                    message_body.origin_of_condition = orig_of_condition; 
                    message_body.message_id = "ResourceEvent.1.0.3.ResourceCreated";
                    m_Event_Port_Resource_Add.push_back(message_body);
                }
            }
            else if ((id >= 64) && (id < 128))
            {
                int A64_id = id - 64;

                // Check port present //
                unsigned long long m_bit = (1ULL << A64_id);
                unsigned long long p_bit = ((m_pre_Port_Present_A64 & m_bit) >> A64_id);
                unsigned long long c_bit = ((m_Port_Present_A64 & m_bit) >> A64_id);

                if ((p_bit == 1) && (c_bit == 0))
                { // port unplug
                    std::string event("Event");
                    std::string servrity("OK");
                    std::string sensor_type("Entity Presence");
                    std::string message("Port unplug.");
                    Entry.get_current_time();
                    Entry.set_log_entry(event, sensor_type, servrity, message, id + 1);

                    std::string message_event = "The resource has been removed successfully.";
                    EventMsgContent_T message_body;
                    message_body.message_content = message_event;
                    message_body.message_id = "ResourceEvent.1.0.3.ResourceRemoved";
                    message_body.origin_of_condition = orig_of_condition; 
                    m_Event_Port_Resource_Remove.push_back(message_body);
                }
                else if ((p_bit == 0) && (c_bit == 1))
                { // port plug in
                    std::string event("Event");
                    std::string servrity("OK");
                    std::string sensor_type("Entity Presence");
                    std::string message("Port plug in.");
                    Entry.get_current_time();
                    Entry.set_log_entry(event, sensor_type, servrity, message, id + 1);

                    std::string message_event = "The resource has been created successfully.";
                    EventMsgContent_T message_body;
                    message_body.message_content = message_event;
                    message_body.origin_of_condition = orig_of_condition; 
                    message_body.message_id = "ResourceEvent.1.0.3.ResourceCreated";
                    m_Event_Port_Resource_Add.push_back(message_body);
                }
            }
            else
                gADbg.acc_printf(LEVEL_INFO, "Port number are more then 128 port!!!!not enough bits to present");
        }

        m_pre_Port_Present = m_Port_Present;
        m_pre_Port_Present_A64 = m_Port_Present_A64;

        return;
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch update_port_present_event() - exception : " << e.what() << std::endl;
        return;
    }
}

void Switch::update_psu_present_event()
{
    try
    {
        auto &Entry = RFLogEntry::get_instance();
        int id = 0;

        for (id = 0; id < get_psu_num(); id++)
        {
            // Check PSU present //
            unsigned int m_bit = (1 << id);
            unsigned int p_bit = ((m_pre_Psu_Present & m_bit) >> id);
            unsigned int c_bit = ((m_Psu_Present & m_bit) >> id);

            std::string orig_of_condition = "/redfish/v1/Chassis/1/Power/" + std::to_string(id + 1);

            if ((p_bit == 1) && (c_bit == 0))
            { // PSU unplug
                std::string event("Event");
                std::string servrity("OK");
                std::string sensor_type("Power Supply / Converter");
                std::string message("PSU unplug.");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message, id + 1);

                std::string message_event ="The resource has been removed successfully."; 

                EventMsgContent_T message_body;
                message_body.message_id = "ResourceEvent.1.0.3.ResourceRemoved";
                message_body.message_content = message_event;
                message_body.origin_of_condition =orig_of_condition;
                m_Event_Resource_Remove.push_back(message_body);
            }
            else if ((p_bit == 0) && (c_bit == 1))
            { // PSU plug in
                std::string event("Event");
                std::string servrity("OK");
                std::string sensor_type("Power Supply / Converter");
                std::string message("PSU plug in.");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message, id + 1);

                std::string message_event = "The resource has been created successfully."; 
                EventMsgContent_T message_body;
                message_body.message_content = message_event;
                message_body.message_id = "ResourceEvent.1.0.3.ResourceCreated";
                message_body.origin_of_condition = orig_of_condition; 
                m_Event_Resource_Add.push_back(message_body);
            }
        }
        m_pre_Psu_Present = m_Psu_Present;

        return;
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch update_psu_present_event() - exception : " << e.what() << std::endl;
        return;
    }
}

void Switch::get_fan_info()
{
    try
    {
        for (int ii = 1; ii <= m_fan_max_num; ii++)
        {
            rfpal_fan_info fv = {};
            int rv = rfpal_fan_info_get(ii, &fv);

            for (vector<Fan_Info *>::iterator pObj = m_vec_Fan_Info.begin(); pObj != m_vec_Fan_Info.end(); ++pObj)
            {
                if (rv < 0 && (*pObj)->m_ID == ii)
                {
                    set_fan_present(ii, false);
                    (*pObj)->set_info(ii, fv.model[0] ? fv.model : "NULL", fv.serial[0] ? fv.serial : "NULL", 0, 0, false);
                    gADbg.acc_printf(LEVEL_WARN,"get_fan_info [%d] error", ii);
                }
                else if ((*pObj)->m_ID == ii)
                {
                    if (fv.status & 1)
                    {
                        set_fan_present(ii, true);
                        (*pObj)->set_info(ii, fv.model[0] ? fv.model : "NULL", fv.serial[0] ? fv.serial : "NULL", fv.front_rpm, fv.front_percentage, true);
                    }
                    else
                    {
                        set_fan_present(ii, false);
                        (*pObj)->set_info(ii, "NULL", "NULL", 0, 0, false);
                    }
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch get_fan_info() - exception : " << e.what() << std::endl;
        return;
    }
}

int Switch::get_fan_info_by_(int fanid, Fan_Content id)
{
    try
    {
        for (int ii = 1; ii <= get_fan_num(); ii++)
        {
            for (vector<Fan_Info *>::iterator pObj = m_vec_Fan_Info.begin(); pObj != m_vec_Fan_Info.end(); ++pObj)
            {
                if ((*pObj)->m_ID == fanid)
                {
                    switch (id)
                    {
                    case RPM:
                        return (*pObj)->m_RPM;
                        break;
                    case Per:
                        return (*pObj)->m_Per;
                        break;
                    case Type:
                        return (*pObj)->m_Type;
                        break;
                    case Fan_Present:
                        return (*pObj)->m_Present;
                        break;

                    default:
                        return 0;
                        break;
                    }
                }
            }
        }
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch get_fan_info_by_() - exception : " << e.what() << std::endl;
        return 0;
    }
}

std::string Switch::get_fan_info_by_(int fanid, std::string type)
{
    try
    {
        for (int ii = 1; ii <= get_fan_num(); ii++)
        {
            for (vector<Fan_Info *>::iterator pObj = m_vec_Fan_Info.begin(); pObj != m_vec_Fan_Info.end(); ++pObj)
            {
                if ((*pObj)->m_ID == fanid)
                {
                    if (type == "Status_Health")
                        return (*pObj)->m_Status_Health;
                    else if (type == "Status_State")
                        return (*pObj)->m_Status_State;
                    else
                        return "na";
                }
            }
        }
        return "na";
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch string get_fan_info_by_() - exception : " << e.what() << std::endl;
        return "na";
    }
}

void Switch::update_fan_present_event()
{
    try
    {
        auto &Entry = RFLogEntry::get_instance();
        int id = 0;

        for (id = 0; id < get_fan_num(); id++)
        {
            // Check FAN present //
            unsigned long long m_bit = (1 << id);
            unsigned long long p_bit = ((m_pre_Fan_Present & m_bit) >> id);
            unsigned long long c_bit = ((m_Fan_Present & m_bit) >> id);

            std::string orig_of_condition = "/redfish/v1/Chassis/1/Thermal/Fans/"+ std::to_string(id + 1);

            if ((p_bit == 1) && (c_bit == 0))
            { // FAN unplug
                std::string event("Event");
                std::string servrity("OK");
                std::string sensor_type("Fan");
                std::string message("FAN unplug.");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message, id + 1);

                std::string message_event ="The resource has been removed successfully."; 
                EventMsgContent_T message_body;
                message_body.message_content = message_event;
                message_body.message_id = "ResourceEvent.1.0.3.ResourceRemoved";
                message_body.origin_of_condition = orig_of_condition; 
                m_Event_Resource_Remove.push_back(message_body);
            }
            else if ((p_bit == 0) && (c_bit == 1))
            { // FAN plug in
                std::string event("Event");
                std::string servrity("OK");
                std::string sensor_type("Fan");
                std::string message("FAN plug in.");
                Entry.get_current_time();
                Entry.set_log_entry(event, sensor_type, servrity, message, id + 1);

                std::string message_event = "The resource has been created successfully."; 
                EventMsgContent_T message_body;
                message_body.message_content = message_event;
                message_body.origin_of_condition = orig_of_condition; 
                message_body.message_id = "ResourceEvent.1.0.3.ResourceCreated";
                m_Event_Resource_Add.push_back(message_body);
            }
        }
        m_pre_Fan_Present = m_Fan_Present;

        return;
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch update_fan_present_event() - exception : " << e.what() << std::endl;
        return;
    }
}

void Switch::update_thermal_present_event()
{
    try
    {
        int id = 0;

        for (id = 0; id < get_thermal_num(); id++)
        {

            // Check Thermal present //
            unsigned int m_bit = (1 << id);
            unsigned int p_bit = ((m_pre_Thermal_Present & m_bit) >> id);
            unsigned int c_bit = ((m_Thermal_Present & m_bit) >> id);

            if ((p_bit == 1) && (c_bit == 0))
            { // Thermal unplug
                std::string event("Event");
                std::string servrity("OK");
                std::string sensor_type("Thermal");
                std::string message("Thermal sensor unplug.");
            }
            else if ((p_bit == 0) && (c_bit == 1))
            { // Thermal Thermal in
                std::string event("Event");
                std::string servrity("OK");
                std::string sensor_type("Thermal");
                std::string message("Thermal sensor plug in.");
            }
        }
        m_pre_Thermal_Present = m_Thermal_Present;

        return;
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch update_thermal_present_event() - exception : " << e.what() << std::endl;
        return;
    }
}

void Switch::get_basic_info()
{
    try
    {
        rfpal_sys_info si;
        memset(&si, 0, sizeof(rfpal_sys_info));
        int rv = rfpal_sys_info_get(&si);

        if (rv < 0)
        {
            gADbg.acc_printf(LEVEL_WARN,"error-get_basic_info");
            return;
        }
        else
        {
            //HW info.//
            m_Product_Name = si.product_name;
            m_Part_Number = si.part_number;
            m_Serial_Number = si.serial_number;
            memcpy(this->m_MAC, si.mac, ECRF_PAL_CONST_6);
            m_MAC_Range = si.mac_range;
            m_Manufacturer = si.manufacturer;
            m_Manu_Date = si.manufacture_date;
            m_Vendor = si.vendor;
            m_Platform_Name = si.platform_name;
            m_Device_Version = si.device_version;
            m_Label_Revision = si.label_revision;
            m_Country_Code = si.country_code;
            m_Service_Tag = si.service_tag;
            //SW info.
            m_Diag_Version = si.diag_version;
            m_ONIE_Version = si.onie_version;
            m_Cpld_version = si.cpld_version;
            m_Fpga_version = si.fpga_version;
            m_BIOS_version = si.bios_version;
            m_BAL_version = si.bal_version;
            m_BAL_BSP_version = si.acc_bal_bsp_version;
            m_NOS_version = si.nos_version;
            m_NOS_type = si.nos_type;

            gADbg.acc_printf(LEVEL_INFO,"Product Name: %s", si.product_name);
            gADbg.acc_printf(LEVEL_INFO,"Part Number: %s", si.part_number);
            gADbg.acc_printf(LEVEL_INFO,"Serial Number: %s", si.serial_number);
            gADbg.acc_printf(LEVEL_INFO,"MAC: [0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:", si.mac[0], si.mac[1], si.mac[2], si.mac[3], si.mac[4], si.mac[5]);
            gADbg.acc_printf(LEVEL_INFO,"MAC Range: %d", si.mac_range);
            gADbg.acc_printf(LEVEL_INFO,"Manufacturer: %s", si.manufacturer);
            gADbg.acc_printf(LEVEL_INFO,"Manufacture Date: %s", si.manufacture_date);
            gADbg.acc_printf(LEVEL_INFO,"Vendor: %s", si.vendor);
            gADbg.acc_printf(LEVEL_INFO,"Platform Name: %s", si.platform_name);
            gADbg.acc_printf(LEVEL_INFO,"Device Version: %u", si.device_version);
            gADbg.acc_printf(LEVEL_INFO,"Label Revision: %s", si.label_revision);
            gADbg.acc_printf(LEVEL_INFO,"Country Code: %s", si.country_code);
            gADbg.acc_printf(LEVEL_INFO,"Diag Version: %s", si.diag_version);
            gADbg.acc_printf(LEVEL_INFO,"Service Tag: %s", si.service_tag);
            gADbg.acc_printf(LEVEL_INFO,"ONIE Version: %s", si.onie_version);
            gADbg.acc_printf(LEVEL_INFO,"Cpld Version: %s", si.cpld_version);
            gADbg.acc_printf(LEVEL_INFO,"BIOS Version: %s", si.bios_version);
            gADbg.acc_printf(LEVEL_INFO,"BAL Version: %s", si.bal_version);
            gADbg.acc_printf(LEVEL_INFO,"BAL_BSP Version: %s", si.acc_bal_bsp_version);
            gADbg.acc_printf(LEVEL_INFO,"NOS Version: %s", si.nos_version);
            gADbg.acc_printf(LEVEL_INFO,"NOS Type : %s", si.nos_type);
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch get_basic_info() - exception : " << e.what() << std::endl;
        return;
    }
}

void Switch::get_fan_thermal_threshold_info()
{
    try
    {
            for (vector<Thermal_Info *>::iterator pObj = m_vec_Thermal_Info.begin(); pObj != m_vec_Thermal_Info.end(); ++pObj)
            {
                (*pObj)->get_thermal_threshold_value();
            }

            for (vector<Fan_Info *>::iterator pObj = m_vec_Fan_Info.begin(); pObj != m_vec_Fan_Info.end(); ++pObj)
            {
                (*pObj)->get_fan_speed_threshold_value();
            }
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch get_fan_thermal_threshold_info - exception : " << e.what() << std::endl;
        return;
    }
}

Switch::~Switch()
{
    for (vector<Thermal_Info *>::iterator pObj = m_vec_Thermal_Info.begin(); pObj != m_vec_Thermal_Info.end(); ++pObj)
    {
        delete *pObj;
    }

    for (vector<Psu_Info *>::iterator pObj = m_vec_Psu_Info.begin(); pObj != m_vec_Psu_Info.end(); ++pObj)
    {
        delete *pObj;
    }

    for (vector<Fan_Info *>::iterator pObj = m_vec_Fan_Info.begin(); pObj != m_vec_Fan_Info.end(); ++pObj)
    {
        delete *pObj;
    }

    for (vector<Port_Info *>::iterator pObj = m_vec_Port_Info.begin(); pObj != m_vec_Port_Info.end(); ++pObj)
    {
        delete *pObj;
    }
}

Switch &Switch::get_instance()
{
    try
    {
        if (NULL == g_Switch)
        {
            ifstream ifs("/etc/onl/platform");
            std::string s;
            getline(ifs, s, (char)ifs.eof());
            printf("Creating platform [%s] size[%d]\r\n", s.c_str(), (int)s.size());

            if (s.find("asxvolt16", 0) != std::string::npos)
            {
                printf("x86-64-accton-asxvolt16\r\n");
                g_Switch = new Asxvolt16();
            }
            else if (s.find("asgvolt64", 0) != std::string::npos)
            {
                printf("x86-64-accton-asgvolt64\r\n");
                g_Switch = new Asgvolt64();
            }
            else if (s.find("as5916-54xks", 0) != std::string::npos)
            {
                printf("x86-64-accton-as5916-54xks\r\n");
                g_Switch = new As5916_54xks();
            }
            else if (s.find("as5916-54xk", 0) != std::string::npos)
            {
                printf("x86-64-accton-as5916-54xk\r\n");
                g_Switch = new As5916_54xk();
            }
            else
            {
                printf("x86-64-accton-generic\r\n");
                g_Switch = new Switch();
            }
        }
        return *g_Switch;
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch get_basic_info() - exception : " << e.what() << std::endl;
        return *g_Switch;
    }
}

void Switch::get_per_port_sys_file()
{
    try
    {
        Json::Value mapping_s;
        Json::Reader onulist_j_reader = {};
        std::string mapping_file_path = PORT_MAP_PATH + m_platinfo_name + "-sysfs";
        printf("get_per_port_sys_file() mapping_file_path[%s]\r\n", mapping_file_path.c_str());

        std::ifstream map_files(mapping_file_path);

        bool isJson = (onulist_j_reader.parse(map_files, mapping_s));

        if (isJson)
        {
            printf("Get sys_fs mapping file ok!!\r\n");

            for (int ii = 1; ii <= m_port_max_num; ii++)
            {
                for (vector<Port_Info *>::iterator pObj = m_vec_Port_Info.begin(); pObj != m_vec_Port_Info.end(); ++pObj)
                {
                    if ((*pObj)->m_ID == ii)
                    {
                        (*pObj)->set_sysfile_path(mapping_s[std::to_string(ii)].asString());
                        break;
                    }
                }
            }
        }
        else
            printf("Get sys fs mapping file error!!\r\n");
    }
    catch (const std::exception &e)
    {
        std::cout << "Switch get_per_port_sys_file() - exception : " << e.what() << std::endl;
    }
}

std::vector<EventMsgContent_T> Dev_Info::m_Event_Resource_Alert = {};

std::vector<EventMsgContent_T> Dev_Info::m_Event_Port_Resource_Alert = {};

std::vector<EventMsgContent_T> Dev_Info::get_Event_Resource_Alert()
{
    return m_Event_Resource_Alert;
}

std::vector<EventMsgContent_T> Dev_Info::get_Event_Port_Resource_Alert()
{
    return m_Event_Port_Resource_Alert;
}

void Dev_Info::Clear_Event_Resource_Alert()
{
    m_Event_Resource_Alert.clear();
    return;
}

void Dev_Info::Clear_Event_Port_Resource_Alert()
{
    m_Event_Port_Resource_Alert.clear();
    return;
}

std::vector<EventMsgContent_T>  Switch::m_Event_Resource_Add = {};

std::vector<EventMsgContent_T>  Switch::get_Event_Resource_Add()
{
    return m_Event_Resource_Add;
}

std::vector<EventMsgContent_T> Switch::m_Event_Port_Resource_Add = {};

std::vector<EventMsgContent_T> Switch::get_Event_Port_Resource_Add()
{
    return m_Event_Port_Resource_Add;
}

std::vector<EventMsgContent_T> Switch::m_Event_Resource_Remove = {};

std::vector<EventMsgContent_T> Switch::get_Event_Resource_Remove()
{
    return m_Event_Resource_Remove;
}

std::vector<EventMsgContent_T> Switch::m_Event_Port_Resource_Remove = {};

std::vector<EventMsgContent_T> Switch::get_Event_Port_Resource_Remove()
{
    return m_Event_Port_Resource_Remove;
}

std::vector<EventMsgContent_T> Switch::get_Event_Resource_Alert()
{
    return Dev_Info::get_Event_Resource_Alert();
}

std::vector<EventMsgContent_T> Switch::get_Event_Port_Resource_Alert()
{
    return Dev_Info::get_Event_Port_Resource_Alert();
}

void Switch::clean_Event_Resource_Event()
{
    m_Event_Resource_Add.clear();
    m_Event_Resource_Remove.clear();
    Dev_Info::Clear_Event_Resource_Alert();
    return;
}

void Switch::clean_Event_Port_Resource_Event()
{
    m_Event_Port_Resource_Add.clear();
    m_Event_Port_Resource_Remove.clear();
    Dev_Info::Clear_Event_Port_Resource_Alert();
    return;
}

void SysResource::mon_cpu_usage_info()
{
    try
    {
        struct sysinfo s_info;
        sysinfo(&s_info);

        CPU s1;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        CPU s2;
        CPUStatsPrinter printer(s1, s2);
        double t_cpu_usage_percentage = ((double)printer.GetPercActiveTotal() / (double)1.0);

        gADbg.acc_printf(LEVEL_INFO, "get cpu usage percentage [%f]", cpu_usage_percentage);

        if (cpu_sample_number != 1)
        {
            cpu_sample_number = cpu_sample_number - 1;
            cpu_usage_percentage += t_cpu_usage_percentage;
        }
        else
        {
            cpu_usage_percentage += t_cpu_usage_percentage;
            double avg_percentage = cpu_usage_percentage / ((double)SAMPLE_NUM * 1.0);
            gADbg.acc_printf(LEVEL_INFO, "AVG cpu loading info [%f]", avg_percentage);
            cpu_usage_percentage = 0.0;
            cpu_sample_number = SAMPLE_NUM;

            rfpal_thresholds threshhold_info_cpu;
            rfpal_sys_resource_thresholds_get(ECRF_PAL_SYS_CPU, &threshhold_info_cpu);

            double cpu_upper_th = threshhold_info_cpu.warning_upper / 1000;

            if (avg_percentage >= cpu_upper_th)
            {
                gADbg.acc_printf(LEVEL_INFO, "upperr cpu loading threshold [%f]", cpu_upper_th);
                std::string orig_of_condition = "/redfish/v1/Systems/1/Cpu";
                std::string message_event = "The resource property " + orig_of_condition + " has exceeded its warning threshold of value " + std::to_string(cpu_upper_th) + ".";
                EventMsgContent_T message_body;
                message_body.message_content = message_event;
                message_body.message_id = "ResourceEvent.1.0.3.ResourceWarningThresholdExceeded";
                message_body.origin_of_condition = orig_of_condition;
                Json::Value oem;
                oem["CurrentValue"] = avg_percentage; 
                oem["State"] = "ExceededUpperThresholdWarning";
                oem["ThresholdValue"] = cpu_upper_th; 
                message_body.oem = oem;
                m_Event_Resource_Alert.push_back(message_body);
            }
        }
    }
    catch (const std::exception &e)
    {
        gADbg.acc_printf(LEVEL_WARN, "get cpu usage info error");
    }
}

void SysResource::mon_memory_usage_info()
{
    try
    {
        struct sysinfo s_info;
        sysinfo(&s_info);
        double free_ratio = ((1.0 * (double)s_info.freeram) / (1.0 * (double)s_info.totalram)) * 100.0;
        //double used_ratio = 100.0 - free_ratio;
        //gADbg.acc_printf(LEVEL_INFO, "get mem used info [%f]", used_ratio);
        gADbg.acc_printf(LEVEL_INFO, "get mem free info [%f]", free_ratio);

        if (mem_sample_number != 1)
        {
            mem_sample_number = mem_sample_number - 1;
            free_mem_percentage += free_ratio;
        }
        else
        {
            free_mem_percentage += free_ratio;
            double avg_free_ratio = free_mem_percentage / ((double)SAMPLE_NUM * 1.0);
            gADbg.acc_printf(LEVEL_INFO, "AVG mem free used info [%f]", avg_free_ratio);
            free_mem_percentage = 0.0;
            mem_sample_number = SAMPLE_NUM;
            rfpal_thresholds threshhold_info;
            rfpal_sys_resource_thresholds_get(ECRF_PAL_SYS_MEM, &threshhold_info);

            double lower_mem_usage_th = threshhold_info.warning_lower / 1000.0;

            if (avg_free_ratio <= lower_mem_usage_th)
            {
                gADbg.acc_printf(LEVEL_INFO, "lower free memory usage threshold [%f]", lower_mem_usage_th);
                std::string orig_of_condition = "/redfish/v1/Systems/1/Memory";
                std::string message_event = "The resource property " + orig_of_condition + " has exceeded its warning threshold of value " + std::to_string(lower_mem_usage_th) + ".";
                EventMsgContent_T message_body;
                message_body.message_content = message_event;
                message_body.message_id = "ResourceEvent.1.0.3.ResourceWarningThresholdExceeded";
                message_body.origin_of_condition = orig_of_condition;
                Json::Value oem;
                oem["CurrentValue"] = avg_free_ratio; 
                oem["State"] = "ExceededLowerThresholdWarning";
                oem["ThresholdValue"] = lower_mem_usage_th; 
                message_body.oem = oem;
                m_Event_Resource_Alert.push_back(message_body);
            }
        }
    }
    catch (const std::exception &e)
    {
        gADbg.acc_printf(LEVEL_WARN, "get memory usage info error");
    }
}

void SysResource::mon_storage_usage_info()
{
    try
    {
        double per = m_storage.get_sdx_used_percentage();
        gADbg.acc_printf(LEVEL_INFO, "get storage usage info [%f]", per);

        if (storage_sample_number != 1)
        {
            storage_sample_number = storage_sample_number - 1;
            storage_usage_percentage += per;
        }
        else
        {
            storage_usage_percentage += per;
            double avg_str_usage_per = storage_usage_percentage / ((double)SAMPLE_NUM * 1.0);
            gADbg.acc_printf(LEVEL_INFO, "AVG storage used per info [%f]", avg_str_usage_per);
            storage_usage_percentage = 0.0;
            storage_sample_number = SAMPLE_NUM;
            rfpal_thresholds threshhold_info;
            rfpal_sys_resource_thresholds_get(ECRF_PAL_SYS_STORAGE, &threshhold_info);

            double upper_stor_usage_th = threshhold_info.warning_upper / 1000.0;

            if (avg_str_usage_per >= upper_stor_usage_th)
            {
                gADbg.acc_printf(LEVEL_INFO, "upper storage usage threshold [%f]", upper_stor_usage_th);
                std::string orig_of_condition = "/redfish/v1/Systems/1/Storage";
                std::string message_event = "The resource property " + orig_of_condition + " has exceeded its warning threshold of value " + std::to_string(upper_stor_usage_th) + ".";
                EventMsgContent_T message_body;
                message_body.message_content = message_event;
                message_body.message_id = "ResourceEvent.1.0.3.ResourceWarningThresholdExceeded";
                message_body.origin_of_condition = orig_of_condition;
                Json::Value oem;
                oem["CurrentValue"] = avg_str_usage_per; 
                oem["State"] = "ExceededUpperThresholdWarning";
                oem["ThresholdValue"] = upper_stor_usage_th; 
                message_body.oem = oem;
                m_Event_Resource_Alert.push_back(message_body);
            }
        }
    }
    catch (const std::exception &e)
    {
        gADbg.acc_printf(LEVEL_WARN, "get storage usage info error");
    }
}

void SysResource::mon_openbmc_status()
{
    try
    {
        EventMsgContent_T message_body;
        std::string orig_of_condition = "/redfish/v1/Manager/2";
        message_body.origin_of_condition = orig_of_condition;

        auto &gecOpenBmc_helper = ecOpenBmc_helper::get_instance();
        if (gecOpenBmc_helper.get_status() && openbmc_status == false)
        {
            gADbg.acc_printf(LEVEL_INFO, "Found BMC moudle");
            openbmc_status = true;

            std::string message_event = std::string("The state of resource ") + orig_of_condition + " has changed to state type Enabled.";
            message_body.message_content = message_event;
            message_body.message_id = "ResourceEvent.1.0.3.ResourceStatusChangedOK";
            m_Event_Resource_Alert.push_back(message_body);
        }
        else if (!gecOpenBmc_helper.get_status() && openbmc_status == true)
        {
            gADbg.acc_printf(LEVEL_WARN, "BMC moudle disapper!");
            openbmc_status = false;

            std::string message_event = std::string("The state of resource ") + orig_of_condition + " has changed to state type Absent.";

            message_body.message_content = message_event;
            message_body.message_id = "ResourceEvent.1.0.3.ResourceStatusChangedCritical";
            m_Event_Resource_Alert.push_back(message_body);
        }
    }
    catch (const std::exception &e)
    {
        gADbg.acc_printf(LEVEL_WARN, "get openbmc status error");
    }
}

void Switch::monitor_sys_resource()
{
    m_SysResource.mon_memory_usage_info();
    m_SysResource.mon_storage_usage_info();
    m_SysResource.mon_cpu_usage_info();
    m_SysResource.mon_openbmc_status();
}

} // namespace ecrf_pal_helper
