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

#include "psme/rest/endpoints/manager_serial_interface.hpp"
#include "psme/rest/constants/constants.hpp"
#include <arpa/inet.h>

/*To get terminal info. Begin:*/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
/*To get terminal info. End  :*/

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace agent_framework::model::enums;

namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#SerialInterface.SerialInterface";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#SerialInterface.v1_0_2.SerialInterface";
    r[Common::ID] = json::Value::Type::NIL;
    r[Common::NAME] = "Manager Serial Interface";
    r[Common::DESCRIPTION] = "Management for Serial Interface";
    r[SerialInterface::INTERFACE_ENABLED] = true;                        // always true //
    r[SerialInterface::SIGNAL_TYPE] = "Rs232";                              // Always Rs232 //
    r[SerialInterface::BIT_RATE] = json::Value::Type::NIL;
    r[SerialInterface::PARITY] = json::Value::Type::NIL;
    r[SerialInterface::DATA_BIT] = json::Value::Type::NIL;
    r[SerialInterface::STOP_BITS] = json::Value::Type::NIL;
    r[SerialInterface::FLOW_CONTROL] = "None";                           //always none //
    r[SerialInterface::CONNECTOR_TYPE] = "RJ45";                        // always RJ45 //
    r[SerialInterface::PIN_OUT] = "Cyclades";                                //Cyclades //

    return r;
}
}

endpoint::ManagerSerialInterface::ManagerSerialInterface(const std::string& path) : EndpointBase(path) {}

endpoint::ManagerSerialInterface::~ManagerSerialInterface() {}

void endpoint::ManagerSerialInterface::get(const server::Request &req, server::Response &res)
{
    auto r = ::make_prototype();
    r[Common::ODATA_ID] = PathBuilder(req).build();
    r[Common::ID] = req.params[PathParam::SER_ID];

        try
        {
        char const *portname = "/dev/console";
        int fd;
    
        fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
                if (fd < 0)
                {
            printf("Error opening %s: %s\n", portname, strerror(errno));
            return;
        }

        struct termios tty;
    
                if (tcgetattr(fd, &tty) < 0)
                {
            printf("Error from tcgetattr: %s\n", strerror(errno));
                        close(fd);
            return;
        }
        printf("tty.c_cflag[0x%04x]\r\n", tty.c_cflag);
        printf("tty.c_oflag[0x%04x]\r\n", tty.c_oflag);
    
        int fspeed = (tty.c_cflag & 0xf00f);
        int fparity = (tty.c_cflag & (PARENB|PARODD));
        int fstopb = (tty.c_cflag & CSTOPB  );
        int fdatab = (tty.c_cflag & (CS5|CS6|CS7|CS8));		

        switch(fspeed)
        {
                case B1200 :
                        r[SerialInterface::BIT_RATE] = "1200";
                        break;        
                case B2400 :
                        r[SerialInterface::BIT_RATE] = "2400";
                        break;
                case B4800 :
                        r[SerialInterface::BIT_RATE] = "4800";
                        break;
                case B9600 :
                        r[SerialInterface::BIT_RATE] = "9600";
                        break;
                case B19200 :
                        r[SerialInterface::BIT_RATE] = "19200";
                        break;
                case B38400 :
                        r[SerialInterface::BIT_RATE] = "38400";
                        break;
                case B57600 :
                        r[SerialInterface::BIT_RATE] = "57600";
                        break;
                case B115200 :
                        r[SerialInterface::BIT_RATE] = "115200";
                        break;
                default:
                        r[SerialInterface::BIT_RATE] = json::Value::Type::NIL;
                        break;
                        ;
        }

        if(fparity & PARENB)
        {
            if(fparity & PARODD)
                r[SerialInterface::PARITY] = "Odd";
            else
                r[SerialInterface::PARITY] = "Evne";
        }
        else
            r[SerialInterface::PARITY] = "None";

        if(fstopb & CSTOPB)
            r[SerialInterface::STOP_BITS] = "2";
        else
            r[SerialInterface::STOP_BITS] = "1";

       if((fdatab == CS5))
            r[SerialInterface::DATA_BIT] = "5";
       else if(fdatab == CS6)
            r[SerialInterface::DATA_BIT] = "6";
       else if(fdatab == CS7)
            r[SerialInterface::DATA_BIT] = "7";
       else if(fdatab == CS8)
            r[SerialInterface::DATA_BIT] = "8";
       else
            r[SerialInterface::DATA_BIT] = json::Value::Type::NIL;
                close(fd);
        }
        catch (const std::exception &ex)
        {
    	log_warning(GET_LOGGER("rest"),
    			"Read manage error!!");
    }
    set_response(res, r);
}
