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

#pragma once
#include <ecrf_pal/watcher/task.hpp>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

/*! Agent namspace */
namespace agent {
/*! Chassis namspace */
namespace chassis {
/*! IPMB namspace */
namespace ecrf_pal {
/*! Watcher namspace */
namespace watcher {

/*! ECRF_PAL Watcher */
class Watcher {
public:
    ~Watcher();

    /*! Start Watcher */
    void start();
    /*! Stop Watcher */
    void stop();

    /*! Add task to Watcher */
    void add_task(const TaskSPtr task);
private:
    using TaskVec = std::vector<TaskSPtr>;

    TaskVec m_tasks{};
    std::thread m_thread{};
    std::atomic<bool> m_running{false};

    std::mutex mutex {};
    std::condition_variable signal {};
    void watch();
};

}
}
}
}

