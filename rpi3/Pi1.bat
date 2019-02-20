@
@  Customized from:
@     https://github.com/LdB-ECM/Raspberry-Pi/blob/master/SD_FAT32/Pi3-64.bat
@@

@REM COMPILER COMMAND LINE
@echo off
set "bindir="
set "cpuflags=-Wall -O3 -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -mno-unaligned-access -fno-exceptions"
set "asmflags=-nostdlib -nostartfiles -ffreestanding -fno-asynchronous-unwind-tables -fomit-frame-pointer -Wa,-a>output/list_rp1.txt"
set "linkerflags=-Wl,-gc-sections -Wl,--build-id=none -Wl,-Bdynamic -Wl,-Map,output/kernel_rp1.map"
set "outflags=-o output/kernel_rp1.elf"
set "libflags=-lc -lm -lg -lgcc"
@echo on
%bindir%arm-none-eabi-gcc %cpuflags% %asmflags% %linkerflags% -Wl,-T,linker/rpi32.ld src/main.c src/boot/SmartStart32.S src/boot/rpi-SmartStart.c src/drivers/stdio/emb-stdio.c src/drivers/sdcard/SDCard.c %outflags% %libflags%
@echo off
if %errorlevel% EQU 1 (goto build_fail)

@REM LINKER COMMAND LINE
@echo on
%bindir%arm-none-eabi-objcopy output/kernel_rp1.elf -O binary output/kernel.img
@echo off
if %errorlevel% EQU 1 (goto build_fail)
echo BUILD COMPLETED NORMALLY
exit /b 0

:build_fail
echo ********** BUILD FAILURE **********
exit /b 1
