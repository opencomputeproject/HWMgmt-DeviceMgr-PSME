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

. raw_omci_data

PON_NUM=16

#################################
PON_PORT=1
UPLINK_PORT=1
#################################


#################################
##Disable GPON NNI PORT
sleep 1 
res=`curl --insecure  -v  -X PATCH -d '{"AdministrativeState" : "Down"}'  https://172.17.8.6:8888/redfish/v1/EthernetSwitches/1/Ports/65/`

PON_ID=$(($PON_PORT-1))

NNI_PORT=$(($UPLINK_PORT+$PON_NUM))
NNI_ID=$(($NNI_PORT-17))

echo "NNI_ID[$NNI_ID]"
sleep 2

res=`curl --insecure  -v  -X PATCH -d '{"OltOperateState": true}'  https://"$1":8888/redfish/v1/Olt`

sleep 30 

res=`curl --insecure  -v  -X PATCH -d '{"AdministrativeState" : "Up"}'  https://"$1":8888/redfish/v1/EthernetSwitches/1/Ports/"$PON_PORT"/`


res=`curl --insecure  -v  -X PATCH -d '{"AdministrativeState" : "Up"}'  https://"$1":8888/redfish/v1/EthernetSwitches/1/Ports/"$NNI_PORT"/`

sleep 1 

res=`curl --insecure  -v  -X POST -d '{"onu_id":1,"vendor_id":"ISKT","vendor_specific":"71E80110"}' https://"$1":8888/redfish/v1/EthernetSwitches/1/Ports/"$PON_PORT"/ONUs `

echo "wait 10 secs"
sleep 10
echo "Create Flow!!!"

res=`curl --insecure  -v  -X POST -d '
{
    "onuId": 1,
    "FlowId": 16,
    "PortId": '$PON_ID',
    "NniId": '$NNI_ID',
    "UniId": 1,
    "FlowType": "upstream",
    "PktTagType": "single_tag",
    "GemportId": 1024,
    "Classifier": [
    "BCMOLT_CLASSIFIER_ID_O_VID"
    ],
    "Action": "BCMOLT_ACTION_ID_O_VID",
    "ActionCmd": "BCMOLT_ACTION_CMD_ID_ADD_OUTER_TAG",
    "ActionVal": {
    "OVid": 10,
    "OPbits": 0,
    "OTpid": 0,
    "IVid": 0,
    "IPbits": 0,
    "ITpid": 0,
    "EtherType": 0,
    "IpProto": 0,
    "SrcPort": 0,
    "DstPort": 0
},
"ClassVal": [
{
    "OVid": 20,
    "IVid": 0,      
    "OPbits": 0,
    "IPbits": 0,
    "EtherType": 0,
    "OTpid": 0,
    "IpProto": 0,
    "DstPort": 0,  
    "SrcPort": 0,          
    "ITpid": 0
}
]
}
' https://"$1":8888/redfish/v1/Olt/Flow` 

res=`curl --insecure  -v  -X POST -d '
{
    "onuId": 1,
    "FlowId": 16,
    "PortId": '$PON_ID',
    "NniId": '$NNI_ID',
    "UniId": 1,
    "FlowType": "downstream",
    "PktTagType": "double_tag",
    "GemportId": 1024,
    "Classifier": [
    "BCMOLT_CLASSIFIER_ID_O_VID",
    "BCMOLT_CLASSIFIER_ID_I_VID"
    ],
    "Action": "BCMOLT_ACTION_ID_O_VID",
    "ActionCmd": "BCMOLT_ACTION_CMD_ID_REMOVE_OUTER_TAG",
    "ActionVal": {
    "OVid": 10,
    "OPbits": 0,
    "OTpid": 0,
    "IVid": 0,
    "IPbits": 0,
    "ITpid": 0,
    "EtherType": 0,
    "IpProto": 0,
    "SrcPort": 0,
    "DstPort": 0
},
"ClassVal": [
{
    "OVid": 10,
    "IVid": 20,      
    "OPbits": 0,
    "IPbits": 0,
    "EtherType": 0,
    "OTpid": 0,
    "IpProto": 0,
    "DstPort": 0,  
    "SrcPort": 0,          
    "ITpid": 0
}
]
}
' https://"$1":8888/redfish/v1/Olt/Flow` 

echo "wait 5 seconds then send omci"
sleep 5 

omci_size=${#OMCI_RAW[@]}
echo "omci_size[$omci_size]"

i=0
while [ $i != "$omci_size" ]
do
    echo i=$i
res=`curl --insecure  -v  -X POST -d '
{
"Omci" : "'${OMCI_RAW[$i]}'"
}
' https://"$1":8888/redfish/v1/EthernetSwitches/1/Ports/"$PON_PORT"/ONUs/1/Omci
`
i=$(($i+1))
sleep 0.3
done


