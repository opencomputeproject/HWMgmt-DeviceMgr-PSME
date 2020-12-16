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

#pragma once
#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/enum/enum_builder.hpp"
#include <string>

namespace agent_framework {
namespace eventing {

ENUM(Notification, std::uint32_t,
    Add,
    Remove,
    Update,
    /*added for ResourceAdded/ResourceRemoved/Alert/ResourceUpdated/StatusChange/ subscription*/
    ResourceAdded,
    ResourceRemoved,
    Alert,
    ResourceUpdated,
    StatusChange
);

class EventData
{
public:
    const std::string &get_gami_id() const
    {
        return m_gami_id;
    }

    void set_gami_id(const std::string &gami_id)
    {
        m_gami_id = gami_id;
    }

    const std::string &get_component() const
    {
        return m_component;
    }

    void set_component(const std::string &component)
    {
        m_component = component;
    }

    Notification get_notification() const
    {
        return m_notification;
    }

    void set_notification(const Notification notification)
    {
        m_notification = notification;
    }

    void set_notification(const std::string &notificaton)
    {
        m_notification = Notification::from_string(notificaton);
    }

    const std::string &get_parent() const
    {
        return m_parent;
    }

    void set_parent(const std::string &parent)
    {
        m_parent = parent;
    }

    ::agent_framework::model::enums::Component get_type() const
    {
        return m_type;
    }

    void set_type(const ::agent_framework::model::enums::Component component)
    {
        m_type = component;
    }

    void set_type(const std::string &type)
    {
        m_type = ::agent_framework::model::enums::Component::from_string(type);
    }

    Json::Value to_json() const
    {
        Json::Value json;

        json["gamiId"] = get_gami_id();
        json["parent"] = get_parent();
        json["type"] = get_type().to_string();
        json["component"] = get_component();
        json["notification"] = get_notification().to_string();
        /*added for ResourceAdded/ResourceRemoved/Alert/ResourceUpdated/StatusChange/ subscription*/	
        json["event_content"] = get_event_content();
        json["event_origin_of_condition"] = get_event_origin_of_condition();
        json["event_severity"] = get_event_severity();
        json["event_message_id"] = get_event_message_id();
        Json::Value oem =get_event_oem(); 
        json["Oem"] =std::move(get_event_oem());
        return json;
    }

    /*Nick added for ResourceAdded/ResourceRemoved/Alert/ResourceUpdated/StatusChange/ subscription*/
    const std::string &get_event_content() const
    {
        return m_event_content;
    }

    const std::string &get_event_origin_of_condition() const
    {
        return m_event_origin_of_condition;
    }

    const std::string &get_event_severity() const
    {
        return m_event_severity;
    }

    const std::string&  get_event_old_state() const 
    {
        return m_event_old_state;
    }

    const Json::Value &get_event_oem() const
    {
        return m_event_oem;
    }

    void set_event_content(const std::string&  content) 
    {
        m_event_content = content;
    }

    void set_event_origin_of_condition(const std::string &content)
    {
        m_event_origin_of_condition = content;
    }

    void set_event_oem(const Json::Value &oem)
    {
        m_event_oem = oem;
    }

    void set_event_message_id(const std::string &content)
    {
        m_event_message_id = content;
    }

    const std::string &get_event_message_id() const
    {
        return m_event_message_id;
    }

    void set_event_old_state(const std::string&  content) 
    {
        m_event_old_state = content;
    }

private:
    ::agent_framework::model::enums::Component m_type{
            ::agent_framework::model::enums::Component::Fan};
    Notification m_notification{Notification::Add};
    std::string m_parent{};
    std::string m_component{};
    std::string m_gami_id{};
    /*added for ResourceAdded/ResourceRemoved/Alert/ResourceUpdated/StatusChange/ subscription*/
    std::string m_event_content{};
    std::string m_event_old_state{};	
    std::string m_event_origin_of_condition{};
    std::string m_event_severity{};
    Json::Value m_event_oem{};
    std::string m_event_message_id{};
};
}
}

