################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.c 

OBJS += \
./Drivers/CMSIS/system_stm32h7xx_dualcore_boot_cm4_cm7.o 

C_DEPS += \
./Drivers/CMSIS/system_stm32h7xx_dualcore_boot_cm4_cm7.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/system_stm32h7xx_dualcore_boot_cm4_cm7.o: C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.c Drivers/CMSIS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -I../../../CM7/AZURE_RTOS/App -I../../../CM7/USBX/App -I../../../CM7/USBX/Target -I../../../Middlewares/ST/usbx/common/core/inc/ -I../../../Middlewares/ST/usbx/ports/generic/inc/ -I../../../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../../../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../../../Middlewares/ST/threadx/common/inc/ -I../../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS

clean-Drivers-2f-CMSIS:
	-$(RM) ./Drivers/CMSIS/system_stm32h7xx_dualcore_boot_cm4_cm7.cyclo ./Drivers/CMSIS/system_stm32h7xx_dualcore_boot_cm4_cm7.d ./Drivers/CMSIS/system_stm32h7xx_dualcore_boot_cm4_cm7.o ./Drivers/CMSIS/system_stm32h7xx_dualcore_boot_cm4_cm7.su

.PHONY: clean-Drivers-2f-CMSIS

