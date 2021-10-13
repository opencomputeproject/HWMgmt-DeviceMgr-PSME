#!/bin/bash

#Install required packages
CMAKE_VER_INS=3.18.4
CMAKE_DIR_INS=${CMAKE_VER_INS%.*}

echo ""
echo "Installing dependency Packages"
echo ""

sudo apt-get update

sudo apt-get install -f

PACKAGES=("clang" "libgcrypt20-dev" "libncurses5-dev" "libnl-3-dev" "libudev-dev" "libglibmm-2.4-dev" "libglib3.0-cil-dev"
          "libxml++2.6-dev" "libgnutls28-dev" "libnl-route-3-dev" "flex" "bison" "doxygen" "cpp" "ccache" "build-essential"
	  "linux-libc-dev" "libmpc-dev" "libstdc++6" "libcurl4-openssl-dev" "libmicrohttpd-dev" "libjsoncpp-dev"
          "lcov" "libossp-uuid-dev" "libnl-route-3-200" "libsysfs-dev" "libpopt-dev" "libusb-dev" "patch" "libdevmapper-dev" 
	  "liblvm2-dev" "unzip" "libnl-genl-3-dev" "libblkid-dev" "debsigs" "debsig-verify" "gnupg" "ipmitool" "texlive-xetex" "pandoc" "lsb-release"
          )

count="${#PACKAGES[@]}"	  

for (( c=0; c < "${count}"; c++ ))
do
    PK_NAME=${PACKAGES[$c]}
    echo "install PK_NAME[$PK_NAME]"
    RES=`sudo apt-get install "$PK_NAME" --yes`
    echo "RES[$RES]"
done


#Install cmake version to $CMAKE_VER
CMAKE_VER=`cmake -version | grep "cmake version"  | awk '{print $3}'`
if [ "$CMAKE_VER" != "$CMAKE_VER_INS" ]; then
    sudo apt-get purge cmake
    wget --no-check-certificate  https://cmake.org/files/v$CMAKE_DIR_INS/cmake-$CMAKE_VER_INS.tar.gz
    tar xzvf cmake-$CMAKE_VER_INS.tar.gz
    cd cmake-$CMAKE_VER_INS/
    ./bootstrap
    make -j4
    sudo make install
    if [ -e /usr/bin/cmake ]; then
	sudo rm /usr/bin/cmake
    fi
    sudo ln -s /usr/local/bin/cmake /usr/bin/cmake
fi
