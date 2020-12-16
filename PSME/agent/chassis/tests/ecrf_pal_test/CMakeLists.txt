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

if (NOT GTEST_FOUND)
    return()
endif()

add_gtest(ecrf_pal psme-chassis
    ecrf_pal_test.cpp
    test_runner.cpp
)

include_directories(
    ${EC_NET_HELPER_INCLUDE_DIRS}
    ${EC_LOG_HELPER_INCLUDE_DIRS}
    ${EC_SYS_HELPER_INCLUDE_DIRS}
    ${EC_RF_PAL_HELPER_INCLUDE_DIRS}
    ${SPDLOG_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
)

TARGET_LINK_LIBRARIES(${test_target}
    ${EC_RF_PAL_HELPER_LIBRARIES}
    ${EC_NET_HELPER_LIBRARIES}
    ${EC_LOG_HELPER_LIBRARIES}
    ${EC_SYS_HELPER_LIBRARIES}
    ${EC_BAL_API_DIST_HELPER_LIBRARIES}
    ${SPDLOG_LIBRARIES}
    libm.so
    jsoncpp
    json-cxx
    dl
    )
