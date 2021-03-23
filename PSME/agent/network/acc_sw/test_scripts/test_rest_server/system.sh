#!/bin/bash
export srv_ip=172.17.8.41:8888
token=`cat CURRENT_TOKEN`
export $token

for i in $(seq 1 $1)
  do
    date +%s
    curl -k -H "X-Auth-Token: $token" -X GET https://${srv_ip}/redfish/v1/Systems/1 -o O_SYSTEM
    if [ 0 -eq $? ]; then 
	string=`cat O_SYSTEM`
	if [[ $string == *"Computer System description"* ]]; then
	  echo "GET System $i times OK!"
	else
	   echo "GET System $i times NG! NO content!!"
	   exit
	fi
    else
	echo "GET System $i times NG!"
	exit
    fi;
    date +%s
    echo "----"
done
