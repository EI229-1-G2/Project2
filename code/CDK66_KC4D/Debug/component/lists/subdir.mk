################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/lists/generic_list.c 

OBJS += \
./component/lists/generic_list.o 

C_DEPS += \
./component/lists/generic_list.d 


# Each subdirectory must supply rules for building sources it contributes
component/lists/%.o: ../component/lists/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK66FN2M0VLQ18 -DCPU_MK66FN2M0VLQ18_cm4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DPRINTF_FLOAT_ENABLE=1 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSERIAL_PORT_TYPE_UART=1 -I"D:\Users\胡小雨\Documents\GitHub\Project2\code\CDK66_KC4D\device" -I"D:\Users\胡小雨\Documents\GitHub\Project2\code\CDK66_KC4D\drivers" -I"D:\Users\胡小雨\Documents\GitHub\Project2\code\CDK66_KC4D\component\lists" -I"D:\Users\胡小雨\Documents\GitHub\Project2\code\CDK66_KC4D\CMSIS" -I"D:\Users\胡小雨\Documents\GitHub\Project2\code\CDK66_KC4D\source" -I"D:\Users\胡小雨\Documents\GitHub\Project2\code\CDK66_KC4D\CDK66" -I"D:\Users\胡小雨\Documents\GitHub\Project2\code\CDK66_KC4D\board" -I"D:\Users\胡小雨\Documents\GitHub\Project2\code\CDK66_KC4D" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


