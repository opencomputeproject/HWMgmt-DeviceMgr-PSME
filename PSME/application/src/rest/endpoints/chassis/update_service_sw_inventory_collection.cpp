/*!
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
 * */

#include "psme/rest/endpoints/chassis/update_service_sw_inventory_collection.hpp"
#include "psme/rest/utils/status_helpers.hpp"
#include "agent-framework/module/managers/utils/manager_utils.hpp"
#include <iostream>
#include <string>
#include <stdlib.h>


using namespace psme::rest;
using namespace psme::rest::constants;
using namespace agent_framework::module;
using namespace agent_framework::model::enums;

namespace
{

json::Value make_fw_inventory_prototype()
{
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#SoftwareInventoryCollection.SoftwareInventoryCollection";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#SoftwareInventoryCollection.SoftwareInventoryCollection";
    r[Common::NAME] = "SoftwareInventoryCollection";
    r[Common::DESCRIPTION] = "Firmware Inventory Collection.";
    r[Collection::ODATA_COUNT] =json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;

    return r;
}

} // namespace

namespace psme
{
namespace rest
{
namespace endpoint
{

UpdateServiceSoftWareInventoryCollection::UpdateServiceSoftWareInventoryCollection(const std::string &path) : EndpointBase(path) {}

UpdateServiceSoftWareInventoryCollection::~UpdateServiceSoftWareInventoryCollection() {}

void UpdateServiceSoftWareInventoryCollection::get(const server::Request &req, server::Response &res)
{
    auto json = ::make_fw_inventory_prototype();

    json[Collection::ODATA_COUNT] = 1; //todo dynamic check//
    json[Common::ODATA_ID] = PathBuilder(req).build();

    json::Value link_elem(json::Value::Type::OBJECT);
    link_elem[Common::ODATA_ID] = PathBuilder(req).append("PACKAGE").build();
    json[Collection::MEMBERS].push_back(std::move(link_elem));
    set_response(res, json);
}
} // namespace endpoint
} // namespace rest
} // namespace psme