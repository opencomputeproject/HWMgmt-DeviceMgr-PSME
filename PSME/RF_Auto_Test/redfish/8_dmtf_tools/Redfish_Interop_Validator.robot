*** Settings ***
Documentation      Test PSME using https://github.com/DMTF/Redfish-Interop-Validator.git 
...                DMTF tool.

Resource         ../../lib/resource.robot
Resource         ../../lib/bmc_redfish_resource.robot

Library            OperatingSystem
Resource           ../../lib/dmtf_tools_utils.robot

*** Variables ***

${DEFAULT_PYTHON}  python3
${rsv_dir_path}    Redfish-Interop-Validator 
${rsv_github_url}  https://github.com/DMTF/Redfish-Interop-Validator.git 
${command_string}  ${DEFAULT_PYTHON} ${rsv_dir_path}${/}RedfishInteropValidator.py
...                --ip ${OPENBMC_HOST} --nochkcert --forceauth -u ${OPENBMC_USERNAME}
...                -p ${OPENBMC_PASSWORD} --authtype Basic --logdir ${EXECDIR}${/}logs${/} --debug_logging OCP-Profiles/OCPBaselineHardwareManagement.v1_0_1.json 

*** Test Case ***

Test Redfish Using Redfish Service Validator
    [Documentation]  Check conformance with a Redfish service interface.
    [Tags]  Test_Redfish_Using_Redfish_Service_Validator

    Redfish.Login  admin  redfish 

#    ${payload}=  Create Dictionary
#    ...  ServiceEnabled=${false}  SessionTimeout=${600}
#    Redfish.Post  /redfish/v1/SessionService/  body=&{payload}
#    ...  valid_status_codes=[${HTTP_OK}]

#    Download DMTF Tool  ${rsv_dir_path}  ${rsv_github_url}  "master"
    Download DMTF Tool  ${rsv_dir_path}  ${rsv_github_url}  "1.1.8"

    ${output}=  Run DMTF Tool  ${rsv_dir_path}  ${command_string}

    Redfish Service Validator Result  ${output}
#    # Last one test , disable session authentication
    ${payload}=  Create Dictionary
    ...  ServiceEnabled=${true}  SessionTimeout=${600}
    Redfish.Post  /redfish/v1/SessionService/  body=&{payload}
    ...  valid_status_codes=[${HTTP_OK}]
