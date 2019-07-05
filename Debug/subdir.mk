################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AnimatedTexture.cpp \
../AssetLoader.cpp \
../SnakeGame.cpp \
../SnakeMenu.cpp \
../SnakeTextAnimation.cpp \
../SnakeTextAnimationHelper.cpp \
../global.cpp \
../main.cpp 

OBJS += \
./AnimatedTexture.o \
./AssetLoader.o \
./SnakeGame.o \
./SnakeMenu.o \
./SnakeTextAnimation.o \
./SnakeTextAnimationHelper.o \
./global.o \
./main.o 

CPP_DEPS += \
./AnimatedTexture.d \
./AssetLoader.d \
./SnakeGame.d \
./SnakeMenu.d \
./SnakeTextAnimation.d \
./SnakeTextAnimationHelper.d \
./global.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/SDL2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


