# <license_header>
#
# Copyright (c) 2015-2017 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Edgecore DeviceManager
# Copyright 2020-2021 Edgecore Networks, Inc.
#
# This product includes software developed at
# Edgecore Networks Inc. (http://www.edge-core.com/).
#
# </license_header>

cmake_minimum_required(VERSION 2.8)
project("PSME Chassis" C CXX)

#if (CMAKE_CROSSCOMPILING)
#    message(WARNING "Not supported for crosscompiling.")
#    return()
#endif()

set(AGENT_CHASSIS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(AGENT_CHASSIS_LIBRARIES
    -Wl,--whole-archive chassis-commands -Wl,--no-whole-archive
)

include_directories(
    ${BASE64_INCLUDE_DIRS}
    ${COMMON_INCLUDE_DIRS}
    ${LOGGER_INCLUDE_DIRS}
    ${IPMI_INCLUDE_DIRS}
    ${JSONCXX_INCLUDE_DIRS}
    ${CONFIGURATION_INCLUDE_DIRS}
    ${AGENT_FRAMEWORK_INCLUDE_DIRS}
    ${NET_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${EC_NET_HELPER_INCLUDE_DIRS}
    ${EC_LOG_HELPER_INCLUDE_DIRS}
    ${EC_SYS_HELPER_INCLUDE_DIRS}
    ${EC_RF_PAL_HELPER_INCLUDE_DIRS}
    ${EC_BAL3_API_DIST_HELPER_INCLUDE_DIRS}
)

include_directories(SYSTEM
    ${SAFESTRING_INCLUDE_DIRS}
    ${UUID_INCLUDE_DIRS}
    ${MD5_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
    ${JSONRPCCPP_INCLUDE_DIRS}
    ${CY_USB_SERIAL_INCLUDE_DIRS}
)

add_subdirectory(src)
add_subdirectory(tests)
