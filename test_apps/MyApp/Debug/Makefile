################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../Device_Startup/ \
../minilib


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../main.c \
../minilib/minilib.c


PREPROCESSING_SRCS +=  \
../minilib/minilib_asm.s


ASM_SRCS += 


OBJS +=  \
main.o \
minilib/minilib.o \
minilib/minilib_asm.o

OBJS_AS_ARGS +=  \
main.o \
minilib/minilib.o \
minilib/minilib_asm.o

C_DEPS +=  \
main.d \
minilib/minilib.d \
minilib/minilib_asm.d

C_DEPS_AS_ARGS +=  \
main.d \
minilib/minilib.d \
minilib/minilib_asm.d

OUTPUT_FILE_PATH +=MyApp.elf

OUTPUT_FILE_PATH_AS_ARGS +=MyApp.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+=  \
../device_startup/sam4sd32c_flash.ld \
../device_startup/sam4s_flash.ld


# AVR32/GNU C Compiler





./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAM4SD32C__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\cmsis\4.2.0\CMSIS\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAM4S_DFP\1.0.57\sam4s\include"  -O1 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

minilib/%.o: ../minilib/%.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAM4SD32C__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\cmsis\4.2.0\CMSIS\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAM4S_DFP\1.0.57\sam4s\include"  -O1 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler
minilib/minilib_asm.o: ../minilib/minilib_asm.s
	@echo Building file: $<
	@echo Invoking: ARM/GNU Assembler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-as.exe$(QUOTE) -mcpu=cortex-m4 -mthumb -I "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\cmsis\4.2.0\CMSIS\Include" -I "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAM4S_DFP\1.0.57\sam4s\include"  -g   -o "$@" "$<" 
	@echo Finished building: $<
	


minilib/%.o: ../minilib/%.s
	@echo Building file: $<
	@echo Invoking: ARM/GNU Assembler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-as.exe$(QUOTE) -mcpu=cortex-m4 -mthumb -I "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\cmsis\4.2.0\CMSIS\Include" -I "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAM4S_DFP\1.0.57\sam4s\include"  -g   -o "$@" "$<" 
	@echo Finished building: $<
	




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: ARM/GNU Linker : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="MyApp.map" -Wl,--start-group  -Wl,--end-group -L"..\\Device_Startup"  -Wl,--gc-sections -mcpu=cortex-m4 -Tsam4sd32c_flash.ld  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "MyApp.elf" "MyApp.bin"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "MyApp.elf" "MyApp.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "MyApp.elf" "MyApp.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "MyApp.elf" > "MyApp.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "MyApp.elf" "MyApp.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "MyApp.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "MyApp.elf" "MyApp.a" "MyApp.hex" "MyApp.bin" "MyApp.lss" "MyApp.eep" "MyApp.map" "MyApp.srec"
	