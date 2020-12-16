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

#ifndef PSME_REST_LOG_MANAGER
#define PSME_REST_LOG_MANAGER

#include "agent-framework/generic/singleton.hpp"
#include "psme/rest/log/model/log.hpp"
#include "psme/rest/utils/network_interface_info.hpp"
#include <eclog_helper/eclog_helper.hpp>
#include <mutex>
#include <map>
#include <thread>

namespace psme
{
namespace rest
{
namespace log
{
namespace manager
{
using namespace eclog_helper;
using namespace psme::rest::log;
using namespace psme::rest::log::model;
using namespace psme::rest::utils;


class LogManager : public agent_framework::generic::Singleton<LogManager>
{
public:
    LogManager();
    virtual ~LogManager();
    json::Value to_json() const;
    key_t key = {};
    int msgid = {};
    void create_msg_rev_thread();
    bool m_msg_rev_thread = false;
    void get_msg_queue_info();
    void init();
    ofstream o_file ={};

private:
    std::mutex m_mutex{};
};

} // namespace manager
} // namespace log
} // namespace rest
} // namespace psme

#endif
