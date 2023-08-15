#
# Copyright (C) 2017 MediaTek Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
ifeq ($(CHIP),)
CHIP_NAME = mt7626
else
CHIP_NAME = $(CHIP)
endif

# ---------------------------------------------------
# Directory List
# ---------------------------------------------------
DRV_PATH 		:= $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
SRC_DIR     		:= source
SRC_COMMON_DIR  	:= $(SRC_DIR)/common
SRC_CHIP_DIR    	:= chip
INCLUDE_DIR		:= $(DRV_PATH)

# ---------------------------------------------------
# Compile Options
# ---------------------------------------------------
EXTRA_CFLAGS += \
	-D$(CHIP_NAME) \
	-I$(INCLUDE_DIR)

ifeq ($(CONFIG_MTK_EMI_CHIP_LEOPARD),y)
    EXTRA_CFLAGS += -DCONFIG_EMI_CHIP_LEOPARD
endif

MODULE_FLAGS=$(EXTRA_CFLAGS)
export MODULE_FLAGS

# ---------------------------------------------------
# Objects List
# ---------------------------------------------------
CORE_OBJS := $(SRC_DIR)/wifi_emi_loader.o
CHIP_OBJS := $(SRC_CHIP_DIR)/$(CHIP_NAME).o
COMMON_OBJS := $(SRC_COMMON_DIR)/hw_ctrl.o
wifi_emi_loader-objs := $(CORE_OBJS) $(CHIP_OBJS) $(COMMON_OBJS)

#always build module
obj-m += wifi_emi_loader.o

build_tools:
	ln -sf $(BIN_PATH)
	$(MAKE) -C tool CHIP=$(CHIP)
	rm bin
