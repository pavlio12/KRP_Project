################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/app_threadx.c \
C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/dma.c \
C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/gpio.c \
C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/main.c \
C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/stm32h7xx_hal_msp.c \
C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/stm32h7xx_hal_timebase_tim.c \
C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/stm32h7xx_it.c \
../Application/User/Core/syscalls.c \
../Application/User/Core/sysmem.c \
C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/usart.c \
C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/usb_otg.c 

S_UPPER_SRCS += \
C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/tx_initialize_low_level.S 

OBJS += \
./Application/User/Core/app_threadx.o \
./Application/User/Core/dma.o \
./Application/User/Core/gpio.o \
./Application/User/Core/main.o \
./Application/User/Core/stm32h7xx_hal_msp.o \
./Application/User/Core/stm32h7xx_hal_timebase_tim.o \
./Application/User/Core/stm32h7xx_it.o \
./Application/User/Core/syscalls.o \
./Application/User/Core/sysmem.o \
./Application/User/Core/tx_initialize_low_level.o \
./Application/User/Core/usart.o \
./Application/User/Core/usb_otg.o 

S_UPPER_DEPS += \
./Application/User/Core/tx_initialize_low_level.d 

C_DEPS += \
./Application/User/Core/app_threadx.d \
./Application/User/Core/dma.d \
./Application/User/Core/gpio.d \
./Application/User/Core/main.d \
./Application/User/Core/stm32h7xx_hal_msp.d \
./Application/User/Core/stm32h7xx_hal_timebase_tim.d \
./Application/User/Core/stm32h7xx_it.d \
./Application/User/Core/syscalls.d \
./Application/User/Core/sysmem.d \
./Application/User/Core/usart.d \
./Application/User/Core/usb_otg.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/Core/app_threadx.o: C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/app_threadx.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -I../../../CM7/AZURE_RTOS/App -I../../../CM7/USBX/App -I../../../CM7/USBX/Target -I../../../Middlewares/ST/usbx/common/core/inc/ -I../../../Middlewares/ST/usbx/ports/generic/inc/ -I../../../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../../../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../../../Middlewares/ST/threadx/common/inc/ -I../../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/Core/dma.o: C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/dma.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -I../../../CM7/AZURE_RTOS/App -I../../../CM7/USBX/App -I../../../CM7/USBX/Target -I../../../Middlewares/ST/usbx/common/core/inc/ -I../../../Middlewares/ST/usbx/ports/generic/inc/ -I../../../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../../../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../../../Middlewares/ST/threadx/common/inc/ -I../../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/Core/gpio.o: C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/gpio.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -I../../../CM7/AZURE_RTOS/App -I../../../CM7/USBX/App -I../../../CM7/USBX/Target -I../../../Middlewares/ST/usbx/common/core/inc/ -I../../../Middlewares/ST/usbx/ports/generic/inc/ -I../../../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../../../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../../../Middlewares/ST/threadx/common/inc/ -I../../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/Core/main.o: C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/main.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -I../../../CM7/AZURE_RTOS/App -I../../../CM7/USBX/App -I../../../CM7/USBX/Target -I../../../Middlewares/ST/usbx/common/core/inc/ -I../../../Middlewares/ST/usbx/ports/generic/inc/ -I../../../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../../../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../../../Middlewares/ST/threadx/common/inc/ -I../../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/Core/stm32h7xx_hal_msp.o: C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/stm32h7xx_hal_msp.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -I../../../CM7/AZURE_RTOS/App -I../../../CM7/USBX/App -I../../../CM7/USBX/Target -I../../../Middlewares/ST/usbx/common/core/inc/ -I../../../Middlewares/ST/usbx/ports/generic/inc/ -I../../../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../../../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../../../Middlewares/ST/threadx/common/inc/ -I../../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/Core/stm32h7xx_hal_timebase_tim.o: C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/stm32h7xx_hal_timebase_tim.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -I../../../CM7/AZURE_RTOS/App -I../../../CM7/USBX/App -I../../../CM7/USBX/Target -I../../../Middlewares/ST/usbx/common/core/inc/ -I../../../Middlewares/ST/usbx/ports/generic/inc/ -I../../../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../../../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../../../Middlewares/ST/threadx/common/inc/ -I../../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/Core/stm32h7xx_it.o: C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/stm32h7xx_it.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -I../../../CM7/AZURE_RTOS/App -I../../../CM7/USBX/App -I../../../CM7/USBX/Target -I../../../Middlewares/ST/usbx/common/core/inc/ -I../../../Middlewares/ST/usbx/ports/generic/inc/ -I../../../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../../../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../../../Middlewares/ST/threadx/common/inc/ -I../../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/Core/%.o Application/User/Core/%.su Application/User/Core/%.cyclo: ../Application/User/Core/%.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -I../../../CM7/AZURE_RTOS/App -I../../../CM7/USBX/App -I../../../CM7/USBX/Target -I../../../Middlewares/ST/usbx/common/core/inc/ -I../../../Middlewares/ST/usbx/ports/generic/inc/ -I../../../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../../../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../../../Middlewares/ST/threadx/common/inc/ -I../../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/Core/tx_initialize_low_level.o: C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/tx_initialize_low_level.S Application/User/Core/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -DTX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Application/User/Core/usart.o: C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/usart.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -I../../../CM7/AZURE_RTOS/App -I../../../CM7/USBX/App -I../../../CM7/USBX/Target -I../../../Middlewares/ST/usbx/common/core/inc/ -I../../../Middlewares/ST/usbx/ports/generic/inc/ -I../../../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../../../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../../../Middlewares/ST/threadx/common/inc/ -I../../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/Core/usb_otg.o: C:/Users/oopav/KRP/Ux_Device_HID_CDC_ACM/CM7/Core/Src/usb_otg.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../../../CM7/Core/Inc -I../../../CM7/AZURE_RTOS/App -I../../../CM7/USBX/App -I../../../CM7/USBX/Target -I../../../Middlewares/ST/usbx/common/core/inc/ -I../../../Middlewares/ST/usbx/ports/generic/inc/ -I../../../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../../../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../../../Middlewares/ST/threadx/common/inc/ -I../../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-Core

clean-Application-2f-User-2f-Core:
	-$(RM) ./Application/User/Core/app_threadx.cyclo ./Application/User/Core/app_threadx.d ./Application/User/Core/app_threadx.o ./Application/User/Core/app_threadx.su ./Application/User/Core/dma.cyclo ./Application/User/Core/dma.d ./Application/User/Core/dma.o ./Application/User/Core/dma.su ./Application/User/Core/gpio.cyclo ./Application/User/Core/gpio.d ./Application/User/Core/gpio.o ./Application/User/Core/gpio.su ./Application/User/Core/main.cyclo ./Application/User/Core/main.d ./Application/User/Core/main.o ./Application/User/Core/main.su ./Application/User/Core/stm32h7xx_hal_msp.cyclo ./Application/User/Core/stm32h7xx_hal_msp.d ./Application/User/Core/stm32h7xx_hal_msp.o ./Application/User/Core/stm32h7xx_hal_msp.su ./Application/User/Core/stm32h7xx_hal_timebase_tim.cyclo ./Application/User/Core/stm32h7xx_hal_timebase_tim.d ./Application/User/Core/stm32h7xx_hal_timebase_tim.o ./Application/User/Core/stm32h7xx_hal_timebase_tim.su ./Application/User/Core/stm32h7xx_it.cyclo ./Application/User/Core/stm32h7xx_it.d ./Application/User/Core/stm32h7xx_it.o ./Application/User/Core/stm32h7xx_it.su ./Application/User/Core/syscalls.cyclo ./Application/User/Core/syscalls.d ./Application/User/Core/syscalls.o ./Application/User/Core/syscalls.su ./Application/User/Core/sysmem.cyclo ./Application/User/Core/sysmem.d ./Application/User/Core/sysmem.o ./Application/User/Core/sysmem.su ./Application/User/Core/tx_initialize_low_level.d ./Application/User/Core/tx_initialize_low_level.o ./Application/User/Core/usart.cyclo ./Application/User/Core/usart.d ./Application/User/Core/usart.o ./Application/User/Core/usart.su ./Application/User/Core/usb_otg.cyclo ./Application/User/Core/usb_otg.d ./Application/User/Core/usb_otg.o ./Application/User/Core/usb_otg.su

.PHONY: clean-Application-2f-User-2f-Core

