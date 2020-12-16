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
echo "get ipv6_count if_name"
echo "get ipv6_info if_name index"
echo "get gateway if_name"
}

case "${1}" in

"get")
    case "${2}" in
    "ipv6_count")

        if_name=${3}
        IFS=$'\n'
        IPV6_A=($(ifconfig "$if_name" | grep inet6))
        TOTAL_COUNT=${#IPV6_A[@]}
        echo $TOTAL_COUNT
    ;;

    "ipv6_info")
        if_name=${3}
        IFS=$'\n'

        IPV6_A=($(ip -6 addr show "$if_name" | grep inet6))
        TOTAL_COUNT=${#IPV6_A[@]}

        if [ -f /tmp/IPV6_INFO ];then
            rm  /tmp/IPV6_INFO
        fi

        TOTAL_COUNT=${#IPV6_A[@]}
        for (( c=0; c < $TOTAL_COUNT; c++ ))
        do
            #`echo ${IPV6_A[$c]} | awk -F' ' '{print $3" "$4" "$5}' >> /tmp/IPV6_INFO`
            `echo ${IPV6_A[$c]} |  awk -F' ' '{print $2" "$4" "$5" "$6" "$7}' >> /tmp/IPV6_INFO`
        done

        IPV6_A=($(cat /tmp/IPV6_INFO))
        
        IPV6ADDR="0::0"
        PREFIX_LEN="128"

        IPV6ADDR=`echo ${IPV6_A[$4]} | awk -F'/' '{print $1}'`
        PREFIX_LEN=`echo ${IPV6_A[$4]} | awk -F'/' '{print $2}' | awk -F' ' '{print $1}'`
        TYPE=`echo ${IPV6_A[$4]} | awk -F' ' '{print $2" "$3" "$4}'`
#echo "TYPE[$TYPE]"
        if [ "$TYPE" = "link " ];then
            FTYPE="LinkLocal"
        elif [ "$TYPE" = "global " ];then
            FTYPE="Static"
        elif [[ "$TYPE" =~ "global  " ]] || [[ "$TYPE" =~ "global deprecated" ]] ;then
            FTYPE="DHCPv6"
        elif [ "$TYPE" = "global mngtmpaddr dynamic" ];then
            FTYPE="SLAAC"
        else 
            FTYPE="LinkLocal"
        fi

        echo $IPV6ADDR $PREFIX_LEN $FTYPE
    ;;


    "gateway")
        if_name=${3}
        IPV6GATEWAY=`ip -6 route | egrep default | grep "$if_name" | awk -F' ' '{print $3}'`
        echo $IPV6GATEWAY
    ;;
    esac
;;

*)
    help_p
;;
esac
