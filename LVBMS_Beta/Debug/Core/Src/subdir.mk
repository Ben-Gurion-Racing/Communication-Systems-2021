################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LVBMS_Beta_it.c \
../Core/Src/LVBMS_Beta_main.c \
../Core/Src/LVBMS_Beta_msp.c \
../Core/Src/syscalls.c \
../Core/Src/system_stm32f3xx.c 

OBJS += \
./Core/Src/LVBMS_Beta_it.o \
./Core/Src/LVBMS_Beta_main.o \
./Core/Src/LVBMS_Beta_msp.o \
./Core/Src/syscalls.o \
./Core/Src/system_stm32f3xx.o 

C_DEPS += \
./Core/Src/LVBMS_Beta_it.d \
./Core/Src/LVBMS_Beta_main.d \
./Core/Src/LVBMS_Beta_msp.d \
./Core/Src/syscalls.d \
./Core/Src/system_stm32f3xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F303x8 -I"/Users/johnnydekel/Desktop/stmProjects/LVBMS_Beta/Core/Inc" -I"/Users/johnnydekel/Desktop/stmProjects/LVBMS_Beta/Drivers/STM32F3xx_HAL_Driver/Inc" -I"/Users/johnnydekel/Desktop/stmProjects/LVBMS_Beta/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"/Users/johnnydekel/Desktop/stmProjects/LVBMS_Beta/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"/Users/johnnydekel/Desktop/stmProjects/LVBMS_Beta/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


