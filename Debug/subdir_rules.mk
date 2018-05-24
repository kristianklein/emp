<<<<<<< HEAD
################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DPART_TM4C123GH6PM -I"C:/ti/workspace_emp/final_assignment" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -I"C:/ti/workspace_emp/final_assignment/FRT_Library/FreeRTOS/Source/include" -I"C:/ti/workspace_emp/final_assignment/FRT_Library/FreeRTOS/Source/portable/GCC/ARM_CM4" -I"C:/ti/workspace_emp/final_assignment/include" -I"C:/ti/workspace_emp/final_assignment/tasks" -I"C:/ti/workspace_emp/final_assignment/tasks" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -specs="nosys.specs" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


=======
################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/home/klein/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-gcc" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DPART_TM4C123GH6PM -I"/home/klein/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment" -I"/home/klein/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/arm-none-eabi/include" -I"/home/klein/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment/FRT_Library/FreeRTOS/Source/include" -I"/home/klein/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment/FRT_Library/FreeRTOS/Source/portable/GCC/ARM_CM4" -I"/home/klein/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment/include" -I"/home/klein/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment/tasks" -I"/home/klein/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment/tasks" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -specs="nosys.specs" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


>>>>>>> button
