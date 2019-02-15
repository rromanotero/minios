#
#   Linux version of Pi3-64 script
#

set -e

bindir="/home/ubuntu/gcc-64-bits/bin/"  # <<< --- CHANGE THIS TO MATCH YOUR TOOLCAHIN'S PATH
cpuflags="-O3 -march=armv8-a+simd -mtune=cortex-a53 -mstrict-align -fno-tree-loop-vectorize -fno-tree-slp-vectorize"
asmflags="-nostdlib -nostartfiles -ffreestanding -fno-asynchronous-unwind-tables -fomit-frame-pointer -Wa,-a>output/list_rp3.txt"
linkerflags="-Wl,-gc-sections -Wl,--build-id=none -Wl,-Bdynamic -Wl,-Map,output/kernel_rp3.map"
outflags="-o output/kernel_rp3.elf"
libflags="-lc -lm -lgcc"

echo $bindir
echo $cpuflags

echo "Compiling"
"$bindir"aarch64-elf-gcc $cpuflags $asmflags $linkerflags -Wl,-T,linker/rpi64.ld \
  src/main.c  \
  src/boot/SmartStart64.S \
  src/boot/rpi-SmartStart.c \
  src/drivers/stdio/emb-stdio.c \
  src/drivers/sdcard/SDCard.c \
  src/hal/fonts.c \
  src/hal/hal_io.c \
  $outflags $libflags

echo "Creating kernel8.img"
"$bindir"aarch64-elf-objcopy.exe" output/kernel_rp3.elf -O binary output/kernel8.img

