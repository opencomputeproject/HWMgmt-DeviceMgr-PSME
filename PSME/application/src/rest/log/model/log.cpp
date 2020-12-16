/*
* Edgecore DeviceManager
* Copyright 2020-2021 Edgecore Networks, Inc.
 *
* Licensed to the Apache Software Foundation (ASF) under one
* or more contributor license agreements.  See the NOTICE file
* distributed with this work for additional information
* regarding copyright ownership.  The ASF licenses this file
* to you under the Apache License, Version 2.0 (the
* "License"); you may not use this file except in compliance
* with the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
* KIND, either express or implied.  See the License for the
* specific language governing permissions and limitations
* under the License.
*/


#include "psme/rest/log/model/log.hpp"
#include "psme/rest/constants/constants.hpp"
#include <json/json.hpp>
#include <stdexcept>

using namespace psme::rest::constants;

namespace psme
{
namespace rest
{
namespace log
{
namespace model
{

Log to_model(const json::Value &json);

#define UNUSED(x) (void)(x)
json::Value Log::to_json() const
{
    json::Value json(json::Value::Type::OBJECT);
    return json;
}

Log Log::from_json(const json::Value &json)
{
    UNUSED(json);
    Log log;
    return log;
}

} // namespace model
} // namespace log
} // namespace rest
} // namespace psme
