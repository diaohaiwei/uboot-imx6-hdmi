#
# Copyright (C) 2007, Guennadi Liakhovetski <lg@denx.de>
#
# (C) Copyright 2011 Freescale Semiconductor, Inc.
#
# SPDX-License-Identifier:	GPL-2.0+
#

# official version
# obj-y  := mx6sabresd.o

# e-planet version
obj-y := mx6sabresd-main.o
obj-y += mx6sabresd-display.o
obj-y += mx6sabresd-epdc.o
obj-y += mx6sabresd-logo.o

extra-$(CONFIG_USE_PLUGIN) :=  plugin.bin
$(obj)/plugin.bin: $(obj)/plugin.o
	$(OBJCOPY) -O binary --gap-fill 0xff $< $@
