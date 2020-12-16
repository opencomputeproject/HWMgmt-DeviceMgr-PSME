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
service rmm stop

sleep 3

PIDSERVICE=`pidof netserve`
if [ "$PIDSERVICE" != "" ];then

PLATFORM=`cat /etc/onl/platform`
if [ "$PLATFORM" = "arm-accton-as4610-54-r0" ];then
             ifconfig Drawer1 down
             ifconfig Drawer2 down
             ifconfig Drawer3 down
             ifconfig Drawer4 down            
             ifconfig Drawer5 down
             ifconfig Drawer6 down
             ifconfig Drawer7 down
             ifconfig Drawer8 down              
             ifconfig Drawer9 down
             ifconfig Drawer10 down
             ifconfig Drawer11 down
             ifconfig Drawer12 down            
             ifconfig Drawer13 down
             ifconfig Drawer14 down
             ifconfig Drawer15 down
             ifconfig Drawer16 down  
             ifconfig Drawer17 down
             ifconfig Drawer18 down
             ifconfig Drawer19 down
             ifconfig Drawer20 down            
             ifconfig Drawer21 down
             ifconfig Drawer22 down
             ifconfig Drawer23 down
             ifconfig Drawer24 down              
             ifconfig Drawer25 down
             ifconfig Drawer26 down
             ifconfig Drawer27 down
             ifconfig Drawer28 down            
             ifconfig Drawer29 down
             ifconfig Drawer30 down
             ifconfig Drawer31 down
             ifconfig Drawer32 down  
             ifconfig Drawer33 down
             ifconfig Drawer34 down
             ifconfig Drawer35 down                          
             ifconfig Drawer36 down
             ifconfig Drawer37 down
             ifconfig Drawer38 down            
             ifconfig Drawer39 down
             ifconfig Drawer40 down
             ifconfig Drawer41 down
             ifconfig Drawer42 down  
             ifconfig Drawer43 down
             ifconfig Drawer44 down
             ifconfig Drawer45 down                          
             ifconfig Drawer46 down              
             ifconfig Drawer47 down   
             ifconfig Drawer48 down      
             acc_sw "knetctrl netif destroy 1" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 2" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 3" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 4" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 5" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 6" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 7" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 8" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 9" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 10" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 11" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 12" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 13" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 14" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 15" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 16" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 17" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 18" > /dev/null 2>&1            
             acc_sw "knetctrl netif destroy 19" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 20" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 21" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 22" > /dev/null 2>&1           
             acc_sw "knetctrl netif destroy 23" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 24" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 25" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 26" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 27" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 28" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 29" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 30" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 31" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 32" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 33" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 34" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 35" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 36" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 37" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 38" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 39" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 40" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 41" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 42" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 43" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 44" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 45" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 46" > /dev/null 2>&1            
             acc_sw "knetctrl netif destroy 47" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 48" > /dev/null 2>&1



elif [ "$PLATFORM" = "arm-accton-as4610-30-r0" ];then

             ifconfig Drawer1 down
             ifconfig Drawer2 down
             ifconfig Drawer3 down
             ifconfig Drawer4 down            
             ifconfig Drawer5 down
             ifconfig Drawer6 down
             ifconfig Drawer7 down
             ifconfig Drawer8 down              
             ifconfig Drawer9 down
             ifconfig Drawer10 down
             ifconfig Drawer11 down
             ifconfig Drawer12 down            
             ifconfig Drawer13 down
             ifconfig Drawer14 down
             ifconfig Drawer15 down
             ifconfig Drawer16 down  
             ifconfig Drawer17 down
             ifconfig Drawer18 down
             ifconfig Drawer19 down
             ifconfig Drawer20 down            
             ifconfig Drawer21 down
             ifconfig Drawer22 down
             ifconfig Drawer23 down
             ifconfig Drawer24 down   
             acc_sw "knetctrl netif destroy 1" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 2" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 3" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 4" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 5" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 6" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 7" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 8" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 9" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 10" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 11" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 12" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 13" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 14" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 15" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 16" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 17" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 18" > /dev/null 2>&1             
             acc_sw "knetctrl netif destroy 19" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 20" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 21" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 22" > /dev/null 2>&1            
             acc_sw "knetctrl netif destroy 23" > /dev/null 2>&1
             acc_sw "knetctrl netif destroy 24" > /dev/null 2>&1


fi


    kill -9 $PIDSERVICE
fi

PIDBCM=`pidof example_drivshell`
if [ "$PIDBCM" != "" ];then
    kill -9 $PIDBCM
fi

RESULT=`lsmod | grep linux_bcm_knet -m1`
if [ "$RESULT" != "" ];then
    rmmod linux_bcm_knet
fi

RESULT=`lsmod | grep linux_user_bde -m1`
if [ "$RESULT" != "" ];then
    rmmod linux_user_bde
fi

RESULT=`lsmod | grep linux_kernel_bde -m1`
if [ "$RESULT" != "" ];then
    rmmod linux_kernel_bde
fi


RESULT=`lsmod | grep linux_uk_proxy -m1`
if [ "$RESULT" != "" ];then
    rmmod linux_uk_proxy
fi

