/*
 * (C) Copyright 2009
 * Stefano Babic, DENX Software Engineering, sbabic@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __ASM_ARCH_CLOCK_H
#define __ASM_ARCH_CLOCK_H

#include <common.h>

#ifdef CONFIG_SYS_MX6_HCLK
#define MXC_HCLK	CONFIG_SYS_MX6_HCLK
#else
#define MXC_HCLK	24000000
#endif

#ifdef CONFIG_SYS_MX6_CLK32
#define MXC_CLK32	CONFIG_SYS_MX6_CLK32
#else
#define MXC_CLK32	32768
#endif

enum mxc_clock {
	MXC_ARM_CLK = 0,
	MXC_PER_CLK,
	MXC_AHB_CLK,
	MXC_IPG_CLK,
	MXC_IPG_PERCLK,
	MXC_UART_CLK,
	MXC_CSPI_CLK,
	MXC_AXI_CLK,
	MXC_EMI_SLOW_CLK,
	MXC_DDR_CLK,
	MXC_ESDHC_CLK,
	MXC_ESDHC2_CLK,
	MXC_ESDHC3_CLK,
	MXC_ESDHC4_CLK,
	MXC_SATA_CLK,
	MXC_NFC_CLK,
	MXC_I2C_CLK,
#ifdef CONFIG_UBOOT_LOGO_ENABLE
	MXC_IPU1_LVDS_DI0_CLK,
	MXC_IPU1_LVDS_DI1_CLK,
	MXC_IPU2_LVDS_DI0_CLK,
	MXC_IPU2_LVDS_DI1_CLK,
	MXC_IPU1_DI0_CLK,
	MXC_IPU1_DI1_CLK,
	MXC_IPU2_DI0_CLK,
	MXC_IPU2_DI1_CLK,
#endif
};

enum enet_freq {
	ENET_25MHz,
	ENET_50MHz,
	ENET_100MHz,
	ENET_125MHz,
};

u32 imx_get_uartclk(void);
u32 imx_get_fecclk(void);
unsigned int mxc_get_clock(enum mxc_clock clk);
void enable_ocotp_clk(unsigned char enable);
void enable_usboh3_clk(unsigned char enable);
int enable_sata_clock(void);
int enable_pcie_clock(void);
int enable_i2c_clk(unsigned char enable, unsigned i2c_num);
void enable_ipu_clock(void);
#ifdef CONFIG_FEC_MXC
int enable_fec_anatop_clock(int fec_id, enum enet_freq freq);
#endif
#ifdef CONFIG_SECURE_BOOT
void hab_caam_clock_enable(void);
void hab_caam_clock_disable(void);
#endif
#if (defined(CONFIG_MX6SX))
void enable_qspi_clk(int qspi_num);
void enable_enet_clock(void);
void enable_lcdif_clock(uint32_t base_addr);
void mxs_set_lcdclk(uint32_t base_addr, uint32_t freq);
void enable_lvds(uint32_t lcdif_base);
void mxs_set_vadcclk(void);
#endif
#endif /* __ASM_ARCH_CLOCK_H */
