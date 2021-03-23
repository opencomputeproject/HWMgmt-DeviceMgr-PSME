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

#ifndef EC_SYS_HELPER_HPP
#define EC_SYS_HELPER_HPP

#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <ostream>
#include <string>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <linux/unistd.h>
#include <linux/kernel.h>
#include <blkid/blkid.h>
#include <err.h>
#include <thread>
extern char *__progname;
namespace ecsys_helper
{

using namespace std;

namespace
{
constexpr const size_t MAC_ADDRESS_LENGTH = 18;
constexpr const size_t HT_BUFFER_LEN = 512;
constexpr const size_t HT_BUFFER_LEN_1024 = 1024;
}

class HelperTools
{
public:
    static void exec_shell(const char *cmd, char *result_a, int time_out);
    static void exec_shell_(std::string cmd, std::string &result_a, int time_out);
    static std::vector<std::string> split(const std::string& s, char delimiter); 
};

/*
 Read for a current process' virtual memory size and resident set size,
*/

class ProcMemoryUsage
{
public:
    int m_tsize = 0;
    int m_resident = 0;
    int m_share = 0;
    int m_trs = 0;
    int m_lrs = 0;
    int m_drs = 0;
    int m_dt = 0;
    std::string m_mark_name={};
	long m_page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;
    void GetProcMemUsage(double &tsize_m, double &resident_m, double &share_m,
                         double &trs_m, double &lrs_m,
                         double &drs_m, double &dt_m);
    void mark_first(); 
    void check_diff(); 
    ProcMemoryUsage(std::string name):m_mark_name(name){}
};

class CPUData
{
public:
    void ReadData(const std::string &line);

    std::size_t GetActiveTime() const;
    std::size_t GetIdleTime() const;
    std::size_t GetStateTime(unsigned int state) const;
    std::size_t GetTotalTime() const;

    const std::string &GetLabel() const;

    static bool IsDataCPUStats(const std::string &line);

public:
    enum CPUStates
    {
        S_USER = 0,
        S_NICE,
        S_SYSTEM,
        S_IDLE,
        S_IOWAIT,
        S_IRQ,
        S_SOFTIRQ,
        S_STEAL,
        S_GUEST,
        S_GUEST_NICE,

        NUM_CPU_STATES
    };

private:
    static const std::string STR_CPU;
    static const std::string STR_TOT;

    static const std::size_t LEN_STR_CPU;

    std::string mLabel = {};

    std::size_t mTimes[NUM_CPU_STATES];
};

inline std::size_t CPUData::GetActiveTime() const
{
    return mTimes[S_USER] +
           mTimes[S_NICE] +
           mTimes[S_SYSTEM] +
           mTimes[S_IRQ] +
           mTimes[S_SOFTIRQ] +
           mTimes[S_STEAL] +
           mTimes[S_GUEST] +
           mTimes[S_GUEST_NICE];
}

inline std::size_t CPUData::GetIdleTime() const
{
    return mTimes[S_IDLE] + mTimes[S_IOWAIT];
}

inline std::size_t CPUData::GetStateTime(unsigned int state) const
{
    if (state < NUM_CPU_STATES)
        return mTimes[state];
    else
        return 0;
}

inline std::size_t CPUData::GetTotalTime() const
{
    return mTimes[S_USER] +
           mTimes[S_NICE] +
           mTimes[S_SYSTEM] +
           mTimes[S_IDLE] +
           mTimes[S_IOWAIT] +
           mTimes[S_IRQ] +
           mTimes[S_SOFTIRQ] +
           mTimes[S_STEAL] +
           mTimes[S_GUEST] +
           mTimes[S_GUEST_NICE];
}

inline const std::string &CPUData::GetLabel() const { return mLabel; }

inline bool CPUData::IsDataCPUStats(const std::string &line)
{
    return (!line.compare(0, LEN_STR_CPU, STR_CPU));
}

class CPU
{
public:
    CPU();

    std::size_t GetNumEntries() const;

    std::size_t GetActiveTimeTotal() const;
    std::size_t GetActiveTime(unsigned int cpu) const;

    std::size_t GetIdleTimeTotal() const;
    std::size_t GetIdleTime(unsigned int cpu) const;

    std::size_t GetStateTimeTotal(unsigned int state) const;
    std::size_t GetStateTime(unsigned int state, unsigned int cpu) const;

    std::size_t GetTotalTimeTotal() const;
    std::size_t GetTotalTime(unsigned int cpu) const;

    const char *GetLabelTotal() const;
    const char *GetLabel(unsigned int cpu) const;

private:
    static const int INDEX_TOT;
    std::vector<CPUData> mEntries = {};
};

inline std::size_t CPU::GetNumEntries() const { return mEntries.size() - 1; }

inline const char *CPU::GetLabelTotal() const { return mEntries[INDEX_TOT].GetLabel().c_str(); }

inline const char *CPU::GetLabel(unsigned int cpu) const
{
    ++cpu;

    if (cpu < mEntries.size())
        return mEntries[cpu].GetLabel().c_str();
    else
        return nullptr;
}

inline std::size_t CPU::GetActiveTimeTotal() const { return mEntries[INDEX_TOT].GetActiveTime(); }

inline std::size_t CPU::GetActiveTime(unsigned int cpu) const
{
    ++cpu;

    if (cpu < mEntries.size())
        return mEntries[cpu].GetActiveTime();
    else
        return 0;
}

inline std::size_t CPU::GetIdleTimeTotal() const { return mEntries[INDEX_TOT].GetIdleTime(); }

inline std::size_t CPU::GetIdleTime(unsigned int cpu) const
{
    ++cpu;

    if (cpu < mEntries.size())
        return mEntries[cpu].GetIdleTime();
    else
        return 0;
}

inline std::size_t CPU::GetStateTimeTotal(unsigned int state) const { return mEntries[INDEX_TOT].GetStateTime(state); }

inline std::size_t CPU::GetStateTime(unsigned int state, unsigned int cpu) const
{
    ++cpu;

    if (cpu < mEntries.size())
        return mEntries[cpu].GetStateTime(state);
    else
        return 0;
}

inline std::size_t CPU::GetTotalTimeTotal() const { return mEntries[INDEX_TOT].GetTotalTime(); }

inline std::size_t CPU::GetTotalTime(unsigned int cpu) const
{
    ++cpu;

    if (cpu < mEntries.size())
        return mEntries[cpu].GetTotalTime();
    else
        return 0;
}

class CPUStatsPrinter
{
public:
    CPUStatsPrinter(const CPU &s1, const CPU &s2);

    void PrintActivePercentageTotal();
    void PrintActivePercentageCPU(unsigned int cpu);
    void PrintActivePercentageAll();

    void PrintStatePercentageTotal(unsigned int state);
    void PrintStatePercentageCPU(unsigned int state, unsigned int cpu);
    void PrintStatePercentageAll(unsigned int state);

    void PrintFullStatePercentageTotal();
    void PrintFullStatePercentageCPU(unsigned int cpu);
    void PrintFullStatePercentageAll();

    void PrintStatePercentageNoLabelTotal(unsigned int state);
    void PrintStatePercentageNoLabelCPU(unsigned int state, unsigned int cpu);

    void SetPrecision(unsigned int prec);
    void SetVerbose(bool val);

    float GetPercActiveTotal();
    float GetPercActive(unsigned int cpu);
    float GetPercStateTotal(unsigned int state);
    float GetPercState(unsigned int state, unsigned int cpu);

    float m_ActivePercentageTotal = 0;
    float m_ActivePercentageIndividual = 0;
    float m_PercStateTotal = 0;

    static const int CPU_LABEL_W;
    static const int STATE_PERC_BASE_W;

    static const char *STR_STATES[CPUData::NUM_CPU_STATES];

    const CPU &mS1 = {};
    const CPU &mS2 = {};

    unsigned int mPrecision;

    bool mVerbose;
};

inline void CPUStatsPrinter::SetPrecision(unsigned int prec) { mPrecision = prec; }
inline void CPUStatsPrinter::SetVerbose(bool val) { mVerbose = val; }

typedef struct sdx_info
{
    std::string name={};
    std::string uuid={};
    std::string label={};
    std::string type={};
    std::string mount_dir={};
} sdx_info_t;

class DiskInfo
{
public:
    int m_sdx_number = 0;
    std::vector<sdx_info_t> m_sdx_v_member = {};
    std::string m_harddisk_path = "/dev/sda";
    DiskInfo();
    ~DiskInfo();
    void init();
    DiskInfo(const DiskInfo &) = default;
    DiskInfo &operator=(const DiskInfo &) = default;
    DiskInfo(DiskInfo &&) = default;
    DiskInfo &operator=(DiskInfo &&) = default;
    bool m_init_ok = false;
    void get_all_mounted_free_space_bytes();
    double get_sdx_used_percentage();
    unsigned long long m_available_kbytes = 0;
    unsigned long long m_used_kbytes = 0;
};
} // namespace ecsys_helper

#endif
