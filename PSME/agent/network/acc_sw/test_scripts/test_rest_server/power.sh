#!/bin/bash
export srv_ip=172.17.8.41:8888
token=`cat CURRENT_TOKEN`
export $token

for i in $(seq 1 $1)
  do
    date +%s
    rm O_CHASSIS_POWER
    curl -k -H "X-Auth-Token: $token" -X GET https://${srv_ip}/redfish/v1/Chassis/1/Power -o O_CHASSIS_POWER
    if [ 0 -eq $? ]; then 
	string=`cat O_CHASSIS_POWER`
	if [[ $string == *"Collection of Power"* ]]; then
	  echo "GET Chassis power $i times OK!"
	else
	   echo "GET Chassis power $i times NG! NO content!!"
	   exit
	fi
    else
	echo "GET Chassis power $i times NG!"
	exit
    fi;
    date +%s
    echo "----"
done
