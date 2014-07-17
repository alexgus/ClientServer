################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/iniparseur/INIReader.cpp 

C_SRCS += \
../src/iniparseur/ini.c 

OBJS += \
./src/iniparseur/INIReader.o \
./src/iniparseur/ini.o 

C_DEPS += \
./src/iniparseur/ini.d 

CPP_DEPS += \
./src/iniparseur/INIReader.d 


# Each subdirectory must supply rules for building sources it contributes
src/iniparseur/%.o: ../src/iniparseur/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -DDEBUG -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/iniparseur/%.o: ../src/iniparseur/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


