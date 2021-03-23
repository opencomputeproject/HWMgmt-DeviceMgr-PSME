#!/bin/bash
# Edgecore DeviceManager
# Copyright 2020-2021 Edgecore Networks, Inc.
#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.
# To get/set HW device info.
#
show_help()
{
    echo "psme.sh get max_fan_num                  : get current max fan num                 "
    echo "psme.sh get thermal_sensor_num           : get current thermal sensor number       "
    echo "psme.sh get max_psu_num                  : get current max psu  num                "
    echo "psme.sh get max_port_num                 : get current max port num                "
    echo "psme.sh set force_off                    : set device force off                    "
    echo "psme.sh set shutdown                     : set device shutdown gracefully          "
    echo "psme.sh set force_restart                : set device force restart                "
    echo "psme.sh set restart                      : set device restart gracefully           "
    echo "psme.sh get mgmt_port_name               : get management port name                "
    echo "psme.sh get nos_type                     : get nos type                            "
    echo "psme.sh get rest_server_port             : get rest server port                    "
}

if [ $# -lt 1 ]; then
   show_help 
fi

PlatformPath="/etc/onl/platform"

if [ -f "${PlatformPath}" ];then
    #Real Platform
    HW_type=`cat  ${PlatformPath}`
    ONLPDUMP="/lib/platform-config/${HW_type}/onl/bin/onlpdump"
    REST_SERVER_PORT=8888
else
    HW_type=`/usr/local/bin/sonic-cfggen -d --print-data | grep "platform" | awk '{print $2}' |awk -F '"' '{print $2}'`
    ONLPDUMP="/usr/bin/decode-syseeprom"
    REST_SERVER_PORT=8889
fi

# Common Used #

update_sw_volt()
{
    echo "NOT support."
}

set_forceoff()
{
    echo "NOT support."
}

set_forcerestart()
{
    echo "NOT support."
}

get_max_fan_num()
{
    echo "${CURRENT_FAN_NUM}"
}

. /etc/psme/HW_TYPE

DBG=0

cmd_method=$1
cmd_node=$2

if [ "${DBG}" = 1 ];then
    echo "Platform[${HW_type}]"
fi

CURRENT_FAN_NUM=${#fans_type[@]}
CURRENT_THERMAL_NUM=${#thermals_type[@]}
CURRENT_PSU_NUM=${#psus_type[@]}
CURRENT_PORT_NUM=${#mapping[@]}

get_fan_number()
{
	echo "${CURRENT_FAN_NUM}"
}

get_thermal_sensor_num()
{
    echo "${CURRENT_THERMAL_NUM}"
}

get_max_psu_num()
{
	echo "${CURRENT_PSU_NUM}"
}

get_max_port_num()
{
	echo "${CURRENT_PORT_NUM}"
}

get_inf_vlans_count()
{
	echo `ip -d link show | grep "${1}\." | wc -l` 
}

get_inf_vlans_value()
{
	res=`ip -d link show | grep "$1\." | awk '{print $2}'| sed -e s/$1.//g | awk -F'@' '{print $1}'`
	ARR=($res)
	id=$(($2-1))
	echo ${ARR[$id]}
}

##################################################
# ONL/SONiC has differnet management port name 
# ONL system use onlpdump to get eeprom info. 
# SONiC system use decode-syseeprom to get inf. 
##################################################
get_mgmt_port_name()
{
	PlatformPath="/etc/onl/platform"
	if [ -f "${PlatformPath}" ];then
	    #ONL system
		echo "ma1"
	elif [ -f "/usr/bin/decode-syseeprom" ];then
	    #SONiC system
		echo "eth0"
	else
		echo "ma1"
	fi
}

##################################################
# Get NOS type 
##################################################
get_nos_type()
{
	PlatformPath="/etc/onl/platform"
	if [ -f "${PlatformPath}" ];then
	    #ONL system
		echo "onl"
	else
		echo "sonic"
	fi
}

##################################################
# Get Rest_Server_Port 
##################################################
get_rest_server_port()
{
	echo "$REST_SERVER_PORT" 
}

##################################################
# Set command 
##################################################

set_shutdown()
{
    sync;sync; shutdown -h now;
}

set_restart()
{
    sync;sync; shutdown -r now;
}

case "${1}" in
"get")  
	case "${2}" in
	"thermal_sensor")
	get_thermal_sensor
	;;
	"thermal_sensor_num")
	get_thermal_sensor_num
	;;
	"max_fan_num")
	get_max_fan_num
        ;;
	"max_port_num")
	get_max_port_num
        ;;
	"max_psu_num")
	get_max_psu_num
        ;;
	"inf_vlans_count")
	get_inf_vlans_count "${3}"
	;;
	"inf_vlans_value")
	get_inf_vlans_value "${3}" "${4}"
	;;
	"mgmt_port_name")
	get_mgmt_port_name 
	;;
	"nos_type")
	get_nos_type 
	;;
	"rest_server_port")
	get_rest_server_port 
	;;
	esac
	;;
"set")  
	case "${2}" in
	"force_off")
        set_forceoff	
	;;
	"shutdown")
        set_shutdown	
	;;
	"force_restart")
        set_forcerestart	
	;;
	"restart")
        set_restart
	;;
	esac
	;;
"help")  
   	show_help 
	;;
esac
