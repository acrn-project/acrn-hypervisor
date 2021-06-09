#!/bin/bash

if [ $# -eq 0 ];
then
  target=tee_sos
else
  target=$1
fi

make hypervisor BOARD=adl-rvp SCENARIO=$target RELEASE=0
sleep 2
cp -f tee_config/$target/vm_configurations.c build/hypervisor/configs/scenarios/$target/vm_configurations.c
touch  build/hypervisor/configs/scenarios/$target/vm_configurations.c
make hypervisor BOARD=adl-rvp SCENARIO=$target RELEASE=0
