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

#include "psme/rest/endpoints/endpoint_base.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/server/mux/matchers.hpp"
#include "psme/rest/session/manager/session_manager.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/account/manager/account_manager.hpp"
#include "psme/rest/account/model/accountservice.hpp" 
#include "psme/rest/endpoints/message_privilege_registry_file.hpp"
#include "psme/rest/registries/managers/message_registry_file_manager.hpp"
#include "eclog_helper/eclog_helper.hpp"


#include <locale>
#include <chrono>

using namespace psme::rest::server;
using namespace psme::rest::endpoint;
using namespace psme::rest::session::manager;
using namespace psme::rest::validators;
using namespace psme::rest::account::manager;
using namespace eclog_helper;

namespace {
constexpr std::size_t TIME_BUFFER_SIZE = 26;

auto &gADbg = ADbg::get_instance();

std::string get_current_time() {
    char time_buffer[TIME_BUFFER_SIZE];
    auto now = std::chrono::system_clock::now();
    auto time_now = std::chrono::system_clock::to_time_t(now);

    tzset();

    struct tm local_tm;
    localtime_r(&time_now, &local_tm);
    std::strftime(time_buffer, TIME_BUFFER_SIZE, "%FT%H:%M", &local_tm);

    return time_buffer;
}

}

void psme::rest::server::http_method_not_allowed(const Request&, Response& response) {
    auto message = "Requested operation is not allowed on this resource.";
    auto error = error::ErrorFactory::create_method_not_allowed_error(message);

    response.set_header("Allow", "GET");
    response.set_status(error.get_http_status_code());
    response.set_body(error.as_string());
}


void psme::rest::server::http_method_not_authorized(const Request&, Response& response) {
    auto message = "Requested operation is not authorized.";
    auto error = error::ErrorFactory::create_unauthorized_error(message);

    response.set_status(error.get_http_status_code());
    response.set_body(error.as_string());
    response.set_header("WWW-Authenticate", "Basic realm=<realm>[, charset=\"UTF-8\"]");  
}

EndpointBase::EndpointBase(const std::string& path)
    : MethodsHandler(path), m_modified_time{::get_current_time()}
{
    const auto &filePri = registries::MessageRegistryFileManager::get_instance()->get_file_by_id(2);
    m_pri_reg_file = filePri.get_reg_file_json()["Mappings"];
}

EndpointBase::~EndpointBase() {}

void EndpointBase::update_modified_time() {
    m_modified_time = ::get_current_time();
}

unsigned int get_op_pri_or(Json::Value Privilege);
unsigned int get_op_pri_or(Json::Value Privilege)
{
    int max_Pri = Privilege.size();
    unsigned int tt_Pri = 0;

    for (int j = 0; j < max_Pri; j++)
    {
        if (Privilege[j] == "Login")
            tt_Pri |= P_LOGIN;
        else if (Privilege[j] == "ConfigureManager")
            tt_Pri |= P_CONFIGUREMANAGER;
        else if (Privilege[j] == "ConfigureUsers")
            tt_Pri |= P_CONFIGUREUSERS;
        else if (Privilege[j] == "ConfigureSelf")
            tt_Pri |= P_CONFIGURESELF;
        else if (Privilege[j] == "ConfigureComponents")
            tt_Pri |= P_CONFIGURECOMPONENTS;
        else
            printf("No!");
    }

    return tt_Pri;
}

unsigned int EndpointBase::privilege_ov_check(const Request& request, std::string method, Json::Value PrOv, unsigned int user_privilege)
{
    unsigned int t_Pri = 0;
    int max_PrOv = PrOv.size();
    if (max_PrOv > 0)
    {
        for (int ii = 0; ii < max_PrOv; ii++)
        {
            std::string body = request.get_body();
            Json::Value targets = PrOv[ii]["Targets"];
            int max_Targets = targets.size();

            for (int k = 0; k < max_Targets; k++)
            {
                std::string t = targets[k].asString();
                std::size_t fd = body.find(t);
                if (fd != std::string::npos)
                {
                    Json::Value OpMapsMethod_ov = PrOv[ii]["OperationMap"][method];
                    int max_OpMaps_ov = OpMapsMethod_ov.size();
                    if (max_OpMaps_ov == 1)
                    {
                        // AND privilege
                        Json::Value Privilege_ov = OpMapsMethod_ov[0]["Privilege"];
                        t_Pri = get_op_pri_or(Privilege_ov);

                        unsigned int f = (t_Pri & user_privilege) & user_privilege;
                        if ((t_Pri == f) && t_Pri != 0)
                        {
                            return 1;
                        }
                        else
                        {
                            gADbg.acc_printf(LEVEL_DEBUG, "Override not authen");
                            return 0;
                        }
                    }
                    else if (max_OpMaps_ov > 1)
                    {
                        // OR privilege
                        t_Pri = 0;
                        for (int kk = 0; kk < max_OpMaps_ov; kk++)
                        {
                            Json::Value Privilege_ov_m = OpMapsMethod_ov[kk]["Privilege"];
                            t_Pri |= get_op_pri_or(Privilege_ov_m);
                            unsigned int f = (t_Pri & user_privilege) & user_privilege;
                            printf("Or Fin Pri[0x%x] user_privilege[%x] f[%x]\r\n", t_Pri, user_privilege, f);
                            if ((t_Pri & f) && t_Pri != 0)
                            {
                                return 1;
                            }
                            else
{
                                gADbg.acc_printf(LEVEL_DEBUG, "max_OpMaps_ov > 1 ,Override not authen");
                            }
                        }
                    }
                    else
                    {
                        gADbg.acc_printf(LEVEL_DEBUG, "ov OR No auth!! not have method can get");
                        return 0;
                    }
                }
            }
            return 1;
        }
    }
    gADbg.acc_printf(LEVEL_DEBUG, "ov not found!!");
    return 0;
}

unsigned int EndpointBase::privilege_check(const Request& request, std::string method, unsigned int user_privilege, std::string role)
{
    try
    {
        gADbg.acc_printf(LEVEL_DEBUG,"privilege_check user_privilege[0x%x] method[%s] role[%s]", user_privilege, method.c_str(), role.c_str());
        int max_entity = m_pri_reg_file.size();
        std::string body = request.get_body();
        gADbg.acc_printf(LEVEL_DEBUG,"body[%s]",body.c_str());

        for (int i = 0; i < max_entity; i++)
        {
            std::string entity_name = m_pri_reg_file[i]["Entity"].asString().c_str();
            std::size_t found = request.get_url().find(entity_name);
            if (found != std::string::npos)
            {
                Json::Value OpMapsMethod = m_pri_reg_file[i]["OperationMap"][method];
                int max_OpMaps = OpMapsMethod.size();
                unsigned int t_Pri = 0;

                if (max_OpMaps == 1)
                {
                    Json::Value Privilege_a = OpMapsMethod[0]["Privilege"];
                    t_Pri = get_op_pri_or(Privilege_a);
                    if (m_pri_reg_file[i].isMember("PropertyOverrides"))
                    {
                        if (privilege_ov_check(request, method, m_pri_reg_file[i]["PropertyOverrides"], user_privilege))
                            return 1;
                        }

                    unsigned int f = (t_Pri & user_privilege) & user_privilege;
                    if ((t_Pri == f) && t_Pri != 0)
                    {
                        return 1;
                    }
                    else
                    {
                        gADbg.acc_printf(LEVEL_WARN, "privilege_check,NO AUTHENTICATION ALLOW!!");
                        return 0;
                    }
                }
                else if (max_OpMaps > 1)
                {
                    for (int ll = 0; ll < max_OpMaps; ll++)
                    {
                        Json::Value Privilege_o = OpMapsMethod[ll]["Privilege"];
                        t_Pri |= get_op_pri_or(Privilege_o);
                        unsigned int f = (t_Pri & user_privilege) & user_privilege;
                        if ((t_Pri == f) && t_Pri != 0)
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    gADbg.acc_printf(LEVEL_WARN, "privilege_check,CAN NOT GET METHOD!!");
                    return 0;
                }
            }
        }
        return 0;
    }
    catch (const agent_framework::exceptions::NotFound &ex)
    {
        log_error(GET_LOGGER("rest"), "privilege_check Not found exception: " << ex.what());
        return 0;
    }
}

bool EndpointBase::authen_check(const Request &request, const std::string &method)
{
    try
    {
        std::string username{};
        std::string password{};
        std::string token{};
        std::string url{};
        token = request.get_header("xAuthGen");
        username = request.get_header("UserName");
        password = request.get_header("Password");
        url = request.get_url();
        gADbg.acc_printf(LEVEL_INFO, "authen_check ,username[%s] url[%s] token[%s] password[%s]",
                         username.c_str(), url.c_str(), token.c_str(), password.c_str());

        bool SessionServiceEnable = SessionManager::get_instance()->GetSessionConfigEnable();
        bool BasicAuthenEnable = SessionManager::get_instance()->GetBasicAuthenServiceConfigEnable();

        if ((SessionServiceEnable == false) && (BasicAuthenEnable == false))
            return true;
        else
        {
            if ((token.length() != 0) && (SessionServiceEnable == true))
            { // Use Session Authen //
                int session_size = SessionManager::get_instance()->Session_size();
                if (session_size != 0)
                {
                    if (SessionManager::get_instance()->updateSessionTimestamp(token) == true)
                    {
                        Session new_session = SessionManager::get_instance()->getSession_by_Token(token);

                        if (AccountManager::get_instance()->checkAccount(new_session.get_username()))
                        {
                            const auto &account = AccountManager::get_instance()->getAccount(new_session.get_username());

                            /*Check if this account username locked*/
                            if (account.get_locked() == true)
                            {
                                gADbg.acc_printf(LEVEL_WARN, "authen_check, this account locked!!");
                                return false;
                            }

                            /*Check if this account enabled*/
                            if (account.get_enabled() != true)
                            {
                                gADbg.acc_printf(LEVEL_WARN, "authen_check,this account not enabled!!");
                                return false;
                            }

                            if (privilege_check(request, method, account.get_privilege(), account.get_roleid()))
                            {
                                return true;
                            }
                            else
                                return false;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                        return false;
                }
                else
                {
                    gADbg.acc_printf(LEVEL_WARN, "authen_check ,sessions size is 0 !!");
                    return false;
                }
            }
            else if ((username.length() != 0 && password.length() != 0) && (BasicAuthenEnable == true))
            { // Use Basic Authen //

                const auto &account = AccountManager::get_instance()->getAccount(username);

                /*Check if this account username locked*/
                if (account.get_locked() == true)
                    return false;

                /*Check if this account enabled*/
                if (account.get_enabled() != true)
                    return false;

                int res = AccountManager::get_instance()->login(username, password);

                if (res == 0)
                {
                    if (privilege_check(request, method, account.get_privilege(), account.get_roleid()))
                        return true;
                    else
                        return false;
                }
                else if (res > Accountservice::get_instance()->get_aflt())
                {
                    log_error(GET_LOGGER("rest"), "user [" << username << "] over AuthFailureLoggingThreshold login times !!");
                    return false;
                }
                else
                    return false;
            }
            else
                return false;
        }
    }
    catch (const agent_framework::exceptions::NotFound &ex)
    {
        log_error(GET_LOGGER("rest"), "authen_check Not found exception: " << ex.what());
        return false;
    }
}

void EndpointBase::get(const Request& request, Response& response) {
    http_method_not_allowed(request, response);
}

void EndpointBase::head(const Request& request, Response& response) {
    http_method_not_allowed(request, response);
}

void EndpointBase::del(const Request& request, Response& response) {
    http_method_not_allowed(request, response);
}

void EndpointBase::post(const Request& request, Response& response) {
    http_method_not_allowed(request, response);
}

void EndpointBase::patch(const Request& request, Response& response) {
    http_method_not_allowed(request, response);
}

void EndpointBase::put(const Request& request, Response& response) {
    http_method_not_allowed(request, response);
}
void EndpointBase::trace(const Request& request, Response& response) {
    http_method_not_allowed(request, response);
}
