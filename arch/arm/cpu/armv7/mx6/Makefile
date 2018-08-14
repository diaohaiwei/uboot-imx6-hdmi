#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# (C) Copyright 2011 Freescale Semiconductor, Inc.
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y	:= soc.o clock.o
obj-$(CONFIG_SECURE_BOOT)    += hab.o
# obj-$(CONFIG_UBOOT_LOGO_ENABLE) += ipu.o
obj-$(CONFIG_UBOOT_LOGO_ENABLE) += ipu_reg.o ipu_idmac.o ipu_dmfc.o ipu_dp.o ipu_dc.o ipu_di.o ipu_common.o ipu_main.o
