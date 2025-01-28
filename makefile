########################################################################################################################
# File Name  : Makefile
# Description: STM32F407VGTX project Makefile
# Project    : STM32F407VGTX
# Autor      : mehmtonrr
#-----------------------------------------------------------------------------------------------------------------------
# COPYRIGHT
#-----------------------------------------------------------------------------------------------------------------------
# Copyright (c) 2023 by mehmtonrr All rights reserved.
#
#-----------------------------------------------------------------------------------------------------------------------
# REVISION HISTORY
#-----------------------------------------------------------------------------------------------------------------------
# Version   Date        Author  	Description
# --------  ----------	---------	----------------------------------------------------------------------------------------
# 01.00.00  2023-12-11  mehmtonrr   Initial version
# 
########################################################################################################################
#------------------------------------------------------------------------------
# Includes
#------------------------------------------------------------------------------
::include ./makefile.config

#------------------------------------------------------------------------------
# Functions
#------------------------------------------------------------------------------
#uniq=$(shell echo $(1) | tr " " "\n" | cat -n | sort -uk2 | sort -nk1| cut -f2- | tr "\n" " ")

#------------------------------------------------------------------------------
# Project and tool paths
#------------------------------------------------------------------------------
#PROJECT_NAME = STM32F407VGTX
#
#TARGET = $(PROJECT_NAME).elf
#CPU_CORE = Cortex-M4
#PLATFORM = ARM


######################################
# target
######################################
TARGET = STM32F407VGTX


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -O0


#######################################
# paths
#######################################
# Project path
PROJECT_DIR = .

# External_Library path
EXTERNAL_LIBRARY_DIR = .\..\..\External_Library

# Build path
BUILD_DIR = $(PROJECT_DIR)/Outputs/bin

######################################
# source
######################################
# C sources
C_SOURCES =  \
./Src/led.c  \
./Src/main.c \
./Src/syscalls.c


# STARTUP sources
STARTUP_SOURCES =  \
./Startup/startup_stm32f407xx.c

# C++ sources
CPP_SOURCES = \

# GCC path
GCC_PATH = $(EXTERNAL_LIBRARY_DIR)\CTOOLS\arm-none-eabi-14.2\bin

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)\$(PREFIX)gcc
AS = $(GCC_PATH)\$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)\$(PREFIX)objcopy
SZ = $(GCC_PATH)\$(PREFIX)size
CXX = $(GCC_PATH)\$(PREFIX)g++
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
CXX = $(PREFIX)g++
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F407xx

# C defines
CPP_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F407xx

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-IInc

CPP_INCLUDES = \
#

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CXXFLAGS = $(MCU) $(CPP_DEFS) -Wall -g -std=c++17 -ffunction-sections -fdata-sections $(CPP_INCLUDES)

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = .\Project_Settings\STM32F407VGTX.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) --specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(STARTUP_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(STARTUP_SOURCES)))
# list of C++ program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	$(CXX) -c $(CXXFLAGS) $(CPP_INCLUDES) $< -o $@


$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-del .\$(BUILD_DIR)\*.o
	-del .\$(BUILD_DIR)\*.d
	-del .\$(BUILD_DIR)\*.elf
	-del .\$(BUILD_DIR)\*.hex
	-del .\$(BUILD_DIR)\*.lst
	-del .\$(BUILD_DIR)\*.map
	-del .\$(BUILD_DIR)\*.bin

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
