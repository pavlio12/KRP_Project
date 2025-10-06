################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/USBX/App/app_usbx_device.c \
C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/USBX/App/ux_device_cdc_acm.c \
C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/USBX/App/ux_device_descriptors.c \
C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/USBX/App/ux_device_mouse.c 

OBJS += \
./Application/User/USBX/App/app_usbx_device.o \
./Application/User/USBX/App/ux_device_cdc_acm.o \
./Application/User/USBX/App/ux_device_descriptors.o \
./Application/User/USBX/App/ux_device_mouse.o 

C_DEPS += \
./Application/User/USBX/App/app_usbx_device.d \
./Application/User/USBX/App/ux_device_cdc_acm.d \
./Application/User/USBX/App/ux_device_descriptors.d \
./Application/User/USBX/App/ux_device_mouse.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/USBX/App/app_usbx_device.o: C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/USBX/App/app_usbx_device.c Application/User/USBX/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -I../../../CM7/AZURE_RTOS/App -I../../../CM7/USBX/App -I../../../CM7/USBX/Target -I../../../Middlewares/ST/usbx/common/core/inc/ -I../../../Middlewares/ST/usbx/ports/generic/inc/ -I../../../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../../../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../../../Middlewares/ST/threadx/common/inc/ -I../../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/USBX/App/ux_device_cdc_acm.o: C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/USBX/App/ux_device_cdc_acm.c Application/User/USBX/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -I../../../CM7/AZURE_RTOS/App -I../../../CM7/USBX/App -I../../../CM7/USBX/Target -I../../../Middlewares/ST/usbx/common/core/inc/ -I../../../Middlewares/ST/usbx/ports/generic/inc/ -I../../../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../../../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../../../Middlewares/ST/threadx/common/inc/ -I../../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/USBX/App/ux_device_descriptors.o: C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/USBX/App/ux_device_descriptors.c Application/User/USBX/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -I../../../CM7/AZURE_RTOS/App -I../../../CM7/USBX/App -I../../../CM7/USBX/Target -I../../../Middlewares/ST/usbx/common/core/inc/ -I../../../Middlewares/ST/usbx/ports/generic/inc/ -I../../../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../../../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../../../Middlewares/ST/threadx/common/inc/ -I../../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/USBX/App/ux_device_mouse.o: C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/USBX/App/ux_device_mouse.c Application/User/USBX/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -I../../../CM7/AZURE_RTOS/App -I../../../CM7/USBX/App -I../../../CM7/USBX/Target -I../../../Middlewares/ST/usbx/common/core/inc/ -I../../../Middlewares/ST/usbx/ports/generic/inc/ -I../../../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../../../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../../../Middlewares/ST/threadx/common/inc/ -I../../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-USBX-2f-App

clean-Application-2f-User-2f-USBX-2f-App:
	-$(RM) ./Application/User/USBX/App/app_usbx_device.cyclo ./Application/User/USBX/App/app_usbx_device.d ./Application/User/USBX/App/app_usbx_device.o ./Application/User/USBX/App/app_usbx_device.su ./Application/User/USBX/App/ux_device_cdc_acm.cyclo ./Application/User/USBX/App/ux_device_cdc_acm.d ./Application/User/USBX/App/ux_device_cdc_acm.o ./Application/User/USBX/App/ux_device_cdc_acm.su ./Application/User/USBX/App/ux_device_descriptors.cyclo ./Application/User/USBX/App/ux_device_descriptors.d ./Application/User/USBX/App/ux_device_descriptors.o ./Application/User/USBX/App/ux_device_descriptors.su ./Application/User/USBX/App/ux_device_mouse.cyclo ./Application/User/USBX/App/ux_device_mouse.d ./Application/User/USBX/App/ux_device_mouse.o ./Application/User/USBX/App/ux_device_mouse.su

.PHONY: clean-Application-2f-User-2f-USBX-2f-App

