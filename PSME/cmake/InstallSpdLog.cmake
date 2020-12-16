# Edgecore DeviceManager
# Copyright 2020-2021 Edgecore Networks, Inc.
#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.

if (CMAKE_CROSSCOMPILING)
    message(WARNING "Not supported for crosscompaling.")
    return()
endif()


function(install_spdlog)
    include(ConfigurationPackage OPTIONAL)
    assure_package("spdlog" "1.8.2" "https://github.com/gabime/spdlog/archive/v1.8.1.zip")

    set(ARGS)
    list(APPEND ARGS -DCMAKE_CXX_FLAGS:STRING=-fPIC)
    list(APPEND ARGS -DCMAKE_PREFIX_PATH:PATH=${CMAKE_BINARY_DIR})
    list(APPEND ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR})

    execute_process(
        COMMAND ${CMAKE_COMMAND} ${ARGS} ${source_dir}
        WORKING_DIRECTORY ${binary_dir}
        RESULT_VARIABLE result
    )
    if (NOT ${result} EQUAL 0)
        message(FATAL_ERROR "Error occurs when configure project")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${binary_dir} --target all -- ${BUILD_EXTRA_ARGS}
        WORKING_DIRECTORY ${binary_dir}
        RESULT_VARIABLE result
    )
    if (NOT ${result} EQUAL 0)
        message(FATAL_ERROR "Error occurs when build project")
    endif()

    file(COPY ${binary_dir}/spdlog.pc DESTINATION ${CMAKE_PKG_CONFIG_DIR})

    configure_file(${binary_dir}/libspdlog.a
        ${CMAKE_BINARY_DIR}/lib/libspdlog.a
        COPYONLY
    )

    file(INSTALL ${source_dir}/include/spdlog/
        DESTINATION ${CMAKE_BINARY_DIR}/include/spdlog
        FILES_MATCHING PATTERN "*.h"
    )
endfunction()


install_spdlog()
