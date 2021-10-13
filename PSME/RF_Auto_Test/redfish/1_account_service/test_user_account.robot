*** Settings ***
Documentation    Test Redfish user account.

Resource         ../../lib/resource.robot
Resource         ../../lib/bmc_redfish_resource.robot
#Test Setup       Test Setup Execution
#Test Teardown    Test Teardown Execution
*** Variables ***

${UserName}  admin
${Pwd}  redfish 

${RO_UserName}  readonly 
${RO_Pwd}  readonly 
${RO_ROLE}  ReadOnly

${TEST_RO_UserName}  testreadonly 
${TEST_RO_Pwd}  testreadonly 
${TEST1_RO_UserName}  test1readonlypwd 
${TEST1_RO_Pwd}  test1readonlypwd 
${RO_ROLE}  ReadOnly 
${PATCH_NEW_NAME}  NEWUSERNAME 

${OP_UserName}  Operator 
${OP_Pwd}  Operator 
${OP_ROLE}  Operator 

${TEST_USER_BODY}  {"UserName":"U1","Password":"P1","RoleId":"ReadOnly", "Enabled":true , "Locked":false}
${TEST1_USER_BODY}  {"UserName":"U2","Password":"P2","RoleId":"ReadOnly", "Enabled":true , "Locked":false}
${PATCH_PWD_BODY}  { "UserName": "readonly", "Password": "readonlyU2", "Locked": false, "Enabled": true, "RoleId": "ReadOnly" } 
${OP_PATCH_PWD_BODY}  { "UserName": "Operator", "Password": "OperatorU2", "Locked": false, "Enabled": true, "RoleId": "Operator" } 
${PATCH_TEST_PWD_BODY}  { "UserName": "testreadonly", "Password": "testreadonlyU2", "Locked": false, "Enabled": true, "RoleId": "ReadOnly" } 

** Test Cases **

Verify AccountService Available
    [Documentation]  Verify Redfish account service is available.
    [Tags]  Verify_AccountService_Available

    Redfish.Login  
    ${resp} =  Redfish_utils.Get Attribute  /redfish/v1/AccountService  ServiceEnabled
    Should Be Equal As Strings  ${resp}  ${True}

Verify AccountService_session_timeout
    [Documentation]  Verify Redfish session timeout 
    [Tags]  Verify_AccountService_session_timeout

    Redfish.Login  

    ${payload}=  Create Dictionary
    ...  ServiceEnabled=${True}  SessionTimeout=${6}
    ${resp}=  Redfish.Post  /redfish/v1/SessionService/  body=&{payload}
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    Sleep  10s

    ${resp}=  Redfish.Get  /redfish/v1/Chassis/
    Should Be Equal As Strings  ${resp.status}  ${HTTP_UNAUTHORIZED}

    Redfish.Login  
    ${payload}=  Create Dictionary
    ...  ServiceEnabled=${True}  SessionTimeout=${60}
    ${resp}=  Redfish.Post  /redfish/v1/SessionService/  body=&{payload}
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}
    Redfish.Logout  

Redfish Create and Verify Users
    [Documentation]  Create Redfish enabled users with various roles.
    [Tags]  Redfish_Create_and_Verify_Enabled_Users
    [Template]  Redfish Create And Verify Enabled User

    # username    # username     # password    # role_id       #locked    # enabled 
      nameadmin    admin_user     TestPwd123  Administrator    ${False}   ${True}
      nameadmin_1  admin_user_1   TestPwd123  ReadOnly     ${False}   ${True}
      nameopt_1    operator_user_2  TestPwd123  Operator       ${False}   ${True}

Verify Create Redfish User with Wrong Password
    [Documentation]  Verify Create Redfish User with Wrong Password.
    [Tags]  Verify_Redfish_User_with_Wrong_Password
    [Template]  Verify Create Redfish User with Wrong Password

    # username        # username     # password    # role_id         # locked    # enabled  # wrong_password
    nameadmin         admin_user_3     TestPwd123    Administrator   ${False}    ${True}    aa 
    nameadmin_1       admin_user_4     TestPwd123    ReadOnly    ${False}    ${True}    bb
    nameopt_1         operator_user_5  TestPwd123    Operator        ${False}    ${True}    cc

Verify Login with Deleted Redfish Users
    [Documentation]  Verify login with deleted Redfish Users.
    [Tags]  Verify_Login_with_Deleted_Redfish_Users
    [Template]  Verify Login with Deleted Redfish User

     # username       # username     password    role_id  locked  enabled
     nameadmin        admin_user_6   TestPwd123  Administrator    ${False}   ${True}
     nameadmin_1      admin_user_7   TestPwd123  ReadOnly     ${False}   ${True}
     nameopt_1        operator_user_8  TestPwd123  Operator       ${False}   ${True}

Verify User Creation Without Enabling it
    [Documentation]  Verify User Creation Without Enabling it.
    [Tags]  Verify_User_Creation_Without_Enabling_it
    [Template]  Redfish Create And Verify Disabled User
    # username        # username        password    role_id             locked     enabled
    nameadmin         admin_user_9      TestPwd123  Administrator       ${False}   ${False}
    nameadmin_1       admin_user_10     TestPwd123  ReadOnly        ${False}   ${False}
    nameopt_1         operator_user_11  TestPwd123  Operator            ${False}   ${False}

Verify Create and Patch parameters  
    [Documentation]  Create Redfish users with various roles.
    [Tags]  Redfish_Create_and_Verify_Users
    [Template]  Redfish Create And Patch Parameters 

    # username    # username     # password    # role_id       #locked    # enabled 
      nameadmin    admin_user     TestPwd123  Administrator    ${False}   ${True}

Verify AccountService_ReadOnly
    [Documentation]  Verify Redfish ReadOnly account can get major service 
    [Tags]  Verify_AccountService_UserOnlyUser_Enable

    Redfish.Login  ${UserName}  ${Pwd} 

    Redfish Create User  NewName  ${RO_UserName}  ${RO_Pwd}  ${RO_ROLE}  ${False}  ${True}
    Sleep  1s
    Redfish.Logout
    Redfish.Login  ${RO_UserName}  ${RO_Pwd} 
    Sleep  1s

    ${resp}=  Redfish.Get   /redfish/v1/
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get   /redfish/
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get   /
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get   /redfish/v1/EthernetSwitches/1
    Should Be Equal As Strings  ${resp.status}  ${HTTP_NOT_FOUND}

    ${resp}=  Redfish.Get   /redfish/v1/EthernetSwitches/1/Ports/
    Should Be Equal As Strings  ${resp.status}  ${HTTP_NOT_FOUND}

    ${resp}=  Redfish.Get   /redfish/v1/EthernetSwitches/1/Ports/1
    Should Be Equal As Strings  ${resp.status}  ${HTTP_NOT_FOUND}

    ${resp}=  Redfish.Get  /redfish/v1/EventService/
    Should Be Equal As Strings  ${resp.status}  ${HTTP_NOT_FOUND}

    ${resp}=  Redfish.Get  /redfish/v1/EventService/Subscriptions
    Should Be Equal As Strings  ${resp.status}  ${HTTP_NOT_FOUND}

    ${resp}=  Redfish.Get  /redfish/v1/AccountService/
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/AccountService/Accounts
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/AccountService/Roles
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/AccountService/Roles/Administrator
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/AccountService/Roles/ReadOnly
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/AccountService/Roles/Operator
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/Registries/
    Should Be Equal As Strings  ${resp.status}  ${HTTP_NOT_FOUND}

    ${resp}=  Redfish.Get  /redfish/v1/Registries/1
    Should Be Equal As Strings  ${resp.status}  ${HTTP_NOT_FOUND}

    ${resp}=  Redfish.Get  /redfish/v1/Chassis/
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/Chassis/1
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/Chassis/1/Thermal
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/Chassis/1/Power
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/Chassis/1/Drives/1/
    Should Be Equal As Strings  ${resp.status}  ${HTTP_NOT_FOUND}

    ${resp}=  Redfish.Get  /redfish/v1/Managers
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/Managers/1
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/Managers/1/EthernetInterfaces
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/Managers/1/EthernetInterfaces/1
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/Managers/1/SerialInterfaces/1
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/Systems/1
    Should Be Equal As Strings  ${resp.status}  ${HTTP_NOT_FOUND}

    ${resp}=  Redfish.Get  /redfish/v1/Systems/1/Processors
    Should Be Equal As Strings  ${resp.status}  ${HTTP_NOT_FOUND}

    ${resp}=  Redfish.Get  /redfish/v1/Systems/1/Processors/1
    Should Be Equal As Strings  ${resp.status}  ${HTTP_NOT_FOUND}

    ${resp}=  Redfish.Get  /redfish/v1/Systems/1/Storage/1
    Should Be Equal As Strings  ${resp.status}  ${HTTP_NOT_FOUND}

    ${resp}=  Redfish.Get  /redfish/v1/SessionService/
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${resp}=  Redfish.Get  /redfish/v1/SessionService/Sessions
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    Redfish.Logout
    Redfish.Login  
    # Delete Specified User
    Redfish.Delete  /redfish/v1/AccountService/Accounts/${RO_UserName}
    Redfish.Logout

Verify AccountService_ReadOnly_Cannot_POST_PATCH
    [Documentation]  Verify Redfish ReadOnly account cannot use patch post action 
    [Tags]  Verify_AccountService_ReadOnly_CANNOT_PATCH_POST

    Redfish.Login  ${UserName}  ${Pwd} 
    Redfish Create User  NewName  ${RO_UserName}  ${RO_Pwd}  ${RO_ROLE}  ${False}  ${True}
    Redfish.Logout
    Redfish.Login  ${RO_UserName}  ${RO_Pwd} 

    ${payload}=  Create Dictionary
    ...  ResetType=GracefulRestart 
    ${resp}=  Redfish.Post  /redfish/v1/Systems/1/Actions/ComputerSystem.Reset  body=&{payload}
    Should Be Equal As Strings  ${resp.status}  ${HTTP_NOT_FOUND}

    Redfish.Logout
    Redfish.Login  
    # Delete Specified User
    Redfish.Delete  /redfish/v1/AccountService/Accounts/${RO_UserName}
    Redfish.Logout

*** Keywords ***

Redfish Enable SessionService 
    [Documentation]  Redfish enable sesssion service 
    [Arguments]   ${enabled}

    # Create specified user.
    ${payload}=  Create Dictionary
    ...  ServiceEnabled=${enabled}  SessionTimeout=${600}
    Redfish.Post  /redfish/v1/SessionService/  body=&{payload}
    ...  valid_status_codes=[${HTTP_OK}]

    ${payload}=  Create Dictionary
    ...  SessionTimeout=${700}
    Redfish.Patch  /redfish/v1/SessionService/  body=&{payload}
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


Redfish Verify Enabled User
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


    # Validate Role Id of created user.
    ${role_config}=  Redfish_Utils.Get Attribute
    ...  /redfish/v1/AccountService/Accounts/${username}  RoleId
    Should Be Equal  ${role_id}  ${role_config}

    Redfish.Logout 

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


Redfish Create And Verify Enabled User
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

    Redfish.Login
    Redfish Create User  ${name}  ${username}  ${password}  ${role_id}  ${locked}  ${enabled}
    Redfish.Logout

    ${is_redfish}=  Run Keyword And Return Status  Redfish.Login  ${username}  ${password}

    # Doing a check of the rerurned status
    Should Be Equal  ${is_redfish}  ${enabled}

    # Validate Role Id of created user.
    ${role_config}=  Redfish_Utils.Get Attribute
    ...  /redfish/v1/AccountService/Accounts/${username}  RoleId
    Should Be Equal  ${role_id}  ${role_config}

    Redfish.Logout

    # Delete Specified User
    Redfish.Login
    Redfish.Delete  /redfish/v1/AccountService/Accounts/${username}
    Redfish.Logout

Redfish Create And Verify Disabled User
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

    Redfish.Login
    Redfish Create User  ${name}  ${username}  ${password}  ${role_id}  ${locked}  ${enabled}
    Redfish.Logout

    ${msg}=  Run Keyword And Expect Error  *
    ...  Redfish.Login  ${username}  ${password}

    # Delete Specified User
    Redfish.Login
    Redfish.Delete  /redfish/v1/AccountService/Accounts/${username}
    Redfish.Logout

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

Verify Create Redfish User with Wrong Password
    [Documentation]  Verify Create Redfish User with Wrong Password.
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

    Redfish.Login
    Redfish Create User   ${name}   ${username}  ${password}  ${role_id}  ${locked}  ${enabled}
    Redfish.Logout

    # Attempt to login with created user with invalid password.
    ${msg}=  Run Keyword And Expect Error  *
    ...  Redfish.Login  ${username}  ${wrong_password}

    # Delete newly created user.
    Redfish.Login
    Redfish.Delete  /redfish/v1/AccountService/Accounts/${username}
    Redfish.Logout


Verify Login with Deleted Redfish User
    [Documentation]  Verify Login with Deleted Redfish User.
    [Arguments]   ${name}   ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    # Description of argument(s):
    # name                The name to be created.
    # username            The username to be created.
    # password            The password to be assigned.
    # role_id             The role id of the user to be created
    #                     (e.g. "Administrator", "Operator", etc.).
    # locked              should be enabled (${True}, ${False}).
    # enabled             Indicates whether the username being created
    #                     should be enabled (${True}, ${False}).

    Redfish.Login
    Redfish Create User  ${name}  ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    # Delete newly created user.
    Redfish.Delete  /redfish/v1/AccountService/Accounts/${username}
    Redfish.Logout

    ${is_redfish}=  Run Keyword And Return Status  Redfish.Login  ${username}  ${password}

    # Doing a check of the rerurned status
    Should Be Equal  ${is_redfish}  ${False}
    Redfish.Logout

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
    Redfish.Login
    Redfish Create User  ${name}  ${username}  ${password}  ${role_id}  ${locked}  ${enabled}

    ${payload}=  Create Dictionary
    ...  UserName=TESTUSERNAME
    ${resp}=  Redfish.Patch  /redfish/v1/AccountService/Accounts/${username}  body=&{payload}
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${MO_USERNAME}=  Redfish_Utils.Get Attribute
    ...  /redfish/v1/AccountService/Accounts/TESTUSERNAME  UserName 
    Should Be Equal  ${MO_USERNAME}  TESTUSERNAME

    ${payload}=  Create Dictionary
    ...  RoleId=ReadOnly
    ${resp}=  Redfish.Patch  /redfish/v1/AccountService/Accounts/TESTUSERNAME  body=&{payload}
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${MO_ROLEID}=  Redfish_Utils.Get Attribute
    ...  /redfish/v1/AccountService/Accounts/TESTUSERNAME  RoleId 
    Should Be Equal  ${MO_RoleId}  ReadOnly 

    ${payload}=  Create Dictionary
    ...  RoleId=Operator
    ${resp}=  Redfish.Patch  /redfish/v1/AccountService/Accounts/TESTUSERNAME  body=&{payload}
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    ${MO_ROLEID}=  Redfish_Utils.Get Attribute
    ...  /redfish/v1/AccountService/Accounts/TESTUSERNAME  RoleId 
    Should Be Equal  ${MO_RoleId}  Operator 

    ${payload}=  Create Dictionary
    ...  Name=${name}  UserName=${username}  Password=${password}  RoleId=${role_id}  Locked=${locked}  Enabled=${enabled}
    ${resp}=  Redfish.Patch  /redfish/v1/AccountService/Accounts/TESTUSERNAME  body=&{payload}
    Should Be Equal As Strings  ${resp.status}  ${HTTP_OK}

    # Delete Specified User
    Redfish.Delete  /redfish/v1/AccountService/Accounts/${username}
    Redfish.Logout
