################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Pseudo_ECU_it.c \
../Core/Src/Pseudo_ECU_main.c \
../Core/Src/Pseudo_ECU_msp.c \
../Core/Src/syscalls.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/Pseudo_ECU_it.o \
./Core/Src/Pseudo_ECU_main.o \
./Core/Src/Pseudo_ECU_msp.o \
./Core/Src/syscalls.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/Pseudo_ECU_it.d \
./Core/Src/Pseudo_ECU_main.d \
./Core/Src/Pseudo_ECU_msp.d \
./Core/Src/syscalls.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F446xx -I"/Users/johnnydekel/Desktop/stmProjects/Pseudo_ECU/Core/Inc" -I"/Users/johnnydekel/Desktop/stmProjects/Pseudo_ECU/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/Users/johnnydekel/Desktop/stmProjects/Pseudo_ECU/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"/Users/johnnydekel/Desktop/stmProjects/Pseudo_ECU/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/Users/johnnydekel/Desktop/stmProjects/Pseudo_ECU/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


