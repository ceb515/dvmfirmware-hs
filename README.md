# Digital Voice Modem Firmware (Hotspot)

The DVM firmware provides the embedded microcontroller implementation of a mixed-mode DMR/P25 or dedicated-mode DMR or P25 repeater system. The firmware; is the portion of a complete Over-The-Air modem implementation that uses an ADF7021 to provide a raw RF interface.

This project is a direct fork of the MMDVM_HS (https://github.com/juribeparada/MMDVM_HS) project.

Please feel free to reach out to us for help, comments or otherwise, on our Discord: https://discord.gg/3pBe8xgrEz

## Building

Please see the various Makefile's included in the project for more information. This project includes a few Makefiles to target different hardware. (All following information assumes familiarity with the standard Linux make system.)

* Makefile.STM32FX - This makefile is used for targeting a generic STM32F103 with an ADF7021 RF SoC device.

* For STM32F103 using Ubuntu OS install the standard ARM embedded toolchain (gcc-arm-none-eabi).
  - Make sure to clone this repository with the ```--recurse-submodules``` option, otherwise the STM32 platform files will be missing! ```git clone --recurse-submodules https://github.com/DVMProject/dvmfirmware-hs.git```

To build the firmware, use the ```make``` command, followed by -f and the correct makefile, followed by the type of board you are using. 

> **_NOTE:_**  The only two known tested targets are ```mmdvm-hs-hat-dual``` and ```mmdvm-hs-hat-dual-usb```.

An example of this would be ```make -f Makefile.STM32FX mmdvm-hs-hat-dual``` for a full duplex modem hotspot, attached to GPIO.

## Firmware installation

The device can be used on top on a RPi attached via the GPIO port or standalone and connected via USB. Both variants require different handling of compiling and uploading the firmware, examples on flashing devices are mostly not included here because the methods to flash vary from device to device.

The USB connection requires firmware with bootloader support whereas the GPIO version does not. For USB connection a bootloader has to be installed initally. This requires STlink connection. After that is done the firmware upgrade can be done via the USB connection. The STlink connection can be used as fallback if wrongly configured firmware was installed for example.

### Install the firmware via GPIO on Raspberry Pi

> **_NOTE:_**  Your mileage may vary with these instructions, the hotspot boards are loosely designed around a common factor but not all are created equally.

First you will need to disable the serial console and disable bluetooth. Edit ```/boot/cmdline.txt``` and remove the line ```console=serial0, 115200```.
Next, you will need to disable bluetooth on the board. Edit ```/boot/config.txt``` and add a line containing ```dtoverlay=disable-bt```. Reboot.

> Most sets of instructions reccomend to download stm32flash from online, however we have found the prepackaged version to work fine.

Once the hotspot is back on, navigate to the build folder where you compiled the firmware. Put a jumper across the J1 points on the board, and the RED heartbeat LED should stop flashing. Run the below command to flash.

```stm32flash -v -w dvm-firmware-hs_f1.bin -i 20,-21,21,-20 -R /dev/ttyAMA0```

You should see the below output if the board flashed successfully.
```
Wrote and verified address 0x0800be40 (100.00%) Done.

Resetting device... 
Reset done.
```


### Install the firmware with bootloader support for USB connection

If you want to use the device via USB port you have to install a bootloader and build the firmware with bootloader support. As the raw device cannot be used with USB you have to use a USB-serial adapter or STlink device.

* The bootloader (https://github.com/DVMProject/STM32F10X_Platform/blob/527fee72ae2291486304380cb812c48f36122c32/utils/bootloader/generic_boot20_pc13.bin) should be installed starting at offset 0x8000000.
* The firmware should be installed starting at offset 0x8002000.

An example Using STlink this can be done as follows:
```
user@host:~/dvmfirmware-hs$ -f Makefile.STM32FX mmdvm-hs-hat-usb-dual
...
user@host:~/dvmfirmware-hs$ ./STM32F10X_Platform/utils/linux64/st-flash write ./STM32F10X_Platform/utils/bootloader/generic_boot20_pc13.bin 0x8000000
2018-03-02T10:01:04 INFO src/usb.c: -- exit_dfu_mode
2018-03-02T10:01:04 INFO src/common.c: Loading device parameters....
2018-03-02T10:01:04 INFO src/common.c: Device connected is: F1 Medium-density device, id 0x20036410
2018-03-02T10:01:04 INFO src/common.c: SRAM size: 0x5000 bytes (20 KiB), Flash: 0x10000 bytes (64 KiB) in pages of 1024 bytes
2018-03-02T10:01:04 INFO src/common.c: Attempting to write 7160 (0x1bf8) bytes to stm32 address: 134217728 (0x8000000)
Flash page at addr: 0x08001800 erased
2018-03-02T10:01:04 INFO src/common.c: Finished erasing 7 pages of 1024 (0x400) bytes
2018-03-02T10:01:04 INFO src/common.c: Starting Flash write for VL/F0/F3 core id
2018-03-02T10:01:04 INFO src/common.c: Successfully loaded flash loader in sram
  6/6 pages written
2018-03-02T10:01:05 INFO src/common.c: Starting verification of write complete
2018-03-02T10:01:05 INFO src/common.c: Flash written and verified! jolly good!
user@host:~/dvmfirmware-hs$ ./STM32F10X_Platform/utils/linux64/st-flash write dvm-firmware-hs_f1bl.bin 0x8002000
2018-03-02T10:01:05 INFO src/common.c: Loading device parameters....
2018-03-02T10:01:05 INFO src/common.c: Device connected is: F1 Medium-density device, id 0x20036410
2018-03-02T10:01:05 INFO src/common.c: SRAM size: 0x5000 bytes (20 KiB), Flash: 0x10000 bytes (64 KiB) in pages of 1024 bytes
2018-03-02T10:01:05 INFO src/common.c: Attempting to write 55016 (0xd6e8) bytes to stm32 address: 134225920 (0x8002000)
Flash page at addr: 0x0800f400 erased
2018-03-02T10:01:07 INFO src/common.c: Finished erasing 54 pages of 1024 (0x400) bytes
2018-03-02T10:01:07 INFO src/common.c: Starting Flash write for VL/F0/F3 core id
2018-03-02T10:01:07 INFO src/common.c: Successfully loaded flash loader in sram
 53/53 pages written
2018-03-02T10:01:12 INFO src/common.c: Starting verification of write complete
2018-03-02T10:01:13 INFO src/common.c: Flash written and verified! jolly good!
```
The device should now be usable as /dev/ttyACMx.

## License

This project is licensed under the GPLv2 License - see the [LICENSE.md](LICENSE.md) file for details. Use of this project is intended, for amateur and/or educational use ONLY. Any other use is at the risk of user and all commercial purposes is strictly discouraged.

