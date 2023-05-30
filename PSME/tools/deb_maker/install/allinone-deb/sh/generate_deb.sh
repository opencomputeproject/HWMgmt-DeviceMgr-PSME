#!/bin/bash
CUR_PATH=$(cd "$(dirname "$0")"; pwd)

function generate_deb()
{
    chmod +x $1/DEBIAN/post*
    dpkg -b $1 $1.deb
    echo "==== Move $1.deb to $1/../bin/ ===="
    mv $1.deb $1/../bin/
}

declare -a arr=(
    "psme-allinone")
    
for i in "${arr[@]}"
do
   generate_deb $CUR_PATH/../$i
done  
