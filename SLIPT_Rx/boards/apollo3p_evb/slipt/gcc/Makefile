#******************************************************************************
#
# Makefile - Rules for building the libraries, examples and docs.
#
#******************************************************************************

TARGET := slipt
COMPILERNAME := gcc
PROJECT := slipt_gcc
CONFIG := bin

SHELL:=/bin/bash

#### Setup ####

TOOLCHAIN ?= arm-none-eabi
PART = apollo3p
CPU = cortex-m4
FPU = fpv4-sp-d16
FABI = hard

LINKER_FILE := ./linker_script.ld
STARTUP_FILE := ./startup_$(COMPILERNAME).c

#### Required Executables ####
CC = $(TOOLCHAIN)-gcc
GCC = $(TOOLCHAIN)-gcc
CPP = $(TOOLCHAIN)-cpp
LD = $(TOOLCHAIN)-ld
CP = $(TOOLCHAIN)-objcopy
OD = $(TOOLCHAIN)-objdump
RD = $(TOOLCHAIN)-readelf
AR = $(TOOLCHAIN)-ar
SIZE = $(TOOLCHAIN)-size
RM = $(shell which rm 2>/dev/null)

EXECUTABLES = CC LD CP OD AR RD SIZE GCC
K := $(foreach exec,$(EXECUTABLES),\
        $(if $(shell which $($(exec)) 2>/dev/null),,\
        $(info $(exec) not found on PATH ($($(exec))).)$(exec)))
$(if $(strip $(value K)),$(info Required Program(s) $(strip $(value K)) not found))

ifneq ($(strip $(value K)),)
all clean:
	$(info Tools $(TOOLCHAIN)-$(COMPILERNAME) not installed.)
	$(RM) -rf bin
else

DEFINES = -DPART_$(PART)
DEFINES+= -DAM_PACKAGE_BGA
DEFINES+= -DAM_PART_APOLLO3P
DEFINES+= -Dgcc

# ------------------------
# Include paths (one level up)
# ------------------------
INCLUDES = -I../../../../
INCLUDES+= -I../../../../CMSIS/ARM/Include
INCLUDES+= -I../../../../CMSIS/AmbiqMicro/Include
INCLUDES+= -I../../../../devices
INCLUDES+= -I../../../../mcu/apollo3p
INCLUDES+= -I../../../../utils
INCLUDES+= -I../../bsp
INCLUDES+= -I../src
INCLUDES+= -I../components/S2
INCLUDES+= -I../../../../third_party/FreeRTOSv10.5.1/Source/include
INCLUDES+= -I../../../../third_party/FreeRTOSv10.5.1/Source
INCLUDES+= -I../../../../third_party/FreeRTOSv10.5.1/Source/portable/GCC/AMapollo2
INCLUDES+= -I../../../../third_party/FreeRTOSv10.5.1/Source/portable/MemMang
INCLUDES+= -I../../../../third_party/FreeRTOS9/Demo/Common/Minimal
INCLUDES+= -I../../../../third_party/FreeRTOS9/Demo/Common/Include
INCLUDES+= -I../../../../third_party/FreeRTOSv10.5.1/Source/portable/GCC/ARM_CM4F

# ------------------------
# Source search paths (VPATH) adjusted
# ------------------------
VPATH = ../../../../utils
VPATH+=:../src
VPATH+=:../components/S2
VPATH+=:../src/setup
VPATH+=:../../../../third_party/FreeRTOSv10.5.1/Source
VPATH+=:../../../../third_party/FreeRTOSv10.5.1/Source/include
VPATH+=:../../../../third_party/FreeRTOSv10.5.1/Source/portable/GCC/AMapollo2
VPATH+=:../../../../third_party/FreeRTOSv10.5.1/Source/portable/MemMang
VPATH+=:../../../../third_party/FreeRTOS9/Demo/Common/Minimal
VPATH+=:../../../../third_party/FreeRTOS9/Demo/Common/Include
VPATH+=:../../../../third_party/FreeRTOSv10.5.1/Source/portable/GCC/ARM_CM4F

SRC = am_util_delay.c
SRC += am_util_faultisr.c
SRC += am_util_id.c
SRC += am_util_stdio.c
SRC += main.c
SRC += setup.c
SRC += s2.c
SRC += startup_gcc.c
SRC += tasks.c
SRC += port.c
SRC += list.c
SRC += heap_4.c
SRC += portasm.s
SRC += queue.c
SRC += timers.c
SRC += event_groups.c
SRC += rtos.c

CSRC = $(filter %.c,$(SRC))
ASRC = $(filter %.s,$(SRC))

OBJS = $(CSRC:%.c=$(CONFIG)/%.o)
OBJS+= $(ASRC:%.s=$(CONFIG)/%.o)

DEPS = $(CSRC:%.c=$(CONFIG)/%.d)
DEPS+= $(ASRC:%.s=$(CONFIG)/%.d)

# ------------------------
# Library paths (one level up)
# ------------------------
LIBS = ../../../../mcu/apollo3p/hal/gcc/bin/libam_hal.a
LIBS += ../../bsp/gcc/bin/libam_bsp.a

CFLAGS = -mthumb -mcpu=$(CPU) -mfpu=$(FPU) -mfloat-abi=$(FABI)
CFLAGS+= -ffunction-sections -fdata-sections -fomit-frame-pointer
CFLAGS+= -MMD -MP -std=c99 -Wall -g
CFLAGS+= -O0
CFLAGS+= $(DEFINES)
CFLAGS+= $(INCLUDES)

LFLAGS = -mthumb -mcpu=$(CPU) -mfpu=$(FPU) -mfloat-abi=$(FABI)
LFLAGS+= -nostartfiles -static
LFLAGS+= -Wl,--gc-sections,--entry,Reset_Handler,-Map,$(CONFIG)/$(TARGET).map
LFLAGS+= -Wl,--start-group -lm -lc -lgcc $(LIBS) -Wl,--end-group

CPFLAGS = -Obinary
ODFLAGS = -S

#### Rules ####
all: directories $(CONFIG)/$(TARGET).bin

directories: $(CONFIG)

$(CONFIG):
	@mkdir -p $@

$(CONFIG)/%.o: %.c $(CONFIG)/%.d
	@echo " Compiling $(COMPILERNAME) $<" ;\
	$(CC) -c $(CFLAGS) $< -o $@

$(CONFIG)/%.o: %.s $(CONFIG)/%.d
	@echo " Assembling $(COMPILERNAME) $<" ;\
	$(CC) -c $(CFLAGS) $< -o $@

$(CONFIG)/$(TARGET).axf: $(OBJS) $(LIBS)
	@echo " Linking $(COMPILERNAME) $@" ;\
	$(CC) -Wl,-T,$(LINKER_FILE) -o $@ $(OBJS) $(LFLAGS)

$(CONFIG)/$(TARGET).bin: $(CONFIG)/$(TARGET).axf
	@echo " Copying $(COMPILERNAME) $@..." ;\
	$(CP) $(CPFLAGS) $< $@ ;\
	$(OD) $(ODFLAGS) $< > $(CONFIG)/$(TARGET).lst
	@$(SIZE) $(OBJS) $(LIBS) $(CONFIG)/$(TARGET).axf >$(CONFIG)/$(TARGET).size

clean:
	@echo "Cleaning..."
	$(RM) -f $(OBJS) $(DEPS) \
	    $(CONFIG)/$(TARGET).bin $(CONFIG)/$(TARGET).axf \
	    $(CONFIG)/$(TARGET).lst $(CONFIG)/$(TARGET).map

$(CONFIG)/%.d: ;

../../../../mcu/apollo3p/hal/gcc/bin/libam_hal.a:
	$(MAKE) -C ../../../../mcu/apollo3p/hal

../../bsp/gcc/bin/libam_bsp.a:
	$(MAKE) -C ../../bsp

# Automatically include any generated dependencies
-include $(DEPS)
endif
.PHONY: all clean directories
