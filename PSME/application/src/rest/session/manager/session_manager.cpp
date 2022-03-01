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


#include "psme/rest/session/manager/session_manager.hpp"
#include "psme/rest/server/error/server_exception.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/utils/ec_common_utils.hpp"
#include "eclog_helper/eclog_helper.hpp"
#include <json/json.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/time.h>
#include <vector>
#include <algorithm>

using namespace eclog_helper;
using namespace std;
using namespace psme::rest::constants;
using namespace psme::rest::utils;

namespace psme {
namespace rest {
namespace session {
namespace manager {

bool SessionManager::m_chk_start=false;
auto &gADbg = ADbg::get_instance();
const unsigned int ONE_SECOND = 1000000;

void SessionManager::CheckSessionTimeout()
{
    while(true && m_chk_start == true)
    {
        struct timeval tp;
        gettimeofday(&tp, NULL);
        uint64_t current_timestamp = (uint64_t) tp.tv_sec * 1000L + tp.tv_usec / 1000;
    	 uint64_t timeout = SessionManager::get_instance()->GetSessionConfigTimeOut();
		 
                        for (const auto &item : SessionManager::get_instance()->getSessionMap())
        {
            const auto& session = item.second;
	     uint64_t session_timestamp = session.get_time_stamp();
            uint64_t timediff= (current_timestamp - session_timestamp)/1000; //In second //
			
                            if(timeout > 0)
                            {
            if(timediff >= timeout )
            {
                                SessionManager::get_instance()->delSession_by_token(session.get_authen_token());
            }
        }
                       }
	 usleep(ONE_SECOND);	
    }

    return;
}

void SessionManager::StartSessionThread()
{
    m_chk_start = true;
    std::thread mThread{CheckSessionTimeout};
    mThread.detach();
}

void  SessionManager::addSessionConfig(const json::Value& sessionJson) 
{
    SetBasicAuthenServiceConfigEnable(sessionJson[constants::SessionService::BASICAUTHEN_ENABLED].as_bool());	
    SetSessionConfigEnable(sessionJson[constants::SessionService::SERVICE_ENABLED].as_bool());	
    SetSessionConfigTimeOut(sessionJson[constants::SessionService::SERVICE_TIMEOUT].as_uint64());
    SetSessionConfigMaxSessions(sessionJson[constants::SessionService::SERVICE_MAXSESSIONS].as_uint64());
}

void SessionManager::SetSessionConfigEnable(bool Enable)
{
    if(Enable == false)
        delAllSession();
    m_SessionServiceEnable = Enable;
                    SetBasicAuthenServiceConfigEnable(Enable);
}

bool SessionManager::GetSessionConfigEnable()
{
    return m_SessionServiceEnable;
}


void SessionManager::SetBasicAuthenServiceConfigEnable(bool Enable)
{
    m_BasicAuthenServiceEnable = Enable;
}

bool SessionManager::GetBasicAuthenServiceConfigEnable()
{
    return m_BasicAuthenServiceEnable;
}

void SessionManager::SetSessionConfigTimeOut(uint64_t timeout)
{
    m_SessionTimeout = timeout;
}

void SessionManager::SetSessionConfigMaxSessions(uint64_t MaxSessions)
{
    m_MaxSessions = MaxSessions;
}

uint64_t SessionManager::GetSessionConfigTimeOut()
{
    return m_SessionTimeout;
}

uint64_t SessionManager::GetSessionConfigMaxSessions()
{
    return m_MaxSessions;

}

json::Value SessionManager::to_json() const {
	
    json::Value json(json::Value::Type::OBJECT);
	
    json[SessionService::BASICAUTHEN_ENABLED] = m_BasicAuthenServiceEnable;
    json[SessionService::SERVICE_ENABLED] = m_SessionServiceEnable;
    json[SessionService::SERVICE_TIMEOUT] = m_SessionTimeout;
    json[SessionService::SERVICE_MAXSESSIONS] = m_MaxSessions;
    return json;
}

                std::string SessionManager::addSession(Session session, bool isupdate)
                {
                    std::string random_string;
	
    std::lock_guard<std::mutex> lock{m_mutex};
                    auto sub = m_sessions.find(session.get_authen_token());
                    if (m_sessions.end() != sub)
                    {
        throw error::ServerException(error::ErrorFactory::create_resource_already_exists_error(
                                    "Session '" + session.get_authen_token() + "' already exists."));
    }
    //Set timestamp //
    struct timeval tp;
    gettimeofday(&tp, NULL);
    uint64_t mslong = (uint64_t) tp.tv_sec * 1000L + tp.tv_usec / 1000;

    if (isupdate == false)
    {
                        char command[BUFFER_LEN] = {0};
                        sprintf(command, "%s", "openssl rand -hex 8");
                        EcCommonUtils::exec_shell_(command, random_string, 1);
                        session.set_string_id(random_string.erase(random_string.size()-1));
    }
    session.set_time_stamp(mslong);
                    m_sessions[session.get_authen_token()] = session;

                    return random_string;
}

Session SessionManager::getSession(const std::string& session_username) {
    std::lock_guard<std::mutex> lock{m_mutex};
    return getSession_by_name(session_username);
}


                Session SessionManager::getSessionByStringId(std::string session_string_id)
                {
                    std::lock_guard<std::mutex> lock{m_mutex};

                    for (const auto &item : m_sessions)
                    {
                        const auto &session = item.second;
                        if (session.get_string_id() == session_string_id)
                            return item.second;
                    }
                    throw agent_framework::exceptions::NotFound("Session not found: getSessionByStringId.");
                }

                Session SessionManager::getSession_by_Token(const std::string &session_authen_token)
                {
    std::lock_guard<std::mutex> lock{m_mutex};

    for (const auto& item : m_sessions) 
    {
        const auto& session = item.second;
        if(session.get_authen_token() == session_authen_token)
            return  item.second;
    }
    throw agent_framework::exceptions::NotFound("Session not found.");
}

                std::string SessionManager::getAuthenToken_by_name(const std::string& session_username)
                {
                    std::lock_guard<std::mutex> lock{m_mutex};

                    for (const auto &item : m_sessions)
                    {
                        const auto &session = item.second;
                        if (session.get_username() == session_username)
                            return item.second.get_authen_token();
    }
                    throw agent_framework::exceptions::NotFound("Session not found:getAuthenToken_by_name");
}

                Session SessionManager::getSession_by_name(const std::string &session_username)
                {
                    for (const auto &item : m_sessions)
                    {
                        const auto &session = item.second;
                        if (session.get_username() == session_username)
                            return item.second;
    }
                    throw agent_framework::exceptions::NotFound("Session not found:getSession_by_name");
}

                bool SessionManager::checkSessionExist_by_name(const std::string &session_username)
                {
    std::lock_guard<std::mutex> lock{m_mutex};

                    for (const auto &item : m_sessions)
                    {
        const auto& session = item.second;
                        if (session_username == session.get_username())
                        {
                            return true;
        }
    }
                    return false;
}

                void SessionManager::delSession_by_token(const std::string &session_token)
                {
                    std::lock_guard<std::mutex> lock{m_mutex};

                    auto session = m_sessions.find(session_token);
                    if (m_sessions.end() == session)
                    {
                        throw agent_framework::exceptions::NotFound("Session '" + session_token + "' not found.");
    }
    m_sessions.erase(session);
}

                void SessionManager::delSession_by_name(const std::string &session_username)
                {
                    for (const auto &item : m_sessions)
                    {
                        const auto &session = item.second;
                        if (session_username == session.get_username())
                        {
                            delSession_by_token(session.get_authen_token());
                            return;
                        }
                    }

                    throw agent_framework::exceptions::NotFound("Error delSession_by_name");
                }

void SessionManager::delAllSession()
{
    m_sessions.clear();
    return;
}

bool SessionManager::updateSessionTimestamp(const std::string& authen_token) 
{
    if(authen_token.length() != 0)	
    {
        Session new_session  = getSession_by_Token(authen_token);
        if(new_session.get_authen_token() == authen_token)
        {
                            delSession_by_token(authen_token);
            addSession(new_session, true);
            return true;
        }
        else
        {
            gADbg.acc_printf(LEVEL_WARN, "Wrong AuthenToken!!");
            return false;
        }
    }
    else	
        return false;	
}

                SessionMap SessionManager::getSessionMap() {
    std::lock_guard<std::mutex> lock{m_mutex};
    return m_sessions;
}

                void SessionManager::delSession_by_string_id(std::string string_id)
                {

                    for (const auto &item : m_sessions)
                    {
        const auto& session = item.second;
                        if (string_id == session.get_string_id())
                        {
                            delSession_by_token(session.get_authen_token());
            return;
        }
    }
                    throw agent_framework::exceptions::NotFound("Session (String ID: " + string_id + ") not found.");
}

uint32_t SessionManager::Session_size() {
    std::lock_guard<std::mutex> lock{m_mutex};
    return static_cast<uint32_t>(m_sessions.size());
}

SessionManager::~SessionManager() 
{
    m_chk_start = false;
}

}
}
}
}
