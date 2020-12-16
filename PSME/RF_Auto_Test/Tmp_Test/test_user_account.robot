*** Settings ***
Documentation    Test Redfish user account.

Resource         ../lib/resource.robot
Resource         ../lib/bmc_redfish_resource.robot
Test Setup       Test Setup Execution
Test Teardown    Test Teardown Execution


*** Variables ***

${UserName}  admin
${Pwd}  redfish 

${RO_UserName}  readonly 
${RO_Pwd}  readonly 
${RO_ROLE}  ReadOnlyUser 

${TEST_RO_UserName}  testreadonly 
${TEST_RO_Pwd}  testreadonly 
${TEST1_RO_UserName}  test1readonlypwd 
${TEST1_RO_Pwd}  test1readonlypwd 
${RO_ROLE}  ReadOnlyUser 

${OP_UserName}  Operator 
${OP_Pwd}  Operator 
${OP_ROLE}  Operator 

${TEST_USER_BODY}  {"UserName":"U1","Password":"P1","RoleId":"ReadOnlyUser", "Enabled":true , "Locked":false}
${TEST1_USER_BODY}  {"UserName":"U2","Password":"P2","RoleId":"ReadOnlyUser", "Enabled":true , "Locked":false}
${PATCH_PWD_BODY}  { "UserName": "readonly", "Password": "readonlyU2", "Locked": false, "Enabled": true, "RoleId": "ReadOnlyUser" } 
${OP_PATCH_PWD_BODY}  { "UserName": "Operator", "Password": "OperatorU2", "Locked": false, "Enabled": true, "RoleId": "Operator" } 
${PATCH_TEST_PWD_BODY}  { "UserName": "testreadonly", "Password": "testreadonlyU2", "Locked": false, "Enabled": true, "RoleId": "ReadOnlyUser" } 


** Test Cases **

Verify AccountService Available
    [Documentation]  Verify Redfish account service is available.
    [Tags]  Verify_AccountService_Available

    Redfish.Login  ${UserName}  ${Pwd} 
    ${resp} =  Redfish_utils.Get Attribute  /redfish/v1/AccountService  ServiceEnabled
    Should Be Equal As Strings  ${resp}  ${True}

Redfish Create and Verify Users
    [Documentation]  Create Redfish users with various roles.
    [Tags]  Redfish_Create_and_Verify_Users
    [Template]  Redfish Create And Verify User

    # username    # username     # password    # role_id       #locked    # enabled 
      nameadmin    admin_user     TestPwd123  Administrator    ${False}   ${True}
      nameadmin_1  admin_user_1   TestPwd123  ReadOnlyUser     ${False}   ${True}
      nameopt_1    operator_user_2  TestPwd123  Operator       ${False}   ${True}

Verify Redfish User with Wrong Password
    [Documentation]  Verify Redfish User with Wrong Password.
    [Tags]  Verify_Redfish_User_with_Wrong_Password
    [Template]  Verify Redfish User with Wrong Password

    # username        # username     # password    # role_id         # locked    # enabled  # wrong_password
    nameadmin         admin_user_3     TestPwd123    Administrator   ${False}    ${True}    aa 
    nameadmin_1       admin_user_4     TestPwd123    ReadOnlyUser    ${False}    ${True}    bb
    nameopt_1         operator_user_5  TestPwd123    Operator        ${False}    ${True}    cc

Verify Login with Deleted Redfish Users
    [Documentation]  Verify login with deleted Redfish Users.
    [Tags]  Verify_Login_with_Deleted_Redfish_Users
    [Template]  Verify Login with Deleted Redfish User

     # username       # username     password    role_id  locked  enabled    wrong_password
     nameadmin        admin_user_6   TestPwd123  Administrator    ${False}   ${True}  aa 
     nameadmin_1      admin_user_7   TestPwd123  ReadOnlyUser     ${False}   ${True}  dd
     nameopt_1        operator_user_8  TestPwd123  Operator       ${False}   ${True}  ee

Verify User Creation Without Enabling it
    [Documentation]  Verify User Creation Without Enabling it.
    [Tags]  Verify_User_Creation_Without_Enabling_it
    [Template]  Redfish Create And Verify User
    # username        # username        password    role_id             locked     enabled
    nameadmin         admin_user_9      TestPwd123  Administrator       ${False}   ${False}
    nameadmin_1       admin_user_10     TestPwd123  ReadOnlyUser        ${False}   ${False}
    nameopt_1         operator_user_11  TestPwd123  Operator            ${False}   ${False}

#Verify Redfish User Persistence After Reboot
#    [Documentation]  Verify Redfish user persistence after reboot.
#    [Tags]  Verify_Redfish_User_Persistence_After_Reboot
#    # Create Redfish users.
#
#    Redfish.Login  ${UserName}  ${Pwd} 
#    Redfish Create User  newname_1    admin_user_12     TestPwd123  Administrator       ${False}   ${True}
#    Redfish Create User  newname_2    admin_user_13     TestPwd123  ReadOnlyUser        ${False}   ${True}
#    Redfish Create User  newname_3    operator_user_14  TestPwd123  Operator            ${False}   ${True}
#
#    # Reboot Device 
#    Redfish GracefulRestart
#
#    # Wait device ready
#    Sleep  180s
#
#    # Verify users after reboot.
#
#    Redfish.Login  ${UserName}  ${Pwd} 
#    Redfish Verify User   newname_1   admin_user_12     TestPwd123  Administrator  ${False}  ${True}
#    Redfish Verify User   newname_2   admin_user_13     TestPwd123  ReadOnlyUser   ${False}  ${True}
#    Redfish Verify User   newname_3   operator_user_14  TestPwd123  Operator       ${False}  ${True}
#
#    # Delete created users.
#    Redfish.Delete  /redfish/v1/AccountService/Accounts/admin_user_12
#    Redfish.Delete  /redfish/v1/AccountService/Accounts/admin_user_13
#    Redfish.Delete  /redfish/v1/AccountService/Accounts/operator_user_14
#    Redfish.Logout

Verify User Creation With locking it
    [Documentation]  Verify User Creation With locking it.
    [Tags]  Verify_User_Creation_With_locking_it
    [Template]  Redfish Create And Verify User With Locked

     # username        # username        password    role_id             locked     enabled
     nameadmin         admin_user_15     TestPwd123  Administrator       ${True}   ${True}
     nameadmin_1       admin_user_16     TestPwd123  ReadOnlyUser        ${True}   ${True}
     nameopt_1         operator_user_17  TestPwd123  Operator            ${True}   ${True}

Verify Create and Patch parameters  
    [Documentation]  Create Redfish users with various roles.
    [Tags]  Redfish_Create_and_Verify_Users
    [Template]  Redfish Create And Patch Parameters 

    # username    # username     # password    # role_id       #locked    # enabled 
      nameadmin    admin_user     TestPwd123  Administrator    ${False}   ${True}

#Verify AccountService_ReadOnlyUser
#    [Documentation]  Verify Redfish ReadOnlyUser account can get major service 
#    [Tags]  Verify_AccountService_UserOnlyUser_Enable
#
#    Redfish.Login  ${UserName}  ${Pwd} 
#
#    Redfish Create User  ${RO_UserName}  ${RO_Pwd}  ${RO_ROLE}  ${False}  ${True}
#
#    Redfish.Logout
#
#    Redfish.Login  ${RO_UserName}  ${RO_Pwd} 
#
#    ${resp}=  Redfish.Get   /redfish/v1/EthernetSwitches/1
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get   /redfish/v1/EthernetSwitches/1/Ports/
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get   /redfish/v1/EthernetSwitches/1/Ports/1
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/EventService/
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/EventService/Subscriptions
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/AccountService/
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/AccountService/Accounts
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/AccountService/Roles
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/AccountService/Roles/1
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/AccountService/Roles/2
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/AccountService/Roles/3
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Registries/
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Registries/1
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Chassis/
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Chassis/1
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Chassis/1/Thermal
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Chassis/1/Power
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Chassis/1/Drives/1/
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Managers
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Managers/1
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Managers/1/EthernetInterfaces
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Managers/1/EthernetInterfaces/1
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Managers/1/SerialInterfaces/1
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Systems/1
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Systems/1/Processors
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Systems/1/Processors/1
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/Systems/1/Storage/1
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/SessionService/
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    ${resp}=  Redfish.Get  /redfish/v1/SessionService/Sessions
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
#    Redfish.Login  
#    # Delete Specified User
#    Redfish.Delete  /redfish/v1/AccountService/Accounts/${RO_UserName}
#    Redfish.Logout
#
#Verify AccountService_ReadOnlyUser_Cannot_POST_PATCH
#    [Documentation]  Verify Redfish ReadOnlyUser account cannot use patch post action 
#    [Tags]  Verify_AccountService_ReadOnlyUser_CANNOT_PATCH_POST
#
#    Redfish.Login  ${UserName}  ${Pwd} 
#    Redfish Create User  ${RO_UserName}  ${RO_Pwd}  ${RO_ROLE}  ${False}  ${True}
#    Redfish Create User  ${TEST_RO_UserName}  ${TEST_RO_Pwd}  ${RO_ROLE}  ${False}  ${True}
#    Redfish.Logout
#    Redfish.Login  ${RO_UserName}  ${RO_Pwd} 
#
#    ${payload}=  Evaluate  json.loads($OP_UP)    json 
#    ${resp}=  Redfish.Patch  /redfish/v1/EthernetSwitches/1/Ports/1  body=${payload}
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_UNAUTHORIZED}
#
#    ${payload}=  Create Dictionary
#    ...  ResetType=GracefulRestart 
#    Redfish.Post  /redfish/v1/Systems/1/Actions/ComputerSystem.Reset  body=&{payload}
#    ...  valid_status_codes=[${HTTP_UNAUTHORIZED}]
#
##   ReadOnlyUser can't create account
#    Redfish Create User  ${TEST1_RO_UserName}  ${TEST1_RO_Pwd}  ${RO_ROLE}  ${False}  ${True}
#    ${role_config}=  Redfish_Utils.Get Attribute
#    ...  /redfish/v1/AccountService/Accounts/${TEST1_RO_UserName}  RoleId
#    Should Not Be Equal  ${RO_ROLE}  ${role_config}
#
##   ReadOnlyUser can't get Accounts related info. 
##   Check in upper layer
#
#    ${resp}=  Redfish.Get  /redfish/v1/AccountService/Accounts/
#    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
#
##   ReadOnlyUser can patch its own pasword
#    ${payload}=  Evaluate  json.loads($PATCH_PWD_BODY)    json 
#    Redfish.Patch  /redfish/v1/AccountService/Accounts/${RO_UserName}  body=&{payload}
#    ...  valid_status_codes=[${HTTP_OK}]
#
##   ReadOnlyUser can't patch other users pasword
#    ${payload}=  Evaluate  json.loads($PATCH_TEST_PWD_BODY)    json 
#    Redfish.Patch  /redfish/v1/AccountService/Accounts/${TEST_RO_UserName}  body=&{payload}
#    ...  valid_status_codes=[${HTTP_UNAUTHORIZED}]
#
#    Redfish.Logout
#    Redfish.Login  
#    Redfish.Delete  /redfish/v1/AccountService/Accounts/${RO_UserName}
#    Redfish.Delete  /redfish/v1/AccountService/Accounts/${TEST_RO_UserName}
#    Redfish.Logout
#
#
#Verify AccountService_OPUser_Cannot_POST_PATCH
#    [Documentation]  Verify Redfish OPUser account cannot use patch post action 
#    [Tags]  Verify_AccountService_OPUser_CANNOT_PATCH_POST
#
#    Redfish.Login  ${UserName}  ${Pwd} 
#    Redfish Create User  ${OP_UserName}  ${OP_Pwd}  ${OP_ROLE}  ${False}  ${True}
#    Redfish Create User  ${TEST_RO_UserName}  ${TEST_RO_Pwd}  ${RO_ROLE}  ${False}  ${True}
#    Redfish.Logout
#    Redfish.Login  ${OP_UserName}  ${OP_Pwd} 
#
##   OPUser can't create account
#    Redfish Create User  ${TEST1_RO_UserName}  ${TEST1_RO_Pwd}  ${RO_ROLE}  ${False}  ${True}
#    ${role_config}=  Redfish_Utils.Get Attribute
#    ...  /redfish/v1/AccountService/Accounts/${TEST1_RO_UserName}  RoleId
#    Should Not Be Equal  ${RO_ROLE}  ${role_config}
#
##   OPUser can patch its own pasword
#    ${payload}=  Evaluate  json.loads($OP_PATCH_PWD_BODY)    json 
#    Redfish.Patch  /redfish/v1/AccountService/Accounts/${OP_UserName}  body=&{payload}
#    ...  valid_status_codes=[${HTTP_OK}]
#
##   ReadOnlyUser can't patch other users pasword
#    ${payload}=  Evaluate  json.loads($PATCH_TEST_PWD_BODY)    json 
#    Redfish.Patch  /redfish/v1/AccountService/Accounts/${TEST_RO_UserName}  body=&{payload}
#    ...  valid_status_codes=[${HTTP_UNAUTHORIZED}]
#
#
#    # Op can reset device
#    ${payload}=  Create Dictionary
#    ...  ResetType=GracefulRestart 
#    Redfish.Post  /redfish/v1/Systems/1/Actions/ComputerSystem.Reset  body=&{payload}
#    ...  valid_status_codes=[${HTTP_OK}]
#    Sleep  180s
#    # Wait device ready
#    
##    Redfish.Logout
#    Redfish.Login   
#    Redfish.Delete  /redfish/v1/AccountService/Accounts/${OP_UserName}
#    Redfish.Delete  /redfish/v1/AccountService/Accounts/${TEST_RO_UserName}
#    Redfish.Logout

*** Keywords ***

Test Setup Execution
    [Documentation]  Do test case setup tasks.
    Redfish.Login

Test Teardown Execution
    [Documentation]  Do the post test teardown.
    Redfish.Logout


Redfish GracefulRestart 
    [Documentation]  Redfish GracefulRestart 

    # Create specified user.
    ${payload}=  Create Dictionary
    ...  ResetType=GracefulRestart 
    Redfish.Post  /redfish/v1/Systems/1/Actions/ComputerSystem.Reset  body=&{payload}
    Sleep  1s
    ...  valid_status_codes=[${HTTP_OK}]


Redfish Enable SessionService 
    [Documentation]  Redfish enable sesssion service 
    [Arguments]   ${enabled}

    # Create specified user.
    ${payload}=  Create Dictionary
    ...  ServiceEnabled=${enabled}  SessionTimeout=${600}
    Redfish.Post  /redfish/v1/SessionService/  body=&{payload}
    ...  valid_status_codes=[${HTTP_OK}]

Redfish Create User
    [Documentation]  Redfish create user.
    [Arguments]   ${name}  ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    # Description of argument(s):
    # name                The name to be created.
    # username            The username to be created.
    # password            The password to be assigned.
    # role_id             The role id of the user to be created
    #                     (e.g. "Administrator", "Operator", etc.).
    # locked              should be enabled (${True}, ${False}).
    # enabled             Indicates whether the username being created
    #                     should be enabled (${True}, ${False}).

    # Create specified user.
    ${payload}=  Create Dictionary
    ...  Name=${name}  UserName=${username}  Password=${password}  RoleId=${role_id}  Locked=${locked}  Enabled=${enabled}
    Redfish.Post  /redfish/v1/AccountService/Accounts  body=&{payload}
    ...  valid_status_codes=[${HTTP_CREATED}]


Redfish Verify User
    [Documentation]  Redfish user verification.
    [Arguments]   ${name}  ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    # Description of argument(s):
    # name                The name to be created.
    # username            The username to be created.
    # password            The password to be assigned.
    # role_id             The role id of the user to be created
    # locked              should be enabled (${True}, ${False}).
    #                     (e.g. "Administrator", "Operator", etc.).

    # Trying todo a login with created user
    ${is_redfish}=  Run Keyword And Return Status  Redfish.Login  ${username}  ${password}

    # Doing a check of the rerurned status
    Should Be Equal  ${is_redfish}  ${enabled}

    # We do not needed to login with created user (user could be in disabled status)
     Redfish.Login

    # Validate Role Id of created user.
    ${role_config}=  Redfish_Utils.Get Attribute
    ...  /redfish/v1/AccountService/Accounts/${username}  RoleId
    Should Be Equal  ${role_id}  ${role_config}


Redfish Verify User With Locked
    [Documentation]  Redfish user verification with locked.
    [Arguments]   ${name}     ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    # Description of argument(s):
    # name                The name to be created.
    # username            The username to be created.
    # password            The password to be assigned.
    # role_id             The role id of the user to be created
    # locked              should be enabled (${True}, ${False}).
    #                     (e.g. "Administrator", "Operator", etc.).

    # Trying todo a login with created user
    ${is_redfish}=  Run Keyword And Return Status  Redfish.Login  ${username}  ${password}

    # Doing a check of the rerurned status
    Should Be Equal  ${is_redfish}  ${False}


Redfish Create And Verify User
    [Documentation]  Redfish create and verify user.
    [Arguments]   ${name}  ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    # Description of argument(s):
    # name                The name to be created.
    # username            The username to be created.
    # password            The password to be assigned.
    # role_id             The role id of the user to be created
    #                     (e.g. "Administrator", "Operator", etc.).
    # locked              should be enabled (${True}, ${False}).
    # enabled             Indicates whether the username being created
    #                     should be enabled (${True}, ${False}).

    Redfish Create User  ${name}  ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    Redfish Verify User  ${name}  ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    # Delete Specified User
    Redfish.Delete  /redfish/v1/AccountService/Accounts/${username}

Redfish Create And Verify User With Locked
    [Documentation]  Redfish create and verify user with locked.
    [Arguments]   ${name}     ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    # Description of argument(s):
    # name                The name to be created.
    # username            The username to be created.
    # password            The password to be assigned.
    # role_id             The role id of the user to be created
    #                     (e.g. "Administrator", "Operator", etc.).
    # locked              should be enabled (${True}, ${False}).
    # enabled             Indicates whether the username being created
    #                     should be enabled (${True}, ${False}).

    Redfish Create User  ${name}    ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    Redfish Verify User With Locked  ${name}  ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    # Delete Specified User
    Redfish.Delete  /redfish/v1/AccountService/Accounts/${username}

Verify Redfish User with Wrong Password
    [Documentation]  Verify Redfish User with Wrong Password.
    [Arguments]   ${name}   ${username}  ${password}  ${role_id}  ${locked}  ${enabled}  ${wrong_password}

    # Description of argument(s):
    # name                The name to be created.
    # username            The username to be created.
    # password            The password to be assigned.
    # role_id             The role id of the user to be created
    #                     (e.g. "Administrator", "Operator", etc.).
    # locked              should be enabled (${True}, ${False}).
    # enabled             Indicates whether the username being created
    #                     should be enabled (${True}, ${False}).
    # wrong_password      Any invalid password.

    Redfish Create User   ${name}   ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    # Attempt to login with created user with invalid password.
    ${msg}=  Run Keyword And Expect Error  *
    ...  Redfish.Login  ${username}  ${wrong_password}

    # Delete newly created user.
    Redfish.Delete  /redfish/v1/AccountService/Accounts/${username}


Verify Login with Deleted Redfish User
    [Documentation]  Verify Login with Deleted Redfish User.
    [Arguments]   ${name}   ${username}  ${password}  ${role_id}  ${locked}  ${enabled}  ${wrong_password}

    # Description of argument(s):
    # name                The name to be created.
    # username            The username to be created.
    # password            The password to be assigned.
    # role_id             The role id of the user to be created
    #                     (e.g. "Administrator", "Operator", etc.).
    # locked              should be enabled (${True}, ${False}).
    # enabled             Indicates whether the username being created
    #                     should be enabled (${True}, ${False}).

    Redfish Create User  ${name}  ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    # Delete newly created user.
    Redfish.Delete  /redfish/v1/AccountService/Accounts/${username}

    ${is_redfish}=  Run Keyword And Return Status  Redfish.Login  ${username}  ${password}

    # Doing a check of the rerurned status
    Should Be Equal  ${is_redfish}  ${False}
    Redfish.Login

Redfish Create And Patch Parameters 
    [Documentation]  Redfish create and verify user.
    [Arguments]   ${name}  ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    # Description of argument(s):
    # name                The name to be created.
    # username            The username to be created.
    # password            The password to be assigned.
    # role_id             The role id of the user to be created
    #                     (e.g. "Administrator", "Operator", etc.).
    # locked              should be enabled (${True}, ${False}).
    # enabled             Indicates whether the username being created
    #                     should be enabled (${True}, ${False}).

    # Create specified user.
    Redfish Create User  ${name}  ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    # Modify parameters
    ${payload}=  Create Dictionary
    ...  Name=TESTNAME
    Redfish.Patch  /redfish/v1/AccountService/Accounts/${username}  body=&{payload}
    ...  valid_status_codes=[${HTTP_OK}]
    ${MO_NAME}=  Redfish_Utils.Get Attribute
    ...  /redfish/v1/AccountService/Accounts/${username}  Name 
    Should Be Equal  ${MO_NAME}  TESTNAME

    ${payload}=  Create Dictionary
    ...  UserName=TESTUSERNAME
    Redfish.Patch  /redfish/v1/AccountService/Accounts/${username}  body=&{payload}
    ...  valid_status_codes=[${HTTP_OK}]
    ${MO_USERNAME}=  Redfish_Utils.Get Attribute
    ...  /redfish/v1/AccountService/Accounts/TESTUSERNAME  UserName 
    Should Be Equal  ${MO_USERNAME}  TESTUSERNAME

    ${payload}=  Create Dictionary
    ...  RoleId=ReadOnlyUser
    Redfish.Patch  /redfish/v1/AccountService/Accounts/TESTUSERNAME  body=&{payload}
    ...  valid_status_codes=[${HTTP_OK}]
    ${MO_ROLEID}=  Redfish_Utils.Get Attribute
    ...  /redfish/v1/AccountService/Accounts/TESTUSERNAME  RoleId 
    Should Be Equal  ${MO_RoleId}  ReadOnlyUser 

    ${payload}=  Create Dictionary
    ...  RoleId=Operator
    Redfish.Patch  /redfish/v1/AccountService/Accounts/TESTUSERNAME  body=&{payload}
    ...  valid_status_codes=[${HTTP_OK}]
    ${MO_ROLEID}=  Redfish_Utils.Get Attribute
    ...  /redfish/v1/AccountService/Accounts/TESTUSERNAME  RoleId 
    Should Be Equal  ${MO_RoleId}  Operator 

    ${payload}=  Create Dictionary
    ...  Name=${name}  UserName=${username}  Password=${password}  RoleId=${role_id}  Locked=${locked}  Enabled=${enabled}
    Redfish.Patch  /redfish/v1/AccountService/Accounts/TESTUSERNAME  body=&{payload}
    ...  valid_status_codes=[${HTTP_OK}]

    # Delete Specified User
    Redfish.Delete  /redfish/v1/AccountService/Accounts/${username}
