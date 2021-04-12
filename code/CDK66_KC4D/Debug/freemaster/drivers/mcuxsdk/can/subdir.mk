################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freemaster/drivers/mcuxsdk/can/freemaster_flexcan.c \
../freemaster/drivers/mcuxsdk/can/freemaster_mcan.c \
../freemaster/drivers/mcuxsdk/can/freemaster_mscan.c 

OBJS += \
./freemaster/drivers/mcuxsdk/can/freemaster_flexcan.o \
./freemaster/drivers/mcuxsdk/can/freemaster_mcan.o \
./freemaster/drivers/mcuxsdk/can/freemaster_mscan.o 

C_DEPS += \
./freemaster/drivers/mcuxsdk/can/freemaster_flexcan.d \
./freemaster/drivers/mcuxsdk/can/freemaster_mcan.d \
./freemaster/drivers/mcuxsdk/can/freemaster_mscan.d 


# Each subdirectory must supply rules for building sources it contributes
freemaster/drivers/mcuxsdk/can/%.o: ../freemaster/drivers/mcuxsdk/can/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK66FN2M0VLQ18 -DCPU_MK66FN2M0VLQ18_cm4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=1 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSERIAL_PORT_TYPE_UART=1 -I"D:\MCUXpresso.work\CDK66_Ref2\device" -I"D:\MCUXpresso.work\CDK66_Ref2\drivers" -I"D:\MCUXpresso.work\CDK66_Ref2\component\lists" -I"D:\MCUXpresso.work\CDK66_Ref2\freemaster" -I"D:\MCUXpresso.work\CDK66_Ref2\freemaster\drivers\mcuxsdk\can" -I"D:\MCUXpresso.work\CDK66_Ref2\freemaster\drivers\mcuxsdk\serial" -I"D:\MCUXpresso.work\CDK66_Ref2\freemaster\platforms" -I"D:\MCUXpresso.work\CDK66_Ref2\CMSIS" -I"D:\MCUXpresso.work\CDK66_Ref2\source" -I"D:\MCUXpresso.work\CDK66_Ref2\drivers" -I"D:\MCUXpresso.work\CDK66_Ref2\source" -I"D:\MCUXpresso.work\CDK66_Ref2\device" -I"D:\MCUXpresso.work\CDK66_Ref2\CDK66" -I"D:\MCUXpresso.work\CDK66_Ref2\CMSIS" -I"D:\MCUXpresso.work\CDK66_Ref2\component\lists" -I"D:\MCUXpresso.work\CDK66_Ref2\board" -I"D:\MCUXpresso.work\CDK66_Ref2" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


