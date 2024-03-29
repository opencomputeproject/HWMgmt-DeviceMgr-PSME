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

#pragma once



#include "psme/rest/endpoints/endpoint_base.hpp"
#include "psme/rest/session/model/session.hpp"




namespace psme {
namespace rest {
namespace endpoint {


/*!
 * A class representing the rest api SessionCollection endpoint
 */
class SessionCollection : public EndpointBase {
public:

    /*!
     * @brief The constructor for SessionCollection class
     */
    explicit SessionCollection(const std::string& path);


    /*!
     * @brief SessionCollection class destructor
     */
    virtual ~SessionCollection();

    void get(const server::Request& request, server::Response& response) override;
    void post(const server::Request& request, server::Response& response) override;
    void del(const server::Request& request, server::Response& response) override;	
    void patch(const server::Request& request, server::Response& response) override;
};


}
}
}

