#!/bin/bash

make hypervisor BOARD=tgl-rvp SCENARIO=logical_partition RELEASE=0
sleep 2
cp -f tee_config/vm_configurations.c build/hypervisor/configs/scenarios/logical_partition/vm_configurations.c
touch  build/hypervisor/configs/scenarios/logical_partition/vm_configurations.c
make hypervisor BOARD=tgl-rvp SCENARIO=logical_partition RELEASE=0
