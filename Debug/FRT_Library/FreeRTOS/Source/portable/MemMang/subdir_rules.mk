################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
FRT_Library/FreeRTOS/Source/portable/MemMang/%.o: ../FRT_Library/FreeRTOS/Source/portable/MemMang/%.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"D:/Windows Application Programs/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major-win32/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DPART_TM4C123GH6PM -I"D:/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment" -I"D:/Windows Application Programs/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major-win32/arm-none-eabi/include" -I"D:/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment/FRT_Library/FreeRTOS/Source/include" -I"D:/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment/FRT_Library/FreeRTOS/Source/portable/GCC/ARM_CM4" -I"D:/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment/include" -I"D:/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment/tasks" -I"D:/Dropbox/Elektro/4. semester/EMP/Assignments/Final assignment/EMPfinal_workspace/final_assignment/tasks" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -specs="nosys.specs" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


