################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freemaster/freemaster_appcmd.c \
../freemaster/freemaster_can.c \
../freemaster/freemaster_pdbdm.c \
../freemaster/freemaster_pipes.c \
../freemaster/freemaster_protocol.c \
../freemaster/freemaster_rec.c \
../freemaster/freemaster_scope.c \
../freemaster/freemaster_serial.c \
../freemaster/freemaster_sha.c \
../freemaster/freemaster_tsa.c \
../freemaster/freemaster_ures.c \
../freemaster/freemaster_utils.c 

OBJS += \
./freemaster/freemaster_appcmd.o \
./freemaster/freemaster_can.o \
./freemaster/freemaster_pdbdm.o \
./freemaster/freemaster_pipes.o \
./freemaster/freemaster_protocol.o \
./freemaster/freemaster_rec.o \
./freemaster/freemaster_scope.o \
./freemaster/freemaster_serial.o \
./freemaster/freemaster_sha.o \
./freemaster/freemaster_tsa.o \
./freemaster/freemaster_ures.o \
./freemaster/freemaster_utils.o 

C_DEPS += \
./freemaster/freemaster_appcmd.d \
./freemaster/freemaster_can.d \
./freemaster/freemaster_pdbdm.d \
./freemaster/freemaster_pipes.d \
./freemaster/freemaster_protocol.d \
./freemaster/freemaster_rec.d \
./freemaster/freemaster_scope.d \
./freemaster/freemaster_serial.d \
./freemaster/freemaster_sha.d \
./freemaster/freemaster_tsa.d \
./freemaster/freemaster_ures.d \
./freemaster/freemaster_utils.d 


# Each subdirectory must supply rules for building sources it contributes
freemaster/%.o: ../freemaster/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK66FN2M0VLQ18 -DCPU_MK66FN2M0VLQ18_cm4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=1 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSERIAL_PORT_TYPE_UART=1 -I"D:\MCUXpresso.work\CDK66_Ref2\device" -I"D:\MCUXpresso.work\CDK66_Ref2\drivers" -I"D:\MCUXpresso.work\CDK66_Ref2\component\lists" -I"D:\MCUXpresso.work\CDK66_Ref2\freemaster" -I"D:\MCUXpresso.work\CDK66_Ref2\freemaster\drivers\mcuxsdk\can" -I"D:\MCUXpresso.work\CDK66_Ref2\freemaster\drivers\mcuxsdk\serial" -I"D:\MCUXpresso.work\CDK66_Ref2\freemaster\platforms" -I"D:\MCUXpresso.work\CDK66_Ref2\CMSIS" -I"D:\MCUXpresso.work\CDK66_Ref2\source" -I"D:\MCUXpresso.work\CDK66_Ref2\drivers" -I"D:\MCUXpresso.work\CDK66_Ref2\source" -I"D:\MCUXpresso.work\CDK66_Ref2\device" -I"D:\MCUXpresso.work\CDK66_Ref2\CDK66" -I"D:\MCUXpresso.work\CDK66_Ref2\CMSIS" -I"D:\MCUXpresso.work\CDK66_Ref2\component\lists" -I"D:\MCUXpresso.work\CDK66_Ref2\board" -I"D:\MCUXpresso.work\CDK66_Ref2" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


