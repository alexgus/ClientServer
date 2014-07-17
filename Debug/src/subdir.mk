################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Client.cpp \
../src/ClientData.cpp \
../src/CmdLine.cpp \
../src/CmdOption.cpp \
../src/Com.cpp \
../src/Log.cpp \
../src/PythonModuleLoader.cpp \
../src/SCP.cpp \
../src/SQLite3.cpp \
../src/SendFile.cpp \
../src/Server.cpp \
../src/ServerCmdHandler.cpp 

OBJS += \
./src/Client.o \
./src/ClientData.o \
./src/CmdLine.o \
./src/CmdOption.o \
./src/Com.o \
./src/Log.o \
./src/PythonModuleLoader.o \
./src/SCP.o \
./src/SQLite3.o \
./src/SendFile.o \
./src/Server.o \
./src/ServerCmdHandler.o 

CPP_DEPS += \
./src/Client.d \
./src/ClientData.d \
./src/CmdLine.d \
./src/CmdOption.d \
./src/Com.d \
./src/Log.d \
./src/PythonModuleLoader.d \
./src/SCP.d \
./src/SQLite3.d \
./src/SendFile.d \
./src/Server.d \
./src/ServerCmdHandler.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -DDEBUG -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


