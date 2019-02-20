@echo off
REM
REM  Customized from:
REM     https://github.com/LdB-ECM/Raspberry-Pi/blob/master/SD_FAT32/Pi3-64.bat
REM


@REM COMPILER COMMAND LINE
@echo off
set "bindir=C:\Program Files (x86)\GNU ARM AARCH64\bin\"
set "cpuflags=-O3 -march=armv8-a+simd -mtune=cortex-a53 -mstrict-align -fno-tree-loop-vectorize -fno-tree-slp-vectorize"
set "asmflags=-nostdlib -nostartfiles -ffreestanding -fno-asynchronous-unwind-tables -fomit-frame-pointer -Wa,-a>output/list_rp3.txt"
set "linkerflags=-Wl,-gc-sections -Wl,--build-id=none -Wl,-Bdynamic -Wl,-Map,output/kernel_rp3.map"
set "outflags=-o output/kernel_rp3.elf"
set "libflags=-lc -lm -lgcc"
@echo on
"%bindir%aarch64-elf-gcc.exe" %cpuflags% %asmflags% %linkerflags% -Wl,-T,linker/rpi64.ld ^
  src/main.c ^
  src/boot/SmartStart64.S ^
  src/boot/rpi-SmartStart.c ^
  src/drivers/stdio/emb-stdio.c ^
  src/drivers/sdcard/SDCard.c ^
  src/hal/fonts.c ^
  src/hal/hal_io.c ^
  %outflags% %libflags%

@echo off
if %errorlevel% EQU 1 (goto build_fail)

@REM LINKER COMMAND LINE
@echo on
"%bindir%aarch64-elf-objcopy.exe" output/kernel_rp3.elf -O binary output/kernel8.img
@echo off
if %errorlevel% EQU 1 (goto build_fail)
echo BUILD COMPLETED NORMALLY
exit /b 0

:build_fail
echo ********** BUILD FAILURE **********
exit /b 1
