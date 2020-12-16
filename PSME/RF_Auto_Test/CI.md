## Jenkins Installation Setup Guide ## 

* [Reference to](https://oranwind.org/-devops-jenkins-yu-centos-ubuntu-an-zhuang-jiao-xue/)

   1st Setup 


   ```
   sudo mkdir -p /data/jenkins
   ```

   ```
   sudo chown -R $USER:$GROUP /data
   ```

   ```
   docker run \
   --name jenkins \
   -d --restart always \
   -p 8080:8080 -p 50000:50000 \
   -v /data/jenkins:/var/jenkins_home \
   jenkins/jenkins:lts
   ```

   After container run up, enter container shell

   ```
   docker exec -i -t d672f161ae08 /bin/bash
   ```

   ```
   cat /data/jenkins/secrets/initialAdminPassword
   #cp above Password
   exit
   ```

   Open host browser

   ```
   http://localhost:8080
   ```

   Start config jenkins.


   Download plugins

   * [Robot framework](http://mirrors.jenkins-ci.org/plugins/robot/) 
   * [Publish Over SSH](https://updates.jenkins.io/download/plugins/publish-over-ssh/) 
   * [SSH plugin](https://updates.jenkins.io/download/plugins/ssh/) 

## Add Slave nodes ##

* Create 2 Slave Agents.  

   Add slave node 1 172.17.10.65 for building ecrf_pal and redfish or related SDK.

   ![Screenshot](img/mgt_node_1.png) 
   
*  Add new node.

   ![Screenshot](img/mgt_node_2.png)  
   
*  Add node name "Build_RSD_PSME" and this name can be bind by specific task later.

   ![Screenshot](img/mgt_node_3.png)   
   
*  Because use "SSH" to start java agent, we need add SSH credentials username and password. 

   ![Screenshot](img/mgt_node_4.png)   
   
*  Add credentials.

   ![Screenshot](img/mgt_node_4_1.png) 
   
*  This 172.17.10.65 username and password.

   ![Screenshot](img/mgt_node_4_2.png)   

*  Get into advanced items. 

   ![Screenshot](img/mgt_node_4_2_1.png)    


*  Add Java path , if not install java, please use "sudo apt-get install default-jdk" to install. 

   ![Screenshot](img/mgt_node_4_2_2.png)    
 
*  Add another build server slave note for robo framework to auto testing.

   ![Screenshot](img/mgt_node_4_3.png)  
   
*  Add 2nd node. 

   ![Screenshot](img/mgt_node_2.png)    

   
*  Add node name "Robot_PSME_Auto_Test" and this name can be bind by specific task later.

   And use the same procedures to set credentials and advanced items.

   ![Screenshot](img/mgt_node_5.png)   

*  Launch both 2 agents. 

   ![Screenshot](img/mgt_node_5_1.png)    

*  2 slaves node shouble in good status. 

   ![Screenshot](img/mgt_node_5_2.png)    

[Continue add tasks](CI_TaskAdd.md)

