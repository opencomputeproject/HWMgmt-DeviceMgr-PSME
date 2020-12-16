*** Settings ***
Documentation      Test PSME using https://github.com/DMTF/Redfish-Service-Validator.
...                DMTF tool.

Resource         ../../lib/resource.robot
Resource         ../../lib/bmc_redfish_resource.robot

Library            OperatingSystem
Resource           ../../lib/dmtf_tools_utils.robot

*** Variables ***

${DEFAULT_PYTHON}  python3
${rsv_dir_path}    Redfish-Service-Validator
${rsv_github_url}  https://github.com/DMTF/Redfish-Service-Validator.git
${command_string}  ${DEFAULT_PYTHON} ${rsv_dir_path}${/}RedfishServiceValidator.py
...                --ip ${OPENBMC_HOST} --nochkcert --forceauth -u ${OPENBMC_USERNAME}
...                -p ${OPENBMC_PASSWORD} --logdir ${EXECDIR}${/}logs${/} --debug_logging

*** Test Case ***

Test Redfish Using Redfish Service Validator
    [Documentation]  Check conformance with a Redfish service interface.
    [Tags]  Test_Redfish_Using_Redfish_Service_Validator

    Redfish.Login  admin  redfish 

    ${payload}=  Create Dictionary
    ...  ServiceEnabled=${false}  SessionTimeout=${600}
    Redfish.Post  /redfish/v1/SessionService/  body=&{payload}
    ...  valid_status_codes=[${HTTP_OK}]

#    Download DMTF Tool  ${rsv_dir_path}  ${rsv_github_url}  "1.3.4"
    Download DMTF Tool  ${rsv_dir_path}  ${rsv_github_url}  "master"

    ${output}=  Run DMTF Tool  ${rsv_dir_path}  ${command_string}

    Redfish Service Validator Result  ${output}
# Last one test , disable session authentication
    ${payload}=  Create Dictionary
    ...  ServiceEnabled=${false}  SessionTimeout=${600}
    Redfish.Post  /redfish/v1/SessionService/  body=&{payload}
    ...  valid_status_codes=[${HTTP_OK}]
