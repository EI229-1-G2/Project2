################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/freertos_kernel/croutine.c \
../freertos/freertos_kernel/event_groups.c \
../freertos/freertos_kernel/list.c \
../freertos/freertos_kernel/queue.c \
../freertos/freertos_kernel/stream_buffer.c \
../freertos/freertos_kernel/tasks.c \
../freertos/freertos_kernel/timers.c 

OBJS += \
./freertos/freertos_kernel/croutine.o \
./freertos/freertos_kernel/event_groups.o \
./freertos/freertos_kernel/list.o \
./freertos/freertos_kernel/queue.o \
./freertos/freertos_kernel/stream_buffer.o \
./freertos/freertos_kernel/tasks.o \
./freertos/freertos_kernel/timers.o 

C_DEPS += \
./freertos/freertos_kernel/croutine.d \
./freertos/freertos_kernel/event_groups.d \
./freertos/freertos_kernel/list.d \
./freertos/freertos_kernel/queue.d \
./freertos/freertos_kernel/stream_buffer.d \
./freertos/freertos_kernel/tasks.d \
./freertos/freertos_kernel/timers.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/freertos_kernel/%.o: ../freertos/freertos_kernel/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK66FN2M0VLQ18 -DCPU_MK66FN2M0VLQ18_cm4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=1 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSERIAL_PORT_TYPE_UART=1 -I"D:\MCUXpresso.work\CDK66_Ref1\drivers" -I"D:\MCUXpresso.work\CDK66_Ref1\component\serial_manager" -I"D:\MCUXpresso.work\CDK66_Ref1\utilities" -I"D:\MCUXpresso.work\CDK66_Ref1\source" -I"D:\MCUXpresso.work\CDK66_Ref1\device" -I"D:\MCUXpresso.work\CDK66_Ref1\component\uart" -I"D:\MCUXpresso.work\CDK66_Ref1\drivers\freertos" -I"D:\MCUXpresso.work\CDK66_Ref1\CMSIS" -I"D:\MCUXpresso.work\CDK66_Ref1\freertos\freertos_kernel\include" -I"D:\MCUXpresso.work\CDK66_Ref1\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\MCUXpresso.work\CDK66_Ref1\component\lists" -I"D:\MCUXpresso.work\CDK66_Ref1\board" -I"D:\MCUXpresso.work\CDK66_Ref1\source" -I"D:\MCUXpresso.work\CDK66_Ref1" -I"D:\MCUXpresso.work\CDK66_Ref1\freertos\freertos_kernel\include" -I"D:\MCUXpresso.work\CDK66_Ref1\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\MCUXpresso.work\CDK66_Ref1\drivers" -I"D:\MCUXpresso.work\CDK66_Ref1\device" -I"D:\MCUXpresso.work\CDK66_Ref1\CMSIS" -I"D:\MCUXpresso.work\CDK66_Ref1\drivers\freertos" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


