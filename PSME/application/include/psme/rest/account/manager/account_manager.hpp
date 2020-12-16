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

#ifndef PSME_REST_ACCOUNT_MANAGER
#define PSME_REST_ACCOUNT_MANAGER

#include "agent-framework/generic/singleton.hpp"
#include "psme/rest/account/model/account.hpp"
#include "psme/rest/account/model/role.hpp"
#include <mutex>
#include <map>

namespace psme {
namespace rest {
namespace account {
namespace manager{

using namespace psme::rest::account;
using namespace psme::rest::account::model;

using AccountMap = std::map<std::string, Account>;  //username, Account
using RoleMap = std::map<std::string, Role>;        //roleid, Role
/*!
 * AccountManager implementation
 */
class AccountManager : public agent_framework::generic::Singleton<AccountManager> {
public:

    /*!
     * @brief Add role
     *
     * @param Role
     */
    uint64_t addRole(Role role);

    /*!
     * @brief Get role by role id
     *
     * @param role_id role id
     * @return Role
     */
    Role getRole(uint64_t role_id);

    /*!
     * @brief Returns role by roleid
     *
     * @param roleid
     * @return Role
     */
    Role getRole(const std::string& roleid);

    /*!
     * @brief Get all roles
     *
     * @return Role collection
     */
    RoleMap getRole();

    /*!
     * @brief Removes role by role id
     *
     * @param role_id
     */
    void delRole(uint64_t role_id);

    /*!
     * @brief Removes role by roleid
     *
     * @param roleid
     */
    void delRole(const std::string& roleid);

    /*!
     * @brief Get number of roles
     *
     * @return Number of roles
     */
    uint32_t Role_size();



    /*!
     * @brief Add account
     *
     * @param account Account
     */
    uint64_t addAccount(Account account);

    /*!
     * @brief Get account by account id
     *
     * @param account_id Account id
     * @return Account
     */
    Account getAccount(uint64_t account_id);

    /*!
     * @brief Returns account by username
     *
     * @param account_username Account name
     * @return Account
     */
    Account getAccount(const std::string& account_username);

    /*!
     * @brief Get all accounts
     *
     * @return Account collection
     */
    AccountMap getAccount();

    /*!
     * @brief Removes subscription by subscription id
     *
     * @param subscription_id Subscription id
     */
    void delAccount(uint64_t account_id);
    
    
    /*!
     *
     * @param modify account by Username 
     */
    
    void modAccount(const std::string& account_username,Account account);

    /*!
     * @brief Removes account by username
     *
     * @param username
     */
    void delAccount(const std::string& account_username);
    
    /*!
     * @brief Check account existed  by username
     *
     * @param username
     */
    bool AccountExisted(const std::string& account_username);    
    
    /*!
     * @brief login by username/password
     *
     * @param account_username,account_password
     * @return 0: ok  n: login fail times
     */    
    
    int login(const std::string& account_username, const std::string& account_password);
    /*!
     * @brief timekick Only Called by WallClock_5sec Task for every 5 seconds
     *
     * @param now
     * 
     */    
    
    void timekick(const std::chrono::steady_clock::time_point& now);


    /*!
     * @brief Get number of subscriptions
     *
     * @return Number of subscriptions
     */
    uint32_t Account_size();


    /*!
     * @brief Destructor
     */
    virtual ~AccountManager();

private:


    Account getAccount_by_name(const std::string& account_username);

    void delAccount_by_name(const std::string& account_username);
    
    
    Role getRole_by_roleid(const std::string& roleid);

    void delRole_by_roleid(const std::string& roleid);

    AccountMap m_accounts{};
    RoleMap   m_roles{};
    
    std::mutex m_role_mutex{};
    std::mutex m_mutex{};

    static std::uint64_t id;
    static std::uint64_t rid;
};


}
}
}
}

#endif //PSME_REST_EVENTING_SUBSCRIPTION_MANAGER
