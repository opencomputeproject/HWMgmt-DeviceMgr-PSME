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

if [ -f "${PlatformPath}" ];then
HW_type=`cat  ${PlatformPath}`
else
    HW_type=`/usr/local/bin/sonic-cfggen -d --print-data | grep "platform" | awk '{print $2}' |awk -F '"' '{print $2}'`
fi

cmd_array=("NAME" "MSIZE" "SERIAL" "MODEL_ID" "REVISION")

if [ "$HW_type" = "arm-accton-as4610-54-r0" ];then
    c=1
    eval mem$c[1]=sda #id 1 for drv name 
    let mem$c[2]=8
    eval mem$c[3]="n/a" #id 3 for drv SERIAL 
    eval mem$c[4]="n/a"  #id 4 for drv ID_MODEL 
    eval mem$c[5]="n/a"  #id 5 for drv ID_REVISION 

else

device_num_r=`lsblk --nodeps -o name | grep -v loop* | grep -v NAME | wc -l`
device_num=`lsblk --nodeps -o name | wc -l`
device_num=$(($device_num-1))

memory_index=($(lsblk --nodeps -o name))
rc=1

for (( c=1; c <= $device_num; c++ ))
do  
    id=${memory_index[$c]}

    if [[ $id == *"loop"* ]]; then
	    continue
    fi

    content=`fdisk -l`    
#    echo $id
    content2=`udevadm info --query=all --name /dev/$id`
# Show total content
# echo "[$content2]" 
    MSIZE=`echo "[$content]" |  grep "Disk /dev/$id" | awk -F': ' '{print $2}' | awk -F' ' '{print $3}'`
    eval mem$rc[1]='${id}' #id 1 for drv name 
    let mem$rc[2]=${MSIZE}
#echo ${MSIZE}

    SERIAL=`echo "[$content2]" | grep ID_SERIAL= | awk -F'=' '{print $2}'` 
    eval mem$rc[3]='${SERIAL}' #id 3 for drv SERIAL 
#echo ${SERIAL}

    ID_MODEL=`echo "[$content2]" | grep ID_MODEL_ID= | awk -F'=' '{print $2}'` 
    eval mem$rc[4]='${ID_MODEL}' #id 4 for drv ID_MODEL 
#echo ${ID_MODEL}

    ID_REVISION=`echo "[$content2]" | grep ID_REVISION= | awk -F'=' '{print $2}'` 
    eval mem$rc[5]='${ID_REVISION}' #id 5 for drv ID_REVISION 
#echo ${ID_REVISION}
    rc=$(($rc+1))
done

fi

help_p()
{
    for (( i=0; i < ${#cmd_array[@]} ; i++ ))
    do 
        echo "collect_drv_info.sh" "[1..4]" ${cmd_array[$i]}
    done
}

if [ "$#" -eq 1 ]; then
    case "$1" in
    "MCOUNT")
    echo $device_num_r;
    exit
    ;;
    esac
fi

case "$2" in

"NAME")
var=mem$1[1]
echo ${!var}
;;

"MSIZE")
var=mem$1[2]
echo ${!var}
;;
"SERIAL")
var=mem$1[3]
echo ${!var}
;;

"MODEL_ID")
var=mem$1[4]
echo ${!var}
;;

"REVISION")
var=mem$1[5]
echo ${!var}
;;




*)
help_p
;;

esac


