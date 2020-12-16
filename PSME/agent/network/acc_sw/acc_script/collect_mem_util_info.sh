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
	top5_mem_util_a=($(ps -eo pid,pmem,vsize,rss,cmd | sort -k 3 -nr | head -5))
	a_num=${#top5_mem_util_a[@]}

	if [ "$HW_type" = "arm-accton-as4610-54-r0" ];then
		echo todo
	else
		if [ -f /tmp/top5memutil ];then
			rm /tmp/top5memutil
		fi

		for (( i=0; i < ${a_num} ; i++ ))
		do 
			echo "${top5_mem_util_a[$i]}" >> /tmp/top5memutil
		done
	fi
	top5_mem_util_at=($(cat /tmp/top5memutil))

	free_status=`free | grep Mem:`
	echo $free_status > /tmp/freemem
	echo ${#top5_mem_util_at[@]}
;;

"INDEX")
	IFS=$'\n'
	ID=$(($2-1))
	top5_mem_util_at=($(cat /tmp/top5memutil))
	echo ${top5_mem_util_at[$ID]}
;;

"FREE")
	#total       used       free     shared    buffers     cached
	FREE=`cat /tmp/freemem`
	echo $FREE
;;



esac

