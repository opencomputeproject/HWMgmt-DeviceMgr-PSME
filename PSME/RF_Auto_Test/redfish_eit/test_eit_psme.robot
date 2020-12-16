*** Settings ***
Documentation    Test PSME EIT ECRF_PAL functionality.
Resource         ../../lib/resource.robot
Resource         ../../lib/bmc_redfish_resource.robot
Resource         ../../lib/common_utils.robot

Test Setup       Test Setup Execution
Test Teardown    Test Teardown Execution


*** Variables ***


*** Test Cases ***


#EIT Thermal Sensor 
#    [Documentation]  Verify Thermal sensor 
#    [Tags]  Verify_Thermal_sensor 
#
#    ${resp}=  Redfish.Get   /redfish/v1/Chassis/1/Thermal/ 
# 
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${item_count}  Get Length  ${resp.dict['Temperatures']}
#    Log to console             ${item_count} 
#
#    ${n1}  Set Variable  1
#    ${n2}  Set Variable  ${item_count} 
#
#    : FOR  ${i}  IN RANGE   ${n1}   ${n2} 
#    \    Test Thermal Sensor   ${i}
#
#EIT Fan 
#
#    [Documentation]  Verify Port and verify status
#    [Tags]  Verify_FAN_and_verify_status
#
#    ${resp}=  Redfish.Get   /redfish/v1/Chassis/1/Thermal/ 
# 
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${item_count}  Get Length  ${resp.dict['Fans']}
#    Log to console             ${item_count} 
#
#    ${ITEM_COUNT}=  Evaluate  ${item_count} + ${1}
#
#    ${n1}  Set Variable  1
#    ${n2}  Set Variable  5 
#
#    # We assume there always have 2 PSU devices
#
#    ${SYS_FAN_COUNT}=  Evaluate  ${ITEM_COUNT} - ${2}
#
#    Log to console  "#################################################" 
#    : FOR  ${i}  IN RANGE   ${n1}   ${SYS_FAN_COUNT} 
#      \    ${var1} =  Run Keyword If  '${i}' >= '${SYS_FAN_COUNT}'        
#      \    ...          Log to console  "####### Un Plug PSU Power Core ${i} ######" 
#      \    ...        ELSE          
#      \    ...          Log to console  "####### Un Plug FAN ${i} ######" 
#      \  Sleep  15s 
#      \  Un Plug Fan  ${i}
#      \  Run Keyword If  '${i}' >= '${SYS_FAN_COUNT}'
#      \    ...  Log to console  "####### Plug In PSU ${i} ######"
#
#
#    Log to console  "#################################################" 
#    : FOR  ${i}  IN RANGE   ${n1}   ${SYS_FAN_COUNT} 
#      \    ${var1} =  Run Keyword If  '${i}' >= '${SYS_FAN_COUNT}'        
#      \    ...          Log to console  "#############" 
#      \    ...        ELSE          
#      \    ...          Log to console  "####### Plug In FAN ${i} ######" 
#      \  Sleep  15s 
#      \  Plug In Fan  ${i}
#

   
EIT Port

    [Documentation]  Verify Port and verify status
    [Tags]  Verify_Port_and_verify_status

    ${resp}=  Redfish.Get  /redfish/v1/EthernetSwitches/1/Ports/

    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${Port_Counts}=  Set Variable  ${resp.dict['Members@odata.count']}
    ${ITEM_COUNT}=  Evaluate  ${Port_Counts} + ${1}

    Should Not Be Equal As Integers   ${Port_Counts}  0

    ${n1}  Set Variable  73 
    ${n2}  Set Variable  ${ITEM_COUNT}

    : FOR  ${i}  IN RANGE   ${n1}   ${n2} 

    \    Log to console  "####### Plug In Port ${i} ######" 
    \    Sleep  16s 
    \    Plug In Port  ${i}

    \    Log to console  "####### Un Plug In Port ${i} ######" 
    \    Sleep  16s 
    \    Un Plug Port  ${i} 

    \    Log to console  "####### Plug In Port ${i} ######" 
    \    Sleep  16s 
    \    Plug In Port  ${i}



   
#EIT PSU
#
#    [Documentation]  Verify PSU and verify status
#    [Tags]  Verify_PSU_and_verify_status
#
#    Log to console  "######## Plug In PSU 1 with Power Core #######" 
#    Sleep  20s 
#    Plug In PSU  1 
#
#    Log to console  "######## Plug In PSU 2 with Power Core #######" 
#    Sleep  20s 
#    Plug In PSU  2 
#
#    Log to console  "######## Un Plug PSU 1 #######" 
#    Sleep  30s 
#    Un Plug PSU  1
#
#    Log to console  "######## Plug In PSU 1 without Power Core #######" 
#    Sleep  30s 
#    Plug In PSU Withoit Power Core  1
#
#    Log to console  "######## Plug In PSU 1 with Power Core #######" 
#    Sleep  30s 
#    Plug In PSU  1
#
#    Log to console  "######## Un Plug PSU 2 #######" 
#    Sleep  30s 
#    Un Plug PSU  2
#
#    Log to console  "######## Plug In PSU 2 without Power Core #######" 
#    Sleep  30s 
#    Plug In PSU Withoit Power Core  2
#
#    Log to console  "######## Plug In PSU 2 with Power Core #######" 
#    Sleep  30s 
#    Plug In PSU  2

#Verify Plug In PSU
#    [Documentation]  Verify plug PSU and verify status
#    [Tags]  Verify_PSU_IN_and_verify_status
#    [Template]  Plug In PSU 
#
#    #PSU_ID
#    1
#
#Verify Plug In PSU Withoit Power Core
#    [Documentation]  Verify plug PSU Without Power Coreand verify status
#    [Tags]  Verify_PSU_IN_Without_Power_core_and_verify_status
#    [Template]  Plug In PSU Withoit Power Core
#
#    #PSU_ID
#    2
#    

*** Keywords ***

Test Setup Execution
    [Documentation]  Do test case setup tasks.

    redfish.Login


Test Teardown Execution
    [Documentation]  Do the post test teardown.

    redfish.Logout

Plug In PSU Withoit Power Core
    [Documentation]  Plug In PSU Withoit Power Core 
    [Tags]  Plug_In_PSU_Withoit_Power_Core 
    [Arguments]   ${PSU_ID} 
    ${ID}=  Evaluate  ${PSU_ID} - ${1}

    ${resp}=  Redfish.Get  /redfish/v1/Chassis/1/Power 
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    Log to console              ${resp.dict['PowerControl'][${ID}]['Status']['State']}
    Should Be Equal As Strings  ${resp.dict['PowerControl'][${ID}]['Status']['State']}  UnavailableOffline 

    Log to console   ${resp.dict['PowerControl'][${ID}]['PowerConsumedWatts']}
    Should Be Equal As Integers  ${resp.dict['PowerControl'][${ID}]['PowerConsumedWatts']}  0

Plug In PSU
    [Documentation]  Plugi In PSU and verify status
    [Tags]  Plug_In_PSU_and_verify_status
    [Arguments]   ${PSU_ID} 
    ${ID}=  Evaluate  ${PSU_ID} - ${1}

    ${resp}=  Redfish.Get  /redfish/v1/Chassis/1/Power 
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    Log to console              ${resp.dict['PowerControl'][${ID}]['Status']['State']}
    Should Be Equal As Strings  ${resp.dict['PowerControl'][${ID}]['Status']['State']}  Enabled 

    Log to console   ${resp.dict['PowerControl'][${ID}]['PowerConsumedWatts']}
    Should Not Be Equal As Integers  ${resp.dict['PowerControl'][${ID}]['PowerConsumedWatts']}  0


Un Plug PSU
    [Documentation]  Un Plug PSU and verify status
    [Tags]  Un_Plug_PSU_and_verify_status
    [Arguments]   ${PSU_ID} 
    ${ID}=  Evaluate  ${PSU_ID} - ${1}

    ${resp}=  Redfish.Get  /redfish/v1/Chassis/1/Power 
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    Log to console              ${resp.dict['PowerControl'][${ID}]['Status']['State']}
    Should Be Equal As Strings  ${resp.dict['PowerControl'][${ID}]['Status']['State']}  Absent 

    Log to console   ${resp.dict['PowerControl'][${ID}]['PowerConsumedWatts']}
    Should Be Equal As Integers  ${resp.dict['PowerControl'][${ID}]['PowerConsumedWatts']}  0


Un Plug Port 
    [Documentation]  Un Plug Port  and verify status
    [Tags]  Un_Plug_Port_and_verify_status
    [Arguments]   ${PORT_ID} 

    ${resp}=  Redfish.Get  redfish/v1/EthernetSwitches/1/Ports/${PORT_ID}
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    Log to console              ${resp.dict['Status']['State']}
    Should Be Equal As Strings  ${resp.dict['Status']['State']}   Absent 


Plug In Port
    [Documentation]  Plug In Port  and verify status
    [Tags]  Plug_IN_Port_and_verify_status
    [Arguments]   ${PORT_ID} 

    ${resp}=  Redfish.Get  redfish/v1/EthernetSwitches/1/Ports/${PORT_ID}
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    Log to console              ${resp.dict['Status']['State']}
    Should Be Equal As Strings  ${resp.dict['Status']['State']}  Enabled 

    Log to console       ${resp.dict['TransceiverStatistics']['SFP Vendor Name']}  
    Should Not Be Empty  ${resp.dict['TransceiverStatistics']['SFP Vendor Name']}  

    Log to console       ${resp.dict['TransceiverStatistics']['Part Number']}  
    Should Not Be Empty  ${resp.dict['TransceiverStatistics']['Part Number']}  

    Log to console       ${resp.dict['TransceiverStatistics']['Serial Number']}  
    Should Not Be Empty  ${resp.dict['TransceiverStatistics']['Serial Number']}  

    Log to console       ${resp.dict['TransceiverStatistics']['Manufacture Date']}  
    Should Not Be Empty  ${resp.dict['TransceiverStatistics']['Manufacture Date']}  

    Log to console       ${resp.dict['TransceiverStatistics']['Temperature']}  
    Should Not Be Empty  ${resp.dict['TransceiverStatistics']['Temperature']}  
    
    Log to console       ${resp.dict['TransceiverStatistics']['Voltage']}  
    Should Not Be Empty  ${resp.dict['TransceiverStatistics']['Voltage']}  

    Log to console       ${resp.dict['TransceiverStatistics']['BiasCurrent']}  
    Should Not Be Empty  ${resp.dict['TransceiverStatistics']['BiasCurrent']}  

    Log to console       ${resp.dict['TransceiverStatistics']['TxPower']}  
    Should Not Be Empty  ${resp.dict['TransceiverStatistics']['TxPower']}  



Un Plug Fan 
    [Documentation]  Un Plug Fan and verify status
    [Tags]  Un_Plug_Fan_and_verify_status
    [Arguments]   ${FAN_ID} 
    ${ID}=  Evaluate  ${FAN_ID} - ${1}

    ${resp}=  Redfish.Get  /redfish/v1/Chassis/1/Thermal 
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    Log to console              ${resp.dict['Fans'][${ID}]}
    Should Be Equal As Strings  ${resp.dict['Fans'][${ID}]['Status']['State']}  Absent  

    Log to console               ${resp.dict['Fans'][${ID}]['Reading']}
    Should Be Equal As Integers  ${resp.dict['Fans'][${ID}]['Reading']}  0


Plug In Fan 
    [Documentation]  Plug In Fan and verify status
    [Tags]  Plug_In_Fan_and_verify_status
    [Arguments]   ${FAN_ID} 
    ${ID}=  Evaluate  ${FAN_ID} - ${1}

    ${resp}=  Redfish.Get  /redfish/v1/Chassis/1/Thermal 
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    Log to console              ${resp.dict['Fans'][${ID}]}
    Should Be Equal As Strings  ${resp.dict['Fans'][${ID}]['Status']['State']}  Enabled 

    Log to console               ${resp.dict['Fans'][${ID}]['Reading']}
    Should Not Be Equal As Integers  ${resp.dict['Fans'][${ID}]['Reading']}  0


Test Thermal Sensor  
    [Documentation]  Test Thermal Sensor  
    [Tags]  Test Thermal Sensor  
    [Arguments]   ${THERMAL_SENSOR_ID} 
    ${ID}=  Evaluate  ${THERMAL_SENSOR_ID} - ${1}

    ${resp}=  Redfish.Get  /redfish/v1/Chassis/1/Thermal 
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    Log to console              ${resp.dict['Temperatures'][${ID}]}
    Should Be Equal As Strings  ${resp.dict['Temperatures'][${ID}]['Status']['State']}  Enabled 

    Log to console               ${resp.dict['Temperatures'][${ID}]['ReadingCelsius']}
    Should Not Be Equal As Integers  ${resp.dict['Temperatures'][${ID}]['ReadingCelsius']}  0




