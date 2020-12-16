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
project(ecrf_pal_helper CXX)

set(EC_RF_PAL_HELPER_LIBRARIES
    ecrf_pal_helper 
    CACHE PATH "ecrf_pal_helper libraries"
)

set(EC_RF_PAL_HELPER_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    CACHE PATH "ecrf_pal_helper include directory"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive ")

#include current include path
include_directories(
    include
)

include_directories(SYSTEM
    ${SAFESTRING_INCLUDE_DIRS}
    ${UUID_INCLUDE_DIRS}
    ${MD5_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
    ${JSONRPCCPP_INCLUDE_DIRS}
    ${EC_NET_HELPER_INCLUDE_DIRS}
    ${EC_SYS_HELPER_INCLUDE_DIRS}
    ${EC_LOG_HELPER_INCLUDE_DIRS}
    ${EC_RF_PAL_HELPER_INCLUDE_DIRS}
)

#include OpenNetworkLinux path
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../OpenNetworkLinux/packages/base/armel/faultd/builds/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../OpenNetworkLinux/packages/base/any/onlp/src/onlplib/module/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../OpenNetworkLinux/packages/base/any/onlp/src/onlp/module/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../OpenNetworkLinux/packages/base/amd64/onlp/builds/onlp-platform-defaults/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../OpenNetworkLinux/sm/bigcode/modules/IOF/module/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../OpenNetworkLinux/sm/bigcode/modules/cjson_util/module/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../OpenNetworkLinux/sm/bigcode/modules/cjson/module/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../OpenNetworkLinux/sm/bigcode/modules/sff/module/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../OpenNetworkLinux/sm/infra/modules/AIM/module/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common/json-cxx/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common/agent-framework/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common/logger/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common/agent-framework/include/agent-framework/exceptions/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common/include/
)

add_library(ecrf_pal_helper STATIC
    src/api/ecrf_pal_helper.cpp
    src/api/ecrf_pal_helper_api.cpp
    src/api/ecrf_pal_com.cpp
    src/api/ecrf_pal_com_onl.cpp
    src/api/ecrf_pal_com_sonic.cpp
    src/asgvolt64.cpp
    src/switch_sys_mode.cpp
    src/asxvolt16.cpp
    src/as5916_54xks.cpp
    src/as5916_54xk.cpp
)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU|Clang)
  set_source_files_properties(
    src/api/ecrf_pal_helper.cpp
    src/api/ecrf_pal_helper_api.cpp
    src/api/ecrf_pal_com.cpp
    src/api/ecrf_pal_com_onl.cpp
    src/api/ecrf_pal_com_sonic.cpp
    src/asgvolt64.cpp
    src/switch_sys_mode.cpp
    src/asxvolt16.cpp
    src/as5916_54xks.cpp
    src/as5916_54xk.cpp
    PROPERTIES COMPILE_FLAGS "-Wno-conversion"
    )
endif()

install(TARGETS ecrf_pal_helper 
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

