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
 * @file command.cpp
 *
 * @brief Command implementation
 * */

#pragma once
#include "agent-framework/generic/singleton.hpp"
#include "agent-framework/validators/procedure_validator.hpp"

#include <bits/stl_bvector.h>
#include <assert.h>
#include <memory>
#include <functional>

/*! Agent Framework */
namespace agent_framework {
/*! Command */
namespace command_ref {

/*
 * @brief Generic interface of the json rpc commands
 * */
class CommandBase {
public:
    virtual ~CommandBase();

    /*
     * @brief Generic JSON RPC Method Call
     * @param[in] req request
     * @param[out] rsp response
     * */
    virtual void method(const Json::Value& req, Json::Value& rsp) = 0;


    /*
     * @brief Generic JSON RPC Notification Call
     * @param[in] req request
     * */
    virtual void notification(const Json::Value& req) = 0;

    /*
     * @brief Returns scheme defining procedure format
     * @return Scheme of the procedure
     * */
    virtual const jsonrpc::Procedure& get_procedure() const = 0;
};

/*
 * @brief Generic implementation of the CommandBase based on the agent_framework
 *        model classes
 * */
template <typename REQ, typename RSP>
class Command : public CommandBase {
public:

    using Request = REQ;
    using Response = RSP;
    using HandlerFunc = std::function<void(REQ, RSP&)>;

    Command(HandlerFunc handler):
        m_handler(handler)
    {}

    /*
     * @brief Method handler implementation based on model request/response classes
     * @param[in] req request
     * @param[out] rsp response
     * */
    void method(const Json::Value& req, Json::Value& rsp)  {
        Request::get_procedure().validate(req);

        Request request = Request::from_json(req);
        Response response{};
        m_handler(request, response);

        rsp = response.to_json();
    }

    /*
     * @brief Notification handler implementation based on model request classes
     * @param[in] req request
     * */
    void notification(const Json::Value& req)  {
        Request::get_procedure().validate(req);

        Request request = Request::from_json(req);
        Response response{};
        m_handler(request, response);
    }

    /*
     * @brief Returns JSON RPC command scheme
     * @return Scheme of the procedure, to be used by jsonrpc library
     * */
    const jsonrpc::Procedure& get_procedure() const {
        return Request::get_procedure();
    }

private:
    HandlerFunc m_handler {};

};

}
}

