To generate xml config once BIOS changes, xml will be in tools/target/out
	
	python3 tools/target/board_parser.py adl_tvp


Compile:
	1. make hypervisor BOARD=tgl-rvp SCENARIO=logical_partition RELEASE=0
	2. use tee_config/vm_configurations.c to instead of build/hypervisor/configs/scenarios/logical_partition/vm_configurations.c
	3. touch build/hypervisor/configs/scenarios/logical_partition/vm_configurations.c to make it new
	4. make hypervisor BOARD=tgl-rvp SCENARIO=logical_partition RELEASE=0

Grub:

menuentry 'ACRN TEE' {
echo 'Loading hypervisor tee'
multiboot2 /EFI/BOOT/acrn_tee.bin
module2 /EFI/BOOT/ACPI_VM0_TEE.bin ACPI_VM0
module2 /EFI/BOOT/tee-pager_v2_padding.bin Tee_RawImage
module2 /EFI/BOOT/ACPI_VM1_TEE.bin ACPI_VM1
module2 /bzImage_uos Linux_bzImage
}