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
PLATFORM=`cat /etc/onl/platform`
KO_PATH=/usr/bin/opennsl-accton/examples


if [ -d /usr/bin/opennsl-accton/examples ];then

if [ "$PLATFORM" = "arm-accton-as4610-54-r0" -o  "$PLATFORM" = "arm-accton-as4610-30-r0" ];then

    /usr/bin/opennsl_setup

    PIDOSERVICE=`pidof netserve`
   
    if [ "$PIDOSERVICE" = "" ];then
        cd $KO_PATH 
        netserve -d 2001 example_drivshell > /dev/null 2>&1
        sleep 2
        
        if [ "$PLATFORM" = "arm-accton-as4610-54-r0" ];then
             acc_sw "knetctrl netif create port=26 IFName=Drawer1" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=1 StripTag=yes IngPort=26" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=25 IFName=Drawer2" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=2 StripTag=yes IngPort=25" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=27 IFName=Drawer3" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=3 StripTag=yes IngPort=27" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=28 IFName=Drawer4" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=4 StripTag=yes IngPort=28" > /dev/null 2>&1             
             acc_sw "knetctrl netif create port=30 IFName=Drawer5" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=5 StripTag=yes IngPort=30" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=29 IFName=Drawer6" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=6 StripTag=yes IngPort=29" > /dev/null 2>&1                  
             acc_sw "knetctrl netif create port=31 IFName=Drawer7" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=7 StripTag=yes IngPort=31" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=32 IFName=Drawer8" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=8 StripTag=yes IngPort=32" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=34 IFName=Drawer9" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=9 StripTag=yes IngPort=34" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=33 IFName=Drawer10" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=10 StripTag=yes IngPort=33" > /dev/null 2>&1             
             acc_sw "knetctrl netif create port=35 IFName=Drawer11" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=11 StripTag=yes IngPort=35" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=36 IFName=Drawer12" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=12 StripTag=yes IngPort=36" > /dev/null 2>&1                            
             acc_sw "knetctrl netif create port=38 IFName=Drawer13" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=13 StripTag=yes IngPort=38" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=37 IFName=Drawer14" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=14 StripTag=yes IngPort=37" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=39 IFName=Drawer15" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=15 StripTag=yes IngPort=39" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=40 IFName=Drawer16" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=16 StripTag=yes IngPort=40" > /dev/null 2>&1             
             acc_sw "knetctrl netif create port=42 IFName=Drawer17" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=17 StripTag=yes IngPort=42" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=41 IFName=Drawer18" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=18 StripTag=yes IngPort=41" > /dev/null 2>&1                  
             acc_sw "knetctrl netif create port=43 IFName=Drawer19" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=19 StripTag=yes IngPort=43" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=44 IFName=Drawer20" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=20 StripTag=yes IngPort=44" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=46 IFName=Drawer21" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=21 StripTag=yes IngPort=46" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=45 IFName=Drawer22" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=22 StripTag=yes IngPort=45" > /dev/null 2>&1             
             acc_sw "knetctrl netif create port=47 IFName=Drawer23" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=23 StripTag=yes IngPort=47" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=48 IFName=Drawer24" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=24 StripTag=yes IngPort=48" > /dev/null 2>&1               

             acc_sw "knetctrl netif create port=2 IFName=Drawer25" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=25 StripTag=yes IngPort=2" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=1 IFName=Drawer26" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=26 StripTag=yes IngPort=1" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=3 IFName=Drawer27" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=27 StripTag=yes IngPort=3" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=4 IFName=Drawer28" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=28 StripTag=yes IngPort=4" > /dev/null 2>&1             
             acc_sw "knetctrl netif create port=6 IFName=Drawer29" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=29 StripTag=yes IngPort=6" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=5 IFName=Drawer30" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=30 StripTag=yes IngPort=5" > /dev/null 2>&1                  
             acc_sw "knetctrl netif create port=7 IFName=Drawer31" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=31 StripTag=yes IngPort=7" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=8 IFName=Drawer32" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=32 StripTag=yes IngPort=8" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=10 IFName=Drawer33" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=33 StripTag=yes IngPort=10" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=9 IFName=Drawer34" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=34 StripTag=yes IngPort=9" > /dev/null 2>&1             
             acc_sw "knetctrl netif create port=11 IFName=Drawer35" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=35 StripTag=yes IngPort=11" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=12 IFName=Drawer36" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=36 StripTag=yes IngPort=12" > /dev/null 2>&1  
             
             acc_sw "knetctrl netif create port=14 IFName=Drawer37" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=37 StripTag=yes IngPort=14" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=13 IFName=Drawer38" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=38 StripTag=yes IngPort=13" > /dev/null 2>&1             
             acc_sw "knetctrl netif create port=15 IFName=Drawer39" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=39 StripTag=yes IngPort=15" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=16 IFName=Drawer40" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=40 StripTag=yes IngPort=16" > /dev/null 2>&1                  
             acc_sw "knetctrl netif create port=18 IFName=Drawer41" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=41 StripTag=yes IngPort=18" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=17 IFName=Drawer42" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=42 StripTag=yes IngPort=17" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=19 IFName=Drawer43" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=43 StripTag=yes IngPort=19" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=20 IFName=Drawer44" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=44 StripTag=yes IngPort=20" > /dev/null 2>&1             
             acc_sw "knetctrl netif create port=22 IFName=Drawer45" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=45 StripTag=yes IngPort=22" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=21 IFName=Drawer46" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=46 StripTag=yes IngPort=21" > /dev/null 2>&1               
             acc_sw "knetctrl netif create port=23 IFName=Drawer47" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=47 StripTag=yes IngPort=23" > /dev/null 2>&1                
             acc_sw "knetctrl netif create port=24 IFName=Drawer48" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=48 StripTag=yes IngPort=24" > /dev/null 2>&1                   
             
             
             
             ifconfig Drawer1 up
             ifconfig Drawer2 up
             ifconfig Drawer3 up
             ifconfig Drawer4 up            
             ifconfig Drawer5 up
             ifconfig Drawer6 up
             ifconfig Drawer7 up
             ifconfig Drawer8 up              
             ifconfig Drawer9 up
             ifconfig Drawer10 up
             ifconfig Drawer11 up
             ifconfig Drawer12 up            
             ifconfig Drawer13 up
             ifconfig Drawer14 up
             ifconfig Drawer15 up
             ifconfig Drawer16 up  
             ifconfig Drawer17 up
             ifconfig Drawer18 up
             ifconfig Drawer19 up
             ifconfig Drawer20 up            
             ifconfig Drawer21 up
             ifconfig Drawer22 up
             ifconfig Drawer23 up
             ifconfig Drawer24 up              
             ifconfig Drawer25 up
             ifconfig Drawer26 up
             ifconfig Drawer27 up
             ifconfig Drawer28 up            
             ifconfig Drawer29 up
             ifconfig Drawer30 up
             ifconfig Drawer31 up
             ifconfig Drawer32 up  
             ifconfig Drawer33 up
             ifconfig Drawer34 up
             ifconfig Drawer35 up                          
             ifconfig Drawer36 up
             ifconfig Drawer37 up
             ifconfig Drawer38 up            
             ifconfig Drawer39 up
             ifconfig Drawer40 up
             ifconfig Drawer41 up
             ifconfig Drawer42 up  
             ifconfig Drawer43 up
             ifconfig Drawer44 up
             ifconfig Drawer45 up                          
             ifconfig Drawer46 up              
             ifconfig Drawer47 up              
             ifconfig Drawer48 up              
                                   
                                      
        else
             acc_sw "knetctrl netif create port=2 IFName=Drawer1" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=1 StripTag=yes IngPort=2" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=1 IFName=Drawer2" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=2 StripTag=yes IngPort=1" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=3 IFName=Drawer3" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=3 StripTag=yes IngPort=3" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=4 IFName=Drawer4" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=4 StripTag=yes IngPort=4" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=6 IFName=Drawer5" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=5 StripTag=yes IngPort=6" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=5 IFName=Drawer6" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=6 StripTag=yes IngPort=5" > /dev/null 2>&1                  
             acc_sw "knetctrl netif create port=7 IFName=Drawer7" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=7 StripTag=yes IngPort=7" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=8 IFName=Drawer8" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=8 StripTag=yes IngPort=8" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=10 IFName=Drawer9" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=9 StripTag=yes IngPort=10" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=9 IFName=Drawer10" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=10 StripTag=yes IngPort=9" > /dev/null 2>&1             
             acc_sw "knetctrl netif create port=11 IFName=Drawer11" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=11 StripTag=yes IngPort=11" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=12 IFName=Drawer12" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=12 StripTag=yes IngPort=12" > /dev/null 2>&1  
             
            acc_sw "knetctrl netif create port=14 IFName=Drawer13" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=13 StripTag=yes IngPort=14" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=13 IFName=Drawer14" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=14 StripTag=yes IngPort=13" > /dev/null 2>&1             
             acc_sw "knetctrl netif create port=15 IFName=Drawer15" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=15 StripTag=yes IngPort=15" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=16 IFName=Drawer16" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=16 StripTag=yes IngPort=16" > /dev/null 2>&1                  
             acc_sw "knetctrl netif create port=18 IFName=Drawer17" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=17 StripTag=yes IngPort=18" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=17 IFName=Drawer18" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=18 StripTag=yes IngPort=17" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=19 IFName=Drawer19" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=19 StripTag=yes IngPort=19" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=20 IFName=Drawer20" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=20 StripTag=yes IngPort=20" > /dev/null 2>&1             
             acc_sw "knetctrl netif create port=22 IFName=Drawer21" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=21 StripTag=yes IngPort=22" > /dev/null 2>&1
             acc_sw "knetctrl netif create port=21 IFName=Drawer22" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=22 StripTag=yes IngPort=21" > /dev/null 2>&1               
             acc_sw "knetctrl netif create port=23 IFName=Drawer23" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=23 StripTag=yes IngPort=23" > /dev/null 2>&1                
             acc_sw "knetctrl netif create port=24 IFName=Drawer24" > /dev/null 2>&1
             acc_sw "knetctrl filter create DestType=NetIF DestID=24 StripTag=yes IngPort=24" > /dev/null 2>&1                      
             
             
                          
             ifconfig Drawer1 up
             ifconfig Drawer2 up
             ifconfig Drawer3 up
             ifconfig Drawer4 up            
             ifconfig Drawer5 up
             ifconfig Drawer6 up
             ifconfig Drawer7 up
             ifconfig Drawer8 up              
             ifconfig Drawer9 up
             ifconfig Drawer10 up
             ifconfig Drawer11 up
             ifconfig Drawer12 up   
             ifconfig Drawer13 up
             ifconfig Drawer14 up
             ifconfig Drawer15 up
             ifconfig Drawer16 up  
             ifconfig Drawer17 up
             ifconfig Drawer18 up
             ifconfig Drawer19 up
             ifconfig Drawer20 up            
             ifconfig Drawer21 up
             ifconfig Drawer22 up
             ifconfig Drawer23 up   
             ifconfig Drawer24 up                                   
                                  
        fi

        acc_sw "fp init" > /dev/null 2>&1
	acc_sw "fp qset clear" > /dev/null 2>&1
	acc_sw 'fp qset add inports'
	acc_sw 'fp qset add DstMac'
	acc_sw "fp qset add stageingress" > /dev/null 2>&1
	acc_sw "fp group create 1 1" > /dev/null 2>&1
	acc_sw "fp entry create 1 1" > /dev/null 2>&1
	acc_sw 'fp qual 1 InPorts 0x2 0x7fffffffffffff'
	acc_sw 'fp qual 1 DstMac Data=01:80:C2:00:00:0E Mask=FF:FF:FF:FF:FF:FF'
	acc_sw "fp action add 1 redirect2cpu 0 0" > /dev/null 2>&1
	acc_sw "fp entry install 1" > /dev/null 2>&1
        acc_sw "vlan add 1 pbm=ge,xe ubm=ge,xe"  > /dev/null 2>&1

        service rmm start
    fi
fi

fi
