#!/bin/bash
export srv_ip=172.17.8.41:8888
curl -k -X GET https://${srv_ip}/redfish/v1/SessionService/
curl -k -X POST -d '{ "ServiceEnabled": true,"SessionTimeout":600}' https://${srv_ip}/redfish/v1/SessionService/
r=`rm headers;curl --insecure -X POST -D headers.txt https://${srv_ip}/redfish/v1/SessionService/Sessions -d '{"UserName":"admin","Password":"redfish"}'`
token=`cat headers.txt | grep X-Auth-Token | awk -F ": " {'print $2'}`
export $token;echo "$token" > CURRENT_TOKEN
curl -k -H "X-Auth-Token: $token" -X  GET https://${srv_ip}/redfish/v1/SessionService/

for j in {1..100};
do
  echo "//////////////////[$j]/////////////////////////"

  for i in {1..74};
  do
    date +%s

    curl -k -H "X-Auth-Token: $token" -X GET https://${srv_ip}/redfish/v1/EthernetSwitches/1/Ports/$i -o O_PORT
    if [ 0 -eq $? ]; then 
	string=`cat O_PORT`
	if [[ $string == *"Ethernet Switch Port description"* ]]; then
	  echo "GET PORT $i OK!"
	else
	   echo "GET PORT $i times NG! NO content!!"
	   exit
	fi
    else
	echo "Test $j time and GET PORT[$i] NG!"
	exit
    fi;

    date +%s
    echo "----"
  done
done
