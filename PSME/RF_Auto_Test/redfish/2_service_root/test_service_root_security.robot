*** Settings ***
Documentation    Test Redfish service root login security.

Resource         ../../lib/resource.robot
Resource         ../../lib/bmc_redfish_resource.robot
Resource         ../../lib/openbmc_ffdc.robot

Resource         ../../lib/bmc_network_utils.robot
Library          ../../lib/bmc_network_utils.py


*** Variables ***
# Currentl only support single session #

${SESSION_TIME_OUT_TEST}   ${35}

*** Test Cases ***

Redfish Login With Invalid Credentials
    [Documentation]  Login to BMC web using invalid credential.
    [Tags]  Redfish_Login_With_Invalid_Credentials
    [Template]  Login And Verify Redfish Response

    # Expect status            Username               Password
    *                            ${OPENBMC_USERNAME}    deadpassword
    *                            groot                  ${OPENBMC_PASSWORD}
    *                            ${EMPTY}               ${OPENBMC_PASSWORD}
    *                            ${OPENBMC_USERNAME}    ${EMPTY}
    *                            ${EMPTY}               ${EMPTY}

Attempt Login With Expired Session
    [Documentation]  Authenticate to redfish, then log out and attempt to
    ...   use the session.
    [Tags]  Attempt_Login_With_Expired_Session

    Redfish.Login
    ${saved_session_info}=  Get Redfish Session Info

    ${payload}=  Create Dictionary
    ...  ServiceEnabled=${true}  SessionTimeout=${30}
    Redfish.Post  /redfish/v1/SessionService/  body=&{payload}
    ...  valid_status_codes=[${HTTP_OK}]

    Sleep  ${SESSION_TIME_OUT_TEST}s 

    # Attempt login with expired session.
    # By default 600 seconds of inactivity closes the session.
    Redfish.Set Session Key  ${saved_session_info["key"]}
    Redfish.Set Session Location  ${saved_session_info["location"]}

    Redfish.Get  ${saved_session_info["location"]}  valid_status_codes=[${HTTP_NOT_FOUND}]

    # Set back to default value 600s
    Redfish.Login

    ${payload}=  Create Dictionary
    ...  ServiceEnabled=${true}  SessionTimeout=${600}
    Redfish.Post  /redfish/v1/SessionService/  body=&{payload}
    ...  valid_status_codes=[${HTTP_OK}]

    Redfish.Logout

*** Keywords ***

Login And Verify Redfish Response
    [Documentation]  Login and verify redfish response.
    [Arguments]  ${expected_response}  ${username}  ${password}

    # Description of arguments:
    # expected_response   Expected REST status.
    # username            The username to be used to connect to the server.
    # password            The password to be used to connect to the server.

    # The redfish object may preserve a valid username or password from the
    # last failed login attempt.  If we then try to login with a null username
    # or password value, the redfish object may prefer the preserved value.
    # Since we're testing bad path, we wish to avoid this scenario so we will
    # clear these values.

    Redfish.Set Username  ${EMPTY}
    Redfish.Set Password  ${EMPTY}

    Run Keyword And Expect Error  ${expected_response}
    ...  Redfish.Login  ${username}  ${password}


Create New Login Session
    [Documentation]  Multiple login session keys.

    Redfish.Login
    ${session_info}=  Get Redfish Session Info

    # Append the session location to the list.
    # ['/redfish/v1/SessionService/Sessions/uDzihgDecs',
    #  '/redfish/v1/SessionService/Sessions/PaHF5brPPd']
    Append To List  ${session_list}  ${session_info["location"]}

