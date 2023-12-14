################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Final_project.c \
../source/bmp280.c \
../source/bmp280_test.c \
../source/cbfifo.c \
../source/cmd_process.c \
../source/i2c.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/sysclock.c \
../source/uart0.c 

C_DEPS += \
./source/Final_project.d \
./source/bmp280.d \
./source/bmp280_test.d \
./source/cbfifo.d \
./source/cmd_process.d \
./source/i2c.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/sysclock.d \
./source/uart0.d 

OBJS += \
./source/Final_project.o \
./source/bmp280.o \
./source/bmp280_test.o \
./source/cbfifo.o \
./source/cmd_process.o \
./source/i2c.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/sysclock.o \
./source/uart0.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\91912\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Final_project\board" -I"C:\Users\91912\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Final_project\source" -I"C:\Users\91912\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Final_project" -I"C:\Users\91912\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Final_project\drivers" -I"C:\Users\91912\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Final_project\CMSIS" -I"C:\Users\91912\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Final_project\utilities" -I"C:\Users\91912\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Final_project\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/Final_project.d ./source/Final_project.o ./source/bmp280.d ./source/bmp280.o ./source/bmp280_test.d ./source/bmp280_test.o ./source/cbfifo.d ./source/cbfifo.o ./source/cmd_process.d ./source/cmd_process.o ./source/i2c.d ./source/i2c.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/sysclock.d ./source/sysclock.o ./source/uart0.d ./source/uart0.o

.PHONY: clean-source

