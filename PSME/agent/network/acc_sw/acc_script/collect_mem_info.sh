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

cmd_array=("MSIZE" "TWIDTH" "DWIDTH" "FFACTOR" "LOCATOR" "BLOCATOR" "TYPE" "TYPED" "SPEED" "MANU" "SERIALN" "ASSETTAG" "PNUM" "RANK" "CCS" "MVOLT" "MAVOLT" "CAVOLT" )

if [ "$HW_type" = "arm-accton-as4610-54-r0" ];then
    W_C=1
    device_num=1
    let mem$W_C[2]=2048
    let mem$W_C[3]=32 #id 3 for Total Width 
    eval mem$W_C[4]=32 #id 4 for Data Width 
    eval mem$W_C[5]="n/a" #id 5 for Form Factor 
    eval mem$W_C[6]="n/a"  #id 6 for Locator
    eval mem$W_C[7]="n/a"  #id 7 for Bank Locator
    eval mem$W_C[8]='DDR3' #id 8 for Type 
    eval mem$W_C[9]="n/a" #id 9 for Type Detail
    let mem$W_C[10]=1900 #id 10 for Speed 
    eval mem$W_C[11]="n/a" #id 11 for MANU 
    eval mem$W_C[12]="n/a" #id 12 for Serial Number 
    eval mem$W_C[13]="n/a" #id 13 for Asset Tag: 
    eval mem$W_C[14]="n/a" #id 14 for Part Number: 
    eval mem$W_C[15]="n/a" #id 15 for Rank 
    eval mem$W_C[16]=1900 #id 16 for Configured Clock Speed 
    eval mem$W_C[17]=1.3 #id 17 for Minimum voltage: 
    eval mem$W_C[18]=1.5 #id 18 for Maximum voltage: 
    eval mem$W_C[19]=1.3 #id 19 for Configured voltage: 
else

device_num=`dmidecode -t 16 | grep "Number Of Devices:" | awk -F'Devices:' '{print $2}' | sed -e 's/ //g'`
#echo num[$device_num]

W_C=0
memory_index=($(dmidecode -t 17 | grep DMI | awk -F' ' '{print $2}' | sed -s 's/,//g'))

for (( c=0; c < $device_num; c++ ))
do  
#    echo ${memory_index[$c]}
    id=${memory_index[$c]}
    content=`dmidecode -t 17 | grep "$id"  -A 23`    
# Show total content
# echo "[$content]" 
    MSIZE=`echo "[$content]" | grep "Size" | awk -F'Size: ' '{print $2}' | awk -F' ' '{print $1}'`
    if [ "${MSIZE}" != "No" ];then
        W_C=$(($W_C + 1))
        #echo [${W_C}]
    else
        continue
    fi 
    let mem$W_C[1]=${memory_index[$c]} #id 1 for index
    let mem$W_C[2]=${MSIZE}
    #echo ${MSIZE}
    TWIDTH=`echo "[$content]" | grep "Total Width:" | awk -F'Width: ' '{print $2}' | awk -F' ' '{print $1}'`
    let mem$W_C[3]=${TWIDTH} #id 3 for Total Width 
    #echo ${TWIDTH}
    DWIDTH=`echo "[$content]" | grep "Data Width:" | awk -F'Width: ' '{print $2}' | awk -F' ' '{print $1}'`
    eval mem$W_C[4]='${DWIDTH}' #id 4 for Data Width 
    #echo ${DWIDTH}
    FFACTOR=`echo "[$content]" | grep "Form Factor:" | awk -F'Factor: ' '{print $2}' | awk -F' ' '{print $1}'`
    eval mem$W_C[5]='${FFACTOR}' #id 5 for Form Factor 
    #echo ${FFACTOR}
    LOCATOR=`echo "[$content]" | grep "Locator:" -m1 | awk -F'Locator: ' '{print $2}' | awk -F' ' '{print $1}'`
    eval mem$W_C[6]='${LOCATOR}' #id 6 for Locator
    #echo ${LOCATOR}
    BLOCATOR=`echo "[$content]" | grep "Bank Locator:" | awk -F'Locator: ' '{print $2}' | awk -F' ' '{print $1$2}'`
    eval mem$W_C[7]='${BLOCATOR}' #id 7 for Bank Locator
    #echo ${BLOCATOR}
    TYPE=`echo "[$content]" | grep "Type:" -m1 | awk -F'Type: ' '{print $2}' | awk -F' ' '{print $1}'`
    eval mem$W_C[8]='${TYPE}' #id 8 for Type 
    #echo ${TYPE}
    TYPED=`echo "[$content]" | grep "Type Detail:" | awk -F'Detail: ' '{print $2}' | awk -F' ' '{print $1}'`
    eval mem$W_C[9]='${TYPED}' #id 9 for Type Detail
    #echo ${TYPED}
    SPEED=`echo "[$content]" | grep "Speed:" -m1 | awk -F'Speed: ' '{print $2}' | awk -F' ' '{print $1}'`
    let mem$W_C[10]=${SPEED} #id 10 for Speed 
    #echo ${SPEED}
    MANU=`echo "[$content]" | grep "Manufacturer:" | awk -F'Manufacturer: ' '{print $2}' | awk -F' ' '{print $1$2}'`
    eval mem$W_C[11]='${MANU}' #id 11 for MANU 
    #echo ${MANU}
    SERIALN=`echo "[$content]" | grep "Serial Number:" | awk -F'Serial Number: ' '{print $2}' | awk -F' ' '{print $1$2}'`
    eval mem$W_C[12]='${SERIALN}' #id 12 for Serial Number 
    #echo ${SERIALN}
    ASSETTAG=`echo "[$content]" | grep "Asset Tag:" | awk -F'Asset Tag: ' '{print $2}' | awk -F' ' '{print $1$2}'`
    eval mem$W_C[13]='${ASSETTAG}' #id 13 for Asset Tag: 
    #echo ${ASSETTAG}
    PNUM=`echo "[$content]" | grep "Part Number:" | awk -F'Part Number: ' '{print $2}' | awk -F' ' '{print $1$2}'`
    eval mem$W_C[14]='${PNUM}' #id 14 for Part Number: 
    #echo ${PNUM}
    RANK=`echo "[$content]" | grep "Rank:" | awk -F'Rank: ' '{print $2}' | awk -F' ' '{print $1}'`
    eval mem$W_C[15]='${RANK}' #id 15 for Rank 
    #echo ${RANK}
    CCS=`echo "[$content]" | grep "Configured Clock Speed:" | awk -F'Speed: ' '{print $2}' | awk -F' ' '{print $1}'`
    eval mem$W_C[16]='${CCS}' #id 16 for Configured Clock Speed 
    #echo ${CCS}
    MVOLT=`echo "[$content]" | grep "Minimum voltage:" | awk -F'Minimum voltage:  ' '{print $2}' | awk -F' ' '{print $1}'`
    eval mem$W_C[17]='${MVOLT}' #id 17 for Minimum voltage: 
    #echo ${MVOLT}
    MAVOLT=`echo "[$content]" | grep "Maximum voltage:" | awk -F'Maximum voltage:' '{print $2}' | awk -F' ' '{print $1}'`
    eval mem$W_C[18]='${MAVOLT}' #id 18 for Maximum voltage: 
    #echo ${MAVOLT}
    CAVOLT=`echo "[$content]" | grep "Configured voltage:" | awk -F'Configured voltage:' '{print $2}' | awk -F' ' '{print $1}'`
    eval mem$W_C[19]='${CAVOLT}' #id 19 for Configured voltage: 
    #echo ${CAVOLT}
done


fi



help_p()
{
    for (( i=1; i < ${#cmd_array[@]} ; i++ ))
    do 
        echo "collect_mem_info.sh" "[1..4]" ${cmd_array[$i]}
    done
}


if [ "$#" -eq 1 ]; then
    case "$1" in
    "MCOUNT")
    echo $W_C;
    exit
    ;;
    esac
fi

case "$2" in

"MCOUNT")
echo $W_C;
;;

"MSIZE")
var=mem$1[2]
echo ${!var}
;;
"TWIDTH")
var=mem$1[3]
echo ${!var}
;;
"DWIDTH")
var=mem$1[4]
echo ${!var}
;;
"FFACTOR")
var=mem$1[5]
echo ${!var}
;;
"LOCATOR")
var=mem$1[6]
echo ${!var}
;;
"BLOCATOR")
var=mem$1[7]
echo ${!var}
;;
"TYPE")
var=mem$1[8]
echo ${!var}
;;
"TYPED")
var=mem$1[9]
echo ${!var}
;;
"SPEED")
var=mem$1[10]
echo ${!var}
;;
"MANU")
var=mem$1[11]
echo ${!var}
;;
"SERIALN")
var=mem$1[12]
echo ${!var}
;;
"ASSETTAG")
var=mem$1[13]
echo ${!var}
;;
"PNUM")
var=mem$1[14]
echo ${!var}
;;
"RANK")
var=mem$1[15]
echo ${!var}
;;
"CCS")
var=mem$1[16]
echo ${!var}
;;
"MVOLT")
var=mem$1[17]
echo ${!var}
;;
"MAVOLT")
var=mem$1[18]
echo ${!var}
;;
"CAVOLT")
var=mem$1[19]
echo ${!var}
;;

*)
help_p
;;

esac


