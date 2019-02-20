
Clear-Host;

$exec=$args[0]
$ErrorActionPreference = "Stop"

Write-Host "Building..."

& "C:\Program Files (x86)\GNU Tools ARM Embedded\8 2018-q4-major\bin\arm-none-eabi-gcc.exe" `
	-mcpu=cortex-a7 `
	-fpic `
	-ffreestanding `
  --specs=nosys.specs `
	-std=gnu99 `
	-T linker.ld  `
	src/hal/hal_io_asm.s `
	src/boot.s  `
  src/drivers/smartstart/rpi-SmartStart.s `
	src/drivers/stdio/emb-stdio.c `
  src/drivers/sdcard/SDCard.c `
  src/drivers/smartstart/rpi-SmartStart.c `
	src/hal/hal_io.c src/kernel.c `
	src/hal/fonts.c `
  -o output/kernel.elf `
	-O0	`
	-lc -lm -lg -lgcc


& 'C:\Program Files (x86)\GNU Tools ARM Embedded\8 2018-q4-major\bin\arm-none-eabi-objcopy.exe' kernel.elf -O binary kernel.img


Write-Host "Success"

if ( $exec -eq "exec=true" )
{
  Write-Host "         P I ' S     U A R T 0          "
  Write-Host "= = = = = = = = = = = = = = = = = = = = "
  & 'C:\Program Files\qemu\qemu-system-arm.exe' -m 256 -M raspi2 -serial stdio -kernel output/kernel.elf -drive file=SDCard/SDCard.img,if=sd,format=raw
}
