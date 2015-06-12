################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USART.c \
../dallas_one_wire.c \
../dallas_paku_lib.c \
../lcd_lib.c \
../main.c 

C_DEPS += \
./USART.d \
./dallas_one_wire.d \
./dallas_paku_lib.d \
./lcd_lib.d \
./main.d 

OBJS += \
./USART.o \
./dallas_one_wire.o \
./dallas_paku_lib.o \
./lcd_lib.o \
./main.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


