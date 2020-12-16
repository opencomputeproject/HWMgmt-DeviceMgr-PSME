#/bin/bash

COMMIT_ID="$1"
BAL_VER="$2"
PLT="$3"
BAL_PLT_NAME="$3-bal"

###########################################################################################
# ex: ./Build_BAL_SDK_ALL.sh "b4af32e" "3.2.2.2" "asgvolt64" 
# 1. Build ONL
# 2. Build SDK
# 3. Build PSME
# All build exist in docker environment 
##########################################################################################
echo COMMIT_ID[$COMMIT_ID]
echo BAL_VER[$BAL_VER]
echo PLT[$PLT]
echo BAL_PLT_NAME["$BAL_PLT_NAME"]

echo 1[$1]
echo 2[$2]
echo 3[$3]

ContainerName="BAL3_RSD_213"

BAL3_RSD_213_CONTAINER_ID=`docker ps -a | grep ${ContainerName} | awk '{print $1}'`

USERNAME=`whoami`

echo "USERNAME[$USERNAME]"
echo "BAL3_RSD_213_CONTAINER_ID[$BAL3_RSD_213_CONTAINER_ID]"

#Remove exist docker if exist#

[ "$(docker ps -a | grep $ContainerName)" ] && docker stop $ContainerName && docker rm $ContainerName 

uid=`id -u`
gid=`id -g`
PWD=`pwd`

BUILD_AGENT_DIR=$PWD/bal_sdk_allinone
TOP_DIR=$PWD

echo "TOP_DIR[$TOP_DIR]"

sudo rm -rf "$BUILD_AGENT_DIR"

mkdir -p $BUILD_AGENT_DIR/build;cd $BUILD_AGENT_DIR/build

if [ "$BAL_VER" == "3.1.1.1" ];then
    #Chcekout BAL SOURCE
    #git clone https://USERNAME:PASSWORD@GIT.ACCTON.COM.TW
    git clone https://github.com/balapi/bal-api-3.2.git
    mv BAL $BAL_PLT_NAME 
    cd $BAL_PLT_NAME 
    SDK_DIR=`pwd`
    echo "SDK_DIR[$SDK_DIR]"
    git checkout 3.1.1.1-dev
    BAL_VER="bal31"
    echo "BAL_VER[$BAL_VER]"
elif [ "$BAL_VER" == "3.2.2.2" ];then
    #Chcekout BAL SOURCE
    git clone https://nick_huang:password@git88.accton.com.tw/cicada/vOLT/BAL.git
    git clone https://github.com/balapi/bal-api-3.2.git
    mv BAL $BAL_PLT_NAME 
    cd $BAL_PLT_NAME 
    SDK_DIR=`pwd`
    echo "SDK_DIR[$SDK_DIR]"
    git checkout 3.2.2.2-dev
    BAL_VER="bal32"
    echo "BAL_VER[$BAL_VER]"

else
    BAL_VER="X"
    echo "Need SDK Version info.!!"
    exit
fi

cd $BUILD_AGENT_DIR/build

mkdir -p asvolt-onl;cd asvolt-onl

git clone https://github.com/opencomputeproject/OpenNetworkLinux.git; 

cd OpenNetworkLinux/

ONL_DIR=`pwd`

echo "OLN_DIR[$ONL_DIR]"

git reset --hard $COMMIT_ID 

echo "NETAUTO=dhcp" >> builds/amd64/installer/installed/builds/boot-config;

git clone https://github.com/edge-core/RSD-Redfish.git

cd RSD-Redfish/PSME/build

PSME_BUILD_DIR=`pwd`

echo "PSME_BUILD_DIR[$PSME_BUILD_DIR]"

docker run --privileged -i -t -e DOCKER_IMAGE=opennetworklinux/builder8:1.10 --name ${ContainerName} -d -v /lib/modules:/lib/modules -v ${PWD}:${PWD} -e USER=${USERNAME} --net host -w ${PWD} -e HOME=/home/${USERNAME} -v /home/${USERNAME}:/home/${USERNAME} opennetworklinux/builder8:1.10 /bin/docker_shell --user ${USERNAME}:${gid} -c bash
#Build ONL
docker exec ${ContainerName} /bin/docker_shell -c "./Build_ONL.sh"
#Install required packages
docker exec ${ContainerName} /bin/docker_shell -c "./pre-inst-X86-pkgs.sh"

echo "cd $SDK_DIR;make BOARD=$PLT ONL_DIR=$ONL_DIR SW_UTIL_SHELL=y OPEN_SOURCE=y;export BOARD=$PLT;./release_board.sh" > $PSME_BUILD_DIR/Build_SDK.sh
chmod 777 $PSME_BUILD_DIR/Build_SDK.sh
#Build SDK
docker exec ${ContainerName} /bin/docker_shell -c "./Build_SDK.sh"
#Build PSME
docker exec ${ContainerName} /bin/docker_shell -c "sudo ./Build_all.sh C;./Build_all.sh $BAL_VER"
