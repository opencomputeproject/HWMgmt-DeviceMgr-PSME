# Redfish for Edgecore bare-metal Switch/vOLT

Repository source provide Redfish API on Edgecore Networks' Switch/vOLT with a choice of open software of [NOS](https://github.com/opencomputeproject/OpenNetworkLinux).
Framework modified and based on Intel Rack Scale Design Software. 
For more details on Intel Rack Scale Design, please visit [Official Intel Rack Scale Design Project Website](http://intel.com/intelRSD).

# Intel Rack Scale Design Reference Software

Intel Rack Scale Design Software is a logical architecture that disaggregates compute, storage, and network resources, 
and introduces the ability to more efficiently pool and utilize these resources. 
Intel Rack Scale Design Software APIs simplify resource management and provide the ability to dynamically compose resources 
based on workload-specific demands.

More detailed information can be found at [official Intel Rack Scale Design Site](http://intel.com/intelRSD).

## How To Build PSME Redfish Service for Edgecore bare-metal devices

### Build NOS OpenNetworkLinux -

 Require host PC OS Ubuntu 18.04 server with container supported environment.
  
 How To Install Docker on Ubuntu 18.04
```  
$ sudo apt install docker.io
$ sudo systemctl start docker
$ sudo systemctl enable docker
$ sudo apt install python

```  
 How To Build ONL in Docker on Ubuntu 18.04
```    
$ cd ~
$ mkdir NOS
$ cd NOS
$ git clone https://github.com/opencomputeproject/OpenNetworkLinux.git 
$ cd OpenNetworkLinux
$ git checkout f0bcb230 

```  
  If switch use NOS ONL(Debian 8 Jessie)
``` 
$ sudo docker/tools/onlbuilder -8 
$ apt-cacher-ng
$ source setup.env
$ sed -i "s/wget \$(K_ARCHIVE_URL)/wget --no-check-certificate \$(K_ARCHIVE_URL)/g"  make/kbuild.mk
$ make amd64  2>&1 | tee  onl.log   

```  
  Or if switch use ecSONiC (Debian 10 Buster)
``` 

$ sudo docker/tools/onlbuilder -10
$ apt update -y
$ export DEBIAN_FRONTEND=noninteractive
$ apt install apt-cacher-ng -yq
$ apt-cacher-ng
$ source setup.env
$ cd packages/platforms/
$ ls  | grep -v "Makefile\|accton" | xargs rm -rf
$ cd ../../
$ apt install ca-certificates -y
$ apt-get install libgnutls30 -y
$ make amd64  2>&1 | tee  onl.log  

```    
### Build Redfish -

 Git clone Redfish source code.
  
If use OCP baseline git repo.
```   
$ cd ~/NOS/OpenNetworkLinux
$ mkdir HWMgmt-DeviceMgr-PSME
$ git clone https://github.com/opencomputeproject/HWMgmt-DeviceMgr-PSME.git ./HWMgmt-DeviceMgr-PSME
$ cd HWMgmt-DeviceMgr-PSME/PSME/build/
```  

If use Edge-core internal baseline git repo.
```   
$ cd ~/NOS/OpenNetworkLinux
$ mkdir Redfish
$ git clone https://edge-core-Redfish.git ./Redfish
$ cd Redfish/PSME/build/
``` 

 Install necessary packages only at first time and start build
```  
$ ./pre-inst-X86-pkgs.sh
```  
 Build all
  
```  
$ ./Build_all.sh
``` 
 Output file will be in ~/NOS/OpenNetworkLinux/HWMgmt-DeviceMgr-PSME/PSME/build/install/allinone-deb/bin/psme-allinone.deb

 Clean Build
``` 
$ ./Build_all.sh C
$ ./Build_all.sh
``` 

## Installing
cp psme-allinone.deb to Switch or vOLT ~/ directory and
```
dpkg -i psme-allinone.deb
```
## Start PSME Redfish service

```
$ service psme start
```

## Start PSME at boot time
```   
$ update-rc.d psme defaults    
```
And you can start to use PSME Redfish Service now.


## Do basic API query.

On Switch/vOLT devices side:

Assume DUT management IP is 172.17.10.9

On client side:

Use curl command to get basic information.

ex. For ONL platform (Port : 8888)
```
curl --insecure -v https://172.17.10.9:8888/redfish/v1/ | json_pp
```

ex. For ecSONiC platform (Port : 8889)
```
curl --insecure -v https://172.17.10.9:8889/redfish/v1/ | json_pp
```

Then you can get json data respones like:
```
{

   "Description" : "Service Root description",
   "Managers" : {
      "@odata.id" : "/redfish/v1/Managers"
   },
   "Links" : {
      "Sessions" : {
         "@odata.id" : "/redfish/v1/SessionService/Sessions"
      }
   },
   "Chassis" : {
      "@odata.id" : "/redfish/v1/Chassis"
   },
   "AccountService" : {
      "@odata.id" : "/redfish/v1/AccountService"
   },
   "@odata.context" : "/redfish/v1/$metadata#ServiceRoot.ServiceRoot",
   "@odata.id" : "/redfish/v1",

   "SessionService" : {
      "@odata.id" : "/redfish/v1/SessionService"
   },

   "Id" : "RootService",
   "UUID" : "cec9c1d0-30e6-11e9-820c-53995703a902",
   "RedfishVersion" : "1.0.2",
   "Name" : "PSME Service Root",
   "@odata.type" : "#ServiceRoot.v1_1_1.ServiceRoot"
}
``` 

If want to stop / remvoe PSME service

## Stop PSME Redfish service

```
$ service psme stop
```
## Remove auto start PSME at boot time
```   
$ update-rc.d -f psme remove   
```
## Remvoe psme-allinone.deb package
```
$ dpkg -r psme-allinone
```
## How to run auto-test for PSME service.
Please refer the README document at

```
$ ./PSME/RF_Auto_Test/README.md
```

## Co-Work with Edgecore Device Manager
```
        +---------------------------+            
        |  Edgecore Device manager  |            
        +---------------------------+            
                |             |    
                |             |  
        +-------v--+  +-------v------+                
        |   VOLT   |  |   SWITCH     |               
        |   PSME   |  |    PSME      |               
        +----------+  +--------------+  
``` 
After make sure PSME can provide Restful API service, we can also setup Edgecore Device Manager to co-work with PSME. 
Edgecore's Device Manager will use a standard interface and dataflow to manage and monitor for deployment in the field.

More detailed information can be found at [Device-Manager](https://github.com/opencomputeproject/Device-Manager).

## Support on 

PSME current tested on ONL(OpenNetworkLinux) and ecSONiC system. 
   

	+-----------------------------|--------------------+  	+------------------------------|--------------------+		
	|            Platform         | ONL kernel 4.14.x  |    |            Platform          | SONiC kernel4.19.x |
 	|-----------------------------|--------------------|    |------------------------------|--------------------|             
	|x86-64-accton-as5712-54x     |        o           |    |  x86-64-accton-as7712-32x    |         o          | 
	|-----------------------------|--------------------|    |------------------------------|--------------------|             
	|x86-64-accton-as5812-54t     |        o           |    |  x86-64-accton-as4630-54pe   |         o          |   
	|-----------------------------|--------------------|    |------------------------------|--------------------|             
	|x86-64-accton-as5812-54x     |        o           |    |  x86-64-accton-as5835-54x    |         o          |     
	|-----------------------------|--------------------|    |------------------------------|--------------------|             
	|x86-64-accton-as5912-54x     |        o           |    |  x86-64-accton-as5835-54t    |         o          |
	|-----------------------------|--------------------|    |------------------------------|--------------------|             
	|x86-64-accton-as5916-54xk    |        o           |    |  x86-64-accton-as7326-56x    |         o          |  
	|-----------------------------|--------------------|    |------------------------------|--------------------|             
	|x86-64-accton-as5916-54xks   |        o           |    |  x86-64-accton-as7726-32x    |         o          |    
	|-----------------------------|--------------------|    |------------------------------|--------------------|             
	|x86-64-accton-as5916-54xm    |        o           |    |  x86-64-accton-as9716-32d    |         o          |   
	|-----------------------------|--------------------|    |------------------------------|--------------------|             
	|x86-64-accton-as6712-32x     |        o           |    |  x86-64-accton-as7816-64x    |         o          |     
	|-----------------------------|--------------------|    |------------------------------|--------------------|             
	|x86-64-accton-as6812-32x     |        o           |    |  x86-64-accton-wdge100bf-32X |         o          |
    |-----------------------------|--------------------|    |------------------------------|--------------------|                                                    
    |x86-64-accton-as7312-54x     |        o           |    |  x86-64-accton-wdge100bf-65X |         o          | 
    |-----------------------------|--------------------|    |------------------------------|--------------------|
    |x86-64-accton-as7316-26xb    |        o           |    |x86-64-accton-as8000(Minipack)|         o          |
    |-----------------------------|--------------------|    |------------------------------|--------------------| 
	|x86-64-accton-as7326-56x     |        o           |    |  x86-64-accton-as4630-54te   |         o          | 
	|-----------------------------|--------------------|    |---------------------------------------------------|
	|x86-64-accton-as7712-32x     |        o           |    <Not Support functioni on ecSONiC>     
	|-----------------------------|--------------------|    1. Get/Set thermal threshold and temperature alert function= 
	|x86-64-accton-as7726-32x     |        o           |         
	|-----------------------------|--------------------|                 
	|x86-64-accton-as7816-64x     |        o           |         
	|-----------------------------|--------------------|                 
	|x86-64-accton-as7926-40xke   |        o           |         
	|-----------------------------|--------------------|                 
	|x86-64-accton-as7926-80xk    |        o           |         
	|-----------------------------|--------------------|                 
	|x86-64-accton-as9716-32d     |        o           |         
	|-----------------------------|--------------------|                 
	|x86-64-accton-asxvolt16      |        o           |         
	|-----------------------------|--------------------|                 
	|x86-64-accton-asgvolt64      |        o           |
	|-----------------------------|--------------------|                 
	|x86-64-accton-as5835-54x     |        o           |
	|-----------------------------|--------------------|                 
	|x86-64-accton-as5915-18x     |        o           |
	|-----------------------------|--------------------|                 
	|x86-64-accton-as4630-54pe    |        o           |         
	+--------------------------------------------------+
