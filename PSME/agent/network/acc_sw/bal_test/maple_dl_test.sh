#!/bin/sh
i=0
while [ 1 ];
do
echo "start dev_mgmt"
/etc/init.d/dev_mgmt start
echo "run test"
./unittest_psme-chassis_acc_api_bal_dist_test
#r=`./unittest_psme-chassis_acc_api_bal_dist_test | grep "Maple Download Finished"`
#echo $r
echo "test done"
/etc/init.d/dev_mgmt stop
sleep 5
i=$(($i+1))
date
echo "[$i]time"
done
