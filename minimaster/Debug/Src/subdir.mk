################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/it.c \
../Src/main.c \
../Src/msp.c \
../Src/syscalls.c \
../Src/system_stm32f3xx.c 

OBJS += \
./Src/it.o \
./Src/main.o \
./Src/msp.o \
./Src/syscalls.o \
./Src/system_stm32f3xx.o 

C_DEPS += \
./Src/it.d \
./Src/main.d \
./Src/msp.d \
./Src/syscalls.d \
./Src/system_stm32f3xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F303x8 -I"/Users/johnnydekel/Desktop/stmProjects/minimaster/Inc" -I"/Users/johnnydekel/Desktop/stmProjects/minimaster/Drivers/STM32F3xx_HAL_Driver/Inc" -I"/Users/johnnydekel/Desktop/stmProjects/minimaster/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"/Users/johnnydekel/Desktop/stmProjects/minimaster/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"/Users/johnnydekel/Desktop/stmProjects/minimaster/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


