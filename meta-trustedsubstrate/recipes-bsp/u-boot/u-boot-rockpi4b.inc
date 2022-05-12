# Generate Rockchip style loader binaries
inherit deploy

FILESEXTRAPATHS:prepend := "${THISDIR}/u-boot/rockpi4b:"

SRC_URI:append = " file://rockpi4b_defconfig"
SRC_URI:append = " file://0004-phy-rockchip-inno-usb2-fix-hang-when-multiple-controllers-exit.patch"
SRC_URI:append = " file://0001-rk3399-rock-pi-4b-u-boot-add-optee-node.patch"
SRC_URI:append = " file://0001-rk3399-rock-pi-4b-u-boot.dtsi-reserve-optee-memory-t.patch"
SRC_URI:append = " file://0001-tee-optee-bind-ftpm-optee-driver.patch"
SRC_URI:append = " file://0001-RockPi4-Add-UEFI-capsule-update-support.patch"
SRC_URI:append = " file://0001-capsule-FMP-Populate-the-image-descriptor-array-from.patch"
SRC_URI:append = " file://0002-capsule-Put-a-check-for-image-index-before-the-updat.patch"
SRC_URI:append = " file://0003-efi-Define-set_dfu_alt_info-for-boards-with-UEFI-cap.patch"
SRC_URI:append = " file://0004-test-capsule-Modify-the-capsule-tests-to-use-GUID-va.patch"
SRC_URI:append = " file://0005-FMP-Remove-GUIDs-for-FIT-and-raw-images.patch"
SRC_URI:append = " file://0006-mkeficapsule-Remove-raw-and-FIT-GUID-types.patch"
SRC_URI:append = " file://0007-doc-uefi-Update-the-capsule-update-related-documenta.patch"
SRC_URI:append = " file://0001-capsule-board-Add-information-needed-for-capsule-upd.patch"
SRC_URI:append = " file://0001-mach-rockchip-make_fit_atf.py-support-OP-TEE-tee.bin.patch"

COMPATIBLE_MACHINE = "rockpi4b"

DEPENDS += "trusted-firmware-a gnutls-native"

PARALLEL_MAKE = ""

do_compile:prepend() {
	export BL31="${RECIPE_SYSROOT}/firmware/bl31.elf"
	ls -l ${BL31}
	export TEE="${RECIPE_SYSROOT}/lib/firmware/tee.bin"
	ls -l ${TEE}
}

do_deploy:append() {
	mkdir -p ${DEPLOYDIR}
	cp ${B}/rockpi4b_defconfig/idbloader.img  ${DEPLOYDIR}/idbloader.img
	cp ${B}/rockpi4b_defconfig/u-boot.itb ${DEPLOYDIR}/u-boot.itb
}

ATF_DEPENDS = " trusted-firmware-a:do_deploy"
do_compile[depends] .= "${ATF_DEPENDS}"
