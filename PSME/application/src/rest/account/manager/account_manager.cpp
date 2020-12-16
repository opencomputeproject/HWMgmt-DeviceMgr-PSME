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


#include "psme/rest/account/manager/account_manager.hpp"
#include "psme/rest/server/error/server_exception.hpp"
#include "psme/rest/server/error/error_factory.hpp"

#include <json/json.hpp>
#include <fstream>



namespace psme {
namespace rest {
namespace account {
namespace manager {

std::uint64_t AccountManager::id = 1;
std::uint64_t AccountManager::rid = 1;

uint64_t AccountManager::addRole(Role role) {
    std::lock_guard<std::mutex> lock{m_role_mutex};
    auto item = m_roles.find(role.get_roleid());
    if (m_roles.end() != item) {
        throw error::ServerException(error::ErrorFactory::create_resource_already_exists_error(
            "role '" + role.get_roleid() + "' already exists."
        ));
    }
    role.set_id(rid++);
    m_roles[role.get_roleid()] = role;
    return  role.get_id();
}

Role AccountManager::getRole(const std::string& roleid) {
    std::lock_guard<std::mutex> lock{m_role_mutex};
    return getRole_by_roleid(roleid);
}

Role AccountManager::getRole_by_roleid(const std::string& roleid) {
    auto role = m_roles.find(roleid);
    if (m_roles.end() == role) {
        throw agent_framework::exceptions::NotFound("Role '" + roleid + "' not found.");
    }
    return role->second;
}

Role AccountManager::getRole(uint64_t role_id) {
    std::lock_guard<std::mutex> lock{m_role_mutex};
    for (const auto& item : m_roles) {
        const auto& role = item.second;
        if (role_id == role.get_id()) {
            return getRole_by_roleid(role.get_roleid());
        }
    }
    throw agent_framework::exceptions::NotFound("Role (ID: " + std::to_string(role_id) + ") not found.");
}

void AccountManager::delRole_by_roleid(const std::string& roleid) {
    auto item = m_roles.find(roleid);
    if (m_roles.end() == item) {
        throw agent_framework::exceptions::NotFound("RoleID '" + roleid + "' not found.");
    }
    m_roles.erase(item);
}

RoleMap AccountManager::getRole() {
    std::lock_guard<std::mutex> lock{m_role_mutex};
    return m_roles;
}

void AccountManager::delRole(const std::string& roleid) {
    std::lock_guard<std::mutex> lock{m_role_mutex};
    delRole_by_roleid(roleid);
}

void AccountManager::delRole(uint64_t role_id) {
    std::lock_guard<std::mutex> lock{m_role_mutex};
    for (const auto& item : m_roles) {
        const auto& role = item.second;
        if (role_id == role.get_id()) {
            delRole_by_roleid(role.get_roleid());
            return;
        }
    }
    throw agent_framework::exceptions::NotFound("Role (ID: " + std::to_string(role_id) + ") not found.");
}

uint32_t AccountManager::Role_size() {
    std::lock_guard<std::mutex> lock{m_role_mutex};
    return static_cast<uint32_t>(m_roles.size());
}


uint64_t AccountManager::addAccount(Account account) {
    std::lock_guard<std::mutex> lock{m_mutex};
    auto sub = m_accounts.find(account.get_username());
    if (m_accounts.end() != sub) {
        throw error::ServerException(error::ErrorFactory::create_resource_already_exists_error(
            "Account '" + account.get_username() + "' already exists."
        ));
    }
//viclin check roleid is existed or not
    
    account.set_id(id++);
    m_accounts[account.get_username()] = account;
    return account.get_id();
}

bool AccountManager::AccountExisted(const std::string& account_username)   
{

  std::lock_guard<std::mutex> lock{m_mutex};
  auto account = m_accounts.find(account_username);
  if (m_accounts.end() == account) {
      return false;
  }
  else return true;

}
Account AccountManager::getAccount(const std::string& account_username) {
    std::lock_guard<std::mutex> lock{m_mutex};
    return getAccount_by_name(account_username);
}

Account AccountManager::getAccount_by_name(const std::string& account_username) {
    auto account = m_accounts.find(account_username);
    if (m_accounts.end() == account) {
        throw agent_framework::exceptions::NotFound("Account '" + account_username + "' not found.");
    }
    return account->second;
}

Account AccountManager::getAccount(uint64_t account_id) {
    std::lock_guard<std::mutex> lock{m_mutex};
    for (const auto& item : m_accounts) {
        const auto& account = item.second;
        if (account_id == account.get_id()) {
            return getAccount_by_name(account.get_username());
        }
    }
    throw agent_framework::exceptions::NotFound("Account (ID: " + std::to_string(account_id) + ") not found.");
}

void AccountManager::delAccount_by_name(const std::string& account_username) {
    auto account = m_accounts.find(account_username);
    if (m_accounts.end() == account) {
        throw agent_framework::exceptions::NotFound("Account '" + account_username + "' not found.");
    }
//Can not Delete admin/redfish account
    if ( account_username == "admin" ) {
        throw error::ServerException(error::ErrorFactory::create_error_for_not_allowable_value(
            "'admin' Can not be removed","username","admin",""));    
     }
//    
    m_accounts.erase(account);
}

AccountMap AccountManager::getAccount() {
    std::lock_guard<std::mutex> lock{m_mutex};
    return m_accounts;
}

void AccountManager::delAccount(const std::string& account_username) {
    std::lock_guard<std::mutex> lock{m_mutex};
    delAccount_by_name(account_username);
}

void AccountManager::modAccount(const std::string& account_username, Account account) {
    std::lock_guard<std::mutex> lock{m_mutex};
    if (account_username != account.get_username() )
        delAccount_by_name(account_username);
    m_accounts[account.get_username()] = account;
}


int AccountManager::login(const std::string& account_username, const std::string& account_password) {
    std::lock_guard<std::mutex> lock{m_mutex};
    
    auto account = m_accounts.find(account_username);
    if ( account == m_accounts.end())
        throw agent_framework::exceptions::NotFound("Account '" + account_username + "' not found.");
    else
    {
    	auto res = account->second.checkpw(account_password);
    	return res;
    }
    
}

void AccountManager::timekick(const std::chrono::steady_clock::time_point& now) {
    std::lock_guard<std::mutex> lock{m_mutex};
    for (auto& item : m_accounts) {
    	 auto& account = item.second;
    	 account.timekick(now);
    }	
}


void AccountManager::delAccount(uint64_t account_id) {
    std::lock_guard<std::mutex> lock{m_mutex};
    for (const auto& item : m_accounts) {
        const auto& account = item.second;
        if (account_id == account.get_id()) {
            delAccount_by_name(account.get_username());
            return;
        }
    }
    throw agent_framework::exceptions::NotFound("Account (ID: " + std::to_string(account_id) + ") not found.");
}

uint32_t AccountManager::Account_size() {
    std::lock_guard<std::mutex> lock{m_mutex};
    return static_cast<uint32_t>(m_accounts.size());
}



AccountManager::~AccountManager() { }

}
}
}
}
