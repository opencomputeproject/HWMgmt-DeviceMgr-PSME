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
 * @file evens_queue.hpp
 * */

#pragma once
#include "event_data.hpp"
#include "generic/threadsafe_queue.hpp"
#include "agent-framework/generic/singleton.hpp"

namespace agent_framework {
/*! Eventing namespace */
namespace eventing {

class EventsQueue : public ::generic::ThreadSafeQueue<EventData>,
        public agent_framework::generic::Singleton<EventsQueue> {
public:
    virtual ~EventsQueue();

private:
    friend class agent_framework::generic::Singleton<EventsQueue>;
    EventsQueue() {}
};

}
}
