################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../jni/activity/gcodeActivity.cpp \
../jni/activity/mainActivity.cpp \
../jni/activity/navibar.cpp \
../jni/activity/statusbar.cpp 

OBJS += \
./jni/activity/gcodeActivity.o \
./jni/activity/mainActivity.o \
./jni/activity/navibar.o \
./jni/activity/statusbar.o 

CPP_DEPS += \
./jni/activity/gcodeActivity.d \
./jni/activity/mainActivity.d \
./jni/activity/navibar.d \
./jni/activity/statusbar.d 


# Each subdirectory must supply rules for building sources it contributes
jni/activity/%.o: ../jni/activity/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\flythings\bin\/../sdk/platforms/z6s/toolchain/include" -I"C:\flythings\bin\/../sdk/platforms/z6s/prebuilt/include" -I"C:\flythings\bin\/../sdk/platforms/z6s/prebuilt/include/uClibc++" -I"C:\flythings\bin\/../sdk/platforms/z6s/prebuilt/include/gui" -I"C:\Users\live\Downloads\w64devkit-1.17.0\w64devkit" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


