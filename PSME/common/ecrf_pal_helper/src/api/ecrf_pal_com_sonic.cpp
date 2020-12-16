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

#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <dirent.h>
#include <string.h>
#include <chrono>
#include <ctime>

#define UNUSED(x) (void)(x)

using namespace ecrf_pal_com_s;

int ecrf_pal_com_sonic::rfpal_init()
{
    return ECRF_PAL_STATUS_OK;
}

ecrf_pal_com_sonic::ecrf_pal_com_sonic()
{
}

int ecrf_pal_com_sonic::rfpal_sys_info_com_get(rfpal_sys_info *p_sys_info)
{
	
    this->rfpal_system_cmd_set("sudo /usr/bin/decode-syseeprom | grep \"Platform Name\" | awk '{print $5}'",p_sys_info->platform_name);
    this->rfpal_system_cmd_set("sudo /usr/bin/decode-syseeprom | grep \"Product Name\" | awk '{print $5}'",p_sys_info->product_name);
    this->rfpal_system_cmd_set("sudo /usr/bin/decode-syseeprom | grep \"Part Number\" | awk '{print $5}'",p_sys_info->part_number);
    this->rfpal_system_cmd_set("sudo /usr/bin/decode-syseeprom | grep \"Serial Number\" | awk '{print $5}'",p_sys_info->serial_number);
    //MAC
    this->rfpal_system_cmd_set("sudo /usr/bin/decode-syseeprom | grep \"Base MAC Address\" | awk '{print $6}'",p_sys_info->mac);
    //manufacturer
    this->rfpal_system_cmd_set("sudo /usr/bin/decode-syseeprom | grep \"Manufacturer\" | awk '{print $4}'",p_sys_info->manufacturer);
    //manufacture_date
    this->rfpal_system_cmd_set("sudo /usr/bin/decode-syseeprom | grep \"Manufacture Date\" | awk '{print $5, $6}'",p_sys_info->manufacture_date);
    //vendor
    this->rfpal_system_cmd_set("sudo /usr/bin/decode-syseeprom | grep \"Vendor Name\" | awk '{print $5}'",p_sys_info->vendor);
    //device_version
    //label_revision
    this->rfpal_system_cmd_set("sudo /usr/bin/decode-syseeprom | grep \"Label Revision\" | awk '{print $5}'",p_sys_info->label_revision);
    // country_code
    // diag_version           
    this->rfpal_system_cmd_set("sudo /usr/bin/decode-syseeprom | grep \"Diag Version\" | awk '{print $5}'",p_sys_info->diag_version);
    // service_tag
    // mac_range
    // onie_version
    this->rfpal_system_cmd_set("sudo /usr/bin/decode-syseeprom | grep \"ONIE Version\" | awk '{print $5}'",p_sys_info->onie_version);   
    // nos_type
            
    return ECRF_PAL_STATUS_OK;
}

int ecrf_pal_com_sonic::rfpal_fan_num_com_get()
{
    std::ifstream m_ifile("/etc/psme/platform_info.conf");
    Json::Value content;
    int num = 0;
    if (m_ifile.good())
    {
        Json::Reader reader;
        bool isJsonOK = (m_ifile != NULL && reader.parse(m_ifile, content));

        if (isJsonOK)
        {
            const Json::Value defValue; //used for default reference
            const Json::Value s = content.get("managers", defValue);             
            num = s[0]["ecrf_pal"]["fan_max_num"].asInt();
    	}
    }
    return num;        
}

int ecrf_pal_com_sonic::rfpal_fan_info_com_get(unsigned id, rfpal_fan_info *p_fan_info)
{
    p_fan_info->id = id;   
    rfpal_thresholds threshold_info;
    this->rfpal_fan_speed_thresholds_com_get(id, &threshold_info);
    p_fan_info->m_threshold.warning_upper = threshold_info.warning_upper;
    p_fan_info->m_threshold.warning_lower = threshold_info.warning_lower;
    p_fan_info->m_threshold.error = threshold_info.error;
    p_fan_info->m_threshold.shutdown = threshold_info.shutdown;
    
    std::ifstream m_ifile("/etc/psme/platform_info.conf");
    Json::Value content;
    if (m_ifile.good())
    {
        Json::Reader reader;
        bool isJsonOK = (m_ifile != NULL && reader.parse(m_ifile, content));

        if (isJsonOK)
        {
            char str_fan[16]="";
            char str_fan_node[16]="";
            const Json::Value defValue; //used for default reference
            const Json::Value s = content.get("managers", defValue);
            sprintf(str_fan, "fan%d", id);
            sprintf(str_fan_node, "fan%d_node", id);
             
            std::string fan_type = s[0]["ecrf_pal"]["fans"][str_fan].asString();
            if(strcmp(fan_type.c_str(),"System") == 0 || strcmp(fan_type.c_str(),"Psu") == 0){
                char str_node_result[64]="";
                // name
                std::string fan_name = s[0]["ecrf_pal"]["fans"][str_fan_node]["name"].asString();
                this->rfpal_sysfs_node_get(fan_name.c_str(), str_node_result);
                strncpy(p_fan_info->model, str_node_result, strlen(str_node_result));
                // serial
                str_node_result[0]='\0';
                std::string fan_serial = s[0]["ecrf_pal"]["fans"][str_fan_node]["serial"].asString();
                this->rfpal_sysfs_node_get(fan_serial.c_str(), str_node_result);
                strncpy(p_fan_info->serial, str_node_result, strlen(str_node_result));            	  
                // present
                str_node_result[0]='\0';
                std::string fan_present = s[0]["ecrf_pal"]["fans"][str_fan_node]["present"].asString();
                this->rfpal_sysfs_node_get(fan_present.c_str(), str_node_result);
                p_fan_info->status = atoi(str_node_result);
                // front_speed
                str_node_result[0]='\0';
                std::string fan_front_speed = s[0]["ecrf_pal"]["fans"][str_fan_node]["front_speed"].asString();
                this->rfpal_sysfs_node_get(fan_front_speed.c_str(), str_node_result);
                p_fan_info->front_rpm = atoi(str_node_result);            	  
                // rear_speed
                str_node_result[0]='\0';
                std::string fan_rear_speed = s[0]["ecrf_pal"]["fans"][str_fan_node]["rear_speed"].asString();
                this->rfpal_sysfs_node_get(fan_rear_speed.c_str(), str_node_result);
                p_fan_info->rear_rpm = atoi(str_node_result);            	  
                // front_percentage
                str_node_result[0]='\0';
                std::string fan_front_percentage = s[0]["ecrf_pal"]["fans"][str_fan_node]["front_percentage"].asString();
                this->rfpal_sysfs_node_get(fan_front_percentage.c_str(), str_node_result);
                p_fan_info->front_percentage = atoi(str_node_result);            	  
                // rear_percentage 
                str_node_result[0]='\0';
                std::string fan_rear_percentage = s[0]["ecrf_pal"]["fans"][str_fan_node]["rear_percentage"].asString();
                this->rfpal_sysfs_node_get(fan_rear_percentage.c_str(), str_node_result);
                p_fan_info->rear_percentage = atoi(str_node_result);            	  ;
            }
            else{
              	printf("fan type error\n");
            }
        }
    }
    return ECRF_PAL_STATUS_OK;
}

int ecrf_pal_com_sonic::rfpal_port_num_com_get()
{
    std::ifstream m_ifile("/etc/psme/platform_info.conf");
    Json::Value content;
    int num = 0;
    if (m_ifile.good())
    {
        Json::Reader reader;
        bool isJsonOK = (m_ifile != NULL && reader.parse(m_ifile, content));

        if (isJsonOK)
        {
            const Json::Value defValue; //used for default reference
            const Json::Value s = content.get("managers", defValue);
            num = s[0]["ecrf_pal"]["port_max_num"].asInt();
    	}
    }
    return num;     
}


int ecrf_pal_com_sonic::rfpal_port_com_trans_is_present(int id)
{
    std::ifstream m_ifile("/etc/psme/platform_info.conf");
    Json::Value content;
    int is_present = -1;
    if (m_ifile.good())
    {
        Json::Reader reader;
        bool isJsonOK = (m_ifile != NULL && reader.parse(m_ifile, content));

        if (isJsonOK)
        {
            char str_port_node[16]="";
            char str_node_result[64]="";
            const Json::Value defValue; //used for default reference
            const Json::Value s = content.get("managers", defValue);
            sprintf(str_port_node, "port%d_node", id);                        
            std::string port_present = s[0]["ecrf_pal"]["ports"][str_port_node]["present"].asString();
            this->rfpal_sysfs_node_get(port_present.c_str(), str_node_result);
            is_present =atoi(str_node_result);            
    	}
    }
    return is_present;
}

int ecrf_pal_com_sonic::rfpal_sfp_tx_disable_com_set(unsigned id, int value)
{
    std::ifstream m_ifile("/etc/psme/platform_info.conf");
    Json::Value content;
    if (m_ifile.good())
    {
        Json::Reader reader;
        bool isJsonOK = (m_ifile != NULL && reader.parse(m_ifile, content));

        if (isJsonOK)
        {
            char str_port_node[16]="";
            char str_node_result[64]="";
            const Json::Value defValue; //used for default reference
            const Json::Value s = content.get("managers", defValue);
            sprintf(str_port_node, "port%d_node", id);                        
            std::string port_tx_disable = s[0]["ecrf_pal"]["ports"][str_port_node]["tx_disable"].asString();
            sprintf(str_node_result, "%d", value);
            if(this->rfpal_sysfs_node_set(port_tx_disable.c_str(), str_node_result) == ECRF_PAL_STATUS_E_INVALID){
                return ECRF_PAL_STATUS_E_GENERIC;
            }            
    	  }else{
            return ECRF_PAL_STATUS_E_GENERIC;    	  	
    	  }
    }
    else{
        return ECRF_PAL_STATUS_E_GENERIC;
    }
    return ECRF_PAL_STATUS_OK;    
}

int ecrf_pal_com_sonic::rfpal_sfp_tx_disable_com_get(unsigned id)
{
    std::ifstream m_ifile("/etc/psme/platform_info.conf");
    Json::Value content;
    int tx_disable_value = -1;
    if (m_ifile.good())
    {
        Json::Reader reader;
        bool isJsonOK = (m_ifile != NULL && reader.parse(m_ifile, content));

        if (isJsonOK)
        {
            char str_port_node[16]="";
            char str_node_result[64]="";
            const Json::Value defValue; //used for default reference
            const Json::Value s = content.get("managers", defValue);
            sprintf(str_port_node, "port%d_node", id);                        
            std::string port_tx_disable = s[0]["ecrf_pal"]["ports"][str_port_node]["tx_disable"].asString();
            if(this->rfpal_sysfs_node_get(port_tx_disable.c_str(), str_node_result) != ECRF_PAL_STATUS_E_INVALID){
                tx_disable_value =atoi(str_node_result);
            }            
    	  }
    }
    return tx_disable_value;
}

int ecrf_pal_com_sonic::rfpal_sfp_tx_eeprom_com_get(unsigned int  id, char * p_eeprom, int size)
{
    //Todo: 
    UNUSED(id);
    UNUSED(p_eeprom);
    UNUSED(size);
    return ECRF_PAL_STATUS_E_GENERIC;
}

int ecrf_pal_com_sonic::rfpal_thermal_num_com_get()
{
    std::ifstream m_ifile("/etc/psme/platform_info.conf");
    Json::Value content;
    int num = 0;
    if (m_ifile.good())
    {
        Json::Reader reader;
        bool isJsonOK = (m_ifile != NULL && reader.parse(m_ifile, content));

        if (isJsonOK)
        {
            const Json::Value defValue; //used for default reference
            const Json::Value s = content.get("managers", defValue);             
            num = s[0]["ecrf_pal"]["thermal_sen_max_num"].asInt();
    	}
    }
    return num;
}

int ecrf_pal_com_sonic:: rfpal_thermal_info_com_get(unsigned id_in, rfpal_thermal_info *p_thermal_info)
{
    p_thermal_info->status = 0;
    p_thermal_info->id = id_in;
    p_thermal_info->mcelsius = 0;
    p_thermal_info->m_threshold.warning_lower = 0;
    p_thermal_info->m_threshold.warning_upper = 0;
    p_thermal_info->m_threshold.error = 0;
    p_thermal_info->m_threshold.shutdown = 0;

    rfpal_thresholds threshold_info;
    this->rfpal_thermal_thresholds_com_get(id_in, &threshold_info);
    p_thermal_info->m_threshold.warning_upper = threshold_info.warning_upper;
    p_thermal_info->m_threshold.warning_lower = threshold_info.warning_lower;
    p_thermal_info->m_threshold.error = threshold_info.error;
    p_thermal_info->m_threshold.shutdown = threshold_info.shutdown;
    		
    std::ifstream m_ifile("/etc/psme/platform_info.conf");
    Json::Value content;
    if (m_ifile.good())
    {
        Json::Reader reader;
        bool isJsonOK = (m_ifile != NULL && reader.parse(m_ifile, content));

        if (isJsonOK)
        {
            char str_thermal[16]="";
            char str_thermal_node[16]="";
            const Json::Value defValue; //used for default reference
            const Json::Value s = content.get("managers", defValue);
            sprintf(str_thermal, "thermal%d", id_in);
            sprintf(str_thermal_node, "thermal%d_node", id_in);
             
            std::string thermal_type = s[0]["ecrf_pal"]["thermals"][str_thermal].asString();
            if(strcmp(thermal_type.c_str(), "CPU") == 0){
                p_thermal_info->status = 1;
                std::string thermal_temp;
                char str_cpu_thermal_node[12]="";
                int cpu_temp_total = 0;
                char str_node_result[64]="";
               	
                for(int i = 0 ; i < 4; i++){
	            sprintf(str_cpu_thermal_node, "temp%d", i+1);
	            thermal_temp = s[0]["ecrf_pal"]["thermals"][str_thermal_node][str_cpu_thermal_node].asString();
	            this->rfpal_sysfs_node_get(thermal_temp.c_str(), str_node_result);
	            cpu_temp_total += atoi(str_node_result);                 		
                }
                p_thermal_info->mcelsius =(cpu_temp_total/4);
            }
            else if(strcmp(thermal_type.c_str(),"System") == 0 || strcmp(thermal_type.c_str(),"Psu") == 0){ 
                if(strcmp(thermal_type.c_str(),"System") == 0){
              	    printf("Thermal - system\n");
              	    p_thermal_info->status = 1;
                }
              	else{
              	    char str_node_result[64]="";
                    std::string thermal_present = s[0]["ecrf_pal"]["thermals"][str_thermal_node]["present"].asString();
                    this->rfpal_sysfs_node_get(thermal_present.c_str(), str_node_result);
                    p_thermal_info->status =atoi(str_node_result);            			
              	}
                char str_node_result[64]="";
                char str_get_temp_cmd[64]="";
                std::string thermal_temp = s[0]["ecrf_pal"]["thermals"][str_thermal_node]["temp"].asString();
                sprintf(str_get_temp_cmd, "sudo cat %s", thermal_temp.c_str());
                this->rfpal_system_cmd_set(str_get_temp_cmd, str_node_result);
                p_thermal_info->mcelsius = atoi(str_node_result);
            }
            else{
                printf("Thermal type error\n");
            }
        }
    }
    return ECRF_PAL_STATUS_OK;
}

int ecrf_pal_com_sonic::rfpal_psu_num_com_get()
{
    std::ifstream m_ifile("/etc/psme/platform_info.conf");
    Json::Value content;
    int num = 0;
    if (m_ifile.good())
    {
        Json::Reader reader;
        bool isJsonOK = (m_ifile != NULL && reader.parse(m_ifile, content));

        if (isJsonOK)
        {
            const Json::Value defValue; //used for default reference
            const Json::Value s = content.get("managers", defValue);             
            num = s[0]["ecrf_pal"]["psu_max_num"].asInt();
    	}
    }
    return num;    
}

int ecrf_pal_com_sonic::rfpal_psu_info_com_get(unsigned id_in, rfpal_psu_info *p_psu_info)
{
    p_psu_info->id = id_in;

    std::ifstream m_ifile("/etc/psme/platform_info.conf");
    Json::Value content;
    if (m_ifile.good())
    {
        Json::Reader reader;
        bool isJsonOK = (m_ifile != NULL && reader.parse(m_ifile, content));

        if (isJsonOK)
        {
            char str_psu_node[20]="";
            const Json::Value defValue; //used for default reference
            const Json::Value s = content.get("managers", defValue);
            sprintf(str_psu_node, "psu%d_node", id_in); 
            char str_node_result[64]="";
            // name
            std::string psu_name = s[0]["ecrf_pal"]["psus"][str_psu_node]["name"].asString();
            this->rfpal_sysfs_node_get(psu_name.c_str(), str_node_result);
            strncpy(p_psu_info->model, str_node_result, strlen(str_node_result));
	    // type
            if(((strncmp(str_node_result, "YM-2651Y", 8)==0) || (strncmp(str_node_result, "YPEB1200", 8)==0) || (strncmp(str_node_result, "YESM1300", 8)==0))){
                p_psu_info->psu_type = ecrf_pal_com_s::RFPAL_PSU_TYPE_AC;    
            }
            else if(strncmp(str_node_result, "PSU-12V", 7)==0){
                p_psu_info->psu_type = ecrf_pal_com_s::RFPAL_PSU_TYPE_DC12;    			  	
            }
            else if(strncmp(str_node_result, "YM-2651V", 8)==0){
                p_psu_info->psu_type = ecrf_pal_com_s::RFPAL_PSU_TYPE_DC48;
            }
            else{
                p_psu_info->psu_type = ecrf_pal_com_s::RFPAL_PSU_TYPE_INVALID;	
            }
            // serial
            str_node_result[0]='\0';
            std::string psu_serial = s[0]["ecrf_pal"]["psus"][str_psu_node]["serial"].asString();
            this->rfpal_sysfs_node_get(psu_serial.c_str(), str_node_result);
	    if(strlen(str_node_result) > 0){
                strncpy(p_psu_info->serial, str_node_result, strlen(str_node_result)-1);            	  
            }
            // present
            str_node_result[0]='\0';
            std::string psu_present = s[0]["ecrf_pal"]["psus"][str_psu_node]["present"].asString();
            this->rfpal_sysfs_node_get(psu_present.c_str(), str_node_result);
            p_psu_info->status = atoi(str_node_result);
            // vin
            str_node_result[0]='\0';
            std::string psu_vin = s[0]["ecrf_pal"]["psus"][str_psu_node]["vin"].asString();
            this->rfpal_sysfs_node_get(psu_vin.c_str(), str_node_result);
            p_psu_info->mvin = atoi(str_node_result);            	  
            // vout
            str_node_result[0]='\0';
            std::string psu_vout = s[0]["ecrf_pal"]["psus"][str_psu_node]["vout"].asString();
            this->rfpal_sysfs_node_get(psu_vout.c_str(), str_node_result);
            p_psu_info->mvout = atoi(str_node_result);            	  
            // iin
            str_node_result[0]='\0';
            std::string psu_iin = s[0]["ecrf_pal"]["psus"][str_psu_node]["iin"].asString();
            this->rfpal_sysfs_node_get(psu_iin.c_str(), str_node_result);
            // iout 
            str_node_result[0]='\0';
            std::string psu_iout = s[0]["ecrf_pal"]["psus"][str_psu_node]["iout"].asString();
            this->rfpal_sysfs_node_get(psu_iout.c_str(), str_node_result);
            p_psu_info->miout = atoi(str_node_result);
            // pin
            str_node_result[0]='\0';
            std::string psu_pin = s[0]["ecrf_pal"]["psus"][str_psu_node]["pin"].asString();
            this->rfpal_sysfs_node_get(psu_pin.c_str(), str_node_result);
            p_psu_info->mpin = atoi(str_node_result);						
            // pout 
            str_node_result[0]='\0';
            std::string psu_pout = s[0]["ecrf_pal"]["psus"][str_psu_node]["pout"].asString();
            this->rfpal_sysfs_node_get(psu_pout.c_str(), str_node_result);
            p_psu_info->mpout = atoi(str_node_result);						
        }
    }
    return ECRF_PAL_STATUS_OK;    
}
