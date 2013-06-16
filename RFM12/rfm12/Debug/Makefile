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

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../commands/


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../commands/Command.cpp \
../commands/DataRateCommand.cpp \
../commands/FrequencyCommand.cpp \
../Rfm12.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
commands/Command.o \
commands/DataRateCommand.o \
commands/FrequencyCommand.o \
Rfm12.o

OBJS_AS_ARGS +=  \
commands/Command.o \
commands/DataRateCommand.o \
commands/FrequencyCommand.o \
Rfm12.o

C_DEPS +=  \
commands/Command.d \
commands/DataRateCommand.d \
commands/FrequencyCommand.d \
Rfm12.d

C_DEPS_AS_ARGS +=  \
commands/Command.d \
commands/DataRateCommand.d \
commands/FrequencyCommand.d \
Rfm12.d

OUTPUT_FILE_PATH +=librfm12.a

OUTPUT_FILE_PATH_AS_ARGS +=librfm12.a

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler









commands/%.o: ../commands/%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DDEBUG  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega328p -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	

./%.o: .././%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DDEBUG  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega328p -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)


$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP)
	@echo Building target: $@
	@echo Invoking: AVR8/GNU Archiver : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-ar.exe$(QUOTE) -r  -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS)
	@echo Finished building target: $@
	
	




# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "librfm12.elf" "librfm12.a" "librfm12.hex" "librfm12.lss" "librfm12.eep" "librfm12.map" "librfm12.srec"
	