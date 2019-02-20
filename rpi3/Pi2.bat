@echo off
REM
REM  Customized from:
REM     https://github.com/LdB-ECM/Raspberry-Pi/blob/master/SD_FAT32/Pi3-64.bat
REM

@REM COMPILER COMMAND LINE
@echo off
set "bindir=C:\Program Files (x86)\GNU Tools ARM Embedded\8 2018-q4-major\bin\"
set "cpuflags=-Wall -O3 -mfpu=neon -mfloat-abi=hard -march=armv7ve -mtune=cortex-a7 -mno-unaligned-access -fno-tree-loop-vectorize -fno-tree-slp-vectorize"
set "asmflags=-nostdlib -nostartfiles -ffreestanding -fno-asynchronous-unwind-tables -fomit-frame-pointer -Wa,-a>output/list_rp2.tx"
set "linkerflags=-Wl,-gc-sections -Wl,--build-id=none -Wl,-Bdynamic -Wl,-Map,output/kernel_rp2.map"
set "outflags=-o output/kernel_rp2.elf"
set "libflags=-lc -lm -lg -lgcc"
@echo on
"%bindir%arm-none-eabi-gcc" %cpuflags% %asmflags% %linkerflags% -Wl,-T,linker/rpi32.ld ^
   src/main.c ^
   src/boot/SmartStart32.S ^
   src/boot/rpi-SmartStart.c ^
   src/drivers/stdio/emb-stdio.c ^
   src/drivers/sdcard/SDCard.c ^
   src/hal/fonts.c ^
   src/hal/hal_io.c ^
   src/hal/hal_io_asm.s ^
   %outflags% %libflags%

@echo off
if %errorlevel% EQU 1 (goto build_fail)

@REM LINKER COMMAND LINE
@echo on
"%bindir%arm-none-eabi-objcopy" output/kernel_rp2.elf -O binary output/kernel7.img
@echo off
if %errorlevel% EQU 1 (goto build_fail)
echo BUILD COMPLETED NORMALLY
exit /b 0

:build_fail
echo ********** BUILD FAILURE **********
exit /b 1
