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
message(STATUS "Current cmake is ${CMAKE_VERSION}")

project("PSME FW Build" C CXX)

###############################################################################
add_definitions(-DCOMCAST -DCOCP -DTHERMAL_PSU -DCACCOUNT -DCSESSION -DDECRF_PAL)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -Wno-missing-field-initializers")

string(TIMESTAMP BUILD_TIME "%H:%M:%S, %d-%m-%Y")

# Target folder defaults
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

IF(CMAKE_CROSSCOMPILING)
    set(CMAKE_INCLUDE_PATH "${CMAKE_BINARY_DIR}/include;${CMAKE_SYSTEM_INCLUDE_PATH}"; /usr/include/arm-linux-gnueabi/;/usr/include/)
    set(CMAKE_LIBRARY_PATH "${CMAKE_SYSTEM_LIBRARY_PATH};${CMAKE_BINARY_DIR}/lib; /usr/lib/arm-linux-gnueabi/")
    set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR};${CMAKE_SYSTEM_PREFIX_PATH}; /usr/lib/arm-linux-gnueabi/")
ELSE()
   set(CMAKE_INCLUDE_PATH "${CMAKE_BINARY_DIR}/include;${CMAKE_SYSTEM_INCLUDE_PATH}")
   set(CMAKE_LIBRARY_PATH "${CMAKE_BINARY_DIR}/lib;${CMAKE_SYSTEM_LIBRARY_PATH}")
   set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR};${CMAKE_SYSTEM_PREFIX_PATH}")
   set(LIBBAL_API_DIST "${CMAKE_BINARY_DIR}/../../../../../libbal_api_dist.so")
   set(LIBBAL3_API_DIST "${CMAKE_BINARY_DIR}/../../../../../libbal_host_api.so")
ENDIF()

set(CMAKE_INCLUDE_PATH "${CMAKE_BINARY_DIR}/include;${CMAKE_SYSTEM_INCLUDE_PATH}"; /usr/include/arm-linux-gnueabi/;/usr/include/)

# Path to folder with all cmake scripts
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# add pkg-config directory to search path
IF(CMAKE_CROSSCOMPILING)
    set(CMAKE_PKG_CONFIG_DIR "${CMAKE_BINARY_DIR}/pkgconfig/")
ELSE()
    set(CMAKE_PKG_CONFIG_DIR ${CMAKE_BINARY_DIR}/pkg-config)
ENDIF()

file(MAKE_DIRECTORY "${CMAKE_PKG_CONFIG_DIR}")
set(ENV{PKG_CONFIG_PATH} "${CMAKE_PKG_CONFIG_DIR}" "${CMAKE_MODULE_PATH}")

# Setup compilers
include(AddGnuCompiler)
include(AddClangCompiler)

# Add code coverage
include(AddCodeCoverage)

# Add documentation
include(AddDocumentation)

# Add function which skips global constructor warnings
include(PsmeAgentFramework)
include(InstallScriptsCommon)

option(ENABLE_HTTPS "Enable HTTPS support" ON)

find_package(PkgConfig REQUIRED)
find_package(PackageHandleStandardArgs REQUIRED)

IF(CMAKE_CROSSCOMPILING)

	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DARM_PLATFORM")

	message(STATUS "Init Cross compile ARM 3rd party .......")

	find_package(SafeString)
	if (SAFESTRING_FOUND)
		message(STATUS "For ARM 3rd party src SafeString installed already...")
	else()
		build_and_exit(SafeString)
	endif()

	find_package(JsonCpp)
	if (JSONCPP_FOUND)
		message(STATUS "For ARM 3rd party src JsonCpp installed already...")
	else()
		build_and_exit(JsonCpp)
	endif()

	find_package(JsonRpcCpp)
	if (JSONRPCCPP_FOUND)
		message(STATUS "For ARM 3rd party src JsonRpcCpp installed already...")
	else()
		build_and_exit(JsonRpcCpp)
	endif()

	find_package(UUID)
	if (UUID_FOUND)
		message(STATUS "For ARM 3rd party src UUID installed already...")
	else()
		build_and_exit(UUID)
	endif()

	find_package(Ipmitool)
	if (IPMITOOL_FOUND)
		message(STATUS "For ARM 3rd party src IPMITOOL installed already...")
	else()
		build_and_exit(Ipmitool)
	endif()
ENDIF()


# TODO Issues to be done, order not significant..
#
# TODO parsing args in find_or_install/install_third_party (URL/REQUIRED)
# TODO cleanup with "local" pkgconfig -> store all files in lib/pkgconfig
# TODO GoogleTest/Mock with silent handling
# TODO Check what about google_tests (last "version" on github..)
# TODO extract tar/zip to package-version directory (with stripped "master" directory)
# TODO ac_cv_va_copy env should not be used (is there any option to replace it?)
# TODO remove PCA95XX: it is not used!
# TODO add ABI tags checker for all find scripts (link small file with the lib.. if fails library is not found)
# TODO add warning to all "non-999" packages created by Install

###############################################################################
# Find packages provided by us or by system

install_third_party(Nghttp2)
install_third_party(SpdLog)
#error( "Just for debug" )
# will not link if not ENABLE_HTTPS: some symbols are referenced directly
if (ENABLE_HTTPS)
    find_package(GnuTLS REQUIRED)
endif()

find_package(Curl REQUIRED)

install_third_party(Microhttpd)


IF(NOT CMAKE_CROSSCOMPILING)
install_third_party(JsonCpp)
install_third_party(JsonRpcCpp)
ENDIF()

if (NOT CMAKE_CROSSCOMPILING)
    find_package(UUID)
else()
    install_third_party(UUID)
endif()

IF (NOT CMAKE_CROSSCOMPILING)
find_package(UDEV REQUIRED)
#install_third_party(Ipmitool)
find_package(Sysfs)
ENDIF()
#find_package(Lvm2App REQUIRED)
find_package(NL3 REQUIRED)
find_package(DevMapper REQUIRED)
#find_package(Fm10kd)


IF (NOT CMAKE_CROSSCOMPILING)
install_third_party(SafeString)
ENDIF()

IF (NOT CMAKE_CROSSCOMPILING)
# for agent-simulator
find_package(Libxmlpp)
ENDIF()

if (NOT CMAKE_CROSSCOMPILING)
    # Enable google testing
    install_third_party(GoogleTest)
    install_third_party(GoogleMock)
    enable_testing()
endif()

if (NOT CMAKE_CROSSCOMPILING)
    install_third_party(Nghttp2)
endif()

# Enable sanitizers
if (CMAKE_BUILD_TYPE STREQUAL "asanitize")
    find_package(Asan)
    if (NOT ASAN_FOUND)
        message(FATAL_ERROR "ASAN library not found, Address/Memory SANITIZE version will not be compiled.")
        return()
    endif()
endif()

if (CMAKE_BUILD_TYPE STREQUAL "tsanitize")
    find_package(Tsan)
    if (NOT TSAN_FOUND)
        message(FATAL_ERROR "TSAN library not found, Thread SANITIZE version will not be compiled.")
        return()
    endif()
endif()

###############################################################################
message(STATUS "Building sources...")

add_subdirectory(common)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/agent)
	add_subdirectory(agent)
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/agent-simulator)
	add_subdirectory(agent-simulator)
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/agent-stubs)
	add_subdirectory(agent-stubs)
endif()

add_subdirectory(application)
#add_subdirectory(encrypter)

if (GTEST_FOUND)
    get_property(ut_targets GLOBAL PROPERTY unittest_all_targets)
    list(REMOVE_DUPLICATES ut_targets)
    file(WRITE ${CMAKE_BINARY_DIR}/unittest_targets.txt " ")
    foreach(ut_target ${ut_targets})
        file(APPEND ${CMAKE_BINARY_DIR}/unittest_targets.txt " ${ut_target}")
    endforeach()
endif()
