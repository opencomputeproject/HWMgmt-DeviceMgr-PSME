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
		 
        for (const auto& item : SessionManager::get_instance()->getSession()) 
        {
            const auto& session = item.second;
	     uint64_t session_timestamp = session.get_time_stamp();
            uint64_t timediff= (current_timestamp - session_timestamp)/1000; //In second //
			
            if(timediff >= timeout )
            {
                 SessionManager::get_instance()->delSession(session.get_id());		
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

bool inline comp_by_value(pair<string, Session> &p1, pair<string, Session> &p2)
{
//  return p1.second.get_id() > p2.second.get_id();   // Decrease
   return p1.second.get_id() < p2.second.get_id();   // Increase   
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

uint64_t SessionManager::addSession(Session session, bool isupdate) {

    uint32_t id=0;
	
    std::lock_guard<std::mutex> lock{m_mutex};
    auto sub = m_sessions.find(session.get_username());
    if (m_sessions.end() != sub) {
        throw error::ServerException(error::ErrorFactory::create_resource_already_exists_error(
            "Session '" + session.get_username() + "' already exists."
        ));
    }
    //Set timestamp //
    struct timeval tp;
    gettimeofday(&tp, NULL);
    uint64_t mslong = (uint64_t) tp.tv_sec * 1000L + tp.tv_usec / 1000;

    if (isupdate == false)
    {
    // Sort increase of m_session by Session ID //
        vector<pair<string, Session>> myVec(m_sessions.begin(), m_sessions.end());
        sort(myVec.begin(),  myVec.end(), comp_by_value);
    //Get  first not used ID from 1 //
        for(id = 1 ; (id <= m_MaxSessions ) ; id++)
        {
            bool found = false;	    
    	 //Check if can find id in myVec //
            for (const auto& item : myVec) 
            {
                const auto& tmp_session = item.second;
    
                if(tmp_session.get_id() == (id))
                {
                    found = true;
                }
            }
    	 if (found == true)
    	 	continue;
    	 else
    	 	break;
        }
       session.set_id(id);
    }
    session.set_time_stamp(mslong);
    m_sessions[session.get_username()] = session;

    return id;


}

Session SessionManager::getSession(const std::string& session_username) {
    std::lock_guard<std::mutex> lock{m_mutex};
    return getSession_by_name(session_username);
}

Session SessionManager::getSession_by_Token(const std::string& session_authen_token) {
    std::lock_guard<std::mutex> lock{m_mutex};

    for (const auto& item : m_sessions) 
    {
        const auto& session = item.second;
        if(session.get_authen_token() == session_authen_token)
            return  item.second;
    }
    throw agent_framework::exceptions::NotFound("Session not found.");
}


Session SessionManager::getSession_by_name(const std::string& session_username) {
    auto session = m_sessions.find(session_username);
    if (m_sessions.end() == session) {
        throw agent_framework::exceptions::NotFound("Session '" + session_username + "' not found.");
    }
    return session->second;
}

bool SessionManager::checkSession_by_name(const std::string& session_username) {
    auto session = m_sessions.find(session_username);
    if (m_sessions.end() == session) {
        return false;
    }
    return true;
}

Session SessionManager::getSession(uint64_t session_id) {
    std::lock_guard<std::mutex> lock{m_mutex};
    for (const auto& item : m_sessions) {
        const auto& session = item.second;
        if (session_id == session.get_id()) {
            return getSession_by_name(session.get_username());
        }
    }
    throw agent_framework::exceptions::NotFound("Session (ID: " + std::to_string(session_id) + ") not found.");
}

void SessionManager::delSession_by_name(const std::string& session_username) {

    auto  session = m_sessions.find(session_username);
    if (m_sessions.end() == session) {
        throw agent_framework::exceptions::NotFound("Session '" + session_username + "' not found.");
    }

    const auto& sessions = session->second;
    m_sessions.erase(session);
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
            delSession(new_session.get_username());
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


SessionMap SessionManager::getSession() {
    std::lock_guard<std::mutex> lock{m_mutex};
    return m_sessions;
}

void SessionManager::delSession(const std::string& session_username) {
	
    std::lock_guard<std::mutex> lock{m_mutex};
    delSession_by_name(session_username);
}

void SessionManager::delSession(uint64_t session_id) {
    std::lock_guard<std::mutex> lock{m_mutex};
    for (const auto& item : m_sessions) {
        const auto& session = item.second;
        if (session_id == session.get_id()) {
            delSession_by_name(session.get_username());
            return;
        }
    }
    throw agent_framework::exceptions::NotFound("Session (ID: " + std::to_string(session_id) + ") not found.");
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
