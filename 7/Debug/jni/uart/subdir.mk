################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../jni/uart/ProtocolParser.cpp \
../jni/uart/ProtocolSender.cpp \
../jni/uart/UartContext.cpp 

OBJS += \
./jni/uart/ProtocolParser.o \
./jni/uart/ProtocolSender.o \
./jni/uart/UartContext.o 

CPP_DEPS += \
./jni/uart/ProtocolParser.d \
./jni/uart/ProtocolSender.d \
./jni/uart/UartContext.d 


# Each subdirectory must supply rules for building sources it contributes
jni/uart/%.o: ../jni/uart/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\flythings\bin\/../sdk/platforms/z6s/toolchain/include" -I"C:\flythings\bin\/../sdk/platforms/z6s/prebuilt/include" -I"C:\flythings\bin\/../sdk/platforms/z6s/prebuilt/include/uClibc++" -I"C:\flythings\bin\/../sdk/platforms/z6s/prebuilt/include/gui" -I"C:\Users\live\Downloads\w64devkit-1.17.0\w64devkit" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


