/*!
 * @copyright
 * Copyright (c) 2015-2017 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * */

#include "psme/rest/eventing/manager/subscription_manager.hpp"
#include "psme/rest/eventing/event_service.hpp"
#include "psme/rest/server/error/server_exception.hpp"
#include "psme/rest/server/error/error_factory.hpp"

#include <json/json.hpp>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

namespace psme {
namespace rest {
namespace eventing {
namespace manager {

bool inline comp_by_value(pair<string, Subscription> &p1, pair<string, Subscription> &p2)
{
   return p1.second.get_id() < p2.second.get_id();
}

uint64_t SubscriptionManager::get_sub_id()
{
    uint64_t id=0;
    // Sort increase of m_subscriptions by event member ID //
    vector<pair<string, Subscription>> myVec(m_subscriptions.begin(), m_subscriptions.end());
    sort(myVec.begin(),  myVec.end(), comp_by_value);
    //Get  first not used ID from 1 //
    for(id = 1 ; (id <= m_MaxEvents ) ; id++)
    {
        bool found = false;	    
        //Check if can find id in myVec //
        for (const auto& item : myVec) 
        {
            const auto& tmp_event = item.second;
            
            if(tmp_event.get_id() == (id))
            {
                found = true;
            }
        }
        if (found == true)
            continue;
        else
            return id;
    }
    return 0;
}

uint64_t SubscriptionManager::add(Subscription subscription) {
    std::lock_guard<std::mutex> lock{m_mutex};
    auto sub = m_subscriptions.find(subscription.get_name());
    if (m_subscriptions.end() != sub) {
        throw error::ServerException(error::ErrorFactory::create_resource_already_exists_error(
            "Subscription '" + subscription.get_name() + "' already exists."
        ));
    }
	
    if (subscription.get_id() != 0) 
    {
        //From subscription tmp file  //    
        uint64_t id = subscription.get_id();

        subscription.set_id(id);
        m_subscriptions[subscription.get_name()] = subscription;
        return subscription.get_id();
    }
    else  //From web post add //
    {
        uint64_t id = get_sub_id();
    
        if ( id != 0)
        {
            subscription.set_id(id);
            m_subscriptions[subscription.get_name()] = subscription;
            return subscription.get_id();
        }
        else
            return 0;    
    }
}

Subscription SubscriptionManager::get(const std::string& subscription_name) {
    std::lock_guard<std::mutex> lock{m_mutex};
    return get_by_name(subscription_name);
}

Subscription SubscriptionManager::get_by_name(const std::string& subscription_name) {
    auto subscription = m_subscriptions.find(subscription_name);
    if (m_subscriptions.end() == subscription) {
        throw agent_framework::exceptions::NotFound("Subscription '" + subscription_name + "' not found.");
    }
    return subscription->second;
}

Subscription SubscriptionManager::get(uint64_t subscription_id) {
    std::lock_guard<std::mutex> lock{m_mutex};
    for (const auto& item : m_subscriptions) {
        const auto& subscription = item.second;
        if (subscription_id == subscription.get_id()) {
            return get_by_name(subscription.get_name());
        }
    }
    throw agent_framework::exceptions::NotFound("Subscription (ID: " + std::to_string(subscription_id) + ") not found.");
}

void SubscriptionManager::del_by_name(const std::string& subscription_name) {
    auto subscription = m_subscriptions.find(subscription_name);
    if (m_subscriptions.end() == subscription) {
        throw agent_framework::exceptions::NotFound("Subscription '" + subscription_name + "' not found.");
    }
    m_subscriptions.erase(subscription);
}

SubscriptionMap SubscriptionManager::get() {
    std::lock_guard<std::mutex> lock{m_mutex};
    return m_subscriptions;
}

void SubscriptionManager::del(const std::string& subscription_name) {
    std::lock_guard<std::mutex> lock{m_mutex};
    del_by_name(subscription_name);
}

void SubscriptionManager::del(uint64_t subscription_id) {
    std::lock_guard<std::mutex> lock{m_mutex};
    for (const auto& item : m_subscriptions) {
        const auto& subscription = item.second;
        if (subscription_id == subscription.get_id()) {
            del_by_name(subscription.get_name());
            return;
        }
    }
    throw agent_framework::exceptions::NotFound("Subscription (ID: " + std::to_string(subscription_id) + ") not found.");
}

uint32_t SubscriptionManager::size() {
    std::lock_guard<std::mutex> lock{m_mutex};
    return static_cast<uint32_t>(m_subscriptions.size());
}

void SubscriptionManager::notify(const Event& event) {
    std::lock_guard<std::mutex> lock{m_mutex};
    do_notify(event);
}

void SubscriptionManager::notify(const EventVec& events) {
    std::lock_guard<std::mutex> lock{m_mutex};
    for (const auto& event : events) {
        do_notify(event);
    }
}

void SubscriptionManager::do_notify(const Event& event) {
    EventUPtr add_event(new Event(event));
    EventService::post_event(std::move(add_event));
    log_info(GET_LOGGER("rest"), "Upstream event enqueued: type=" << event.get_type().to_string()
                                 << ", link=" << event.get_origin_of_condition());
}

SubscriptionManager::~SubscriptionManager() { }

}
}
}
}
