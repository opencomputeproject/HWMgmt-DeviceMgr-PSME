#!/bin/bash
#rfip=172.17.10.7:8888
#rfip=172.17.8.122:8888
rfip=172.17.10.6:8888
#Enable seesion service#
curl --insecure -X POST -D headers.txt https://${rfip}/redfish/v1/SessionService/ -d '{"ServiceEnabled":true,"SessionTimeout":600}'
robot  -v OPENBMC_HOST:${rfip} redfish/eit/test_eit_psme.robot
