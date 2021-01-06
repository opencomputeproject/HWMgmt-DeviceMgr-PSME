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
project(ecopenbmc_helper CXX)

set(EC_OPENBMC_HELPER_LIBRARIES
    ecopenbmc_helper 
    CACHE PATH "ecopenbmc_helper libraries"
)

set(EC_OPENBMC_HELPER_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    CACHE PATH "ecopenbmc_helper include directory"
)

include_directories(
    include
    ../ecsys_helper/include
    ../eclog_helper/include
)

include_directories(SYSTEM
    ${SAFESTRING_INCLUDE_DIRS}
    ${UUID_INCLUDE_DIRS}
    ${MD5_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
    ${JSONRPCCPP_INCLUDE_DIRS}
    ${EC_LOG_HELPER_INCLUDE_DIRS}
)

add_library(ecopenbmc_helper STATIC
    src/ecopenbmc_helper.cpp
)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU|Clang)
  set_source_files_properties(
    src/ecopenbmc_helper.cpp
    PROPERTIES COMPILE_FLAGS "-Wno-conversion"
    )
endif()

install(TARGETS ecopenbmc_helper 
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

