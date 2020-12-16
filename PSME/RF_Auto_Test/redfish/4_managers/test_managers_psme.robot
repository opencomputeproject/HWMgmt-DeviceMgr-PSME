*** Settings ***
Documentation    Test PSME Manager functionality.
Resource         ../../lib/resource.robot
Resource         ../../lib/bmc_redfish_resource.robot
Resource         ../../lib/common_utils.robot

Test Setup       Test Setup Execution
Test Teardown    Test Teardown Execution

*** Variables ***

${FirmwareVersion}  ${FWV}
${TIP}  ${TESTIP}
${IPV4}  { "DHCPv4": { "DHCPEnabled": false }, "IPv4Addresses": [ { "Address": "${TIP}", "SubnetMask": "255.255.252.0", "Gateway": "172.17.10.251" } ], "StaticNameServers": [ { "Address": "8.8.8.8" } ] }

*** Test Cases ***

Verify PSME Redfish Version
    [Documentation]  Get PSME version from manager board.
    [Tags]  Verify_PSME_Redfish_Version 

    ${resp}=  Redfish.Get  /redfish/v1/Managers/1
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    Should Be Equal As Strings
    ...  ${resp.dict["FirmwareVersion"]}  ${FirmwareVersion} 


Verify Redfish PSME Manager Properties
    [Documentation]  Verify PSME managers resource properties.
    [Tags]  Verify_Redfish_PSME_Manager_Properties

    ${resp}=  Redfish.Get  /redfish/v1/Managers/1
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    Should Be Equal As Strings
    ...  ${resp.dict["Description"]}  Manager description 
    Should Be Equal As Strings  ${resp.dict["Id"]}  1 
    Should Be Equal As Strings  ${resp.dict["Name"]}  Manager 
    Should Not Be Empty  ${resp.dict["UUID"]}
    Should Be Equal As Strings  ${resp.dict["Status"]["State"]}  Enabled 


*** Keywords ***

Test Setup Execution
    [Documentation]  Do test case setup tasks.

    redfish.Login


Test Teardown Execution
    [Documentation]  Do the post test teardown.

    redfish.Logout
