################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/rm_zmod4xxx/rm_zmod4xxx.c \
../ra/fsp/src/rm_zmod4xxx/rm_zmod4xxx_ra_driver.c 

C_DEPS += \
./ra/fsp/src/rm_zmod4xxx/rm_zmod4xxx.d \
./ra/fsp/src/rm_zmod4xxx/rm_zmod4xxx_ra_driver.d 

CREF += \
btl4.cref 

OBJS += \
./ra/fsp/src/rm_zmod4xxx/rm_zmod4xxx.o \
./ra/fsp/src/rm_zmod4xxx/rm_zmod4xxx_ra_driver.o 

MAP += \
btl4.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/rm_zmod4xxx/%.o: ../ra/fsp/src/rm_zmod4xxx/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mlittle-endian -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -ffunction-sections -fdata-sections -fno-strict-aliasing -fmessage-length=0 -funsigned-char -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -Wno-parentheses-equality -Wfloat-equal -g3 -std=c99 -fshort-enums -fno-unroll-loops -I"D:\\LTHTN\\e2Studio\\Workspace1\\btl4\\src" -I"." -I"D:\\LTHTN\\e2Studio\\Workspace1\\btl4\\ra\\fsp\\inc" -I"D:\\LTHTN\\e2Studio\\Workspace1\\btl4\\ra\\fsp\\inc\\api" -I"D:\\LTHTN\\e2Studio\\Workspace1\\btl4\\ra\\fsp\\inc\\instances" -I"D:\\LTHTN\\e2Studio\\Workspace1\\btl4\\ra\\arm\\CMSIS_6\\CMSIS\\Core\\Include" -I"D:\\LTHTN\\e2Studio\\Workspace1\\btl4\\ra_gen" -I"D:\\LTHTN\\e2Studio\\Workspace1\\btl4\\ra_cfg\\fsp_cfg\\bsp" -I"D:\\LTHTN\\e2Studio\\Workspace1\\btl4\\ra_cfg\\fsp_cfg" -D_RENESAS_RA_ -D_RA_CORE=CM33 -D_RA_ORDINAL=1 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -x c "$<" -c -o "$@")
	@clang --target=arm-none-eabi @"$@.in"

