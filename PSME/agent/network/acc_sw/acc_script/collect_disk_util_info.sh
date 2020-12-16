#!/bin/bash
# Edgecore DeviceManager
# Copyright 2020-2021 Edgecore Networks, Inc.
#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.
PlatformPath="/etc/onl/platform"

if [ -f "${PlatformPath}" ];then
	HW_type=`cat  ${PlatformPath}`
else
	HW_type=`/usr/local/bin/sonic-cfggen -d --print-data | grep "platform" | awk '{print $2}' |awk -F '"' '{print $2}'`
fi

case "$1" in

"COUNT")
	IFS=$'\n'
	disk_util_a=($( df -h | grep '/dev/sd.'))
	a_num=${#disk_util_a[@]}

	if [ "$HW_type" = "arm-accton-as4610-54-r0" ];then
		echo todo
	else
		if [ -f /tmp/disk_util ];then
			rm /tmp/disk_util
		fi

		for (( i=0; i < ${a_num} ; i++ ))
		do 
			echo "${disk_util_a[$i]}" >> /tmp/disk_util
		done
	fi
	disk_util_at=($(cat /tmp/disk_util))

	free_status=`free | grep Mem:`
	echo $free_status > /tmp/freemem
	echo ${#disk_util_at[@]}
;;

"INDEX")
	IFS=$'\n'
	ID=$(($2-1))
	disk_util_at=($(cat /tmp/disk_util))
	echo ${disk_util_at[$ID]}
;;
esac

