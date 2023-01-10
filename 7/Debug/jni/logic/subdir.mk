################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../jni/logic/gcodeLogic.cc \
../jni/logic/mainLogic.cc \
../jni/logic/navibar.cc \
../jni/logic/statusbar.cc 

CC_DEPS += \
./jni/logic/gcodeLogic.d \
./jni/logic/mainLogic.d \
./jni/logic/navibar.d \
./jni/logic/statusbar.d 

OBJS += \
./jni/logic/gcodeLogic.o \
./jni/logic/mainLogic.o \
./jni/logic/navibar.o \
./jni/logic/statusbar.o 


# Each subdirectory must supply rules for building sources it contributes
jni/logic/%.o: ../jni/logic/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\flythings\bin\/../sdk/platforms/z6s/toolchain/include" -I"C:\flythings\bin\/../sdk/platforms/z6s/prebuilt/include" -I"C:\flythings\bin\/../sdk/platforms/z6s/prebuilt/include/uClibc++" -I"C:\flythings\bin\/../sdk/platforms/z6s/prebuilt/include/gui" -I"C:\Users\live\Downloads\w64devkit-1.17.0\w64devkit" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


