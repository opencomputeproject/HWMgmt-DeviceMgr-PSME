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

#ifndef PSME_REST_SESSION_MANAGER
#define PSME_REST_SESSION_MANAGER

#include "agent-framework/generic/singleton.hpp"
#include "psme/rest/session/model/session.hpp"
#include <mutex>
#include <map>
#include <thread>

namespace psme {
namespace rest {
namespace session {
namespace manager{

using namespace psme::rest::session;
using namespace psme::rest::session::model;

using SessionMap = std::map<std::string, Session>;  //username, Session
/*!
 * SubscriptionManager implementation
 */
class SessionManager : public agent_framework::generic::Singleton<SessionManager> {
public:


    /*!
     * @brief Start a session thread
     *
     */
    void StartSessionThread();

    /*!
     * @brief Check session timeout
     *
     */
    static void CheckSessionTimeout();


    /*!
     * @brief Add session config
     *
     * @param session Session config
     */
    void addSessionConfig(const json::Value& sessionJson);


    /*!
     * @brief Set session config enable
     *
     * @param session Session config enable
     */
    void SetSessionConfigEnable(bool Enable);	

    /*!
     * @brief Get session config enable
     *
     */
    bool GetSessionConfigEnable();


    /*!
     * @brief Set Basic Authen service config enable
     *
     * @param Basic Authen service  config enable
     */
    void SetBasicAuthenServiceConfigEnable(bool Enable);	

    /*!
     * @brief Get Basic Authen service config enable
     *
     */
    bool GetBasicAuthenServiceConfigEnable();

    /*!
     * @brief Set session config enable
     *
     * @param session Session config enable
     */
    void SetSessionConfigTimeOut(uint64_t timeout);

    /*!
     * @brief Get session config enable
     *
     * @param session Session config enable
     */
    uint64_t GetSessionConfigTimeOut();

    /*!
     * @brief Get session config MaxSessions
     *
     */
    uint64_t GetSessionConfigMaxSessions();

    /*!
     * @brief Set session config MaxSessions
     * @param session Session config MaxSessions
     
     *
     */
    void SetSessionConfigMaxSessions(uint64_t MaxSessions);

    /*!
     * @brief Add session
     *
     * @param session Session
     */
    uint64_t addSession(Session session, bool isupdate);

    /*! Mod session
     *
     * @param session Session
     */
    uint64_t modSession(Session session);

    /*!
     * @brief Get session by session id
     *
     * @param session_id Session id
     * @return Session
     */
    Session getSession(uint64_t session_id);

    /*!
     * @brief Returns session by username
     *
     * @param session_username Session name
     * @return Session
     */
    Session getSession(const std::string& session_username);

    /*!
     * @brief Returns if this session exist or not by authen token
     *
     * @param session_authen_token
     * @return Session
     */
    Session getSession_by_Token(const std::string& session_authen_token);

    /*!
     * @brief Get all sessions
     *
     * @return Session collection
     */
    SessionMap getSession();

    /*!
     * @brief Removes subscription by subscription id
     *
     * @param subscription_id Subscription id
     */
    void delSession(uint64_t session_id);
    
    /*!
     * @brief Removes session by username
     *
     * @param username
     */
    void delSession(const std::string& session_username);

    /*!
     * @brief Get number of subscriptions
     *
     * @return Number of subscriptions
     */
    uint32_t Session_size();

    bool updateSessionTimestamp(const std::string& authen_token);
    /*!
     * @brief Destructor
     */
    virtual ~SessionManager();

    /*!
     * @brief Creates json representation of session config
     *
     * @return JSON representation of subscription
     */
    json::Value to_json() const;


    /*!
     * @brief Removes all sessions 
     *
     */
    void delAllSession();
    
    /*!
     * @brief init exception uri path 
     *
     */
    void initExceptionUri();

    bool chkExceptionUri(const std::string& uri);

    bool checkSession_by_name(const std::string& session_username);

private:


    Session getSession_by_name(const std::string& session_username);


    void delSession_by_name(const std::string& session_username);

    uint64_t m_MaxSessions{20};

    uint64_t m_SessionTimeout{600};

    bool m_SessionServiceEnable {false};	

    bool m_BasicAuthenServiceEnable {false};		
    
    SessionMap m_sessions{};
	
    std::mutex m_mutex{};

    static bool m_chk_start;

    std::vector<std::string> m_exception_uri{};	

    static std::uint64_t id;
};


}
}
}
}

#endif //PSME_REST_EVENTING_SUBSCRIPTION_MANAGER
