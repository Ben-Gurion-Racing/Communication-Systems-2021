################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/src/apps/mqtt/mqtt.c 

OBJS += \
./Middlewares/Third_Party/LwIP/src/apps/mqtt/mqtt.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/src/apps/mqtt/mqtt.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/src/apps/mqtt/%.o: ../Middlewares/Third_Party/LwIP/src/apps/mqtt/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F767xx -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Inc" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Drivers/STM32F7xx_HAL_Driver/Inc" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Drivers/CMSIS/Include" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Middlewares/Third_Party/LwIP/src/include" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Middlewares/Third_Party/LwIP/system" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Middlewares/Third_Party/LwIP/src/include/netif/ppp" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Middlewares/Third_Party/LwIP/src/include/lwip" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Middlewares/Third_Party/LwIP/src/include/lwip/apps" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Middlewares/Third_Party/LwIP/src/include/lwip/priv" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Middlewares/Third_Party/LwIP/src/include/lwip/prot" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Middlewares/Third_Party/LwIP/src/include/netif" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Middlewares/Third_Party/LwIP/src/include/posix" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Middlewares/Third_Party/LwIP/src/include/posix/sys" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Middlewares/Third_Party/LwIP/system/arch" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Drivers/CMSIS/Include" -I"/Users/johnnydekel/Desktop/stmProjects/ECU_beta/Drivers/CMSIS/Include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


