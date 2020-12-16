#!/bin/bash
rfip=172.17.8.213:8888
curl --insecure -X POST -D headers.txt https://${rfip}/redfish/v1/SessionService/ -d '{"ServiceEnabled":true,"SessionTimeout":600}'
robot -v OPENBMC_HOST:${rfip} ./Redfish_Service_Validator.robot 
