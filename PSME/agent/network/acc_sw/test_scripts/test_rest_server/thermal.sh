#!/bin/bash
export srv_ip=172.17.8.41:8888
token=`cat CURRENT_TOKEN`
export $token

for i in $(seq 1 $1)
  do
    date +%s
    rm O_CHASSIS_THERMAL
    curl -k -H "X-Auth-Token: $token" -X GET https://${srv_ip}/redfish/v1/Chassis/1/Thermal -o O_CHASSIS_THERMAL
    if [ 0 -eq $? ]; then 
	string=`cat O_CHASSIS_THERMAL`
	if [[ $string == *"Collection of Thermal Sensors"* ]]; then
	  echo "GET Chassis thermal $i times OK!"
	else
	   echo "GET Chassis thermal $i times NG! NO content!!"
	   exit
	fi
    else
	echo "GET Chassis thermal $i times NG!"
	exit
    fi;
    date +%s
    echo "----"
done
