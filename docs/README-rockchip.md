Rock PI 4B trusted substrate instructions
=========================================

Build
-----

To build trusted substrate firmware for rockpi4b issue the following commands.

```
$ pip install kas
$ git clone https://git.codelinaro.org/linaro/dependable-boot/meta-ts.git
$ cd meta-ts
$ kas build ci/rockpi4b.yml
```

Upon a sucessful build a wic image that can be flashed to the SD card can be
found at meta-ts/build/tmp/deploy/images/rockpi4b/ts-firmware-rockpi4b.wic.gz

Run
---

Typically we flash the firmware to the SD card and the rootfs and ESP
partition are put on a USB stick. This allows a generic rootfs to be
easily booted on multiple boards.


1. Flash the image to SD card:

`zcat ts-firmware-rockpi4b.rootfs.wic.gz > /dev/sdX`

2. Prepare a USB stick with rootfs:
zcat rootfs.wic.gz > /dev/sdUSB
You can use LEDGE RP for example:
http://snapshots.linaro.org/components/ledge/oe/ledge-multi-armv8/1254/rpb/ledge-qemuarm64/ledge-iot-ledge-qemuarm64-20220206235911.rootfs.wic.gz

Note: if you used USB stick in other machine with current firmware before
booting delete ubootefi.var file for ESP (first one) partition.

3. Plug both USB stick and SD card into the board. Power it on and trap it
in the U-boot command line.

4. Add kernel board specific kernel parameters and EFI boot order.

```
efidebug boot add -b 1 BootLedge usb 0:1 efi/boot/bootaa64.efi -i usb 0:1 ledge-initramfs.rootfs.cpio.gz -s 'console=ttyS2,1500000 console=tty0 root=UUID=6091b3a4-ce08-3020-93a6-f755a22ef03b rootwait panic=60'
efidebug boot order 1
```

5. Power cycle board it and it has to boot automatically now.
NOTE: first boot with a fresh root fs is slow, please wait for a couple of
minutes (really) when no output is shown.

UEFI Capsule Update
-------------------

Firmware updates are supported using the UEFI Capsule update mechanism.
Assuming rm_work is disabled in the meta-ts yocto build. To create the
capsules issue the following commands:

Generate capsules
-----------------

```
$ cd meta-ts/build/tmp/work/rockpi4b-poky-linux/u-boot/1_2022.01-r1.ledge/build/rockpi4b_defconfig
$ ./tools/mkeficapsule -i 0x1 -r idbloader.img idbloader.capsule
$ ./tools/mkeficapsule -i 0x2 -r u-boot.itb u-boot.capsule
```

Applying the capsules
---------------------

1. Put the generated capsules in the ESP (on USB stick) in the \EFI\UpdateCapsule directory
2. `setenv -e -nv -bs -rt -v OsIndications =0x0000000000000004`
3. Reboot the board, and the capsule should be detected and applied

To force the capsule to be processed you can issue following command
`efidebug capsule disk-update`

If processing the capsule is sucessful you should see something like the following
in the log.

```
#
Applying capsule idbloader.capsule succeeded.
#
Applying capsule u-boot.capsule succeeded.
Reboot after firmware updateresetting ...
```

More information about capsules and uefi in U-Boot can be found
https://u-boot.readthedocs.io/en/latest/develop/uefi/uefi.html
