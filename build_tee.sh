#!/bin/bash

make hypervisor BOARD=adl-rvp SCENARIO=tee RELEASE=0
sleep 2
cp -f tee_config/vm_configurations.c build/hypervisor/configs/scenarios/tee/vm_configurations.c
touch  build/hypervisor/configs/scenarios/tee/vm_configurations.c
make hypervisor BOARD=adl-rvp SCENARIO=tee RELEASE=0
