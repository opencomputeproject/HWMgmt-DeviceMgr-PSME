#!/bin/bash
# Build ONL 
PATH=$PATH:$HOME/bin:./:/bin/:/usr/bin/
export PATH
cd ../../../
source setup.env
apt-cacher-ng
make amd64 2>&1 | tee onl.log
