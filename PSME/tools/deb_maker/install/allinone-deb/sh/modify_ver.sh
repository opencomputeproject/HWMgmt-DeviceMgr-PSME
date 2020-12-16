#!/bin/bash
CUR_PATH=$(cd "$(dirname "$0")"; pwd)
PROJ_PATH=$CUR_PATH/../../../..

ver=$(grep VER_STRING $PROJ_PATH/common/agent-framework/Version.cmake| awk 'BEGIN {FS="\""}; {print $2}' )
sed "/Version:/{s/.*$/Version: ${ver}/}" $1/DEBIAN/control > ./control.tmp
mv ./control.tmp $1/DEBIAN/control
