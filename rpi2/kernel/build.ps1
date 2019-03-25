
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
	src/hal/hal_io.c `
	src/kernel.c `
	src/hal/fonts.c `
	src/console/console.c `
	src/console/commands.c `
	src/hal/hal_nvmem.c `
	src/hal/hal_memreg.c `
	src/loader.c `
	src/syscalls.c `
	src/hal/hal_cpu.c `
	src/hal/hal_cpu_asm.s `
	src/irqs.c `
	src/drivers/per_core_timer/per_core_timer.c `
  -o output/kernel.elf `
	-O0	`
	-lc -lm -lg -lgcc


	arm-none-eabi-objdump -D .\output\kernel.elf | Out-File -filepath output/kernel.lss -Encoding ASCII
	arm-none-eabi-objdump -s .\output\kernel.elf | Out-File -filepath output/kernel.dump -Encoding ASCII
& 'C:\Program Files (x86)\GNU Tools ARM Embedded\8 2018-q4-major\bin\arm-none-eabi-objcopy.exe' output/kernel.elf -O binary output/kernel7.img


Write-Host "Success"

if ( $exec -eq "exec=true" )
{
  Write-Host "         P I ' S     U A R T 0          "
  Write-Host "= = = = = = = = = = = = = = = = = = = = "
  & 'C:\Program Files\qemu\qemu-system-arm.exe' -m 256 -M raspi2 -serial stdio -kernel output/kernel.elf -drive file=SDCard/SDCard.img,if=sd,format=raw
}
