################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/main.o \
../src/xmldom.o \
../src/xmlmessagehandle.o 

CPP_SRCS += \
../src/main.cpp \
../src/xmldom.cpp \
../src/xmlmessagehandle.cpp 

OBJS += \
./src/main.o \
./src/xmldom.o \
./src/xmlmessagehandle.o 

CPP_DEPS += \
./src/main.d \
./src/xmldom.d \
./src/xmlmessagehandle.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../include -I/usr/include -I/usr/include/libxml2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


