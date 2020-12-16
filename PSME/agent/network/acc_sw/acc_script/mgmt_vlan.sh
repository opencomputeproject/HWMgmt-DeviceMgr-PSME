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

help_p()
{
echo "get vlan_port_count mgmt_name"
echo "get vlan_port_value mgmt_name index"
}

case "${1}" in

"get")
    case "${2}" in
    "vlan_port_count")
        port=${3}
        IFS=$'\n'
        VLAN_A=($(ifconfig -a | grep "$port"\\.))
        TOTAL_VLAN_COUNT=${#VLAN_A[@]}
        echo $TOTAL_VLAN_COUNT
    ;;

    "vlan_port_value")
        port=${3} index=${4}
        IFS=$'\n'
        VLAN_A=($(ifconfig -a | grep "$port"\\.))
        TOTAL_VLAN_COUNT=${#VLAN_A[@]}
        VLAN=`echo ${VLAN_A[$index]} | awk -F' ' '{print $1}' | sed -e "s/$port.//g"` 
        echo $VLAN
    ;;
    esac
;;

*)
    help_p
;;
esac
