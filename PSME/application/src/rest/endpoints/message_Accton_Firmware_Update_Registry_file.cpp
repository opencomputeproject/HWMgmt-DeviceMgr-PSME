/*!
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
 * */

#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/message_Accton_Firmware_Update_Registry_file.hpp"
#include "psme/rest/registries/managers/message_registry_file_manager.hpp"

using namespace psme::rest;
using namespace psme::rest::endpoint;
using namespace psme::rest::registries;

AcctonFirmwareUpdateRegistryFile::AcctonFirmwareUpdateRegistryFile(const std::string& path) : EndpointBase(path) { }
AcctonFirmwareUpdateRegistryFile::~AcctonFirmwareUpdateRegistryFile() { }
#define UNUSED(x) (void)(x)

void AcctonFirmwareUpdateRegistryFile::get(const server::Request& request, server::Response& response) 
{
    //Get AcctonFirmwareUpdateRegistryFile files and its file ID is 3 //
    UNUSED(request);
    const auto& file = registries::MessageRegistryFileManager::get_instance()->get_file_by_id(3);
    Json::Value Jsons = file.get_reg_file_json(); 
    Json::FastWriter fwriter;
    json::Deserializer deserializer(fwriter.write(Jsons));
    json::Value c_json;
    deserializer >> c_json; 
    set_response(response, c_json);
}

