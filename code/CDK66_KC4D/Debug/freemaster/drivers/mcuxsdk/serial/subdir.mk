################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freemaster/drivers/mcuxsdk/serial/freemaster_serial_lpsci.c \
../freemaster/drivers/mcuxsdk/serial/freemaster_serial_lpuart.c \
../freemaster/drivers/mcuxsdk/serial/freemaster_serial_miniusart.c \
../freemaster/drivers/mcuxsdk/serial/freemaster_serial_uart.c \
../freemaster/drivers/mcuxsdk/serial/freemaster_serial_usart.c \
../freemaster/drivers/mcuxsdk/serial/freemaster_serial_usb.c 

OBJS += \
./freemaster/drivers/mcuxsdk/serial/freemaster_serial_lpsci.o \
./freemaster/drivers/mcuxsdk/serial/freemaster_serial_lpuart.o \
./freemaster/drivers/mcuxsdk/serial/freemaster_serial_miniusart.o \
./freemaster/drivers/mcuxsdk/serial/freemaster_serial_uart.o \
./freemaster/drivers/mcuxsdk/serial/freemaster_serial_usart.o \
./freemaster/drivers/mcuxsdk/serial/freemaster_serial_usb.o 

C_DEPS += \
./freemaster/drivers/mcuxsdk/serial/freemaster_serial_lpsci.d \
./freemaster/drivers/mcuxsdk/serial/freemaster_serial_lpuart.d \
./freemaster/drivers/mcuxsdk/serial/freemaster_serial_miniusart.d \
./freemaster/drivers/mcuxsdk/serial/freemaster_serial_uart.d \
./freemaster/drivers/mcuxsdk/serial/freemaster_serial_usart.d \
./freemaster/drivers/mcuxsdk/serial/freemaster_serial_usb.d 


# Each subdirectory must supply rules for building sources it contributes
freemaster/drivers/mcuxsdk/serial/%.o: ../freemaster/drivers/mcuxsdk/serial/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK66FN2M0VLQ18 -DCPU_MK66FN2M0VLQ18_cm4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=1 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSERIAL_PORT_TYPE_UART=1 -I"D:\MCUXpresso.work\CDK66_Ref2\device" -I"D:\MCUXpresso.work\CDK66_Ref2\drivers" -I"D:\MCUXpresso.work\CDK66_Ref2\component\lists" -I"D:\MCUXpresso.work\CDK66_Ref2\freemaster" -I"D:\MCUXpresso.work\CDK66_Ref2\freemaster\drivers\mcuxsdk\can" -I"D:\MCUXpresso.work\CDK66_Ref2\freemaster\drivers\mcuxsdk\serial" -I"D:\MCUXpresso.work\CDK66_Ref2\freemaster\platforms" -I"D:\MCUXpresso.work\CDK66_Ref2\CMSIS" -I"D:\MCUXpresso.work\CDK66_Ref2\source" -I"D:\MCUXpresso.work\CDK66_Ref2\drivers" -I"D:\MCUXpresso.work\CDK66_Ref2\source" -I"D:\MCUXpresso.work\CDK66_Ref2\device" -I"D:\MCUXpresso.work\CDK66_Ref2\CDK66" -I"D:\MCUXpresso.work\CDK66_Ref2\CMSIS" -I"D:\MCUXpresso.work\CDK66_Ref2\component\lists" -I"D:\MCUXpresso.work\CDK66_Ref2\board" -I"D:\MCUXpresso.work\CDK66_Ref2" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


