################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/New_Pedals_it.c \
../Core/Src/New_Pedals_main.c \
../Core/Src/New_Pedals_msp.c \
../Core/Src/syscalls.c \
../Core/Src/system_stm32f3xx.c 

OBJS += \
./Core/Src/New_Pedals_it.o \
./Core/Src/New_Pedals_main.o \
./Core/Src/New_Pedals_msp.o \
./Core/Src/syscalls.o \
./Core/Src/system_stm32f3xx.o 

C_DEPS += \
./Core/Src/New_Pedals_it.d \
./Core/Src/New_Pedals_main.d \
./Core/Src/New_Pedals_msp.d \
./Core/Src/syscalls.d \
./Core/Src/system_stm32f3xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F303x8 -I"/Users/johnnydekel/Desktop/stmProjects/New_Pedals/Core/Inc" -I"/Users/johnnydekel/Desktop/stmProjects/New_Pedals/Drivers/STM32F3xx_HAL_Driver/Inc" -I"/Users/johnnydekel/Desktop/stmProjects/New_Pedals/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"/Users/johnnydekel/Desktop/stmProjects/New_Pedals/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"/Users/johnnydekel/Desktop/stmProjects/New_Pedals/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


