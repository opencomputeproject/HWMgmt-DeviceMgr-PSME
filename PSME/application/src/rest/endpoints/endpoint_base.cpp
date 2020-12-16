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

    response.set_status(error.get_http_status_code());
    response.set_body(error.as_string());
}


void psme::rest::server::http_method_not_authorized(const Request&, Response& response) {
    auto message = "Requested operation is not authorized.";
    auto error = error::ErrorFactory::create_unauthorized_error(message);

    response.set_status(error.get_http_status_code());
    response.set_body(error.as_string());
}


EndpointBase::EndpointBase(const std::string& path)
    : MethodsHandler(path), m_modified_time{::get_current_time()} {}

EndpointBase::~EndpointBase() {}

void EndpointBase::update_modified_time() {
    m_modified_time = ::get_current_time();
}


bool EndpointBase::authen_check(const Request& request,const std::string& method){

    std::string username{};
    std::string password{};
    std::string token{};
    std::string srcip{};

    token	= request.get_header("xAuthGen");
    username	= request.get_header("UserName");
    password	= request.get_header("Password");
 	
        std::cout << "url[" << request.get_url() << "]token[" << token << "]"
                  << "username[" << username << "]"
                  << "password[" << password << "]";

    bool SessionServiceEnable = SessionManager::get_instance()->GetSessionConfigEnable();	
    bool BasicAuthenEnable = SessionManager::get_instance()->GetBasicAuthenServiceConfigEnable();	

    if((SessionServiceEnable == false) && (BasicAuthenEnable == false) )
        return true;
    else 
    {
        if( (token.length() !=0) && (SessionServiceEnable == true))
        { // Use Session Authen //
            int session_size = SessionManager::get_instance()->Session_size();
            if (session_size != 0)
            {
                if (SessionManager::get_instance()->updateSessionTimestamp(token) == true)
                {
                    Session new_session = SessionManager::get_instance()->getSession_by_Token(token);
                    const auto &account = AccountManager::get_instance()->getAccount(new_session.get_username());

                    /*Check read/write privilege */
                    /*Todo , Use "Privilege Mappings" to do more specific constrain*/
                    if ((account.get_roleid() == "ReadOnlyUser") && (method == "POST" || method == "PATCH" || method == "DELETE"))
                        return false;
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }
        else if((username.length()!=0 && password.length() !=0) && (BasicAuthenEnable == true))
        {   //Use Basic Authen //
            const auto  & account  =AccountManager::get_instance()->getAccount(username);	

            /*Check if this account username locked*/
            if(account.get_locked() == true)
                return false;

            /*Check if this account enabled*/
            if(account.get_enabled() != true)
                return false;
			
            /*Check read/write priviledge */
            /*Todo , Use "Privilege Mappings" to do more specific constrain*/					
             if((account.get_roleid() == "ReadOnlyUser") && (method == "POST" || method == "PATCH" || method == "DELETE") )
                return false;			 	

            int res  =AccountManager::get_instance()->login(username, password);	

            if(res == 0)
                return true;
            else if(res > Accountservice::get_instance()->get_aflt())
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

void EndpointBase::get(const Request& request, Response& response) {
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
