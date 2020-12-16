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



#include "psme/rest/account/model/accountservice.hpp"
#include "psme/rest/constants/constants.hpp"
#include <json/json.hpp>
#include <stdexcept>
#include <iostream>

using namespace psme::rest::constants;

namespace psme {
namespace rest {
namespace account {
namespace model {

json::Value Accountservice::to_json() const {
    json::Value json(json::Value::Type::OBJECT);

    return json;
}

void Accountservice::from_json(const json::Value& json) {
    const auto& enable = json[AccountService::SERVICE_ENABLED].as_bool();
    const auto& aflt = json[AccountService::AUTHFAILURELOGGINGTHRESHOLD].as_int();
    const auto& alt = json[AccountService::ACCOUNTLOCKOUTTHRESHOLD].as_int();
    const auto& ald = json[AccountService::ACCOUNTLOCKOUTDURATION].as_int();
    const auto& alcra =  json[AccountService::ACCOUNTLOCKOUTCOUNTERRESETAFTER].as_int();

    set_enabled(enable);
    set_aflt(aflt);   
    set_alt(alt);
    set_ald(ald);  
    set_alcra(alcra);
    
    std::cout << "enable is " << enable << " aflt is " << aflt <<  " aflt is " << alt <<" aflt is " << ald <<" aflt is " << alcra ;

}



}
}
}
}
