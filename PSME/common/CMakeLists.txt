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

# Add internal libraries & includes
add_subdirectory(include)
add_subdirectory(md5)
add_subdirectory(base64)
#add_subdirectory(ipmi)
add_subdirectory(logger)
add_subdirectory(json-cxx)
add_subdirectory(configuration)
add_subdirectory(agent-framework)
add_subdirectory(netlink)
add_subdirectory(smbios)
add_subdirectory(fru_eeprom)
add_subdirectory(net)
add_subdirectory(ssdp)
add_subdirectory(ecnet_helper)
add_subdirectory(eclog_helper)
add_subdirectory(ecopenbmc_helper)
add_subdirectory(ecsys_helper)
add_subdirectory(ecrf_pal_helper)

IF("${CUSE_EC_BAL3_DISTLIB}" STREQUAL "TRUE" )
    add_subdirectory(ecbal_api_dist_helper)
ENDIF()
