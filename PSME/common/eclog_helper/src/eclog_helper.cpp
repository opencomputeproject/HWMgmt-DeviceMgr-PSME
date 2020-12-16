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
#include "eclog_helper/eclog_helper.hpp"
#include "ecsys_helper/ecsys_helper.hpp"
#include <spdlog/spdlog.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <queue>
#include <unistd.h>	
#include <fcntl.h>	
#include <pthread.h>

using namespace eclog_helper;
using namespace ecsys_helper;

#define BUFFER_SIZE 512
static ADbg *g_ADbg = NULL;
auto rotating_logger = spdlog::rotating_logger_mt("PSME", "/etc/psme/debug.log", 1048576 * 1, 5);

static RFLogEntry *g_RFLogEntry = NULL;
auto rf_logger = spdlog::rotating_logger_mt("RF_LOG_SERVICE",RFLogEntry::get_instance().get_log_path(),300000 ,5);
//auto rf_logger = spdlog::rotating_logger_mt("RF_LOG_SERVICE","/var/log/psme_rf.log" ,300000 ,5);
auto &gADbg = ADbg::get_instance();

 RFLogEntry::RFLogEntry()
 {
     key = ftok("progfile", 65);
     msgid = msgget(key, 0666 | IPC_CREAT);
 }

 void RFLogEntry::clean_log()
 {
     std::ofstream ofs;
     ofs.open(m_psme_rf_log_path.c_str(), std::ofstream::out | std::ofstream::trunc);
     ofs.close();
 }
std::string RFLogEntry::get_current_time()
{
    std::array<char, BUFFER_SIZE> buffer;
    buffer.fill(0);
    time_t rawtime;
    time(&rawtime);
    const auto timeinfo = localtime(&rawtime);
    strftime(buffer.data(), sizeof(buffer), "%Y-%m-%dT%H:%M:%S%z", timeinfo);
    m_timeStr = buffer.data();
    m_timeStr.insert(m_timeStr.length() - 2, ":");
    return m_timeStr;
}

std::string RFLogEntry::get_zone_offset()
{
    std::string time_offset;
    std::array<char, BUFFER_SIZE> buffer;
    buffer.fill(0);
    time_t rawtime;
    time(&rawtime);
    const auto timeinfo = localtime(&rawtime);
    strftime(buffer.data(), sizeof(buffer), "%z", timeinfo);
    time_offset = buffer.data();
    time_offset.insert(time_offset.length() - 2, ":");
    return time_offset;
}

bool RFLogEntry::get_log_status()
{
    std::ifstream m_log_enabled(m_rf_log_enabled_path);
    if (m_log_enabled.good())
        return true;
    else
        return false;
}

bool RFLogEntry::set_max_entry(unsigned int max_entry)
{
    if (max_entry > ENTRY_MAX || max_entry <= 0)
        return false;

    m_max_entry_num = max_entry;
    return true;
}

void RFLogEntry::set_log_status(bool enable)
{
    char result[BUFFER_SIZE] = {};
    char cmd[BUFFER_SIZE] = {};

    if (enable)
    {
        sprintf(cmd, "touch %s", m_rf_log_enabled_path.c_str());
        HelperTools::exec_shell(cmd, result, 0);
    }
    else
    {
        remove(m_rf_log_enabled_path.c_str());
        HelperTools::exec_shell(cmd, result, 0);
    }
    return;
}

void RFLogEntry::set_log_entry(std::string &event_type, std::string &sensor_type, std::string &Serverity, std::string &message_content, int sensor_id)
{
    Json::FastWriter writer;
    if (!get_log_status())
        return;

    Json::Value LogEntry;
    LogEntry["EntryType"] = event_type;
    LogEntry["OemRecordFormat"] = "NULL";
    LogEntry["Severity"] = Serverity;
    LogEntry["Created"] = m_timeStr;
    LogEntry["EntryCode"] = "Assert";
    LogEntry["SensorType"] = sensor_type;
    LogEntry["SensorNumber"] = sensor_id;
    LogEntry["Message"] = message_content;
    LogEntry["Name"] = "Log Message";
    std::string slog = writer.write(LogEntry);
    message_buf sbuf;
    sbuf.mtype = 1;
    sprintf(sbuf.mtext, "%s", slog.c_str());
    int buf_length = strlen(sbuf.mtext) + 1;
    int error = msgsnd(msgid, &sbuf, buf_length, 0);
    if (error)
        perror("msgsnd error");
    return;
}

void RFLogEntry::write_log_entry(std::string slog)
{
    rf_logger->flush_on(spdlog::level::info);
    rf_logger->set_pattern("%v");
    rf_logger->set_level(spdlog::level::info);
    rf_logger->info(slog);
    rf_logger->flush();
    return;
}
void ADbg::acc_printf(ADbg_Level level ,const std::string fmt_str, ...)
{
    va_list ap;
    char *fp = NULL;
    va_start(ap, fmt_str);
    [&] { return vasprintf(&fp, fmt_str.c_str(), ap); }(); //Avoid warning compile message//
    va_end(ap);
    std::unique_ptr<char[]> formatted(fp);
    std::string log_msg(formatted.get());
    rotating_logger->set_pattern("[Tid %t][%+]");

    switch (level)
    {
    case ADbg_Level::INFO:
        rotating_logger->set_level(spdlog::level::info);
        rotating_logger->info(log_msg);
        break;
    case ADbg_Level::DEBUG:
        rotating_logger->set_level(spdlog::level::debug);
        rotating_logger->debug(log_msg);
        break;
    case ADbg_Level::WARN:
        rotating_logger->set_level(spdlog::level::warn);
        rotating_logger->warn(log_msg);
        break;
    case ADbg_Level::TRACE:
        rotating_logger->set_level(spdlog::level::info);
        rotating_logger->trace(log_msg);
        break;
    default:
        rotating_logger->set_level(spdlog::level::info);
        rotating_logger->trace(log_msg);
        break;
    }
    rotating_logger->flush();
}

void ADbg::release()
{
    if (g_ADbg)
    {
        delete g_ADbg;
        g_ADbg = NULL;
    }
}

ADbg::ADbg()
{
    ifstream con_out(m_output_console_enabled.c_str());
    ifstream debug_more(m_more_debug_enabled.c_str());

    if (con_out.good())
    {
        m_output_console = true;
    }

    if (debug_more.good())
    {
        m_more_debug = true;
    }
    return;
}

ADbg::~ADbg()
{
    return;
}

ADbg &ADbg::get_instance()
{
    if (NULL == g_ADbg)
    {
        g_ADbg = new ADbg();
    }
    return *g_ADbg;
}

RFLogEntry &RFLogEntry::get_instance()
{
    if (NULL == g_RFLogEntry)
    {
        g_RFLogEntry= new RFLogEntry();
    }
    return *g_RFLogEntry;
}