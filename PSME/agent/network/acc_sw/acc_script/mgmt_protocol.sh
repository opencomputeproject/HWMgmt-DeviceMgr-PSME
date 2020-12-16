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
PlatformPath="/etc/onl/platform"



help_p()
{
echo "get ssh_status"
echo "get ssh_port"
echo "get snmp_status"
echo "get snmp_port"
}

case "${1}" in

"get")
    case "${2}" in
    "ssh_status")

    if [ -f "${PlatformPath}" ];then
    #For ONL
        STATUS=`service ssh status | grep running`
        if [ "$STATUS" = "sshd is running." ];then
            echo Enable 
        else
            echo Disable 
        fi
    else
    #For SONiC 
        SSH_PID=`pidof sshd`
        if [ "$SSH_PID" = "" ];then
            echo Disable 
        else
            echo Enable 
        fi
    fi
    ;;

    "ssh_port")

    if [ -f "${PlatformPath}" ];then
    #For ONL
        PORT=`cat /etc/ssh/sshd_config  | grep Port | awk '{print $2}'`
    else
    #For SONiC 
        PORT=`netstat -lnutp  | grep sshd | awk -F' ' '{print $4}'  | awk -F':' '{print $2}'`
    fi

    echo $PORT

    ;;

    "snmp_status")

    if [ -f "${PlatformPath}" ];then
    #For ONL
        STATUS=`service snmpd status | grep running`
        if [ "$STATUS" = "snmpd is running." ];then
            echo Enable 
        else
            echo Disable 
        fi
    else
    #For SONiC 
        SNMP_PID=`pidof snmpd`
        if [ "$SNMP_PID" = "" ];then
            echo Disable 
        else
            echo Enable 
        fi
    fi
    ;;

    "snmp_port")

    if [ -f "${PlatformPath}" ];then
    #For ONL
        PORT=`cat /etc/snmp/snmpd.conf  | grep 'agentAddress udp' | awk -F',' '{print $1}' | awk -F' ' '{print $2}' | sed -s 's/udp://g'`
    else
    #For SONiC 
        PORT=`netstat -lnutp  | grep snmpd | awk -F' ' '{print $4}'  | awk -F':' '{print $2}'`
    fi

    echo $PORT
    ;;
 
    esac
;;

*)
    help_p
;;
esac
