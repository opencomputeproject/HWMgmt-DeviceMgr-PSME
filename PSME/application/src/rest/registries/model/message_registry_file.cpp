/*!
 * @brief MessageRegistryFile class implementation
 *
 * @copyright
 * Copyright (c) 2016-2017 Intel Corporation
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
 * @file message_registry_file.hpp
 * */

#include "psme/rest/registries/model/message_registry_file.hpp"

#include <uuid++.hh>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <string>
using namespace std;

namespace psme {
namespace rest {
namespace registries {

MessageRegistryFile::MessageRegistryFile() {
    uuid temporary_uuid;
    temporary_uuid.make(UUID_MAKE_V1);
    char* uuid_char = temporary_uuid.string();
    m_uuid = std::string{uuid_char};
    free(uuid_char);
}


void MessageRegistryFile::set_id(const uint64_t& id) {
    m_id = id;
}


void MessageRegistryFile::set_name(const OptionalField<std::string>& name) {
    m_name = name;
}


void MessageRegistryFile::set_description(const OptionalField<std::string>& description) {
    m_description = description;
}


void MessageRegistryFile::set_languages(const MessageRegistryFile::Languages& languages) {
    m_languages = languages;
}


void MessageRegistryFile::set_registry(const OptionalField<std::string>& registry) {
    m_registry = registry;
}


void MessageRegistryFile::set_locations(const Locations& locations) {
    m_locations = locations;

    for (const auto &lo : m_locations)
    {
        json::Value s = json::Value::Type::OBJECT;
        if (lo.get_uri().has_value())
        {
            std::string full_path(PRI_REG_JSON_FULL_PATH);
            std::string url = lo.get_uri();
            std::string json_file = url.substr(url.find_last_of("/\\") + 1);
            std::string json_file_full_path = full_path + json_file;
            std::ifstream m_source_files = {};
            m_source_files.open(json_file_full_path);
            Json::Reader reader;
            if (reader.parse(m_source_files, m_reg_file))
                m_reg_file_status = true;
            else
                std::cout << " NG!!" << std::endl;
        }
    }
}


const uint64_t& MessageRegistryFile::get_id() const {
    return m_id;
}


const OptionalField<std::string>& MessageRegistryFile::get_name() const {
    return m_name;
}


const OptionalField<std::string>& MessageRegistryFile::get_description() const {
    return m_description;
}


const MessageRegistryFile::Languages& MessageRegistryFile::get_languages() const {
    return m_languages;
}


const OptionalField<std::string>& MessageRegistryFile::get_registry() const {
    return m_registry;
}


const MessageRegistryFile::Locations& MessageRegistryFile::get_locations() const {
    return m_locations;
}


const std::string& MessageRegistryFile::get_uuid() const {
    return m_uuid;
}

}
}
}
