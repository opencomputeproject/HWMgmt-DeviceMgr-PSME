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
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(ecrf_pal)
add_subdirectory(command)
add_subdirectory(loader)
add_subdirectory(tree_stability)

add_library(chassis-commands STATIC
    $<TARGET_OBJECTS:chassis-command-acc>
)

add_library(chassis-utils
    $<TARGET_OBJECTS:chassis-loader>
    $<TARGET_OBJECTS:chassis-tree-stability>
)

add_library(chassis-ecrf_pal
    $<TARGET_OBJECTS:ecrf_pal-service>
)

add_executable(psme-chassis
    main.cpp
)

TARGET_LINK_LIBRARIES(psme-chassis
    ${AGENT_CHASSIS_LIBRARIES}
    chassis-utils
    chassis-ecrf_pal
    ${AGENT_FRAMEWORK_LIBRARIES}
    ${UUID_LIBRARIES}
    ${LOGGER_LIBRARIES}
    ${SPDLOG_LIBRARIES}
    ${CONFIGURATION_LIBRARIES}
    ${JSONCXX_LIBRARIES}
    ${BASE64_LIBRARIES}
    pthread
    jsonrpccpp-server
    jsonrpccpp-client
    jsonrpccpp-common
    jsoncpp
    ${MICROHTTPD_LIBRARIES}
    ${CURL_LIBRARIES}
    ${SAFESTRING_LIBRARIES}
    ${EC_RF_PAL_HELPER_LIBRARIES}
    ${EC_NET_HELPER_LIBRARIES}
    ${EC_LOG_HELPER_LIBRARIES}
    ${EC_SYS_HELPER_LIBRARIES}
    ${EC_OPENBMC_HELPER_LIBRARIES}
    ${SPDLOG_LIBRARIES}
    libm.so
    md5
    dl
    )