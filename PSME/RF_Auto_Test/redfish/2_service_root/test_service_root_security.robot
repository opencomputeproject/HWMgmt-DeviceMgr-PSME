*** Settings ***
Documentation    Test Redfish service root login security.

Resource         ../../lib/resource.robot
Resource         ../../lib/bmc_redfish_resource.robot
Resource         ../../lib/openbmc_ffdc.robot

Resource         ../../lib/bmc_network_utils.robot
Library          ../../lib/bmc_network_utils.py


*** Variables ***
# Currentl only support single session #

${LOGIN_SESSION_COUNT}   ${5}
${SESSION_TIME_OUT_TEST}   ${35}

&{header_requirements}  strict-transport-security=max-age=31536000; includeSubdomains; preload
...                     x-frame-options=DENY
...                     pragma=no-cache
...                     cache-control=no-Store,no-Cache
...                     content-security-policy=default-src 'self'; img-src 'self' data:
...                     x-xss-protection=1; mode=block
...                     x-content-type-options=nosniff

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


#Login And Verify HTTP Response Header
#    [Documentation]  Login and verify redfish HTTP response header.
#    [Tags]  Login_And_Verify_HTTP_Response_Header

    # Example of HTTP redfish response header.
    # Strict-Transport-Security: max-age=31536000; includeSubdomains; preload
    # X-Frame-Options: DENY
    # Pragma: no-cache
    # Cache-Control: no-Store,no-Cache
    # Content-Security-Policy: default-src 'self'; img-src 'self' data:
    # X-XSS-Protection: 1; mode=block
    # X-Content-Type-Options: nosniff

#    Rprint Vars  1  header_requirements

#    Redfish.Login
#    ${resp}=  Redfish.Get  /redfish/v1/SessionService/Sessions

    # The getheaders() method returns the headers as a list of tuples:
    # headers:
    #    [Strict-Transport-Security]:        max-age=31536000; includeSubdomains; preload
    #    [X-Frame-Options]:                  DENY
    #    [Pragma]:                           no-cache
    #    [Cache-Control]:                    no-Store,no-Cache
    #    [Content-Security-Policy]:          default-src 'self'; img-src 'self' data:
    #    [X-XSS-Protection]:                 1; mode=block
    #    [X-Content-Type-Options]:           nosniff
    #    [X-UA-Compatible]:                  IE=11
    #    [Content-Type]:                     application/json
    #    [Server]:                           iBMC
    #    [Date]:                             Tue, 16 Apr 2019 17:49:46 GMT
    #    [Content-Length]:                   2177

#    ${headers}=  Key Value List To Dict  ${resp.getheaders()}
#    Rprint Vars  1  headers

#    Dictionary Should Contain Sub Dictionary   ${headers}  ${header_requirements}


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

