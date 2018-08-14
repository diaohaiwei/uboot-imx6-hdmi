/***************************************************************************
*
*    ipu.c
*
*    IPU initialization, connect each module and build a link from memory to  
* display .
*
*
***************************************************************************/

#include <common.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <asm/errno.h>
#include <linux/string.h>
#include <asm/arch/clock.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/crm_regs.h>
#include <asm/arch/iomux.h>
#include <asm/arch/ipuv3h_reg_def.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/imx-common/mxc_ipu.h>


void enable_display(unsigned int IPU, unsigned int DI)
{
	if(DI == 0)
	{
		//enable DI0 (display interface 0)
		ipu_write_field(IPU, IPU_IPU_CONF__DI0_EN, 1);
	}
	else if(DI == 1)
	{
		//enable DI1 (display interface 1)
		ipu_write_field(IPU, IPU_IPU_CONF__DI1_EN, 1);
	}

#ifndef IPU_USE_DC_CHANNEL
	//enble DP (display processor)
	ipu_write_field(IPU, IPU_IPU_CONF__DP_EN, 1);
#endif

	//enble DC (display controller)
	ipu_write_field(IPU, IPU_IPU_CONF__DC_EN, 1);

	//enble DMFC (display multi-fifo controller)
	ipu_write_field(IPU, IPU_IPU_CONF__DMFC_EN, 1);
}


///////////////////////////////////////////////////////////////////////////////////////////
void ipu_set_lvds_clock_split(unsigned int IPU, unsigned int DI, unsigned int pixel_clock)
{
	if (IPU == 1)
	{
		if (DI == 0)
			display_clk_config(MXC_IPU1_LVDS_DI0_CLK, pixel_clock * 7 / 2);
		else if(DI == 1)
			display_clk_config(MXC_IPU1_LVDS_DI1_CLK, pixel_clock * 7 / 2);
	}
	else if(IPU == 2)
	{
		if (DI == 0)
			display_clk_config(MXC_IPU2_LVDS_DI0_CLK, pixel_clock * 7 / 2);
		else if(DI == 1)
			display_clk_config(MXC_IPU2_LVDS_DI1_CLK, pixel_clock * 7 / 2);
	}	
}
void ipu_set_lvds_clock(unsigned int IPU, unsigned int DI, unsigned int pixel_clock)
{
	if (IPU == 1)
	{
		if (DI == 0)
			display_clk_config(MXC_IPU1_LVDS_DI0_CLK, pixel_clock * 7);
		else if(DI == 1)
			display_clk_config(MXC_IPU1_LVDS_DI1_CLK, pixel_clock * 7);
	}
	else if(IPU == 2)
	{
		if (DI == 0)
			display_clk_config(MXC_IPU2_LVDS_DI0_CLK, pixel_clock * 7);
		else if(DI == 1)
			display_clk_config(MXC_IPU2_LVDS_DI1_CLK, pixel_clock * 7);
	}
}


void ipu_set_hdmi_clock(unsigned int IPU, unsigned int DI, unsigned int pixel_clock)
{
	if (IPU == 1)
	{
		if(DI == 0)
			display_clk_config(MXC_IPU1_DI0_CLK, pixel_clock);
		else
			display_clk_config(MXC_IPU1_DI1_CLK, pixel_clock);
	}
	else if(IPU == 2)
	{
		if(DI == 0)
			display_clk_config(MXC_IPU2_DI0_CLK, pixel_clock);
		else
			display_clk_config(MXC_IPU2_DI1_CLK, pixel_clock);
	}	
}

void ipu_set_lcd_clock(unsigned int IPU, unsigned int DI, unsigned int pixel_clock)
{
	if (IPU == 1)
	{
		if(DI == 0)
			display_clk_config(MXC_IPU1_DI0_CLK, pixel_clock * 4);
		else
			display_clk_config(MXC_IPU1_DI1_CLK, pixel_clock * 4);
	}
	else if(IPU == 2)
	{
		if(DI == 0)
			display_clk_config(MXC_IPU2_DI0_CLK, pixel_clock * 4);
		else
			display_clk_config(MXC_IPU2_DI1_CLK, pixel_clock * 4);
	}
}

void ipu_disable_all(void)
{
	unsigned int reg;
	struct mxc_ccm_reg *imx_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;
	
	//Disable IPU and DI clocks
	reg = __raw_readl(&imx_ccm->CCGR3);
	reg &= ~0xFFFF;
	__raw_writel(reg, &imx_ccm->CCGR3);

#ifdef CONFIG_MX6DL
	reg = __raw_readl(&imx_ccm->cscdr3);
	reg |= (0x3 << 9);
	__raw_writel(reg, &imx_ccm->cscdr3);
#endif
}

void ipu_enable_lvds_clock(void)
{
	unsigned int reg;
	struct mxc_ccm_reg *imx_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;
	
	//Enable LDB DI clocks
	reg = __raw_readl(&imx_ccm->CCGR3);
	reg |= (0xF << 12);
	__raw_writel(reg, &imx_ccm->CCGR3);
}

void ipu_enable_hmdi_clock(void)
{
	unsigned int reg;
	struct mxc_ccm_reg *imx_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;
	
	//Enable HDMI clock
	reg = __raw_readl(&imx_ccm->CCGR2);
	reg |= (3 << 0);
	reg |= (3 << 4);
	__raw_writel(reg, &imx_ccm->CCGR2);	
}

void ipu_startup(unsigned int IPU, unsigned int DI)
{
	unsigned int reg;
	struct src *src_regs = (struct src *)SRC_BASE_ADDR;
	struct mxc_ccm_reg *imx_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;
	if (IPU == 1)
	{
		// Reset IPU
		reg = __raw_readl(&src_regs->scr);
		reg |= 0x8;
		__raw_writel(reg, &src_regs->scr);

		// Enable IPU clock
		reg = __raw_readl(&imx_ccm->CCGR3);
		reg |= (3 << 0);
		__raw_writel(reg, &imx_ccm->CCGR3);

		// Reset IPU memory 
		writel(0x807FFFFF, IPU1_ARB_BASE_ADDR + IPU_IPU_MEM_RST__ADDR);
		while (readl(IPU1_ARB_BASE_ADDR + IPU_IPU_MEM_RST__ADDR) & 0x80000000);

		if (DI == 0)
		{
			//Enable DI0 clock
			reg = __raw_readl(&imx_ccm->CCGR3);
			reg |= (3 << 2);
			__raw_writel(reg, &imx_ccm->CCGR3);
		}
		else if(DI == 1)
		{
			//Enable DI1 clock
			reg = __raw_readl(&imx_ccm->CCGR3);
			reg |= (3 << 4);
			__raw_writel(reg, &imx_ccm->CCGR3);
		}
	}
	else if(IPU == 2)
	{
		// Reset IPU
		reg = __raw_readl(&src_regs->scr);
		reg |= 0x1000;
		__raw_writel(reg, &src_regs->scr);

		// Enable IPU clock
		reg = __raw_readl(&imx_ccm->CCGR3);
		reg |= (3 << 6);
		__raw_writel(reg, &imx_ccm->CCGR3);

		// Reset IPU memory	
		writel(0x807FFFFF, IPU2_ARB_BASE_ADDR + IPU_IPU_MEM_RST__ADDR);
		while (readl(IPU2_ARB_BASE_ADDR + IPU_IPU_MEM_RST__ADDR) & 0x80000000);

		if (DI == 0)
		{
			//Enable DI0 clock
			reg = __raw_readl(&imx_ccm->CCGR3);
			reg |= (3 << 8);
			__raw_writel(reg, &imx_ccm->CCGR3);
		}
		else if(DI == 1)
		{
			//Enable DI1 clock
			reg = __raw_readl(&imx_ccm->CCGR3);
			reg |= (3 << 10);
			__raw_writel(reg, &imx_ccm->CCGR3);
		}
	}
	
}

// ===> BEGIN HDMI
void setup_hdmi_iomux(unsigned int IPU, unsigned int DI)
{
	if(IPU == 1)
	{
		if(DI == 0)
			mxc_iomux_set_gpr_register(3, 2, 2, 0);
		else if(DI == 1)
			mxc_iomux_set_gpr_register(3, 2, 2, 1);
	}

	if(IPU == 2)
	{
		if(DI == 0)
			mxc_iomux_set_gpr_register(3, 2, 2, 2);
		else if(DI == 1)
			mxc_iomux_set_gpr_register(3, 2, 2, 3);
	}
}

// ===> END HDMI


/*
	uint32_t lcdblock = 0;
	lcdblock = in32(MX6X_IOMUXC_BASE + MX6X_IOMUX_GPR2);	//MX6X_IOMUX_GPR2 == 0x08
	kprintf("MX6X_IOMUXC_GPR2 (LCD BLOCK): %d\n", lcdblock);
	//lcdblock = lcdblock & 0xFFFFFF9F ^ 0x60;
	lcdblock = lcdblock & 0xFFFFFF9F ^ 0x20;
	kprintf("MX6X_IOMUXC_GPR2 (LCD BLOCK): %d\n", lcdblock);
	out32(MX6X_IOMUXC_BASE + MX6X_IOMUX_GPR2, lcdblock);
*/

/*
	#define IOMUXC_GPR2_BITMAP_SPWG      0
	#define IOMUXC_GPR2_BITMAP_JEIDA     1
	#define IOMUXC_GPR2_BIT_MAPPING_CH1_JEIDA            (IOMUXC_GPR2_BITMAP_JEIDA<<IOMUXC_GPR2_BIT_MAPPING_CH1_OFFSET)
	#define IOMUXC_GPR2_BIT_MAPPING_CH1_SPWG             (IOMUXC_GPR2_BITMAP_SPWG<<IOMUXC_GPR2_BIT_MAPPING_CH1_OFFSET)
	#define IOMUXC_GPR2_BIT_MAPPING_CH0_JEIDA            (IOMUXC_GPR2_BITMAP_JEIDA<<IOMUXC_GPR2_BIT_MAPPING_CH0_OFFSET)
	#define IOMUXC_GPR2_BIT_MAPPING_CH0_SPWG             (IOMUXC_GPR2_BITMAP_SPWG<<IOMUXC_GPR2_BIT_MAPPING_CH0_OFFSET)
*/

// ===> BEGIN LVDS
void setup_lvds_aux(void)
{
	//
	void *base = (void *)IOMUXC_BASE_ADDR;
	//struct iomuxc *iomux = (struct iomuxc *)IOMUXC_BASE_ADDR;//IOMUXC_GPR_BASE_ADDR;
	uint32_t lcdblock = 0;
	//lcdblock = __raw_readl(&iomux->grp[2]);
	lcdblock = readl(base + 8);

	printf("MX6X_IOMUXC_GPR2 (LCD BLOCK): 0x%02x\n", lcdblock);
	//lcdblock = (lcdblock & 0xFFFFFF9F) ^ 0x60;
#if 1	
	// first
	lcdblock = (lcdblock & 0xFFFFFF9F) ^ 0x0020;	//bit 6-7 : low (width) high(jeida)
	lcdblock = (lcdblock & 0xFFFFFE7F) ^ 0x0080;	//bit 8-9 : low (width) high(jeida)
#endif

#if 0
	// second
	lcdblock = (lcdblock & 0xFFFFFF9F) ^ 0x0040;	//bit 6-7 : low (width) high(jeida)
	lcdblock = (lcdblock & 0xFFFFFE7F) ^ 0x0100;	//bit 8-9 : low (width) high(jeida)
#endif

#if 0
	lcdblock = (lcdblock & 0xFFFFFF9F) ^ 0x0060;	//bit 6-7 : low (width) high(jeida)
	lcdblock = (lcdblock & 0xFFFFFE7F) ^ 0x0180;	//bit 8-9 : low (width) high(jeida)
#endif
	printf("MX6X_IOMUXC_GPR2 (LCD BLOCK): 0x%02x\n", lcdblock);

	writel(lcdblock, base + 8);
}

void setup_lvds_iomux_port_0(unsigned int IPU, unsigned int DI)
{
	if(IPU == 1)
	{
		if(DI == 0)
			mxc_iomux_set_gpr_register(3, 6, 2, 0);
		else if(DI == 1)
			mxc_iomux_set_gpr_register(3, 6, 2, 1);
	}

	if(IPU == 2)
	{
		if(DI == 0)
			mxc_iomux_set_gpr_register(3, 6, 2, 2);
		else if(DI == 1)
			mxc_iomux_set_gpr_register(3, 6, 2, 3);
	}
}

void setup_lvds_iomux_port_1(unsigned int IPU, unsigned int DI)
{
	if(IPU == 1)
	{
		if(DI == 0)
			mxc_iomux_set_gpr_register(3, 8, 2, 0);
		else if(DI == 1)
			mxc_iomux_set_gpr_register(3, 8, 2, 1);
	}

	if(IPU == 2)
	{
		if(DI == 0)
			mxc_iomux_set_gpr_register(3, 8, 2, 2);
		else if(DI == 1)
			mxc_iomux_set_gpr_register(3, 8, 2, 3);
	}
}


void setup_lvds_port_0(unsigned int DI)
{
	unsigned int reg;
	reg = 0;
#if (DISPLAY_IF_BPP == 24)
	reg |= (1 << 5);
#endif
	if (DI == 0){
		reg |= (DISPLAY_VSYNC_POLARITY << 9);
		reg |= (1 << 0);	//ch0_mode[1:0]=01 	ROUTE TO DI-0		
#ifdef LVDS_SPLIT_MODE
		printf("LVDS_SPLIT_MODE \n");
		reg |= (1 << 4);	//split_mode_en=1 	SPLIT MODE
		reg |= (1 << 2);	//ch1_mode[1:0]=01 	ROUTE TO DI-0
#endif
	}else if(DI == 1){
		printf("DI 1 use\n");
		reg |= (DISPLAY_VSYNC_POLARITY << 10);
		reg |= (3 << 0);	//ch0_mode[1:0]=11 	ROUTE TO DI-1			
#ifdef LVDS_SPLIT_MODE
		printf("LVDS_SPLIT_MODE \n");
		reg |= (1 << 4);	//split_mode_en=1 	SPLIT MODE
		reg |= (3 << 2);	//ch1_mode[1:0]=11 	ROUTE TO DI-1
#endif
	}
	


	printf("reg = %02x\n", (int)reg);
	writel(reg, IOMUXC_BASE_ADDR + 0x8);  //Set LDB_CTRL
}

void setup_lvds_port_1(unsigned int DI)
{
	unsigned int reg;
	reg = 0;
#if (DISPLAY_IF_BPP == 24)
	reg |= (1 << 7);
#endif
	if (DI == 0){
		reg |= (DISPLAY_VSYNC_POLARITY << 9);
		reg |= (1 << 2);	//ch1_mode[1:0]=01 ROUTE TO DI-0
#ifdef LVDS_SPLIT_MODE
		printf("LVDS_SPLIT_MODE \n");
		reg |= (1 << 4);	//split_mode_en=1 	SPLIT MODE
		reg |= (1 << 0);	//ch0_mode[1:0]=01 ROUTE TO DI-0
#endif
	}else if(DI == 1){
		reg |= (DISPLAY_VSYNC_POLARITY << 10);
		reg |= (3 << 2);	//ch1_mode[1:0]=11 ROUTE TO DI-1
#ifdef LVDS_SPLIT_MODE
		printf("LVDS_SPLIT_MODE \n");
		reg |= (1 << 4);	//split_mode_en=1 	SPLIT MODE
		reg |= (3 << 0);	//ch0_mode[1:0]=11 ROUTE TO DI-1
#endif
	}
	
	printf("reg = %02x\n", (int)reg);
	writel(reg, IOMUXC_BASE_ADDR + 0x8);  //Set LDB_CTRL
}

// ===> END LVDS
