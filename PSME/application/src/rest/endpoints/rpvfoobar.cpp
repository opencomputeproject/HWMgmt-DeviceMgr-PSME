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
 *
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * */

#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/rpvfoobar.hpp"

#include <utility>


using namespace psme::rest;
using namespace psme::rest::constants;


namespace {
    endpoint::RPVfoobar::RPVfoobar(const std::string &path) : EndpointBase(path) {}

    endpoint::RPVfoobar::~RPVfoobar() {}

    void endpoint::RPVfoobar::get(const server::Request &, server::Response &response)
    {
        response.set_status(server::status_4XX::NOT_FOUND);
    }
}

