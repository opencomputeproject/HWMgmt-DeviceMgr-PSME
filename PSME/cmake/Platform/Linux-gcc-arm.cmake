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

set (CMAKE_VERBOSE_MAKEFILE true)

set(COMPILER_PREFIX arm-linux-gnueabi)

SET(TOOLCHAIN_DIR "/usr/bin/")


set(CMAKE_C_COMPILER    /usr/bin/${COMPILER_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER  /usr/bin/${COMPILER_PREFIX}-g++)

set(CMAKE_SYSTEM_NAME       Linux)
set(CMAKE_SYSTEM_VERSION    1)

set(CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH} / )

include_directories(/usr/include/arm-linux-gnueabi/)
#include_directories(/usr/include/)

# Search in host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Search libraries and includes in CMAKE_ROOT_PATH only:
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
