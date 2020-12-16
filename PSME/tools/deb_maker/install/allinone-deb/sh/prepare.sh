#!/bin/bash
CUR_PATH=$(cd "$(dirname "$0")"; pwd)
PROJ_PATH=$CUR_PATH/../../../../
PSME_PROJ_PATH=$CUR_PATH/../../../
PSME_EC_SW_DIR="$PROJ_PATH/agent/network/acc_sw"
PSME_EC_SCRIPT_DIR="${PSME_EC_SW_DIR}/acc_script"
BAL_TEST="${PSME_EC_SW_DIR}/bal_test"
ITEM_PATH=$CUR_PATH/../psme-allinone

if [ "$1" = "arm" ];then
	LIB_PREINSTALL=/usr/lib/arm-linux-gnueabi
else
	LIB_PREINSTALL=/usr/lib/x86_64-linux-gnu
fi

function del_file()
{
    for del in $1*
    do
        if [ -f $del ]
        then
            rm -rf $del
        fi
    done
}

mkdir -p $ITEM_PATH/usr/local/bin
mkdir -p $ITEM_PATH/usr/local/sbin
mkdir -p $ITEM_PATH/usr/local/lib
mkdir -p $ITEM_PATH/etc
mkdir -p $ITEM_PATH/etc/logrotate.d

del_file $ITEM_PATH/usr/local/bin/
del_file $ITEM_PATH/usr/local/lib/
del_file $ITEM_PATH/etc/

mkdir -p $ITEM_PATH/etc/psme/certs   
mkdir -p $ITEM_PATH/broadcom   

cp ${PSME_EC_SCRIPT_DIR}/*                              $ITEM_PATH/usr/local/bin
cp -rf ${PSME_EC_SW_DIR}/mod_conf                       $ITEM_PATH/usr/local/bin
cp ${PSME_EC_SW_DIR}/server.key                         $ITEM_PATH/etc/psme/certs
cp ${PSME_EC_SW_DIR}/server.crt                         $ITEM_PATH/etc/psme/certs
cp ${PSME_EC_SW_DIR}/*                                  $ITEM_PATH/etc/psme 2>/dev/null  

#cp from runtime build
cp $PSME_PROJ_PATH/bin/psme-rest-server                  $ITEM_PATH/usr/local/bin
cp $PSME_PROJ_PATH/bin/psme-chassis                      $ITEM_PATH/usr/local/bin
cp $PSME_PROJ_PATH/lib/libjsoncpp.so.999                 $ITEM_PATH/usr/local/lib
cp $PSME_PROJ_PATH/lib/libmicrohttpd.so                  $ITEM_PATH/usr/local/lib
cp $PSME_PROJ_PATH/lib/libjsonrpccpp-server.so.999       $ITEM_PATH/usr/local/lib
cp $PSME_PROJ_PATH/lib/libjsonrpccpp-client.so.999       $ITEM_PATH/usr/local/lib
cp $PSME_PROJ_PATH/lib/libjsonrpccpp-common.so.999       $ITEM_PATH/usr/local/lib

#For SONiC lib 
cp ${PSME_EC_SW_DIR}/x_86_sonic_lib/*                   $ITEM_PATH/usr/local/lib

if [ "$1" = "arm" ];then
    cp $PSME_PROJ_PATH/lib/libuuid.so.16                 $ITEM_PATH/usr/local/lib
    cp $PSME_PROJ_PATH/lib/libuuid++.so.16               $ITEM_PATH/usr/local/lib
else
    cp $PSME_PROJ_PATH/bin/tests/unittest_psme-chassis_ecrf_pal $ITEM_PATH/usr/local/bin

    if [ -f "$PSME_PROJ_PATH/bin/tests/unittest_psme-chassis_acc_api_bal_dist_test" ];then
        #For bal api unit test
        cp $PSME_PROJ_PATH/bin/tests/unittest_psme-chassis_acc_api_bal_dist_test $ITEM_PATH/broadcom
        cp ${BAL_TEST}/onu_cfg                            $ITEM_PATH/broadcom
        cp ${BAL_TEST}/rm_g_onu_cfg                       $ITEM_PATH/broadcom
        cp ${BAL_TEST}/xgspon_unit_test_onu_cfg           $ITEM_PATH/broadcom
        cp ${BAL_TEST}/gpon_unit_test_onu_cfg             $ITEM_PATH/broadcom
    fi
    #Following lib exist in docker system#
    cp $LIB_PREINSTALL/libossp-uuid.so.16                 $ITEM_PATH/usr/local/lib
    cp $LIB_PREINSTALL/libossp-uuid++.so.16               $ITEM_PATH/usr/local/lib
    cp $LIB_PREINSTALL/libcurl.so.4                       $ITEM_PATH/usr/local/lib
    cp $LIB_PREINSTALL/librtmp.so.1                       $ITEM_PATH/usr/local/lib
    cp $LIB_PREINSTALL/libssh2.so.1                       $ITEM_PATH/usr/local/lib
fi

$CUR_PATH/modify_ver.sh $ITEM_PATH
