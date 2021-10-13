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


#include "psme/rest/log/manager/log_manager.hpp"
#include "psme/rest/server/error/server_exception.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/utils/time_utils.hpp"
#include "psme/rest/utils/ec_common_utils.hpp"
#include <chrono>
#include <json/json.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/time.h>
#include <vector>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <cstdio>
#include <ecnet_helper/ecnet_helper.hpp>
#include <eclog_helper/eclog_helper.hpp>
#include "agent-framework/eventing/event_data.hpp"
#include "agent-framework/eventing/events_queue.hpp"
#include <ecsys_helper/ecsys_helper.hpp>
#include <eclog_helper/eclog_helper.hpp>
#include "psme/rest/eventing/manager/subscription_manager.hpp"
#include "configuration/configuration.hpp"

using namespace psme::rest::eventing;
using SubscriptionManager = psme::rest::eventing::manager::SubscriptionManager;

using namespace eclog_helper;
using namespace std;
using namespace psme::rest::constants;

namespace psme
{
namespace rest
{
namespace log
{
namespace manager
{

using namespace psme::rest::utils;

auto &gADbg = ADbg::get_instance();

bool iterateFile(std::string fileName, std::function<void (const std::string & )> callback);
bool iterateFile(std::string fileName, std::function<void (const std::string & )> callback)
{
    std::ifstream in(fileName.c_str());
    if(!in)
    {
        std::cerr << "Cannot open the File : "<<fileName<<std::endl;
        return false;
    }
    std::string str;
    while (std::getline(in, str))
        callback(str);
    in.close();
    return true;
}

LogManager::LogManager()
{
    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
}

void LogManager::init()
{
    create_msg_rev_thread();
}


[[noreturn]] void LogManager::get_msg_queue_info()
{
    while (true)
    {
        try
        {
            message_buf rbuf;
            if (msgrcv(msgid, &rbuf, MSG_SZ, 1, 0) == -1)
            {
                if (errno != ENOMSG)
                    gADbg.acc_printf(LEVEL_WARN, "msgrcv err[%d]", errno);
            }
            else
            {
                std::string slog = rbuf.mtext;
                slog.erase(slog.length() - 1);
                RFLogEntry::write_log_entry(slog);
            }
        }
        catch (const std::exception &e)
        {
            log_debug(LOGUSR, "get_msg_queue_info - exception : " << e.what());
        }
    }
}

void LogManager::create_msg_rev_thread()
{
    try
    {
        if (!m_msg_rev_thread)
        {
            std::thread mThread_rcv(&LogManager::get_msg_queue_info ,this);
            mThread_rcv.detach();
            m_msg_rev_thread= true;
        }
    }
    catch (const std::exception &e)
    {
        log_debug(LOGUSR, "LogManager create_msg_rev_thread - exception : " << e.what());
    }
    return;
}

json::Value LogManager::to_json() const
{
    json::Value entry(json::Value::Type::OBJECT);
    json::Value entryVec(json::Value::Type::ARRAY);
    std::vector<std::string> vecOfStr;
    bool b_res = iterateFile(RFLogEntry::get_instance().get_log_path(), [&](const std::string &str) { vecOfStr.push_back(str); });
    int max_entry = RFLogEntry::get_instance().get_max_entries_num();

    if (b_res)
    {
        std::reverse(vecOfStr.begin(), vecOfStr.end());
        int count = 0;
        for (std::string &line : vecOfStr)
        {
            if (line.size() == 0)
                continue;
            json::Deserializer deserializer;
            deserializer << line;
            deserializer >> entry;
            if (entry.is_null())
                continue;
            count++;
            std::string odata_id = "/redfish/v1/Managers/1/LogServices/1/Entries/" + to_string(count);
            entry["@odata.id"] = odata_id ; 
            entry["Id"] = to_string(count); 
            entryVec.push_back(std::move(entry));
            if (count >= max_entry)
            {
                return entryVec;
            }
        }
    }
    return entryVec;
}

LogManager::~LogManager()
{
    o_file.close();
    spdlog::drop_all();
    return;
}

} // namespace manager
} // namespace log
} // namespace rest
} // namespace psme