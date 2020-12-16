#!/bin/bash
rm ../agent/storage/include/storage_config.hpp
rm -rf ../application/include/psme/rest/metadata/include-generated/
rm ../application/include/version.hpp
rm ../common/agent-framework/include/agent-framework/version.hpp

mv pre-inst-X86-pkgs.sh ../
mv psme_release.sh ../
mv Build_all.sh ../
mv Build_ONL.sh ../
mv Build_BAL_SDK_ALL.sh ../
cd ../
rm -rf build/*
mv pre-inst-X86-pkgs.sh build
mv psme_release.sh build
mv Build_all.sh build
mv Build_ONL.sh build  
mv Build_BAL_SDK_ALL.sh build
cd build

if [ "$1" != "C" ];then
    echo "Build all"
    cmake ../
    grep -rl Werror . | grep flags.make | xargs sed -i 's/-Werror//g'
    make all 2>&1 | tee  psme.log    
    if [ "$?" == 1 ];then
        echo "Build_all.sh error!!"
        exit 1
    fi

    find ./ -name control  | xargs sed -i 's/armel/amd64/g'
    find ../tools/deb_maker/install/allinone-deb/psme-allinone/DEBIAN -name control  | xargs sed -i 's/armel/amd64/g'

    ./psme_release.sh
fi
