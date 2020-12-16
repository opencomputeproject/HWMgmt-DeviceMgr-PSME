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

#include "ecrf_pal_helper/api/ecrf_pal_helper_api.hpp"
#include "ecrf_pal_helper/api/ecrf_pal_com.hpp"

#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <dirent.h>
#include <string.h>
#include <chrono>
#include <ctime>

using namespace ecrf_pal_com_s;

namespace ecrf_pal_com_api
{
/*
int	rfpal_sys_info_get(rfpal_sys_info * p_sys_info); 

	Get system chassis basic HW/SW information

	Parameters :	
		p_sys_info  Pointer to an allocated rfpal_sys_info_t memory to store data. 
	
	Returns : 
	  1 Get system information without error.
 	 -1 An error condition.		
*/

int rfpal_sys_info_get(rfpal_sys_info *p_sys_info)
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_sys_info_com_get(p_sys_info);
	return rv;
}

/*
int rfpal_fan_num_get()	

	Get FAN total number.
	
	Parameters : 
		none

	Returns : 
		>0 Get FAN sensor count.
		-1 An error condition.
*/

int rfpal_fan_num_get()
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_fan_num_com_get();
	return rv;
}

/*
int rfpal_fan_info_get( unsigned id , rfpal_fan_info * p_fan_info);

  Get individual fan information and status.
  
	Parameters :	
		id          The fan ID.
		p_fan_info  Pointer to an allocated rfpal_fan_info_t memory to store data. 
		
	Returns : 
		 1 Get without error.
		-1 An error condition.		
*/
int rfpal_fan_info_get(unsigned id, rfpal_fan_info *p_fan_info)
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_fan_info_com_get(id, p_fan_info);
	return rv;
}

/*
int rfpal_fan_speed_thresholds_set(unsigned id ,rfpal_thresholds threshold_info)	
  
  Set individual fan speed threshhold value.

	Parameters :	
		id               The FAN ID
		threshhold_info  threshhold values.
		 
	Returns
		1 get without error.
		-1 An error condition.	
*/

int rfpal_fan_speed_thresholds_set(unsigned id, rfpal_thresholds threshold_info)
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_fan_speed_thresholds_com_set(id, threshold_info);
	return rv;
}

/*
int rfpal_fan_speed_thresholds_get(unsigned id ,rfpal_thresholds *threshold_info)	
  
  Get individual fan speed threshhold value.

	Parameters :	
		id               The FAN ID
		*threshhold_info  threshhold values.
		 
	Returns
		1 get without error.
		-1 An error condition.	
*/

int rfpal_fan_speed_thresholds_get(unsigned id, rfpal_thresholds * threshold_info)
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_fan_speed_thresholds_com_get(id, threshold_info);
	return rv;
}



/*
int rfpal_port_trans_is_present	(unsigned id)

  Get individual SWITCH or OLT front port transceiver present/absent status.

	Parameters : 
		id	The Port ID
		
	Returns : 
		1 if present  //RJ45 port always return 1 //
		0 if absent
		-1 An error condition.
*/

int rfpal_port_trans_is_present(unsigned id)
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_port_com_trans_is_present(id);
	return rv;
}

/*
int rfpal_port_num_get()	

  Get SWITCH or OLT front ports total number.

	Parameters : 
		none
		
	Returns : 
		> 0 Get port count(RJ45 port + SFP or PON , front port). 
		-1  An error condition.
*/

int rfpal_port_num_get()
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_port_num_com_get();
	return rv;
}

/*
int rfpal_sfp_tx_disable_set(unsigned id , value);

  Set SFP port Tx power enable/disable

	Parameters : 
		id	The Port ID	
	
		value	
		 0 : Tx enable
		 1 : Tx disable
		

	Returns : 		
	   1 Set without error.
		-1 An error condition.
*/

int rfpal_sfp_tx_disable_set(unsigned id, int value)
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_sfp_tx_disable_com_set(id, value);
	return rv;
}

/*
int rfpal_sfp_tx_disable_get(unsigned int  id);

  Get SFP port Tx power enable/disable status

	Parameters : 
		id	The Port ID	
	
	Returns : 		
	   1 Tx is disable.
	   0 Tx is enable.
		-1 An error condition.

*/

int rfpal_sfp_tx_disable_get(unsigned id)
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_sfp_tx_disable_com_get(id);
	return rv;
}

/*
int rfpal_sfp_tx_eeprom_get(unsigned int  id, char * p_eeprom, int size);

  Get SFP port transceiver eeprom raw data 

	Parameters : 
		id	       The Port ID	
		p_eeprom   pointer to eeprom data
		size       Size of eeprom data want to get
	
	Returns : 		
	    1 Get without error. 
	   -1 An error condition.

*/

int rfpal_sfp_tx_eeprom_get(unsigned int id, char *p_eeprom, int size)
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_sfp_tx_eeprom_com_get(id, p_eeprom, size);
	return rv;
}

/*
int rfpal_thermal_num_get()	

	Get thermal sensor (CPU + mainboard + PSU) total number.
	
	Parameters : 
		none

	Returns : 
		> 0 Get thermal sensor count.
		-1  An error condition.
*/

int rfpal_thermal_num_get()
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_thermal_num_com_get();
	return rv;
}

/*
int rfpal_thermal_thresholds_set ( unsigned id , rfpal_thresholds threshhold_info )	
  
  Set individual thermal sensor threshhold value.

	Parameters :	
		id               The Thermal sensor ID (1: CPU , 2~N Mainboard, N+1 , N+2 ..PSU)
		threshhold_info  threshhold values.
		 
	Returns
		1 get without error.
		-1 An error condition.	
*/

int rfpal_thermal_thresholds_set(unsigned id, rfpal_thresholds threshold_info)
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_thermal_thresholds_com_set(id, threshold_info);
	return rv;
}

/*
int rfpal_thermal_thresholds_get ( unsigned id , rfpal_thresholds *threshhold_info )	
  
  Get individual thermal sensor threshhold value.

	Parameters :	
		id               The Thermal sensor ID (1: CPU , 2~N Mainboard, N+1 , N+2 ..PSU)
		* threshhold_info  threshhold values.
		 
	Returns
		1 get without error.
		-1 An error condition.	
*/

int rfpal_thermal_thresholds_get(unsigned id, rfpal_thresholds *threshold_info)
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_thermal_thresholds_com_get(id, threshold_info);
	return rv;
}



/*
int rfpal_psu_num_get()	

	Get PSU total number.
	
	Parameters : 
		none

	Returns : 
		>0 Get PSU sensor count.
		-1 An error condition.
*/

int rfpal_psu_num_get()
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_psu_num_com_get();
	return rv;
}

/*
int rfpal_psu_info_get( unsigned id , rfpal_psu_info * p_psu_info);

  Get individual psu information and status.
  
	Parameters :	
		id          The psu ID.
		p_psu_info  Pointer to an allocated rfpal_psu_info_t memory to store data. 

	Returns
		 1 get without error.
		-1 An error condition.			
*/

int rfpal_psu_info_get(unsigned id, rfpal_psu_info *p_psu_info)
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_psu_info_com_get(id, p_psu_info);
	return rv;
}

/*
int rfpal_thermal_info_get ( unsigned id , rfpal_thermal_info  * p_thermal_info)	
  
  Get individual thermal sensor information and status.

	Parameters :	
		id              The Thermal sensor ID (1: CPU , 2~N Mainboard, N+1 , N+2 ..PSU).
		p_thermal_info  pointer to an allocated rfpal_thermal_info_t memory to store data.  
		 
	Returns
		1  Get without error.
		-1 An error condition.	
*/

int rfpal_thermal_info_get(unsigned id, rfpal_thermal_info *p_thermal_info)
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_thermal_info_com_get(id, p_thermal_info);
	return rv;
}

/*
int rfpal_sys_resource_thresholds_get (ecrf_sys_threshold_info_t type , rfpal_thresholds *threshhold_info )	
  
  Get system resource /memory/cpu/storage usage threshhold value.

	Parameters :	
	    ecrf_sys_threshold_info_t type	
		* threshhold_info  threshhold values.
		 
	Returns
		1 get without error.
		-1 An error condition.	
*/
int rfpal_sys_resource_thresholds_get(ecrf_sys_threshold_info_t type, rfpal_thresholds *threshold_info)
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_sys_thresholds_com_get(type, threshold_info);
	return rv;
}

/*
int rfpal_sys_resource_thresholds_set (ecrf_sys_threshold_info_t type , rfpal_thresholds threshhold_info )	
  
  Set system resource /memory/cpu/storage usage threshhold value.

	Parameters :	
	    ecrf_sys_threshold_info_t type	
		threshhold_info  threshhold values.
		 
	Returns
		1 get without error.
		-1 An error condition.	
*/
int rfpal_sys_resource_thresholds_set(ecrf_sys_threshold_info_t type, rfpal_thresholds threshold_info)
{
	auto &ecrf_pal_com_r = ecrf_pal_com::ecrf_pal_com::get_instance();
	int rv = ecrf_pal_com_r.rfpal_sys_thresholds_com_set(type, threshold_info);
	return rv;
}

} // namespace ecrf_pal_com_api