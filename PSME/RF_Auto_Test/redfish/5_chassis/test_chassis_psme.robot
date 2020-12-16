*** Settings ***
Documentation    Test PSME Manager functionality.
Resource         ../../lib/resource.robot
Resource         ../../lib/bmc_redfish_resource.robot
Resource         ../../lib/common_utils.robot

Test Setup       Test Setup Execution
Test Teardown    Test Teardown Execution

*** Variables ***

*** Test Cases ***

Verify Redfish PSME Chassis Properties
    [Documentation]  Verify PSME chassis resource properties.
    [Tags]  Verify_Redfish_PSME_Chsssis_Properties

    ${resp}=  Redfish.Get  /redfish/v1/Chassis/1
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

Check Thermal Sensor 
    [Documentation]  Verify Thermal sensor 
    [Tags]  Verify_Thermal_sensor 

    ${resp}=  Redfish.Get   /redfish/v1/Chassis/1/Thermal/ 
 
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${item_count}  Get Length  ${resp.dict['Temperatures']}
    Log to console             ${item_count} 

    ${n1}  Set Variable  1
    ${n2}  Set Variable  ${item_count} 

    FOR  ${i}  IN RANGE   ${n1}   ${n2} 
        Test Thermal Sensor   ${i}
    END


Check Fan 

    [Documentation]  Verify Fan and verify status
    [Tags]  Verify_FAN_and_verify_status

    ${resp}=  Redfish.Get   /redfish/v1/Chassis/1/Thermal/ 
 
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${item_count}  Get Length  ${resp.dict['Fans']}
    Log to console             ${item_count} 

    ${n1}  Set Variable  1
    ${n2}  Set Variable  ${item_count}  

    Log to console  "#################################################" 
    : FOR  ${i}  IN RANGE   ${n1}   ${n2} 
      \    Plug In Fan  ${i}

Check PSU

    [Documentation]  Verify PSU and verify status
    [Tags]  Verify_PSU_and_verify_status

    Log to console  "######## Plug In PSU 1 with Power Core #######" 
    Plug In PSU  1 

    Log to console  "######## Plug In PSU 2 with Power Core #######" 
    Plug In PSU  2 

*** Keywords ***

Test Setup Execution
    [Documentation]  Do test case setup tasks.

    redfish.Login

Test Teardown Execution
    [Documentation]  Do the post test teardown.

    redfish.Logout

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


