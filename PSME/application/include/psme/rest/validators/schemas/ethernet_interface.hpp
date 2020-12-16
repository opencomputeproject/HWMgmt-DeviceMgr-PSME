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

#include "agent-framework/validators/procedure_validator.hpp"

namespace psme {
namespace rest {
namespace validators {
namespace schema {

class EthernetInterfacePatchSchema {

    class STATELESS_ADDRESS_AUTO_Schema {
    public:
        static const jsonrpc::ProcedureValidator& get_procedure();
    };

    class DHCPv4Schema {
    public:
        static const jsonrpc::ProcedureValidator& get_procedure();
    };
    class IPv4Schema {
    public:
        static const jsonrpc::ProcedureValidator& get_procedure();
    };
    class DHCPv6Schema {
    public:
        static const jsonrpc::ProcedureValidator& get_procedure();
    };	
    class IPv6Schema {
    public:
        static const jsonrpc::ProcedureValidator& get_procedure();
    };	
    class IPv4StaticNameServerSchema {
    public:
        static const jsonrpc::ProcedureValidator& get_procedure();
    };


	
public:
    static const jsonrpc::ProcedureValidator& get_procedure();
};

}
}
}
}
