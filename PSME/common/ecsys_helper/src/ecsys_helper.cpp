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
 * http://www.apache.org/licenses/LICENSE-2.0
 *
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
* KIND, either express or implied.  See the License for the
* specific language governing permissions and limitations
* under the License.
 */

#include "ecsys_helper/ecsys_helper.hpp"
#include "eclog_helper/eclog_helper.hpp"
#include <sys/statvfs.h>
#include <sstream>
#include <cstdarg>
#include <mntent.h>

using namespace ecsys_helper;
using namespace eclog_helper;

const int CPU::INDEX_TOT = 0;

std::string format(const std::string fmt_str, ...);
std::string format(const std::string fmt_str, ...)
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

void HelperTools::exec_shell_(std::string cmd, std::string &result_a, int time_out)
{
	try
	{
		FILE *pipe = NULL;
		char buffer[HT_BUFFER_LEN];
		std::string command;
		std::string result = "";

		if (time_out <= 0 || time_out >= 1000)
			time_out = 9; //Set timeout to 9 second to avoid command no response.

		command = format("timeout %d %s", time_out, cmd.c_str());
		pipe = popen(command.c_str(), "r");

		if (!pipe)
			throw std::runtime_error("popen() failed!");

		while (!feof(pipe))
		{
			if (fgets(buffer, HT_BUFFER_LEN, pipe) != NULL)
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

void HelperTools::exec_shell(const char *cmd, char *result_a, int time_out)
{
	try
	{
		FILE *pipe = NULL;
		char buffer[HT_BUFFER_LEN];
		char command[HT_BUFFER_LEN];
		std::string result = "";

		if (time_out <= 0 || time_out >= 1000)
			time_out = 9; //Set timeout to 9 second to avoid command no response.

		if (strlen(cmd) > HT_BUFFER_LEN - 15) // need minus at leaset 15 char size //
		{
			std::cout << "[HelperTools::exec_shell] command size over " << HT_BUFFER_LEN << "bytes!";
			return;
		}

		sprintf(command, "timeout %d %s", time_out, cmd);
		pipe = popen(command, "r");

		if (!pipe)
			throw std::runtime_error("popen() failed!");

		while (!feof(pipe))
		{
			if (fgets(buffer, HT_BUFFER_LEN, pipe) != NULL)
				result += buffer;
		}

		if (result.length() > HT_BUFFER_LEN)
			std::cout << "[HelperTools::exec_shell] command return size over " << HT_BUFFER_LEN << "bytes!";

		memcpy(result_a, result.c_str(), HT_BUFFER_LEN);

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

CPU::CPU()
{
	std::ifstream fileStat("/proc/stat");

	std::string line;

	while(std::getline(fileStat, line))
	{
		if(CPUData::IsDataCPUStats(line))
		{
			mEntries.emplace_back(CPUData());
			CPUData & entry = mEntries.back();

			entry.ReadData(line);
		}
	}
}

const std::string CPUData::STR_CPU("cpu");
const std::string CPUData::STR_TOT("tot");

const std::size_t CPUData::LEN_STR_CPU = 3;

void CPUData::ReadData(const std::string & line)
{
	std::istringstream ss(line);

	ss >> mLabel;

	if(mLabel.size() > LEN_STR_CPU)
	    mLabel.erase(0, LEN_STR_CPU);
	else
	    mLabel = STR_TOT;

	for(int i = 0; i < NUM_CPU_STATES; ++i)
    	    ss >> mTimes[i];
}


const int CPUStatsPrinter::CPU_LABEL_W				= 3;
const int CPUStatsPrinter::STATE_PERC_BASE_W		= 4;

const char * CPUStatsPrinter::STR_STATES[CPUData::NUM_CPU_STATES]	= {"user","user nice","system","idle","io wait","hard irq","soft irq","steal","guest","guest nice"};

CPUStatsPrinter::CPUStatsPrinter(const CPU & s1, const CPU & s2)
	: mS1(s1)
	, mS2(s2)
	, mPrecision(2)
	, mVerbose(false)
{

}

void CPUStatsPrinter::PrintActivePercentageTotal()
{
	if(mVerbose)
		std::cout << mS1.GetLabelTotal() << "] ";

	std::cout.setf(std::ios::fixed, std::ios::floatfield);
	std::cout.precision(mPrecision);
	std::cout << GetPercActiveTotal();

	if(mVerbose)
		std::cout << "%";

	std::cout << std::endl;
}

void CPUStatsPrinter::PrintActivePercentageCPU(unsigned int cpu)
{
	if(cpu >= mS1.GetNumEntries())
	{
		std::cout << "ERROR - CPU " << cpu << " not available." << std::endl;
		return ;
	}

	if(mVerbose)
	{
		std::cout.width(CPU_LABEL_W);
		std::cout << mS1.GetLabel(cpu) << "] ";
	}

	std::cout.setf(std::ios::fixed, std::ios::floatfield);
	std::cout.precision(mPrecision);
	std::cout << GetPercActive(cpu);

	if(mVerbose)
		std::cout << "%";

	std::cout << std::endl;
}

void CPUStatsPrinter::PrintActivePercentageAll()
{
	PrintActivePercentageTotal();

	const unsigned int NUM_ENTRIES = mS1.GetNumEntries();

	for(unsigned int i = 0; i < NUM_ENTRIES; ++i)
		PrintActivePercentageCPU(i);
}

void CPUStatsPrinter::PrintStatePercentageTotal(unsigned int state)
{
	if(mVerbose)
		std::cout << mS1.GetLabelTotal() << "] ";

	PrintStatePercentageNoLabelTotal(state);

	std::cout << std::endl;
}

void CPUStatsPrinter::PrintStatePercentageCPU(unsigned int state, unsigned int cpu)
{
	if(cpu >= mS1.GetNumEntries())
	{
		std::cout << "ERROR - CPU " << cpu << " not available." << std::endl;
		return ;
	}

	if(mVerbose)
	{
		std::cout.width(CPU_LABEL_W);
		std::cout << mS1.GetLabel(cpu) << "] ";
	}

	PrintStatePercentageNoLabelCPU(state, cpu);

	std::cout << std::endl;
}

void CPUStatsPrinter::PrintStatePercentageAll(unsigned int state)
{
	PrintStatePercentageTotal(state);

	const unsigned int NUM_ENTRIES = mS1.GetNumEntries();

	for(unsigned int i = 0; i < NUM_ENTRIES; ++i)
		PrintStatePercentageCPU(state, i);
}

void CPUStatsPrinter::PrintFullStatePercentageTotal()
{
	if(mVerbose)
		std::cout << mS1.GetLabelTotal() << "] ";

	for(int s = 0; s < CPUData::NUM_CPU_STATES; ++s)
	{
		PrintStatePercentageNoLabelTotal(s);
		std::cout << " | ";
	}

	std::cout << std::endl;
}

void CPUStatsPrinter::PrintFullStatePercentageCPU(unsigned int cpu)
{
	if(mVerbose)
	{
		std::cout.width(CPU_LABEL_W);
		std::cout << mS1.GetLabel(cpu) << "] ";
	}

	for(int s = 0; s < CPUData::NUM_CPU_STATES; ++s)
	{
		PrintStatePercentageNoLabelCPU(s, cpu);
		std::cout << " | ";
	}

	std::cout << std::endl;
}

void CPUStatsPrinter::PrintFullStatePercentageAll()
{
	// PRINT TOTAL
	PrintFullStatePercentageTotal();

	// PRINT ALL CPUS
	const unsigned int NUM_ENTRIES = mS1.GetNumEntries();

	for(unsigned int i = 0; i < NUM_ENTRIES; ++i)
		PrintFullStatePercentageCPU(i);
}

// == PRIVATE FUNCTIONS ==

float CPUStatsPrinter::GetPercActiveTotal()
{
	const float ACTIVE_TIME    = mS2.GetActiveTimeTotal() - mS1.GetActiveTimeTotal();
	const float IDLE_TIME        = mS2.GetIdleTimeTotal() - mS1.GetIdleTimeTotal();
	const float TOTAL_TIME     = ACTIVE_TIME + IDLE_TIME;
       m_ActivePercentageTotal = 100.f * ACTIVE_TIME / TOTAL_TIME;
	return m_ActivePercentageTotal;
}

float CPUStatsPrinter::GetPercActive(unsigned int cpu)
{
        const float ACTIVE_TIME = mS2.GetActiveTime(cpu) - mS1.GetActiveTime(cpu);
        const float IDLE_TIME     = mS2.GetIdleTime(cpu) - mS1.GetIdleTime(cpu);
        const float TOTAL_TIME  = ACTIVE_TIME + IDLE_TIME;
        m_ActivePercentageIndividual = 100.f * ACTIVE_TIME / TOTAL_TIME;
        return m_ActivePercentageIndividual;
}

float CPUStatsPrinter::GetPercStateTotal(unsigned int state)
{
        const float STATE_TIME	= mS2.GetStateTimeTotal(state) - mS1.GetStateTimeTotal(state);
        const float TOTAL_TIME	= mS2.GetTotalTimeTotal() - mS1.GetTotalTimeTotal();
        m_PercStateTotal = 100.f * STATE_TIME / TOTAL_TIME;
        return m_PercStateTotal; 
}

float CPUStatsPrinter::GetPercState(unsigned int state, unsigned int cpu)
{
	const float STATE_TIME	= mS2.GetStateTime(state, cpu) - mS1.GetStateTime(state, cpu);
	const float TOTAL_TIME	= mS2.GetTotalTime(cpu) - mS1.GetTotalTime(cpu);

	return 100.f * STATE_TIME / TOTAL_TIME;
}

void CPUStatsPrinter::PrintStatePercentageNoLabelTotal(unsigned int state)
{
	if(mVerbose)
	{
		std:: cout << STR_STATES[state] << ": ";
		std::cout.width(STATE_PERC_BASE_W + mPrecision);
	}

	std::cout.setf(std::ios::fixed, std::ios::floatfield);
	std::cout.precision(mPrecision);
	std::cout << GetPercStateTotal(state);

	if(mVerbose)
		std::cout << "%";
}

void CPUStatsPrinter::PrintStatePercentageNoLabelCPU(unsigned int state, unsigned int cpu)
{
	if(mVerbose)
	{
		std::cout << STR_STATES[state] << ": ";
		std::cout.width(STATE_PERC_BASE_W + mPrecision);
	}

	std::cout.setf(std::ios::fixed, std::ios::floatfield);
	std::cout.precision(mPrecision);
	std::cout << GetPercState(state, cpu);

	if(mVerbose)
		std::cout << "%";
}
/*
 Field    Content  
 size     total program size (pages)            (same as VmSize in status)  
 resident size of memory portions (pages)       (same as VmRSS in status)  
 shared   number of pages that are shared       (i.e. backed by a file)  
 trs      number of pages that are 'code'       (not including libs; broken,  
 lrs      number of pages of library            (always 0 on 2.6)  
 drs      number of pages of data/stack         (including libs; broken,  
 dt       number of dirty pages                 (always 0 on 2.6)  
*/
void ProcMemoryUsage::GetProcMemUsage(double &tsize_m, double &resident_m, double &share_m,
									  double & trs_m, double & lrs_m,
									  double & drs_m, double & dt_m)
{
	int tsize = 0, resident = 0, share = 0, trs = 0, lrs = 0, drs = 0, dt = 0;
	ifstream buffer("/proc/self/statm");
	buffer >> tsize >> resident >> share >> trs >> lrs >> drs >> dt ;
	buffer.close();
	tsize_m = tsize * m_page_size_kb;
	resident_m = resident * m_page_size_kb;
	share_m = share * m_page_size_kb;
	trs_m = trs * m_page_size_kb;
	lrs_m = lrs * m_page_size_kb;
	drs_m = drs * m_page_size_kb;
	dt_m = dt * m_page_size_kb;
}

void ProcMemoryUsage::mark_first()
{
	ifstream buffer("/proc/self/statm");
	buffer >> m_tsize >> m_resident >> m_share >> m_trs >> m_lrs >> m_drs >> m_dt ;
	buffer.close();
}
void ProcMemoryUsage::check_diff()
{
	bool leak = false;
	int tsize = 0, resident = 0, share = 0, trs = 0, lrs = 0, drs = 0, dt = 0;
	ifstream buffer("/proc/self/statm");
	buffer >> tsize >> resident >> share >> trs >> lrs >> drs >> dt;
	buffer.close();

	if (tsize - m_tsize > 0)
	{
		spdlog::warn("{} MIGHT has TSIZE memory leak diff:{}!!!!", m_mark_name, tsize - m_tsize);
		leak = true;
	}

	if (resident - m_resident > 0)
	{
		spdlog::warn("{} MIGHT has Resident memory leak diff:{}!!!!", m_mark_name, resident - m_resident);
		leak = true;
	}

	if (share - m_share > 0)
	{
		spdlog::warn("{} MIGHT has Share memory leak diff:{}!!!!", m_mark_name, share - m_share);
		leak = true;
	}

	if (trs - m_trs > 0)
	{
		spdlog::warn("{} MIGHT has Trs memory leak diff:{}!!!!", m_mark_name, trs - m_trs);
		leak = true;
	}

	if (lrs - m_lrs > 0)
	{
		spdlog::warn("{} MIGHT has Lrs(Library) memory leak diff:{}!!!!", m_mark_name, lrs - m_lrs);
		leak = true;
	}

	if (drs - m_drs > 0)
	{
		spdlog::warn("{} MIGHT has Drs memory leak diff:{}!!!!", m_mark_name, drs - m_drs);
		leak = true;
	}

	if (dt - m_dt > 0)
	{
		spdlog::warn("{} MIGHT has Dt memory leak diff:{}!!!!", m_mark_name, dt - m_dt);
		leak = true;
	}
	if (leak)
	{
		spdlog::info("valgrind --leak-check=full --show-leak-kinds=all --verbose ./unittest_psme-chassis_ecrf_pal > log.txt 2>&1");
		spdlog::info("Please check 'definitely lost' keyword!!");
	}
}

DiskInfo::DiskInfo()
{
	init();
}

void DiskInfo::init()
{
	blkid_probe m_pr = NULL;
	m_pr = blkid_new_probe_from_filename(m_harddisk_path.c_str());

	if (!m_pr)
		spdlog::warn("Failed to open {} error", m_harddisk_path.c_str());
	else
	{
		blkid_partlist ls = NULL;
		ls = blkid_probe_get_partitions(m_pr);
		m_sdx_number = blkid_partlist_numof_partitions(ls);

		for (int i = 1; i <= m_sdx_number; i++)
		{
			sdx_info_t tmp_info;
			const char *uuid;
			const char *label;
			const char *type;
			std::string sdx_name = m_harddisk_path + std::to_string(i);
			blkid_probe pr = NULL;
			pr = blkid_new_probe_from_filename(sdx_name.c_str());
			blkid_do_probe(pr);

			if (!blkid_probe_lookup_value(pr, "UUID", &uuid, NULL))
				tmp_info.uuid = uuid;

			if (!blkid_probe_lookup_value(pr, "LABEL", &label, NULL))
				tmp_info.label = label;

			if (!blkid_probe_lookup_value(pr, "TYPE", &type, NULL))
				tmp_info.type = type;

			tmp_info.name = sdx_name;

			struct mntent *ent;
			FILE *aFile = setmntent("/proc/mounts", "r");
			if (aFile == NULL)
			{
				spdlog::warn("CAN NOT set mntent proc mounts!!!");
				return;
			}

			while (NULL != (ent = getmntent(aFile)))
			{
				if (sdx_name.find(ent->mnt_fsname) != std::string::npos)
				{
					tmp_info.mount_dir = ent->mnt_dir;
					spdlog::info("{} {}", ent->mnt_fsname, ent->mnt_dir);
					spdlog::info("name[{}] uuid[{}] label[{}] type[{}]\r\n", sdx_name.c_str(), uuid, label, type);
					m_sdx_v_member.push_back(tmp_info);
					break;
				}
			}
			endmntent(aFile);
			blkid_free_probe(pr);
		}

		blkid_free_probe(m_pr);
		m_init_ok = true;
	}
}

void DiskInfo::get_all_mounted_free_space_bytes()
{
	if (m_init_ok)
	{
		m_used_kbytes = 0;
		m_available_kbytes = 0;
		struct statvfs fs_usage;
		for (auto &element : m_sdx_v_member)
		{
			if (!statvfs(element.mount_dir.c_str(), &fs_usage))
			{
				m_used_kbytes += (fs_usage.f_frsize * (fs_usage.f_blocks - fs_usage.f_bfree)) / 1024;

				m_available_kbytes += (fs_usage.f_frsize * fs_usage.f_bavail) / 1024;
			}
			else
				spdlog::warn("statvfs error");
		}
		spdlog::info("Total {} kbytes :Available {} kbytes : used {} kbytes ", m_available_kbytes + m_used_kbytes, m_available_kbytes, m_used_kbytes);
	}
}

double  DiskInfo::get_sdx_used_percentage()
{
	get_all_mounted_free_space_bytes();
	double total_space = m_available_kbytes + m_used_kbytes;
	if(total_space>0)
	{
	spdlog::info("get used Total {} kbytes :Available {} kbytes : used {} kbytes: percent {} ",
		m_available_kbytes + m_used_kbytes, m_available_kbytes, m_used_kbytes, ((double)m_used_kbytes / (double)(total_space)));
		return ((double)m_used_kbytes / (double)(total_space)) * 100.0;
	}
	else
	{
		spdlog::warn("total space error");
		return 100;
	}
}

DiskInfo::~DiskInfo()
{

}