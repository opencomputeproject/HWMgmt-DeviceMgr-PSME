
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
#include "psme/rest/endpoints/ethernet/rule.hpp"
#include "psme/rest/endpoints/ethernet/rule_collection.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/model/handlers/handler_manager.hpp"
#include "psme/rest/model/handlers/generic_handler_deps.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"
#include "psme/core/agent/agent_manager.hpp"
#include "ecsys_helper/ecsys_helper.hpp"

#include "agent-framework/module/requests/network.hpp"
#include "agent-framework/module/responses/network.hpp"
#include "psme/rest/utils/ec_common_utils.hpp"
using namespace psme::rest::utils;

using namespace psme::rest;
using namespace psme::rest::validators;
using namespace psme::rest::error;
using namespace psme::core::agent;
using namespace psme::rest::constants;
using namespace agent_framework::model;
using NetworkComponents = agent_framework::module::NetworkComponents;
using namespace ecsys_helper;

namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#EthernetSwitchACLRuleCollection.EthernetSwitchACLRuleCollection";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#EthernetSwitchACLRuleCollection.EthernetSwitchACLRuleCollection";
    r[Common::NAME] = "Ethernet Switch Access Control List Rules Collection";
    r[Common::DESCRIPTION] = "Rules for switch Access Control List. Each rule "
        "defines single action and at least one condition";
    r[Collection::ODATA_COUNT] = json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;

    return r;
}

void validate_mirror_interface(const json::Value& json, AclRule& rule) {
    json::Value schema({
        JsonValidator::mandatory(Common::ODATA_ID,
            JsonValidator::has_type(JsonValidator::STRING_TYPE))
    });

    const auto port_url = JsonValidator::validate_request_body(json, schema)[Common::ODATA_ID].as_string();
    rule.set_forward_mirror_port(port_url);
}

void validate_mirror_region(const json::Value& json, AclRule& rule) {
    json::Value schema({
        JsonValidator::mandatory(Common::ODATA_ID,
            JsonValidator::has_type(JsonValidator::STRING_TYPE))
    });

    for (const auto& key : json) {
        const auto port_url = JsonValidator::validate_request_body(key, schema)[Common::ODATA_ID].as_string();	
        rule.add_mirrored_port(endpoint::utils::get_port_uuid_from_url(port_url));
    }
}

void validate_mirror_type(const json::Value& json, json::Value mirror_types) {
    json::Value schema({
        JsonValidator::mandatory(Rule::MIRROR_TYPE,
            JsonValidator::any_of(JsonValidator::STRING_TYPE, mirror_types))
    });

    JsonValidator::validate_request_body(json, schema);
}

void validate_ip(const json::Value& json) {
    json::Value schema;
    if (!json[Rule::MASK].is_null()) {
        schema = json::Value({
            JsonValidator::mandatory(Rule::IP_ADDRESS,
                JsonValidator::has_type(JsonValidator::STRING_TYPE)),
            JsonValidator::optional(Rule::MASK,
                JsonValidator::has_type(JsonValidator::STRING_TYPE))
        });
    }
    else {
        schema = json::Value({
            JsonValidator::mandatory(Rule::IP_ADDRESS,
                JsonValidator::has_type(JsonValidator::STRING_TYPE)),
        });
    }
    JsonValidator::validate_request_body(json, schema);
}

void validate_mac(const json::Value& json) {
    json::Value schema;
    if (!json[Rule::MASK].is_null()) {
        schema = json::Value({
            JsonValidator::mandatory(Common::MAC_ADDRESS,
                JsonValidator::has_type(JsonValidator::STRING_TYPE)),
            JsonValidator::optional(Rule::MASK,
                JsonValidator::has_type(JsonValidator::STRING_TYPE))
        });
    }
    else {
        schema = json::Value({
            JsonValidator::mandatory(Common::MAC_ADDRESS,
                JsonValidator::has_type(JsonValidator::STRING_TYPE)),
        });
    }

    JsonValidator::validate_request_body(json, schema);
}

void validate_vlan_id(const json::Value& json) {
    json::Value schema;
    if (!json[Rule::MASK].is_null()) {
        schema = json::Value({
            JsonValidator::mandatory(Common::ID,
                JsonValidator::has_type(JsonValidator::UINT_TYPE)),
            JsonValidator::optional(Rule::MASK,
                JsonValidator::has_type(JsonValidator::UINT_TYPE))
        });
    }
    else {
        schema = json::Value({
            JsonValidator::mandatory(Common::ID,
                JsonValidator::has_type(JsonValidator::UINT_TYPE)),
        });
    }

    JsonValidator::validate_request_body(json, schema);
}

void validate_port(const json::Value& json) {
    json::Value schema;
    if (!json[Rule::MASK].is_null()) {
        schema = json::Value({
            JsonValidator::mandatory(Rule::PORT,
                JsonValidator::has_type(JsonValidator::UINT_TYPE)),
            JsonValidator::optional(Rule::MASK,
                JsonValidator::has_type(JsonValidator::UINT_TYPE))
        });
    }
    else {
        schema = json::Value({
            JsonValidator::mandatory(Rule::PORT,
                JsonValidator::has_type(JsonValidator::UINT_TYPE)),
        });
    }

    JsonValidator::validate_request_body(json, schema);
}

void validate_protocol(const json::Value& json) {
    json::Value schema({
        JsonValidator::mandatory(Rule::L4_PROTOCOL,
            JsonValidator::has_type(JsonValidator::UINT_TYPE))
    });

    JsonValidator::validate_request_body(json, schema);
}

void validate_condition(const json::Value& json, AclRule& rule) {
    bool any_condition_found = false;
    std::vector<std::string> valid_conditions {
        Rule::IP_SOURCE,
        Rule::IP_DESTINATION,
        Rule::MAC_SOURCE,
        Rule::MAC_DESTINATION,
        Rule::VLAN_ID,
        Rule::L4_SOURCE_PORT,
        Rule::L4_DESTINATION_PORT,
        Rule::L4_PROTOCOL
    };

    for (const auto& field : valid_conditions) {
        if (json.is_member(field)) {
            // null is acceptable, but at least one of the fields must not be null
            if (json[field].is_null()) {
                continue;
            }
            any_condition_found = true;

            if (Rule::IP_SOURCE == field) {
                validate_ip(json[field]);
                rule.set_source_ip(attribute::AclAddress(
                                    json[field][Rule::IP_ADDRESS],
                                    json[field][Rule::MASK]));
            }
            else if (Rule::IP_DESTINATION == field) {
                validate_ip(json[field]);
                rule.set_destination_ip(attribute::AclAddress(
                                    json[field][Rule::IP_ADDRESS],
                                    json[field][Rule::MASK]));
            }
            else if (Rule::MAC_SOURCE == field) {
                validate_mac(json[field]);
                rule.set_source_mac(attribute::AclAddress(
                                    json[field][Common::MAC_ADDRESS],
                                    json[field][Rule::MASK]));
            }
            else if (Rule::MAC_DESTINATION == field) {
                validate_mac(json[field]);
                rule.set_destination_mac(attribute::AclAddress(
                                    json[field][Common::MAC_ADDRESS],
                                    json[field][Rule::MASK]));
            }
            else if (Rule::L4_SOURCE_PORT == field) {
                validate_port(json[field]);
                rule.set_source_port(attribute::AclPort(
                                    json[field][Rule::PORT],
                                    json[field][Rule::MASK]));
            }
            else if (Rule::L4_DESTINATION_PORT == field) {
                validate_port(json[field]);
                rule.set_destination_port(attribute::AclPort(
                                    json[field][Rule::PORT],
                                    json[field][Rule::MASK]));
            }
            else if (Rule::VLAN_ID == field) {
                validate_vlan_id(json[field]);
                rule.set_vlan_id(attribute::AclVlanId(
                                    json[field][Common::ID],
                                    json[field][Rule::MASK]));
            }
            else if (Rule::L4_PROTOCOL == field) {
                validate_protocol(json);
                rule.set_protocol(json[field]);
            }
        }
    }

    if (!any_condition_found) {
        ServerError error = ErrorFactory::create_property_missing_error(Rule::CONDITION);
        std::string valid_conditions_message{};
        for (const auto& field : valid_conditions) {
            valid_conditions_message += field;
            // do not append a comma for the last element
            if (field != valid_conditions[valid_conditions.size() - 1]) {
                valid_conditions_message += ", ";
            }
        }
        error.add_extended_message(
            "Condition must have at least one of these fields present and not nulled: " + valid_conditions_message);
        throw ServerException(error);
    }
}

json::Value validate_post_request(const server::Request& request, AclRule& rule) {

    json::Value acl_actions{json::Value::Type::ARRAY};
    for (const auto& value : agent_framework::model::enums::AclAction::get_values()) {
        acl_actions.push_back(value);
    }

    json::Value mirror_types{json::Value::Type::ARRAY};
    for (const auto& value : agent_framework::model::enums::AclMirrorType::get_values()) {
        mirror_types.push_back(value);
    }

    json::Value schema({
        JsonValidator::optional(Rule::RULE_ID,
            JsonValidator::has_type(JsonValidator::UINT_TYPE)
        ),
        JsonValidator::mandatory(Rule::ACTION,
            JsonValidator::any_of(JsonValidator::STRING_TYPE, acl_actions)
        ),
        JsonValidator::optional(Rule::FORWARD_MIRROR_INTERFACE,
            JsonValidator::has_type(JsonValidator::OBJECT_TYPE)
        ),
        JsonValidator::optional(Rule::MIRROR_PORT_REGION,
            JsonValidator::has_type(JsonValidator::ARRAY_TYPE)
        ),
        JsonValidator::optional(Rule::MIRROR_TYPE,
            JsonValidator::any_of(JsonValidator::STRING_TYPE, mirror_types)
        ),
        JsonValidator::mandatory(Rule::CONDITION,
            JsonValidator::has_type(JsonValidator::OBJECT_TYPE)
        )
    });

    const auto parsed_json = JsonValidator::validate_request_body(request, schema);

    rule.set_rule_id(parsed_json[Rule::RULE_ID]);
    rule.set_action(parsed_json[Rule::ACTION]);
    rule.set_mirror_type(parsed_json[Rule::MIRROR_TYPE]);

    if (agent_framework::model::enums::AclAction::Mirror == rule.get_action()  ||
        agent_framework::model::enums::AclAction::Forward == rule.get_action() ||
        (parsed_json.is_member(Rule::FORWARD_MIRROR_INTERFACE) && !parsed_json[Rule::FORWARD_MIRROR_INTERFACE].is_null())) {
        validate_mirror_interface(parsed_json[Rule::FORWARD_MIRROR_INTERFACE], rule);
    }

    if (agent_framework::model::enums::AclAction::Mirror == rule.get_action() ||
        (parsed_json.is_member(Rule::MIRROR_PORT_REGION) && !parsed_json[Rule::MIRROR_PORT_REGION].is_null())) {
        validate_mirror_region(parsed_json[Rule::MIRROR_PORT_REGION], rule);
    }

    if (agent_framework::model::enums::AclAction::Mirror == rule.get_action() ||
        (parsed_json.is_member(Rule::MIRROR_TYPE) && !parsed_json[Rule::MIRROR_TYPE].is_null())) {
        validate_mirror_type(parsed_json, mirror_types);
    }


    validate_condition(parsed_json[Rule::CONDITION], rule);

    return parsed_json;
}

}

endpoint::RuleCollection::RuleCollection(const std::string& path) : EndpointBase(path) {}
endpoint::RuleCollection::~RuleCollection() {}

void endpoint::RuleCollection::get(const server::Request &req, server::Response &res)
{
    auto r = ::make_prototype();

    r[Common::ODATA_CONTEXT] = std::regex_replace(
        r[Common::ODATA_CONTEXT].as_string(),
        std::regex("__SWITCH_ID__"),
        req.params[PathParam::ETHERNET_SWITCH_ID]);

    r[Common::ODATA_CONTEXT] = std::regex_replace(
        r[Common::ODATA_CONTEXT].as_string(),
        std::regex("__ACL_ID__"),
        req.params[PathParam::ACL_ID]);

    r[Common::ODATA_ID] = PathBuilder(req).build();

    int iACL_ID = std::stoi(req.params[PathParam::ACL_ID]);
    json::Value link;

    char resultA[BUFFER_LEN] = {0};
    char command[BUFFER_LEN] = {0};

    sprintf(command, "acl.sh get acl_include_rule_count %d", iACL_ID);
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);

    if(strlen(resultA) != 0)
    {
        int icount = atoi(resultA);

        r[Collection::ODATA_COUNT] =icount;
		
	 for (int i = 0; i < icount ; i++) 
	 {
            json::Value link_elem(json::Value::Type::OBJECT);

            sprintf(command, "acl.sh get acl_include_rule %d %d", iACL_ID, i);
            memset(resultA,0x0, sizeof(resultA));
            EcCommonUtils::exec_shell(command, resultA, 0);

	     if(strlen(resultA) != 0)
            {		
                link_elem[Common::ODATA_ID] = PathBuilder(req).append(atoi(resultA)).build();
                r[Collection::MEMBERS].push_back(std::move(link_elem));
	     }
        }	
    }
    set_response(res, r);
}

void endpoint::RuleCollection::post(const server::Request& req,
    server::Response& res) {
    using AclAction = agent_framework::model::enums::AclAction;

    AclRule rule{};

    int iACL_ID = std::stoi(req.params[PathParam::ACL_ID]);


    char resultA[BUFFER_LEN] = {0};	
    char command[BUFFER_LEN] = {0};
    char Action[16] = {0};		
    char MirrorType[16] = {0};		

    std::string temp_path;
    
    unsigned long long   SrcPort  = 0; 
    uint32_t  SrcPortMask = 0;
	
    unsigned long long   DstPort  = 0;
    uint32_t  DstPortMask  = 0;		
		
    uint32_t  Vlan_id  = 0;    
    uint32_t  Vlan_idMask  = 0;
	
    uint32_t  ForwardMirrorPort  = 0;

    char  t_SrcIP[32]="NA";
    char  t_SrcIP_Mask[32]="NA";		
    char  t_DstIP[32]="NA";
    char  t_DstIP_Mask[32]="NA";
    char  t_SrcMAC[32]="NA";
    char  t_SrcMAC_Mask[32]="NA";		
    char  t_DstMAC [32]="NA";
    char  t_DstMAC_Mask [32]="NA";


    sprintf(command, "acl.sh get acl_count");  
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);

    if(atoi(resultA) == 0)
    {
        res.set_status(server::status_5XX::SERVICE_UNAVAILABLE);
        return;
    }

    sprintf(command, "acl.sh get acl_rule_count %d", iACL_ID);  
    memset(resultA,0x0, sizeof(resultA));
    EcCommonUtils::exec_shell(command, resultA, 0);

    if(atoi(resultA)  >= RULE_MAX )
    {
        res.set_status(server::status_5XX::SERVICE_UNAVAILABLE);
        return;
    }
	
    // validate and fill in AclRule at the same time
    const auto json = validate_post_request(req, rule);

    uint32_t rule_id = rule.get_rule_id();

    if (  (rule_id <= RULE_MAX) && (iACL_ID <= ACL_MAX)) 
    {
	 if(rule_id == 0)
	 {
            sprintf(command, "acl.sh set add_acl_rule %d", iACL_ID);  //Create new rule with ACL ID but not have rule index //
            memset(resultA,0x0, sizeof(resultA));
            EcCommonUtils::exec_shell(command, resultA, 0);
    		
            if(strlen(resultA) != 0)
            {
                rule_id = atoi(resultA);
            }
	 }
	 else
	 {
            sprintf(command, "acl.sh set add_acl_rule %d %d", iACL_ID, rule_id);  //Create new rule with ACL ID
    	     memset(resultA,0x0, sizeof(resultA));
            EcCommonUtils::exec_shell(command, resultA, 0);
	 }

        if (rule.get_action().has_value())
        {	 
            if (AclAction::Deny == rule.get_action())
            {
                sprintf(Action, "%s", "Deny");
            } 		
            else if(AclAction::Mirror == rule.get_action())
            {
                sprintf(Action, "%s", "Mirror");
            }  
            else if(AclAction::Forward == rule.get_action()) 
            {
                sprintf(Action, "%s", "Forward");
            }		
            else if (AclAction::Permit == rule.get_action())		
            {
                sprintf(Action, "%s", "Permit");
            }			
            else
                sprintf(Action, "%s", "Deny");
            
            if (enums::AclMirrorType::Ingress == rule.get_mirror_type())
            {
                sprintf(MirrorType, "%s", "Ingress");
            } 				
            else
                sprintf(MirrorType, "%s", "NA");
        }

        if (rule.get_source_port().get_port().has_value())
        {
            SrcPort = rule.get_source_port().get_port();
        }

        if (rule.get_source_port().get_mask().has_value())
        {
            SrcPortMask = rule.get_source_port().get_mask();
        }

        if (rule.get_destination_port().get_port().has_value())
        {
            DstPort = rule.get_destination_port().get_port();
        }	

        if (rule.get_destination_port().get_mask().has_value())
        {
            DstPortMask = rule.get_destination_port().get_mask();
        }	

        if (rule.get_vlan_id().get_id().has_value())
        {
            Vlan_id = rule.get_vlan_id().get_id();
        }

        if (rule.get_vlan_id().get_mask().has_value())
        {
            Vlan_idMask = rule.get_vlan_id().get_mask();
        }	 

        if (rule.get_source_ip().get_address().has_value())
        {
            const string&  SrcIP     = rule.get_source_ip().get_address();
            if(SrcIP.length() != 0)
            {				
                    sprintf(t_SrcIP,"%s", SrcIP.c_str());
            }
        }

        if (rule.get_source_ip().get_mask().has_value())
        {
            const string&  SrcIPMask     = rule.get_source_ip().get_mask();
            if(SrcIPMask.length() != 0)
            {				
                    sprintf(t_SrcIP_Mask ,"%s", SrcIPMask.c_str()); 
            }
        }
		
        if (rule.get_destination_ip().get_address().has_value())
        {
            const string&  DstIP     = rule.get_destination_ip().get_address();
            if(DstIP.length() != 0)
            {				
                    sprintf(t_DstIP,"%s", DstIP.c_str());
            }
        }

        if (rule.get_destination_ip().get_mask().has_value())
        {
            const string&  DstIPMask     = rule.get_destination_ip().get_mask();
            if(DstIPMask.length() != 0)
            {				
                    sprintf(t_DstIP_Mask,"%s", DstIPMask.c_str());
            }
        }		

        if (rule.get_source_mac().get_address().has_value())
        {
            const string&  SrcMAC  = rule.get_source_mac().get_address();
            if(SrcMAC.length() != 0)
            {				
                sprintf(t_SrcMAC,"%s", SrcMAC.c_str());
            }
        }

        if (rule.get_source_mac().get_mask().has_value())
        {
            const string&  SrcMACMask  = rule.get_source_mac().get_mask();
            if(SrcMACMask.length() != 0)
            {				
                sprintf(t_SrcMAC_Mask,"%s", SrcMACMask.c_str()); 
            }
        }

        if (rule.get_destination_mac().get_address().has_value())
        {
            const string&  DstMAC  = rule.get_destination_mac().get_address();
			
            if(DstMAC.length() != 0)
            {				
                sprintf(t_DstMAC,"%s", DstMAC.c_str());
            }
        }

        if (rule.get_destination_mac().get_mask().has_value())
        {
            const string&  DstMACMask  = rule.get_destination_mac().get_mask();
			
            if(DstMACMask.length() != 0)
            {				
                sprintf(t_DstMAC_Mask,"%s",  DstMACMask.c_str() );
            }
        }

//    Binding port must the same with this port  or  permit will not work  // 
        if(rule.get_forward_mirror_port().has_value())
        {
            const string&  ForwardMirroInterface =  rule.get_forward_mirror_port(); 		
    
            if(ForwardMirroInterface.length() != 0)
            {
                temp_path= std::regex_replace(   ForwardMirroInterface.c_str(),    std::regex("/redfish/v1/EthernetSwitches/1/Ports/"), "");
                ForwardMirrorPort = atoi(temp_path.c_str());		   
            }
        }
        
        json::Value schema({JsonValidator::mandatory(Common::ODATA_ID,
                                                     JsonValidator::has_type(JsonValidator::STRING_TYPE))});

        unsigned long long  mirrorports=0;
        char h_mirrorport[128];
		
        for (const auto& key : json["MirrorPortRegion"]) 
        {
            const auto port_url = JsonValidator::validate_request_body(key, schema)[Common::ODATA_ID].as_string();
            temp_path= std::regex_replace(   port_url.c_str() ,    std::regex("/redfish/v1/EthernetSwitches/1/Ports/"), "");
            mirrorports |= (1ULL << atoi(temp_path.c_str()) );
	     sprintf(h_mirrorport,"0x%llx", mirrorports);
        }		
        
//TODO, opional MASK //
//Create new rule with ACL ID
//Action IPSource IPSourceMASK IPDestination IPDestinationMASK MACSource MACSourceMASK
//MACDestination MACDestinationMASK VLANID VLANIDMASK L4SourcePort L4SourcePortMASK
//L4DestPort L4DestPortMASK L4Protocol ForwardMirrorInterface MirrorPortRegion MirrorType

        sprintf(command, "rule.sh set rule_to_acl  %d %d %s %s %s %s %s %s %s %s %s %d 0x%x %llu 0x%x %llu 0x%x NA %d %s %s  ",	
        iACL_ID, rule_id, Action, t_SrcIP, t_SrcIP_Mask, t_DstIP , t_DstIP_Mask, t_SrcMAC, t_SrcMAC_Mask, t_DstMAC, t_DstMAC_Mask , Vlan_id, Vlan_idMask ,SrcPort ,SrcPortMask , DstPort  , DstPortMask, ForwardMirrorPort, h_mirrorport, MirrorType); 

        memset(resultA,0x0, sizeof(resultA));
        HelperTools::exec_shell(command, resultA, 0);

        endpoint::utils::set_location_header(res, PathBuilder(req).append(rule_id).build());
        res.set_status(server::status_2XX::CREATED);
        return;
    }
    res.set_status(server::status_5XX::SERVICE_UNAVAILABLE);
}

