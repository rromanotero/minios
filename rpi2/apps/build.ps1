
Clear-Host;

$exec=$args[0]
$ErrorActionPreference = "Stop"

Write-Host "Building..."

& "C:\Program Files (x86)\GNU Tools ARM Embedded\8 2018-q4-major\bin\arm-none-eabi-gcc.exe" `
	-mcpu=cortex-a7 `
	-fpic	 `
	-fpie `
	-ffreestanding `
  --specs=nosys.specs `
	-std=gnu99 `
	-T linker.ld  `
	src/main.c `
	src/runtime.c `
	src/minilib/minilib_asm.s `
	src/minilib/minilib.c `
	src/stdio/stdio.c  `
  -o output/app.elf `
	-O0	`
	-s `
	#-lc -lm -lg -lgcc  ## THIS IS COMMENTED OUT!


Write-Host "Converting ELF to binary..."

& 'C:\Program Files (x86)\GNU Tools ARM Embedded\8 2018-q4-major\bin\arm-none-eabi-objcopy.exe' output/app.elf -O binary output/app.bin

Write-Host "Writing Dissasembly.."
arm-none-eabi-objdump -D .\output\app.elf | Out-File -filepath output/app.lss -Encoding ASCII
arm-none-eabi-objdump -s .\output\app.elf | Out-File -filepath output/app.dump -Encoding ASCII

Write-Host "Success"
