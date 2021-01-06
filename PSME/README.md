### PSME Redfish Service Architecture Model

```
+---------------------------+            
|  Edgecore Device manager  |            
+-----------^---------------+            
	    |                   
	    |                  
	    |                 
	    +-------+       
		    |      
    https port 8888 |     
	   +--------v------------------+                 
	   |    PSME Redfish Service   |                 
	   |                           |                 
	   |  +--------------------+   |                 
	   |  |                    |   |                 
	   |  | PSME REST Server   |   |                 
	   |  |                    |   |                 
	   |  +---------^----------+   |                 
	   |            | JSON|RPC     |                 
	   |  +---------v----------+   |                 
	   |  |                    |   |                 
	   |  | PSME Chassis Agent |   |                 
	   |  |                    |   |                 
	   |  +---------^----------+   |                 
	   |            |              |                 
	   |  +---------v----------+   |                 
	   |  |                    |   |                 
	   |  |    ONLP API        |   |                 
	   |  +--------------------+   |                             
	   |   ONL(OpenNetworkLinux)   |
	   |                           |
	   |   running on X86 CPU      |                 
	   |   board.                  |                 
	   +---------------------------+    
```                        
### Introduction 

  PSME(Pooled System Management Engine) is a part of whole Intel Rack Scale Design(Intel RSD) application and it 
  is from Intel RSD 2.1.3 version to support PSME RESTful API running on specific Accton/Edgecore's white box 
  SWITCH or OLT devices.

  PSME Redfish Service provides PSME RESTful API with support for the Distributed Management Task Force (DMTF) 
  Redfish standard to get management information or issuing commands to change the configuration or operational 
  state of a server. 

  PSME Redfish Service has been verified by OCPBaselineHardwareManagement OCP-Profiles with DMTF Redfish-Interop-Validator and 
  Redfish-Service-Validator tools.

### PSME Redfish Service consists of 

- **PSME REST Server**

    HTTPs server with REST API and JSON data container responsible for gathering and presenting information about 
    assets and available operations on these assets. 

    This application speaks with agents (PSME Chassis Agent) through JSON-RPC as a transport and Generic Asset 
    Management Interface as a payload protocol. 

    This application also response for answering query from REST RF Client though https security port 8888 (SONiC NOS is 8889).

    If any 3 types of events(ResourceAdd, ResourceRemove, Alert) have subscribed to PSME server from Redfish Event Listener, 
    PSME will send events to Redfish Event Listener while it gets events from PSME Chassis Agent.

- **PSME Chassis Agent**

    Responsible for gathering peripheral information about thermal/fan/PSU/port transceiver Statistics through ONLP API and system 
    and send back to PSME REST Server.

    While gathering peripheral information, it will also check if any event appear then need to send these events
    to PSME REST Server.

### PSME Redfish Service related tools 
          
- **DMTF validator tools**

    [Redfish-Interop-Validator](https://github.com/DMTF/Redfish-Interop-Validator)

    [Redfish-Service-Validator](https://github.com/DMTF/Redfish-Service-Validator)
    
    [Redfish-Event-Listener](https://github.com/DMTF/Redfish-Event-Listener)

    [OCP-Profiles](https://github.com/opencomputeproject/OCP-Profiles)
