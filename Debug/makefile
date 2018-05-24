################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/Windows Application Programs/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major-win32

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.o" \
"./startup_frt.o" \
"./systick_frt.o" \
"./FRT_Library/FreeRTOS/Source/croutine.o" \
"./FRT_Library/FreeRTOS/Source/list.o" \
"./FRT_Library/FreeRTOS/Source/queue.o" \
"./FRT_Library/FreeRTOS/Source/tasks.o" \
"./FRT_Library/FreeRTOS/Source/portable/GCC/ARM_CM4/common.o" \
"./FRT_Library/FreeRTOS/Source/portable/GCC/ARM_CM4/port.o" \
"./FRT_Library/FreeRTOS/Source/portable/MemMang/heap_2.o" \
"./include/button.o" \
"./include/keypad.o" \
"./include/rgb.o" \
"./include/rtc.o" \
"./include/uart0.o" \
"./tasks/button_task.o" \
"./tasks/keypad_task.o" \
"./tasks/rtc_task.o" \
-Wl,-T"../tm4c123gh6pm.lds" \
$(GEN_CMDS__FLAG) \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include FRT_Library/FreeRTOS/Source/subdir_vars.mk
-include FRT_Library/FreeRTOS/Source/portable/GCC/ARM_CM4/subdir_vars.mk
-include FRT_Library/FreeRTOS/Source/portable/MemMang/subdir_vars.mk
-include include/subdir_vars.mk
-include tasks/subdir_vars.mk
-include subdir_rules.mk
-include FRT_Library/FreeRTOS/Source/subdir_rules.mk
-include FRT_Library/FreeRTOS/Source/portable/GCC/ARM_CM4/subdir_rules.mk
-include FRT_Library/FreeRTOS/Source/portable/MemMang/subdir_rules.mk
-include include/subdir_rules.mk
-include tasks/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
final_assignment.out \

EXE_OUTPUTS__QUOTED += \
"final_assignment.out" \

BIN_OUTPUTS += \
final_assignment.hex \

BIN_OUTPUTS__QUOTED += \
"final_assignment.hex" \


# All Target
all: $(OBJS) $(LDS_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "final_assignment.out"

# Tool invocations
final_assignment.out: $(OBJS) $(LDS_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: GNU Linker'
	"D:/Windows Application Programs/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major-win32/bin/arm-none-eabi-gcc.exe" -DPART_TM4C123GH6PM -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -specs="nosys.specs" -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wl,-Map,"final_assignment.map" -o"final_assignment.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

final_assignment.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: GNU Objcopy Utility'
	"D:/Windows Application Programs/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major-win32/bin/arm-none-eabi-objcopy.exe"  $(EXE_OUTPUTS__QUOTED) "final_assignment.hex"
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.o" "startup_frt.o" "systick_frt.o" "FRT_Library\FreeRTOS\Source\croutine.o" "FRT_Library\FreeRTOS\Source\list.o" "FRT_Library\FreeRTOS\Source\queue.o" "FRT_Library\FreeRTOS\Source\tasks.o" "FRT_Library\FreeRTOS\Source\portable\GCC\ARM_CM4\common.o" "FRT_Library\FreeRTOS\Source\portable\GCC\ARM_CM4\port.o" "FRT_Library\FreeRTOS\Source\portable\MemMang\heap_2.o" "include\button.o" "include\keypad.o" "include\rgb.o" "include\rtc.o" "include\uart0.o" "tasks\button_task.o" "tasks\keypad_task.o" "tasks\rtc_task.o" 
	-$(RM) "main.d" "startup_frt.d" "systick_frt.d" "FRT_Library\FreeRTOS\Source\croutine.d" "FRT_Library\FreeRTOS\Source\list.d" "FRT_Library\FreeRTOS\Source\queue.d" "FRT_Library\FreeRTOS\Source\tasks.d" "FRT_Library\FreeRTOS\Source\portable\GCC\ARM_CM4\common.d" "FRT_Library\FreeRTOS\Source\portable\GCC\ARM_CM4\port.d" "FRT_Library\FreeRTOS\Source\portable\MemMang\heap_2.d" "include\button.d" "include\keypad.d" "include\rgb.d" "include\rtc.d" "include\uart0.d" "tasks\button_task.d" "tasks\keypad_task.d" "tasks\rtc_task.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

