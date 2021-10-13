## RSD Redfish PSME Test Automation ##

* Reference from [OpenBMC Test Automation Redfish part](https://github.com/openbmc/openbmc-test-automation)

**Interface Feature List**
* REST
* DMTF Redfish

## Installation Setup Guide ##

* Under Ubuntu 16.04lts version.

* [Robot Framework Install Instruction](https://github.com/robotframework/robotframework/blob/master/INSTALL.rst)

* Pre-install
Packages required to be installed for RF test Automation.
Install the packages and it's dependencies via `pip3`

    ```
    $ sudo apt-get install python3-pip
    $ sudo pip3 install robotframework
    $ sudo pip3 install -U requests
    $ sudo pip3 install -U robotframework-requests
    $ sudo pip3 install -U robotframework-httplibrary
    $ sudo pip3 install redfish
    $ sudo pip3 install robotframework-sshlibrary
    $ sudo pip3 install robotframework-scplibrary
    $ sudo pip3 install beautifulsoup4==4.6.0
    $ sudo pip3 install jsonschema
    $ sudo pip3 install lxml
    ```

## Test item ##

`redfish/`:  Test cases for DMTF Redfish-related feature supported.

`redfish_eit/`: Test cases for ECRF_PAL EIT test.

* How to run auto test:

	Preparation:


    All test:
    
    These test based on some BASELINE service defined by OCP .
    
    Start up PSME on device first and then
    
    ```
    $ ./all.sh <DUT_IP> <Listener_IP> <PSME_Version>
    ```

    Ex.
   
    For SONiC platform : Port number is 8889

    ```
    $ ./all.sh 172.17.8.5:8889 172.17.8.10:8889 2.1.3.59.25 sonic
    ```

    For ONL platform : Port number is 8888

    ```
    $ ./all.sh 172.17.8.5:8888 172.17.8.10:8888 2.1.3.59.25
    ```

    NOTE:

    Listener_IP : No function now, You can type any ip address.

    PSM_Version : You can retrive this information from ```~/NOS/OpenNetwokLinux/Redfish/PSME/build/install/allinone-deb/bin/psme_allinone_baseline2.1.3.59.25-a529d00_amd64.deb
    ```
    And The **2.1.3.59.25** is the version of PSME
 
