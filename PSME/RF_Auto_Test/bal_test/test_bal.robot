*** Settings ***
Documentation    Test BAL Rstful functionality.
Resource         ../lib/resource.robot
Resource         ../lib/bmc_redfish_resource.robot
Resource         ../lib/common_utils.robot
Resource         ./bal_cmd_body.robot

Test Setup       Test Setup Execution  
Test Teardown    Test Teardown Execution 

*** Variables ***

${TX_ENABLE}  {"TxDisabledState": false} 
${TX_DISABLE}  {"TxDisabledState": true} 
${SESSION_DISABLE}  {"ServiceEnabled":false,"SessionTimeout":60}

*** Test Cases ***

Verify Restful API for BAL Test
    [Documentation]  Test BAL Restful Status 
    [Tags]  Test Bal Restful opertaion test 
    Test Bal Component  

Verify Restful API for ONU Range Test
    [Documentation]  Test ONU Range 
    [Tags]  Test ONU Rnage test 
    Test ONU Range

Verify Restful API for flow add
    [Documentation]  Test flow add 
    [Tags]  Test Flow add 
    Test US DS Flow Add 

Verify Restful API for omci send
    [Documentation]  Test omci send 
    [Tags]  Test omci send 
    Test omci send 

*** Keywords ***

Test Setup Execution
    [Documentation]  Do test case setup tasks.

    redfish.Login  

Test Teardown Execution
    [Documentation]  Do the post test teardown.

    #Test Disable Session service
    redfish.Logout  

Test Disable Session service
    [Documentation]  Disable Session Service 
    Log to console  "################### Disable Session Service ###################" 

    ${payload}=  Evaluate  json.loads($SESSION_DISABLE)    json 
    ${resp}=  Redfish.Post  /redfish/v1/SessionService/  body=${payload}
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

Test Bal Component  
    [Documentation]  Do basic test 1.Enable All PON MAC PON NNI port enable/disable. 
    [Tags]  Bal_Component 

    Disable All Tx Port

    #PON_TYPE  SPECIFIC_ID  SPECIFIC_NUM  PON_PORT_ID
    Run Keyword If  '${PON_TYPE}' == 'GPON' 
    \    ...          Log to console  "####### GPON TESTING ######" 
    \    ...    ELSE          
    \    ...          Log to console  "####### XGSPON TESTING ######" 

    Enable All Tx Port
    Test Bal Enable  
    Sleep  5s
    Test Port Present  ${PON_PORT_ID} 
    Sleep  5s
    Enable Port  ${PON_PORT_ID}
    Sleep  5s
    Test NNI Port Enable

Test ONU Range
    [Documentation]  ONU Ranging Test 
    [Tags]  ONU Range 

    #ONU RANGE
    ${ONU_ID_RANGE}=  Run Keyword If  '${PON_TYPE}' == 'GPON' 
    \    ...    set variable   ${150}
    \    ...    ELSE          
    \    ...    set variable  ${255} 

    : FOR  ${id}  IN RANGE   1  ${ONU_ID_RANGE}  50 
    \  Sleep  7s
    \  Test Active ONU  ${id}  ${SPECIFIC_ID}  ${SPECIFIC_NUM}  ${PON_PORT_ID}  ONUID_IN_RANGE
    \  Sleep  7s
    \  Test DeActive ONU  ${id}  ${PON_PORT_ID}

    Test Active ONU  256  ${SPECIFIC_ID}  ${SPECIFIC_NUM}  ${PON_PORT_ID}  NOTONUID_IN_RANGE
    Test Port Present  ${PON_PORT_ID} 
    Enable Port  ${PON_PORT_ID}
    Sleep  3s
    Test Active ONU  1  ${SPECIFIC_ID}  ${SPECIFIC_NUM}  ${PON_PORT_ID}  ONUID_IN_RANGE
    Disable Port  ${PON_PORT_ID}
    Sleep  2s
    ${resp}=  Redfish.Get  /redfish/v1/EthernetSwitches/1/Ports/${PON_PORT_ID}/ONUs 
    ${item_count}  Get Length  ${resp.dict['Members']}
    Should Be Equal As Integers  ${item_count}  0 

    Enable Port  ${PON_PORT_ID}
    Sleep  10s
    ${resp}=  Redfish.Get  /redfish/v1/EthernetSwitches/1/Ports/${PON_PORT_ID}/ONUs 
    ${item_count}  Get Length  ${resp.dict['Members']}
    Should Be Equal As Integers  ${item_count}  1 
    Sleep  5s

Test US DS Flow Add 
    [Documentation]  Flow Add/Remove 
    [Tags]  US DS Flow Add/Remove 

    : FOR  ${flow_id}  IN RANGE   0  2047  500 
    \  Sleep  1s
    \  Test Flow Add  1  ${flow_id}  ${PON_PORT_ID}  ONUID_IN_RANGE
    \  Sleep  1s
    \  Test Flow Del  ${flow_id} 

    Test Flow Add  1  2048  ${PON_PORT_ID}  NOTONUID_IN_RANGE
    Test Flow Add  1  16  ${PON_PORT_ID}  ONUID_IN_RANGE

Test omci send 
    [Documentation]  Send omci raw data
    [Tags]  omci send 
    Test Send Omci  1  ${PON_PORT_ID} 
    Enable All Tx Port

Test Bal Enable 
    [Documentation]  Enable Bal 
    [Tags]  Verify_Bal_Enable

    : FOR  ${i}  IN RANGE   1   80 
    \  ${result} =  Test Wait Bal Enable 
    \  Log to console  "################### resule[${result}] ###################" 
    \  EXIT FOR LOOP IF  '${result}' == 'up' 

    # Check if BAL is ready or not
    Should Be Equal As Strings  ${result}  up 

    ${payload}=  Evaluate  json.loads($OPT_ENABLE)    json 
    ${resp}=  Redfish.Patch  /redfish/v1/Olt  body=${payload}

    : FOR  ${i}  IN RANGE   1   60 
    \  ${result} =  Test Wait BalOpt Enable 
    \  Log to console  "################### resule[${result}] ###################" 
    \  EXIT FOR LOOP IF  ${result}

    # Check Opt state is true or false
    Should Be True     ${result}


Test Wait Bal Enable 
    [Documentation]  Wait Enable Bal 
    [Tags]  Verify_Waut_Bal_Enable
    [Return]  ${bal_state}

    Log to console  "################### Wait Bal Enable ###################" 
    ${resp}=  Redfish.Get  /redfish/v1/Olt 
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
    ${BAL_STATE} =  Set Variable  ${resp.dict['BalState']}

    Run Keyword If  '${BAL_STATE}' == 'up' 
    \    ...          Log to console  "####### Up ######" 
    \    ...          ${bal_state} =  "up" 
    \    ...    ELSE          
    \    ...          Log to console  "####### Down ######" 
    \    ...          ${bal_state} =  "down" 
    Sleep  1s

Test Wait BalOpt Enable 
    [Documentation]  Wait Enable Bal Opt 
    [Tags]  Verify_Waut_Bal_Opt_Enable
    [Return]  ${bal_opt_state}

    Log to console  "################### Wait Bal Opt Enable ###################" 
    ${resp}=  Redfish.Get  /redfish/v1/Olt 
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
    ${bal_opt_state} =  Set Variable  ${resp.dict['OltOperateState']}
    Log to console  "####### Opt ${bal_opt_state} ######" 

    Run Keyword If  ${bal_opt_state}  
    \    ...          Log to console  "####### Opt Up ######" 
    \    ...    ELSE          
    \    ...          Log to console  "####### Opt Down ######" 
    Sleep  1s

Test NNI Port Enable
    [Documentation]  Verify NNI port enable function 
    [Tags]  Verify_nni_enable

    ${resp}=  Redfish.Get  /redfish/v1/EthernetSwitches/1/Ports/
 
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${item_count}  Get Length  ${resp.dict['Members']}
    Log to console  "####### num ${item_count} ######" 

    ${NNI_PORT}=  Run Keyword If  '${PON_TYPE}' == 'GPON' 
    \    ...    set variable   ${65}
    \    ...    ELSE          
    \    ...    set variable  ${17} 

    Test Port Present  ${NNI_PORT}
    Enable Port  ${NNI_PORT}


Test Port Present 
    [Documentation]  Test Pon port present function 
    [Tags]  Test pon_status present 
    [Arguments]   ${ID} 

    ${resp}=  Redfish.Get  /redfish/v1/EthernetSwitches/1/Ports/${ID}
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${PORT_ENABLE} =  Set Variable  ${resp.dict['Status']['State']}
    ${PORT_ID} =  Set Variable  ${resp.dict['PortId']}
    Log to console              ${PORT_ENABLE}
    Log to console              ${PortId}

    Should Be Equal As Strings  '${PORT_ENABLE}'  'Enabled'

Enable Port
    [Documentation]  Enable port function 
    [Tags]  Enable Port 
    [Arguments]   ${ID} 

    Log to console  "####### Enable Port ${ID} ######" 
    ${payload}=  Evaluate  json.loads($ADM_UP)    json 
    ${resp}=  Redfish.Patch  /redfish/v1/EthernetSwitches/1/Ports/${ID}  body=${payload}
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/EthernetSwitches/1/Ports/${ID}
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${ADM_STAT} =  Set Variable  ${resp.dict['AdministrativeState']}

    Should Be Equal As Strings  '${ADM_STAT}'  'Up'


Disable All Tx Port
    [Documentation]  Disable all port function 
    [Tags]  Disable Pon Port Tx 

    ${resp}=  Redfish.Get  /redfish/v1/EthernetSwitches/1/Ports/
    ${item_count}  Get Length  ${resp.dict['Members']}
    Log to console  "####### Disable All Pon Port ${item_count} ######" 

    :FOR    ${ID}  IN RANGE   1    ${item_count} 
    \        Tx Disable Down  ${ID}

Enable All Tx Port
    [Documentation]  Enable all port function 
    [Tags]  Enable Pon Port Tx 

    ${resp}=  Redfish.Get  /redfish/v1/EthernetSwitches/1/Ports/
    ${item_count}  Get Length  ${resp.dict['Members']}
    Log to console  "####### Enable All Pon Port Tx ${item_count} ######" 

    :FOR    ${ID}  IN RANGE   1    ${item_count} 
    \        Tx Disable Up  ${ID}

Tx Disable Down
    [Documentation]  Test SFP port tx disable down function 
    [Tags]  Test tx_disable down 
    [Arguments]   ${ID} 

    ${resp}=  Redfish.Get  /redfish/v1/EthernetSwitches/1/Ports/${ID} 
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${PORT_ENABLE} =  Set Variable  ${resp.dict['Status']['State']}
    ${PORT_ID} =  Set Variable  ${resp.dict['PortId']}
    Log to console              ${PORT_ENABLE}
    Log to console              ${PortId}
    Log to console              ${resp.dict['TxDisabledState']}

    Run Keyword If  '${PORT_ENABLE}' == 'Enabled' 
    \    ...        Run Keyword If  '${PORT_ID}' == 'PON port' or '${PORT_ID}' == 'SFP port'
    \    ...        Test Tx Op Down   ${ID} 
    \    ...    ELSE          
    \    ...          Log to console  "####### Disabled ######" 

Tx Disable Up 
    [Documentation]  Test SFP port tx disable Up function 
    [Tags]  Test tx_disable up 
    [Arguments]   ${ID} 

    ${resp}=  Redfish.Get  /redfish/v1/EthernetSwitches/1/Ports/${ID} 
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${PORT_ENABLE} =  Set Variable  ${resp.dict['Status']['State']}
    ${PORT_ID} =  Set Variable  ${resp.dict['PortId']}
    Log to console              ${PORT_ENABLE}
    Log to console              ${PortId}
    Log to console              ${resp.dict['TxDisabledState']}

    Run Keyword If  '${PORT_ENABLE}' == 'Enabled' 
    \    ...        Run Keyword If  '${PORT_ID}' == 'PON port' or '${PORT_ID}' == 'SFP port'
    \    ...        Test Tx Op Up   ${ID} 
    \    ...    ELSE          
    \    ...          Log to console  "####### Disable ######" 

Disable Port
    [Documentation]  Enable port function 
    [Tags]  Enable Port 
    [Arguments]   ${ID} 

    ${payload}=  Evaluate  json.loads($ADM_DOWN)    json 
    ${resp}=  Redfish.Patch  /redfish/v1/EthernetSwitches/1/Ports/${ID}  body=${payload}
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/EthernetSwitches/1/Ports/${ID}
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${ADM_STAT} =  Set Variable  ${resp.dict['AdministrativeState']}

    Should Be Equal As Strings  '${ADM_STAT}'  'Down'

Test Tx Op Down 
    [Documentation]  Test SFP port tx disable function 
    [Tags]  Test tx_disable 
    [Arguments]   ${ID} 

    ${payload}=  Evaluate  json.loads($TX_DISABLE)    json 
    Redfish.Patch  /redfish/v1/EthernetSwitches/1/Ports/${ID}  body=${payload}

Test Tx Op Up 
    [Documentation]  Test SFP port tx disable function 
    [Tags]  Test tx_disable 
    [Arguments]   ${ID} 

    ${payload}=  Evaluate  json.loads($TX_ENABLE)    json 
    Redfish.Patch  /redfish/v1/EthernetSwitches/1/Ports/${ID}  body=${payload}

Test Active ONU
    [Documentation]  Test Active ONU function 
    [Tags]  Test Active ONU  
    [Arguments]  ${ONU_ID}  ${VEN_ID}  ${VEN_SPEC}  ${PON_PORT_ID}  ${ONUID_IN_RANGE}

    ${JSTRING}=    catenate
    ...  {
    ...    "onu_id": ${ONU_ID},
    ...    "vendor_id": "${VEN_ID}",
    ...    "vendor_specific":"${VEN_SPEC}"
    ...  }

    Log to console  "####### active onu ${JSTRING} ######" 

    ${payload}=  Evaluate  json.loads($JSTRING)    json 
    ${resp}=  Redfish.Post  /redfish/v1/EthernetSwitches/1/Ports/${PON_PORT_ID}/ONUs  body=${payload}

    Run Keyword If  '${ONUID_IN_RANGE}' == 'ONUID_IN_RANGE' 
    \    ...          Should Be Equal As Strings  ${resp.status}  ${HTTP_CREATED} 
    \    ...    ELSE          
    \    ...          Should Not Be Equal As Strings  ${resp.status}  ${HTTP_CREATED}

    Sleep  6s

    ${resp}=  Redfish.Get  /redfish/v1/EthernetSwitches/1/Ports/${PON_PORT_ID}/ONUs 
    Log to console  "####### onu res ${resp} ######" 
    ${item_count}  Get Length  ${resp.dict['Members']}
    Log to console  "####### Onu num ${item_count} ######" 

    Run Keyword If  '${ONUID_IN_RANGE}' == 'ONUID_IN_RANGE' 
    \    ...          Should Be Equal As Integers  ${item_count}  1 
    \    ...    ELSE          
    \    ...          Should Be Equal As Integers  ${item_count}  0 

Test DeActive ONU
    [Documentation]  Test DeActive ONU function 
    [Tags]  Test DeActive ONU  
    [Arguments]  ${ONU_ID}  ${PON_PORT_ID} 
    Log to console  "####### Deactive onu ${ONU_ID} ######" 

    :FOR    ${j}    IN RANGE    5 
    \    Exit For Loop If    ${j} == 4
    \    ${resp}=  Redfish.Delete  /redfish/v1/EthernetSwitches/1/Ports/${PON_PORT_ID}/ONUs/${ONU_ID}
    \    Exit For Loop If    ${resp.status} == ${HTTP_OK} 
    \    Log to console  "retry deactive ${j}"
    \    Sleep  2s 

    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK} 

Test Flow Add 
    [Documentation]  Test Flow Add 
    [Tags]  UP/DOWN stream flow add 
    [Arguments]  ${ONU_ID}  ${FLOW_ID}  ${PON_ID}  ${ONUID_IN_RANGE} 

    ${PON_ID}=    Evaluate   ${PON_ID} - ${1}
    ${JSTRING}=    catenate
    ...  {
    ...   "onuId":${ONU_ID},"FlowId":${FLOW_ID},"PortId":${PON_ID},"NniId":0,"FlowType":"upstream","PktTagType":"single_tag","GemportId":1024,
    ...   "Classifier":["BCMOLT_CLASSIFIER_ID_O_VID"],"Action":"BCMOLT_ACTION_ID_O_VID","ActionCmd":"BCMOLT_ACTION_CMD_ID_ADD_OUTER_TAG",
    ...   "ActionVal":{"OVid":10,"OPbits":0,"OTpid":0,"IVid":0,"IPbits":0,"ITpid":0,"EtherType":0,"IpProto":0,"SrcPort":0,
    ...   "DstPort":0},"ClassVal":[{"OVid":20,"IVid":0,"OPbits":0,"IPbits":0,"EtherType":0,"OTpid":0,"IpProto":0,"DstPort":0,"SrcPort":0,"ITpid":0}]
    ...  }

    ${payload}=  Evaluate  json.loads($JSTRING)    json 
    ${resp}=  Redfish.Post  /redfish/v1/Olt/Flow/  body=${payload}

    Run Keyword If  '${ONUID_IN_RANGE}' == 'ONUID_IN_RANGE' 
    \    ...          Should Be Equal As Strings  ${resp.status}  ${HTTP_OK} 
    \    ...    ELSE          
    \    ...          Should Not Be Equal As Strings  ${resp.status}  ${HTTP_OK}


    ${JSTRING}=    catenate
    ...  { 
    ...  "onuId":${ONU_ID},"FlowId":${FLOW_ID},"PortId":${PON_ID},"NniId":0,"FlowType":"downstream","PktTagType":"double_tag",
    ...  "GemportId":1024,"Classifier":["BCMOLT_CLASSIFIER_ID_O_VID","BCMOLT_CLASSIFIER_ID_I_VID"],"Action":"BCMOLT_ACTION_ID_O_VID",
    ...  "ActionCmd":"BCMOLT_ACTION_CMD_ID_REMOVE_OUTER_TAG","ActionVal":{"OVid":10,"OPbits":0,"OTpid":0,"IVid":0,"IPbits":0,"ITpid":0,
    ...  "EtherType":0,"IpProto":0,"SrcPort":0,"DstPort":0},
    ...  "ClassVal":[{"OVid":10,"IVid":20,"OPbits":0,"IPbits":0,"EtherType":0,"OTpid":0,"IpProto":0,"DstPort":0,"SrcPort":0,"ITpid":0
    ...   }]
    ...  }

    ${payload}=  Evaluate  json.loads($JSTRING)    json 
    ${resp}=  Redfish.Post  /redfish/v1/Olt/Flow/  body=${payload}

    Run Keyword If  '${ONUID_IN_RANGE}' == 'ONUID_IN_RANGE' 
    \    ...          Should Be Equal As Strings  ${resp.status}  ${HTTP_OK} 
    \    ...    ELSE          
    \    ...          Should Not Be Equal As Strings  ${resp.status}  ${HTTP_OK}

Test Flow Del 
    [Documentation]  Test Flow Del 
    [Tags]  UP/DOWN stream flow del 
    [Arguments]  ${FLOW_ID}

    ${resp}=  Redfish.Delete  /redfish/v1/Olt/Flow/${FLOW_ID}_upstream  
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK} 

    ${resp}=  Redfish.Delete  /redfish/v1/Olt/Flow/${FLOW_ID}_downstream 
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK} 

Test Send Omci  
    [Documentation]  Send Omci function 
    [Tags]  Send Omci function 
    [Arguments]  ${ONU_ID}  ${PON_PORT_ID}

    ${payload_}=  Evaluate  json.loads($OMCI_RAW)    json 
    ${length}=  get length  ${payload_["raw_data"]
    Log to console  ====================length ${length} ================

    : FOR  ${i}  IN RANGE   0   171 
    \  Log to console  "################### raw[${payload_["raw_data"][${i}]}] ###################" 
    \  Sleep  0.3s
    \  ${resp}=  Redfish.Post  /redfish/v1/EthernetSwitches/1/Ports/${PON_PORT_ID}/ONUs/${ONU_ID}/Omci  body=${payload_["raw_data"][${i}]}


