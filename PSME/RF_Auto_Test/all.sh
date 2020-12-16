#!/bin/bash
####################################
#
# ${1} DUT's IP address.
# ${2} Event listener's IP address.
# ${3} PSME version want to test.
#
####################################

if [ "${1}" == "" ];then
    rfip=172.17.8.122:8888
    echo "Use default target device IP ${rfip}"
else
    echo "Use target device IP ${1}"
    rfip=${1}
fi

if [ "${2}" == "" ];then
    listener=172.17.10.60:8889
    echo "Use default listener IP ${listener}"
else
    echo "Use listener IP ${2}"
    listener=172.17.10.60:8889
fi

if [ "${3}" == "" ];then
    FirmwareVersion=2.1.3.59.21
    echo "PSME FirmwareVersion ${FirmwareVersion}"
else
    FirmwareVersion=${3}
    echo "PSME FirmwareVersion ${FirmwareVersion}"
fi

testip=`echo $rfip | awk -F ":" '{print $1}'`

####################################
rm output.xml;rm report.html;rm -rf OCP-Profiles redfish-Service-Validator Redfish-Service-Validator log* SchemaFiles
curl --insecure -X PATCH -D headers.txt https://${rfip}/redfish/v1/Managers/1/LogServices/1/ -d '{"ServiceEnabled":true}'
curl --insecure -X POST -D headers.txt https://${rfip}/redfish/v1/SessionService/ -d '{"ServiceEnabled":true,"SessionTimeout":600}'
robot -v OPENBMC_HOST:${rfip} -v LISTENER_HOST:${listener} -v FWV:${FirmwareVersion} -v TESTIP:${testip} redfish/

