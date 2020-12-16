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

#include <ecrf_pal/watcher/watcher.hpp>
#include "logger/logger_factory.hpp"

using namespace agent::chassis::ecrf_pal::watcher;

Watcher::~Watcher() {
    stop();
}

void Watcher::add_task(const TaskSPtr task) {
    m_tasks.emplace_back(task);
}

void Watcher::start() {
    if (!m_running) {
        m_running = true;
        m_thread = std::thread(&Watcher::watch, this);
    }
}

void Watcher::stop() {
    if (m_running) {
        m_running = false;
        signal.notify_one();
        if (m_thread.joinable()) {
            m_thread.join();
            log_debug(GET_LOGGER("rest"), "Watcher job done!");
        }
    }
}

namespace {
    const unsigned SLEEP_TIME = 3;
}

void Watcher::watch() {
    while (m_running) {

        for (std::size_t i = 0, size = m_tasks.size(); i < size; ++i) {
            m_tasks[i]->execute();
        }

        std::unique_lock<std::mutex> lock(mutex);
        signal.wait_for(lock, std::chrono::seconds(SLEEP_TIME));
    }
}
