# <license_header>
#
# Copyright (c) 2022-2024 Celestica Corporation
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
# </license_header>

# check if package exists in the third_party directory
# download and unpack otherwise


function(install_nghttp2)
    assure_package("libnghttp2" "1.49.0" "https://github.com/nghttp2/nghttp2/archive/refs/tags/v1.49.0.zip" "4b7b5458ae53567fc1c1936e7b0f6240")

    set(ARGS)
    list(APPEND ARGS -DCMAKE_PREFIX_PATH:PATH=${CMAKE_BINARY_DIR})
    list(APPEND ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR})

    list(APPEND ARGS -DBUILD_SHARED_LIBS:BOOL=ON)
    list(APPEND ARGS -DBUILD_STATIC_LIBS:BOOL=OFF)

    if (CMAKE_CROSSCOMPILING)
        if (CMAKE_TOOLCHAIN_FILE AND EXISTS ${CMAKE_TOOLCHAIN_FILE})
            list(APPEND ARGS -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE})
        endif()

        list(APPEND ARGS -DCMAKE_FIND_ROOT_PATH:PATH=${CMAKE_BINARY_DIR})
    endif()

    if (CMAKE_BUILD_TYPE)
        list(APPEND ARGS -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE})
    endif()

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

    file(COPY ${binary_dir}/lib/libnghttp2.pc DESTINATION ${CMAKE_PKG_CONFIG_DIR})

    file(INSTALL ${binary_dir}/lib/
        DESTINATION ${CMAKE_BINARY_DIR}/lib
        FILES_MATCHING PATTERN "*.so*"
        PATTERN "CMakeFiles" EXCLUDE
    )

    file(INSTALL ${binary_dir}/lib/includes/nghttp2
        DESTINATION ${CMAKE_BINARY_DIR}/include
    )

    file(INSTALL ${source_dir}/lib/includes/nghttp2/
        DESTINATION ${CMAKE_BINARY_DIR}/include/nghttp2
        FILES_MATCHING PATTERN "*.h"
    )

endfunction()

install_nghttp2()
