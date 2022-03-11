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
#include <stdio.h>
#include <crypt.h>
#include <time.h>

#include "agent-framework/module/enum/enum_builder.hpp"
#include <string>
#include <vector>
#include <chrono> 
#include <iostream>


#include "psme/rest/account/model/accountservice.hpp"

#include "ecnet_helper/ecnet_helper.hpp"
#include "eclog_helper/eclog_helper.hpp"


namespace json {
    class Value;
}



namespace psme {
namespace rest {
namespace account {
namespace model {

//using model::Accountservice;

typedef enum privilege_e
{
    P_LOGIN = (1 << 0),
    P_CONFIGUREMANAGER = (1 << 1),
    P_CONFIGUREUSERS = (1 << 2),
    P_CONFIGURESELF = (1 << 3),
    P_CONFIGURECOMPONENTS = (1 << 4)
} privilege_t;

/*!
 * @brief Subscription representation
 */
class Account final {
private:

std::string encry(const std::string passwd)
{
#if 0
        std::string mypass{""};
    	char salt[] = "$5$........";
    	char *encpass;
    	char* token;
#else
        unsigned long seed[2];
        char salt[] = "$5$........";
        const char *const seedchars =
        "./0123456789ABCDEFGHIJKLMNOPQRST"
        "UVWXYZabcdefghijklmnopqrstuvwxyz";
        int i;
        char *encpass,*token;
        std::string mypass{""};

       if ( m_salt.empty())
       { 
          /* Generate a (not very) random seed.
             You should do it better than this... */
             seed[0] = time(NULL);
             seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);
 
             /* Turn it into printable characters from ¡¥seedchars¡¦. */
             for (i = 0; i < 8; i++)
                 salt[3+i] = seedchars[(seed[i/5] >> (i%5)*6) & 0x3f];
             m_salt.assign(salt+3);
       }
       else
       {
              for (i = 0; i < 8; i++)
                 salt[3+i] = m_salt[i];    	    
       }     
#endif    	        
        encpass = crypt(passwd.c_str(),salt);
        //printf("encpass is %s\n", encpass);
        token = strtok(encpass,"$");
        while (token != NULL)  
        {
        	//printf("token is %s\n",token);
        	mypass.assign(token);
        	token = strtok(NULL, "$");
        }
        return mypass;
}


public:
    /*!
     * @brief Set account id
     *
     * @param id account id
     */
    void set_id(uint64_t id) {
        m_id = id;
    }

    /*!
     * @brief Get account id
     *
     * @return account id
     */
    uint64_t get_id() const {
        return m_id;
    }

    /*!
     * @brief Set account name
     *
     * @param name account name
     */
    void set_name(const std::string& name) {
        m_name = name;
    }

    /*!
     * @brief Get account name
     *
     * @return account name
     */
    const std::string& get_name() const {
        return m_name;
    }


    /*!
     * @brief Set account salt
     *
     * @param name account salt
     */
    void set_salt(const std::string& salt) {
        m_salt = salt;
    }

    /*!
     * @brief Get account salt
     *
     * @return account salt
     */
    const std::string& get_salt() const {
        return m_salt;
    }



    /*!
     * @brief Set account password
     *
     * @param  account password
     */
    void set_password(const std::string& password, bool need_encrypt) {
    	
    	if ( need_encrypt )
    	    m_password = encry( password ); 
    	else
        m_password = password;
    }
    
    int checkpw(const std::string& password) 
    {
        /*Check this acount is enable or not */
        if(!m_enabled)
		return 1;
  
	/*Check this account is been locked */
	if (m_locked)
	        return 1; 
  
    	 if (m_password == encry(password) )
    	 {
    	     m_loginf=0;
        }
        else
        {
            eclog_helper::RFLogEntry &Entry = eclog_helper::RFLogEntry::get_instance();
            m_loginf++;
            if ( (m_loginf % Accountservice::get_instance()->get_aflt()) == 0 )
            {
                  std::string event("Event");
                  std::string serverity("Warning");
                  std::string sensor_type("Session Audit");
                  std::string message(m_username+" Fail over threshold");            	 
                  Entry.get_current_time();
                  Entry.set_log_entry(event,serverity,sensor_type,message,2);
            }
            
            if ( ( m_loginf > Accountservice::get_instance()->get_alt() ) && ( 0 != Accountservice::get_instance()->get_alt() ) )
            {
                m_locked = true;
             }   
             m_loginftime=std::chrono::steady_clock::now();  //no matter locked or not  record login fail time to wait reset loginf counter
        }
        
        return m_loginf;
    }
    
    void timekick(const std::chrono::steady_clock::time_point& now)
    {
    
#if 0   
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(now - m_lastkick);	
        std::cout << "In Account.hpp got kick in " << m_username << " " << time_span.count() << "seconds " << std::endl;       
        std::chrono::duration<double> loginf_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(now- m_loginftime);
        std::cout << "In Account.hpp login fail time span " << m_username << " " << loginf_time_span.count() << "seconds " << std::endl;  
        std::cout << "In Account.hpp Account Lock duration is " << Accountservice::get_instance()->get_ald() << " seconds " <<std::endl;
        std::cout << "In Account.hpp Account Lock counter reset is " << Accountservice::get_instance()->get_alcra() << " seconds " <<std::endl;
       
        m_lastkick = now;
#endif        
      
    	if (m_loginf)
    	{
    	   auto elapse = std::chrono::duration_cast<std::chrono::seconds> (now - m_loginftime).count();
    	   
    	   if (m_locked)  // account is locked
    	   {
    	   	if ( elapse > Accountservice::get_instance()->get_ald())  //check AccountLockoutDuration
    	   	{
    	   	   m_loginf=0;
    	   	   m_locked=false;
    	   	}
    	   	    
    	   }
    	   else  // account is not locked but ever login fail reset the loginf to 0 if alcra reached
    	   {
    	        if ( elapse > Accountservice::get_instance()->get_alcra()) // check AccountLockoutCounterResetAfter
    	           m_loginf=0;
    	   }
    	}
    	else if (m_locked) /* create or patch to locked but no login fail, set it to no locked when ticked */
    	{
    	     m_locked=false;
             std::cout << " locked " << m_username << " try to unlock " ;
        }
    	   
    }

    /*!
     * @brief Get account password
     *
     * @return account password
     */
    const std::string& get_password() const {
        return m_password;
    }

    /*!
     * @brief Set account username
     *
     * @param account username
     */
    void set_username(const std::string& username) {
        m_username = username;
    }

    /*!
     * @brief Get account username
     *
     * @return account username
     */
    const std::string& get_username() const {
        return m_username;
    }

    /*!
     * @brief Set account enabled
     *
     * @param account enabled
     */
    void set_enabled(const bool& enabled) {
        m_enabled= enabled;
    }

    /*!
     * @brief Get account enabled
     *
     * @return account enabled
     */
    const bool& get_enabled() const {
        return m_enabled;
    }

    /*!
     * @brief Set account locked
     *
     * @param account locked
     */
    void set_locked(const bool& locked) {
        m_locked= locked;
    }

    /*!
     * @brief Get account locked
     *
     * @return account locked
     */
    const bool& get_locked() const {
        return m_locked;
    }

    /*!
     * @brief Set account roleid
     *
     * @param account roleid
     */
    void set_roleid(const std::string& roleid) {
        m_roleid = roleid;
    }

    /*!
     * @brief Get account roleid
     *
     * @return account roleid
     */
    const std::string& get_roleid() const {
        return m_roleid;
    }

    /*!
     * @brief Get etag 
     *
     * @return etag value 
     */
    const std::string& get_etag() const {
        return m_etag;
    }

    /*!
     * @brief Set etag 
     *
     * @param etag value 
     */
    void set_etag(const std::string& etag) {
        m_etag= etag;
    }

    /*!
     * @brief Get account privilege 
     *
     * @return account privilege 
     */
    unsigned int get_privilege() const
    {
        if (m_roleid == "Administrator")
        {
            printf("Administrator");
            return P_LOGIN | P_CONFIGUREMANAGER |  P_CONFIGUREUSERS | P_CONFIGURESELF| P_CONFIGURECOMPONENTS;
        }
        else if (m_roleid == "Operator")
        {
            printf("Operator");
            return P_LOGIN | P_CONFIGURESELF | P_CONFIGURECOMPONENTS;
        }
        else if (m_roleid == "ReadOnly")
        {
            printf("ReadOnly");
            return P_LOGIN | P_CONFIGURESELF;
        }
        else
            return P_LOGIN | P_CONFIGURESELF;
    }

    /*!
     * @brief Creates json representation of account
     *
     * @return JSON representation of subscription
     */
    json::Value to_json() const;

    /*!
     * @brief Creates model representation from account JSON
     *
     * @param json JSON representation of account
     * @return Model representation of account
     */
    static Account from_json(const json::Value& json);

private:
    uint64_t m_id{};
    std::string m_name{};
    std::string m_username{};
    std::string m_roleid{};
    std::string m_salt{""};
    std::string m_password{};
    bool m_enabled{true};
    bool m_locked{false};
    int m_loginf{0};
    std::chrono::steady_clock::time_point m_loginftime{};
    std::chrono::steady_clock::time_point m_lastkick{};
    std::string m_etag{};
};


}
}
}
}
