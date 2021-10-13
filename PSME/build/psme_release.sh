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

sed -i '/function get_nos/,+3d' ../tools/deb_maker/install/allinone-deb/psme-allinone/DEBIAN/preinst
sed -i '/get_nos) == $(get_cur_nos/,+8d' ../tools/deb_maker/install/allinone-deb/psme-allinone/DEBIAN/preinst
sed -i '/Description:/d' ../tools/deb_maker/install/allinone-deb/psme-allinone/DEBIAN/control
sed -i '/OS_Ver/d' ../tools/deb_maker/install/allinone-deb/psme-allinone/DEBIAN/control
sed -i '/commit_id/d' ../tools/deb_maker/install/allinone-deb/psme-allinone/DEBIAN/control

OS_Ver=`lsb_release -id | grep 'Description:'`

function get_cur_nos()
{   
    Cur_OS_Ver=`cat /etc/debian_version`
    echo "${Cur_OS_Ver%%.*}"
}

echo "Description:psme_allinone_$P_VER" >> ../tools/deb_maker/install/allinone-deb/psme-allinone/DEBIAN/control
echo "function get_nos() 
{   
    echo "NOS" 
}
" >>  ../tools/deb_maker/install/allinone-deb/psme-allinone/DEBIAN/preinst

echo '
if [ $(get_nos) == $(get_cur_nos) ];then
    killall psme-rest-server > /dev/null
    killall psme-chassis > /dev/null
    killservice 'psme-rest-server';
    killservice 'psme-chassis';
else
    echo "###########  Your PSME is for Debian $(get_nos) version. But your working Linux is Debian $(get_cur_nos) version!! ###############"
    exit -1
fi
' >>  ../tools/deb_maker/install/allinone-deb/psme-allinone/DEBIAN/preinst

sed -i "s/NOS/$(get_cur_nos)/g" ../tools/deb_maker/install/allinone-deb/psme-allinone/DEBIAN/preinst
echo "OS_Ver:${OS_Ver}" >> ../tools/deb_maker/install/allinone-deb/psme-allinone/DEBIAN/control
echo "commit_id:${commit_id}" >> ../tools/deb_maker/install/allinone-deb/psme-allinone/DEBIAN/control


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

DB_VER=`lsb_release -c | grep 'buster'`
if [ "$DB_VER" != "" ];then
    deb_commit_name="psme_allinone_buster_baseline${P_VER}_amd64.deb"
    pandoc $TOP_DIR/PSME_REDFISH_API/README.md --pdf-engine=xelatex -V geometry:"${MD_TO_PDF_OPTIONS}" -o ${DEB_PACKAGE_DIR}/PSME_REDFISH_API.pdf
else
    deb_commit_name="psme_allinone_jessie_baseline${P_VER}_amd64.deb"
    pandoc $TOP_DIR/PSME_REDFISH_API/README.md --latex-engine=xelatex -V geometry:"${MD_TO_PDF_OPTIONS}" -o ${DEB_PACKAGE_DIR}/PSME_REDFISH_API.pdf
fi

tar -zcf $PSME_RELEASE_DIR/$deb_tar_name *
cp ${DEB_PACKAGE_DIR}/psme-allinone.deb ${DEB_PACKAGE_DIR}/${deb_commit_name}


