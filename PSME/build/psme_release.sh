#!/bin/bash

TOP_DIR="$PWD/.."
PSME_RELEASE_DIR="$TOP_DIR/build/release"
PSME_INSTALL_DIR="$TOP_DIR/build/install"
PSME_EC_SW_DIR="$TOP_DIR/agent/network/acc_sw"
DEB_PACKAGE_DIR="$TOP_DIR/build/install/allinone-deb/bin"
DEB_TOOL="$TOP_DIR/build/gener_deb.sh $1"

ver=$(grep VER_STRING $TOP_DIR/common/agent-framework/Version.cmake| awk 'BEGIN {FS="\""}; {print $2}' )
commit_id=`git rev-parse HEAD | cut -c1-7`
P_VER="${ver}-${commit_id}"

if [ -e ${PSME_RELEASE_DIR} ]; then
    /bin/rm -rf ${PSME_RELEASE_DIR}
fi

if [ -e ${PSME_INSTALL_DIR} ]; then
    /bin/rm -rf ${PSME_INSTALL_DIR}
fi

/bin/mkdir -p ${PSME_RELEASE_DIR}

cp -rf $TOP_DIR/tools/deb_maker/* $TOP_DIR/build

$DEB_TOOL

cp -a ${PSME_EC_SW_DIR}/x_86_dmidecode/*.deb ${DEB_PACKAGE_DIR}/

deb_tar_name="psme-bin-${ver}.tar.gz"
cd $DEB_PACKAGE_DIR

deb_commit_name="psme_allinone_baseline${P_VER}_amd64.deb"
tar -zcf $PSME_RELEASE_DIR/$deb_tar_name *
cp ${DEB_PACKAGE_DIR}/psme-allinone.deb ${DEB_PACKAGE_DIR}/${deb_commit_name}
