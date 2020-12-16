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
#include <string>
#include <vector>

namespace psme {
namespace rest {
namespace utils {

namespace {
    constexpr const size_t BUFFER_LEN= 512;
    constexpr const size_t BUFFER_LEN_1024= 1024;
}

class EcCommonUtils
{
public:
    static const std::string  m_onie_boot_dir;
    static void exec_shell(const char *cmd, char *result_a, int time_out);
    static void exec_shell_(std::string cmd, std::string &result_a, int time_out);
    static void mount_onie_boot_part();
    static void umount_onie_boot_part();
    static bool Support_EFI();
    static bool Support_TFTP();
    static bool Support_WGET();
    static std::string GetEfiBootCurrent();
    static std::string GetEfiBootOrder();
    static std::string GetEfiBootIndexByName(std::string);
    static std::string GetONIEBootMode();
    static bool SetONIEBootModeToONIE(std::string mode);
    static std::vector<std::string> split(const std::string &s, char delimiter);
    static bool SetEfiBootCurrent(std::string order_string);
    static bool SetEfiBootNext(std::string order_string);
    static bool DelEfiBootNext();
    ~EcCommonUtils(){};
};

} // namespace utils
} // namespace rest
} // namespace psme
