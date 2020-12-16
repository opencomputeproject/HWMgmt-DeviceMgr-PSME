#!/bin/bash
CUR_PATH=$(cd "$(dirname "$0")"; pwd)
BUILD_PATH=$CUR_PATH/../build/

$BUILD_PATH/install/allinone-deb/sh/prepare.sh "$1"
$BUILD_PATH/install/allinone-deb/sh/generate_deb.sh

