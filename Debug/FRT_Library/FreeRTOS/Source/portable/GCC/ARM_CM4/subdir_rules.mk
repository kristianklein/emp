################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
FRT_Library/FreeRTOS/Source/portable/GCC/ARM_CM4/%.o: ../FRT_Library/FreeRTOS/Source/portable/GCC/ARM_CM4/%.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/home/klein/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-gcc" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DPART_TM4C123GH6PM -I"/home/klein/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment" -I"/home/klein/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/arm-none-eabi/include" -I"/home/klein/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment/FRT_Library/FreeRTOS/Source/include" -I"/home/klein/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment/FRT_Library/FreeRTOS/Source/portable/GCC/ARM_CM4" -I"/home/klein/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment/include" -I"/home/klein/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment/tasks" -I"/home/klein/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment/tasks" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -specs="nosys.specs" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


