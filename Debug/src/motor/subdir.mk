################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/motor/bsp_motor.c \
../src/motor/bsp_motor_gpio.c \
../src/motor/bsp_motor_key.c \
../src/motor/bsp_pump.c 

C_DEPS += \
./src/motor/bsp_motor.d \
./src/motor/bsp_motor_gpio.d \
./src/motor/bsp_motor_key.d \
./src/motor/bsp_pump.d 

OBJS += \
./src/motor/bsp_motor.o \
./src/motor/bsp_motor_gpio.o \
./src/motor/bsp_motor_key.o \
./src/motor/bsp_pump.o 

SREC += \
05_Template.srec 

MAP += \
05_Template.map 


# Each subdirectory must supply rules for building sources it contributes
src/motor/%.o: ../src/motor/%.c
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -gdwarf-4 -D_RENESAS_RA_ -D_RA_CORE=CM33 -I"E:/E2try/src" -I"E:/E2try/ra/fsp/inc" -I"E:/E2try/ra/fsp/inc/api" -I"E:/E2try/ra/fsp/inc/instances" -I"E:/E2try/ra/arm/CMSIS_5/CMSIS/Core/Include" -I"E:/E2try/ra_gen" -I"E:/E2try/ra_cfg/fsp_cfg/bsp" -I"E:/E2try/ra_cfg/fsp_cfg" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

