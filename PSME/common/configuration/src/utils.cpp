/*!
 * @section LICENSE
 *
 * @copyright
 * Copyright (c) 2015-2017 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @section DESCRIPTION
 *
 * */

#include "configuration/utils.hpp"
#include "logger/logger_factory.hpp"
#include "json/json.hpp"

#include <unistd.h>
#include <stdlib.h>
#include <crypt.h>
#include <string.h>
#include <fstream>

void configuration::file_to_json(const std::string& filename,
                                                        json::Value& root) {
    std::string json_string{};
    std::ifstream file_stream;
    file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file_stream.open(filename);

    file_stream.seekg(0, std::ios::end);

    /* Get file size */
    auto size = file_stream.tellg();
    if (size < 0) {
        /* Don't blow string object with large dynamic allocation */
        size = 0;
    }

    json_string.reserve(std::string::size_type(size));

    /* Back to the file begin */
    file_stream.seekg(0, std::ios::beg);

    /* Copy all file from input file stream to string */
    json_string.assign((std::istreambuf_iterator<char>(file_stream)),
                std::istreambuf_iterator<char>());

    if (!string_to_json(json_string, root)) {
        throw std::runtime_error("JSON parse error.");
    }
}

void configuration::json_to_file(const std::string& filename, json::Value& root) {
    std::ofstream file_stream;
    file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file_stream.open(filename, std::ofstream::out | std::ofstream::trunc);
    file_stream << json_value_to_string(root);
}

bool configuration::string_to_json(const std::string& json_string,
                                                        json::Value& root) {
    json::Deserializer deserializer;
    deserializer << json_string >> root;

    if (deserializer.is_invalid()) {
        auto error = deserializer.get_error();
        log_error(GET_LOGGER("validator"),
                "Parsing error:'" << error.decode() <<
                "' at line:" << error.line <<
                " column:" << error.column <<
                " parsed value:'" << json_string << "'\n");
        return false;
    }

    return true;
}

std::string
configuration::json_value_to_string(const json::Value& value) {
    std::string output;
    json::Serializer serial{value};
    output << serial;
    return output;
}
