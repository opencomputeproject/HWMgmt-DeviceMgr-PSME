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

#ifndef EC_LOG_HELPER_HPP
#define EC_LOG_HELPER_HPP

#include <syslog.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <ostream>
#include <string>
#include <limits.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <algorithm>
#include <regex>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <unistd.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>
#include <json/json.h>
#include <mutex>
#include <sys/ipc.h> 
#include <sys/msg.h> 

// For spdlog //
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/null_sink.h"
#include "spdlog/sinks/ostream_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/pattern_formatter.h"

extern char *__progname;
namespace eclog_helper
{

using namespace std;
#define  LEVEL_INFO eclog_helper::ADbg::ADbg_Level::INFO
#define  LEVEL_WARN eclog_helper::ADbg::ADbg_Level::WARN
#define  LEVEL_DEBUG eclog_helper::ADbg::ADbg_Level::DEBUG
#define  LEVEL_TRACE eclog_helper::ADbg::ADbg_Level::TRACE

class ADbg
{
public:
    typedef enum ADbg_Level
    {
        INFO,
        WARN,
        DEBUG,
        TRACE
    } ADbg_level_t;

    std::string m_output_console_enabled = {"/etc/psme/CONSOLE"};
    std::string m_more_debug_enabled = {"/etc/psme/DBG"};
    bool m_output_console = false;
    bool m_more_debug = false;

    void acc_printf(ADbg_Level level, const std::string fmt_str, ...);
    static ADbg &get_instance();
    void release();
    ADbg();
    ~ADbg();
};

#define MSG_SZ 512 
typedef struct msgbuf
{
    long mtype;
    char mtext[MSG_SZ];
} message_buf;

class RFLogEntry
{
public:
    RFLogEntry();
    mutable std::mutex m_mutex{};
    unsigned int ENTRY_MAX = 1000;
    int m_max_entry_num = {1000};

    std::string m_rf_log_enabled_path = {"/etc/psme/enable_log_srv"};
    std::string m_psme_rf_log_path  = {"/var/log/psme_rf.log"};
    int m_max_size = 300000;
    int m_max_files = 5;

    void set_log_entry(std::string &event_type, std::string &sensor_type, std::string &Serverity, std::string &message_content, int sensor_id);
    static void write_log_entry(std::string slog);
    void set_log_status(bool enable);
    bool set_max_entry(unsigned int max_entry);
    void clean_log();

    bool get_log_status();
    int get_max_entries_num() { return m_max_entry_num; };
    int get_max_log_size() { return m_max_size; };
    int get_max_file_num() { return m_max_files; };

    std::string get_current_time();
    std::string get_zone_offset();
    std::string get_log_path() { return m_psme_rf_log_path; };

    static RFLogEntry &get_instance();

    key_t key = {};
    int msgid = {};

private:
    std::string m_timeStr = {};
};

} // namespace ec_log_helper
#endif
