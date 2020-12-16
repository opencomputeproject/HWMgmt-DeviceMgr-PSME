/*
* Edgecore DeviceManager
* Copyright 2020-2021 Edgecore Networks, Inc.
 *
* Licensed to the Apache Software Foundation (ASF) under one
* or more contributor license agreements.  See the NOTICE file
* distributed with this work for additional information
* regarding copyright ownership.  The ASF licenses this file
* to you under the Apache License, Version 2.0 (the
* "License"); you may not use this file except in compliance
* with the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
* KIND, either express or implied.  See the License for the
* specific language governing permissions and limitations
* under the License.
*/
#include "psme/rest/utils/ec_common_utils.hpp"
#include <stdexcept>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cstdarg>
#include <memory>
#include <fstream>
#include <iostream>
#include <vector>
using namespace psme::rest::utils;

const std::string  EcCommonUtils::m_onie_boot_dir = "/mnt/onie-boot/";

std::string format_(const std::string fmt_str, ...);
std::string format_(const std::string fmt_str, ...)
{
	va_list ap;
	char *fp = NULL;
	va_start(ap, fmt_str);
	int unused __attribute__((unused));
	unused = vasprintf(&fp, fmt_str.c_str(), ap);
	va_end(ap);
	std::unique_ptr<char[]> formatted(fp);
	return std::string(formatted.get());
}

void EcCommonUtils::mount_onie_boot_part()
{
	char command[BUFFER_LEN] = {0};
	std::string resultA;
	sprintf(command, "[ -d %s ] || mkdir %s;mount LABEL=ONIE-BOOT %s > /dev/null 2>&1", m_onie_boot_dir.c_str(), m_onie_boot_dir.c_str(), m_onie_boot_dir.c_str());
	exec_shell_(command, resultA, 0);
}

void EcCommonUtils::umount_onie_boot_part()
{
	char command[BUFFER_LEN] = {0};
	std::string resultA;
	sprintf(command, "umount %s > /dev/null 2>&1", m_onie_boot_dir.c_str());
	exec_shell_(command, resultA, 0);
}

bool EcCommonUtils::Support_EFI()
{
	char command[BUFFER_LEN] = {0};
	std::string resultA;
	sprintf(command,"%s", "efibootmgr -v  2>&1"); //add 2>&1 can redir to result//
	exec_shell_(command, resultA, 0);

	if (resultA.find("not supported") == std::string::npos)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool EcCommonUtils::Support_TFTP()
{
	std::ifstream th_e_file("/usr/bin/tftp");
	if (th_e_file.good())
		return true;
	else
		return false;
}

bool EcCommonUtils::Support_WGET()
{
	std::ifstream th_e_file("/usr/bin/wget");
	if (th_e_file.good())
		return true;
	else
		return false;
}

void EcCommonUtils::exec_shell_(std::string cmd, std::string &result_a, int time_out)
{
	try
	{
		FILE *pipe = NULL;
		char buffer[BUFFER_LEN];
		std::string command;
		std::string result = "";

		if (time_out <= 0 || time_out >= 1000)
			time_out = 9; //Set timeout to 9 second to avoid command no response.

		command = format_("timeout %d %s", time_out, cmd.c_str());
		pipe = popen(command.c_str(), "r");

		if (!pipe)
			throw std::runtime_error("popen() failed!");

		while (!feof(pipe))
		{
			if (fgets(buffer, BUFFER_LEN, pipe) != NULL)
				result += buffer;
		}
		result_a = result;

		if (pipe)
			pclose(pipe);

		return;
	}
	catch (const std::exception &e)
	{
		std::cout << "EcCommonUtils::exec_shell- exception : " << e.what() << std::endl;
		return;
	}
}

void EcCommonUtils::exec_shell(const char *cmd, char *result_a, int time_out)
{
    try
    {
        FILE *pipe = NULL;
        char buffer[BUFFER_LEN];
        char command[BUFFER_LEN];
        std::string result = "";

        if (time_out <= 0 || time_out >=1000)
            time_out = 9; //Set timeout to 9 second to avoid command no response.

        if (strlen(cmd) > BUFFER_LEN - 15) // need minus at leaset 15 char size //
        {
            std::cout << "[EcCommonUtils::exec_shell] command size over " << BUFFER_LEN << "bytes!";
            return;
        }

        sprintf(command, "timeout %d %s", time_out, cmd);
        pipe = popen(command, "r");

        if (!pipe)
            throw std::runtime_error("popen() failed!");

        while (!feof(pipe))
        {
            if (fgets(buffer, BUFFER_LEN, pipe) != NULL)
                result += buffer;
        }

        if (result.length() > BUFFER_LEN)
            std::cout << "[EcCommonUtils::exec_shell] command return size over " << BUFFER_LEN << "bytes!";

        memcpy(result_a, result.c_str(), BUFFER_LEN);

        if (pipe)
            pclose(pipe);

        return;
    }
    catch (const std::exception &e)
    {
        std::cout << "EcCommonUtils::exec_shell- exception : " << e.what() << std::endl;
        return; 
    }
}

std::string EcCommonUtils::GetEfiBootCurrent()
{
	char command[BUFFER_LEN] = {0};
	sprintf(command,"%s","efibootmgr | grep BootCurrent | awk -F' ' '{print $2}' | tr -d '\n' ");
	std::string ret;
	exec_shell_(command,ret,0);
	return ret;
}

std::string EcCommonUtils::GetEfiBootOrder()
{
	char command[BUFFER_LEN] = {0};
	sprintf(command,"%s","efibootmgr | grep BootOrder | awk -F' ' '{print $2}' | tr -d '\n' ");
	std::string ret;
	exec_shell_(command,ret,0);
	return ret;
}

bool EcCommonUtils::SetEfiBootNext(std::string order_string)
{
	char command[BUFFER_LEN] = {0};
	sprintf(command,"efibootmgr -n %s", order_string.c_str());
	std::string ret;
	exec_shell_(command,ret,0);
	if (ret.find("BootCurrent") != std::string::npos)
		return true;
	else
		return false;
}

bool EcCommonUtils::DelEfiBootNext()
{
       /*
        NOTE: "efibootmgr -N" command is not workable in some device(as7816-64x, as7326-54x)
        It's a work-around solution using "efibootmgr -n XXX" which XXX is the first setting in the bootorder.
        */

        char command[BUFFER_LEN] = {0};
        sprintf(command,"efibootmgr | grep BootOrder: | awk '{print $2}' |awk 'BEGIN {FS=\",\"}; {print $1}'");
        std::string ret;
        exec_shell_(command,ret,0);

        memset(command, 0, BUFFER_LEN);
        //command[BUFFER_LEN] = {0};
        sprintf(command,"efibootmgr -n %s", ret.c_str());
        std::string ret1;
        exec_shell_(command,ret1,0);

        if (ret1.find("BootCurrent") != std::string::npos)
                return true;
        else
                return false;

        /* mark by vincent */
        /*
        char command[BUFFER_LEN] = {0};
        sprintf(command,"efibootmgr -N");
        std::string ret;
        exec_shell_(command,ret,0);
        if (ret.find("BootCurrent") != std::string::npos)
                return true;
        else
                return false;
        */
}

bool EcCommonUtils::SetEfiBootCurrent(std::string order_string)
{
	char command[BUFFER_LEN] = {0};
	sprintf(command,"efibootmgr -o %s", order_string.c_str());
	std::string ret;
	exec_shell_(command,ret,0);
	if (ret.find("BootCurrent") != std::string::npos)
		return true;
	else
		return false;
}

std::string EcCommonUtils::GetEfiBootIndexByName(std::string name)
{
	char command[BUFFER_LEN] = {0};
	sprintf(command,"efibootmgr | grep -m 1 '%s' | awk -F'*' '{print $1}' | sed -e s/Boot//g | tr -d '\n'", name.c_str());
	std::string ret;
	exec_shell_(command,ret,0);
	return ret;
}

std::string EcCommonUtils::GetONIEBootMode()
{
	mount_onie_boot_part();
	char command[BUFFER_LEN] = {0};
	sprintf(command,"%s","/mnt/onie-boot/onie/tools/bin/onie-boot-mode | grep Default | awk -F' ' '{print $5}' | tr -d '\n'");
	std::string ret;
	exec_shell_(command,ret,0);
	umount_onie_boot_part();
	return ret;
}

bool EcCommonUtils::SetONIEBootModeToONIE(std::string mode)
{
	mount_onie_boot_part();
	char command[BUFFER_LEN] = {0};
	sprintf(command, "/mnt/onie-boot/onie/tools/bin/onie-boot-mode -o %s", mode.c_str());
	std::string ret;
	exec_shell_(command, ret, 0);
	umount_onie_boot_part();
	if (ret.find("ERROR"))
		return false;
	else
		return true;
}

std::vector<std::string> EcCommonUtils::split(const std::string& s, char delimiter)                                                                                                                          
{                                                                                                                                                                                             
   std::vector<std::string> splits;                                                                                                                                                           
   std::string split;                                                                                                                                                                         
   std::istringstream ss(s);                                                                                                                                                                  
   while (std::getline(ss, split, delimiter))                                                                                                                                                 
   {                                                                                                                                                                                          
      splits.push_back(split);                                                                                                                                                                
   }                                                                                                                                                                                          
   return splits;                                                                                                                                                                             
}
