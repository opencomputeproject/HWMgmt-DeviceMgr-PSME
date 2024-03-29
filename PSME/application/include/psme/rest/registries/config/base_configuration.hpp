/*!
 * @brief Contains base registry configuration
 *
 * @copyright
 * Copyright (c) 2016-2017 Intel Corporation
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
 * @file base_configuration.hpp
 * */

#pragma once



#include <string>



namespace psme {
namespace rest {
namespace registries {

/*!
 * Make configuration string for base message registry as defined by DMTF.
 *
 * @return String containing a json object with base registry configuration.
 * */
static std::string make_base_configuration() {
    std::string base_configuration = R"({
        "MessageRegistryFiles": [
            {
                "Name" : "Base Message Registry File",
                "Description" : "Base Message Registry File locations",
                "Registry" : "Base.1.0",
                "MessageRegistries" : [
                    {
                        "Language" : "en",
                        "Type" : "external",
                        "PublicationUri" : "https://www.dmtf.org/sites/default/files/standards/documents/DSP8011_1.0.0a.json"
                    }
                ],
                "Oem" : {}
            }
        ]
    })";

    return base_configuration;
}

/*!
 * Make configuration string for privilege registry as defined by DMTF.
 *
 * @return String containing a json object with privilege registry configuration.
 * */
static std::string make_privilege_configuration() {
    std::string privilege_configuration = R"({
        "MessageRegistryFiles": [
            {
                "Name" : "Privilege Registry File",
                "Description" : "Privilege Registry File locations",
                "Registry" : "Redfish_1.0.1_PrivilegeRegistry",
                "MessageRegistries" : [
                    {
                        "Language" : "en",
                        "Type" : "Privilege",
                        "Uri": "/redfish/v1/Registries/PrivilegeRegistry"
                    }
                ],
                "Oem" : {}
            }
        ]
    })";

    return privilege_configuration;
}

/*!
 * Make configuration string for AcctonSoftwareUpdateRegistry message registry as defined by Accton.
 *
 * @return String containing a json object with base registry configuration.
 * */
static std::string make_AcctonFirmwareUpdateRegistry_configuration() {
    std::string base_configuration = R"({
        "MessageRegistryFiles": [
            {
                "Name" : "AcctonFirmwareUpdateRegistry Message Registry File",
                "Description" : "AcctonFirmwareUpdate Message Registry File locations",
                "Registry" : "AcctonFirmwareUpdateRegistry.1.0",
                "MessageRegistries" : [
                    {
                        "Language" : "en",
                        "Uri": "/redfish/v1/Registries/AcctonFirmwareUpdateRegistry"
                    }
                ],
                "Oem" : {}
            }
        ]
    })";

    return base_configuration;
}

}
}
}
