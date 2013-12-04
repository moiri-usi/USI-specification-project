################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../main.cpp \
../rl_dec.cpp \
../rl_dec_RTL.cpp \
../rl_enc_RTL.cpp 

OBJS += \
./main.o \
./rl_dec.o \
./rl_dec_RTL.o \
./rl_enc_RTL.o 

CPP_DEPS += \
./main.d \
./rl_dec.d \
./rl_dec_RTL.d \
./rl_enc_RTL.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jotroick/workspace/modules -I/home/jotroick/workspace/add2systemc -I/home/jotroick/Documents/systemc-2.3.0/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


