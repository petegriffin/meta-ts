COMPATIBLE_MACHINE:synquacer = "synquacer"
COMPATIBLE_MACHINE:rockpi4b = "rockpi4b"
COMPATIBLE_MACHINE:stm32mp157c-ev1 = "stm32mp157c-ev1"

DEPENDS += " python3-cryptography-native"

do_deploy:append () {
    install -D -p -m 0644 ${S}/Samples/ARM32-FirmwareTPM/optee_ta/out/fTPM/${FTPM_UUID}.elf ${DEPLOYDIR}/optee/
}
