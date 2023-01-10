################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
null./jni/include/ObjectModel/PrinterStatus.d 


# Each subdirectory must supply rules for building sources it contributes
jni/include/ObjectModel/PrinterStatus: ../jni/include/ObjectModel/PrinterStatus.hpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"jni/include/ObjectModel/PrinterStatus.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


