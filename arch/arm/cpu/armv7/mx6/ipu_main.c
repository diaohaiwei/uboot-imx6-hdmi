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

#include "ipu.h"

//#define DISPLAY_EBA0 		(PHYS_SDRAM_1 + PHYS_SDRAM_1_SIZE - 0x2000000)
#define DISPLAY_EBA0		CONFIG_FB_BASE


void di_config(unsigned int IPU, unsigned int DI)
{
#ifdef DI_CLOCK_EXTERNAL_MODE
#if defined(IPU_OUTPUT_MODE_LVDS)  || defined(IPU_OUTPUT_MODE_HDMI)
	int ipuDiClk = DISPLAY_PIX_CLOCK;  //For iMX6Q, external clock, ipu_di_clk
#else
	int ipuDiClk = DISPLAY_PIX_CLOCK * 4;  //For iMX6Q, external clock, ipu_di_clk
#endif
	int extern_clock = 1;
#else
	int ipuDiClk = CONFIG_IPUV3_CLK;  //For iMX6, internel IPU clock, ipu_hsp_clk
	int extern_clock = 0;
#endif
	int typPixClk = DISPLAY_PIX_CLOCK; // typical value of pixel clock, (hSyncStartWidth + hSyncEndWidth + hSyncWidth + hDisp) * (vSyncStartWidth + vSyncEndWidth + vSyncWidth + vDisp) * refresh rate (60Hz)
	int div = (int)((float)ipuDiClk / (float)typPixClk + 0.5);// get the nearest value of typical pixel clock
	
	int hSyncStartWidth = DISPLAY_HSYNC_START;
	int hSyncWidth = DISPLAY_HSYNC_WIDTH;
	int hSyncEndWidth = DISPLAY_HSYNC_END;
	int hDisp = DISPLAY_WIDTH;
	int vSyncStartWidth = DISPLAY_VSYNC_START;
	int vSyncWidth = DISPLAY_VSYNC_WIDTH;
	int vSyncEndWidth = DISPLAY_VSYNC_END;
	int vDisp = DISPLAY_HEIGHT;
	
#ifdef DISPLAY_INTERLACED
	di_config_interlaced(IPU, DI, div, extern_clock,
		DISPLAY_CLOCK_POLARITY, DISPLAY_HSYNC_POLARITY, DISPLAY_VSYNC_POLARITY, DISPLAY_DATA_POLARITY, DISPLAY_DATA_ENABLE_POLARITY,
		hSyncStartWidth, hSyncWidth, hSyncEndWidth, hDisp,
		vSyncStartWidth, vSyncWidth, vSyncEndWidth, vDisp);
		
#else
	di_config_none_interlaced(IPU, DI, div, extern_clock,
		DISPLAY_CLOCK_POLARITY, DISPLAY_HSYNC_POLARITY, DISPLAY_VSYNC_POLARITY, DISPLAY_DATA_POLARITY, DISPLAY_DATA_ENABLE_POLARITY,
		hSyncStartWidth, hSyncWidth, hSyncEndWidth, hDisp,
		vSyncStartWidth, vSyncWidth, vSyncEndWidth, vDisp);
#endif

		
}
	

void dc_config(unsigned int IPU, unsigned int DI)
{
	int mapping = 1;

#if (DISPLAY_IF_BPP == 24)
	mapping = 1;
#endif

#if (DISPLAY_IF_BPP == 18)
	mapping = 2;
#endif

#if (DISPLAY_IF_BPP == 16)
	mapping = 4;
#endif

#ifdef DISPLAY_INTERLACED
	dc_config_interlaced(IPU, DI, DISPLAY_WIDTH, mapping);
#else
	dc_config_none_interlaced(IPU, DI, DISPLAY_WIDTH, mapping);
#endif
}
	

void ipu_setup(unsigned int IPU, unsigned int DI)
{
	int eba = DISPLAY_EBA0 / 8;
	//	eba = (unsigned int)(&logobmp[0]) / 8;

	dmfc_config(IPU);

	idmac_config(IPU, DISPLAY_BPP, DISPLAY_WIDTH, DISPLAY_HEIGHT, eba);

	dp_config(IPU);

	dc_config(IPU, DI);
	di_config(IPU, DI);

	enable_display(IPU, DI);
	
	dp_enable(IPU);

}

#ifdef IPU_OUTPUT_MODE_HDMI
extern void hdmi_display_enable(unsigned int mCode);
static void enable_hdmi(unsigned int IPU, unsigned int DI)
{
	setup_hdmi_iomux(IPU, DI);
	
#ifdef DISPLAY_INTERLACED

#if (DISPLAY_WIDTH == 1920) && (DISPLAY_HEIGHT == 1080)
	hdmi_display_enable(5);  //1080i@60Hz
#endif

#else

#if (DISPLAY_WIDTH == 1920) && (DISPLAY_HEIGHT == 1080)
	hdmi_display_enable(16);  //1080p@60Hz
#else
	hdmi_display_enable(4);  //720p@60Hz
#endif

#endif  //DISPLAY_INTERLACED
	
}

static void setup_hdmi(unsigned int IPU, unsigned int DI)
{
	ipu_disable_all();

#ifdef DI_CLOCK_EXTERNAL_MODE
	ipu_set_hdmi_clock(IPU, DI, DISPLAY_PIX_CLOCK);
#endif

	ipu_startup(IPU, DI);
	
	ipu_enable_hmdi_clock();
	
	ipu_setup(IPU, DI);
	
	enable_hdmi(IPU, DI);
}
#endif



#ifdef IPU_OUTPUT_MODE_LVDS

static void enable_lvds(unsigned int IPU, unsigned int DI)
{
#if (LVDS_PORT == 0)
	setup_lvds_iomux_port_0(IPU, DI);
	debug("DISPLAY_IF_BPP == 24 LVDS_PORT == 0\r\n");
	setup_lvds_port_0(DI);
#endif

#if (LVDS_PORT == 1)
	setup_lvds_iomux_port_1(IPU, DI);
	debug("DISPLAY_IF_BPP == 24 LVDS_PORT == 1\r\n");
	setup_lvds_port_1(DI);
#endif

	setup_lvds_aux();
}

static void setup_lvds(unsigned int IPU, unsigned int DI)
{
	ipu_disable_all();
	
#ifdef DI_CLOCK_EXTERNAL_MODE

#ifdef LVDS_SPLIT_MODE
	ipu_set_lvds_clock_split(IPU, DI, DISPLAY_PIX_CLOCK);
#else
	ipu_set_lvds_clock(IPU, DI, DISPLAY_PIX_CLOCK);
#endif

#endif  //DI_CLOCK_EXTERNAL_MODE

	ipu_startup(IPU, DI);
	
	ipu_enable_lvds_clock();

	ipu_setup(IPU, DI);

	enable_lvds(IPU, DI);
}
#endif

#ifdef IPU_OUTPUT_MODE_LCD
static void setup_lcdif(unsigned int IPU, unsigned int DI)
{
	ipu_disable_all();
	
#ifdef DI_CLOCK_EXTERNAL_MODE
	ipu_set_lcd_clock(IPU, DI, DISPLAY_PIX_CLOCK);
#endif

	ipu_startup(IPU, DI);
	
	ipu_setup(IPU, DI);
}
#endif

int ipu_display_setup(void)
{
#ifdef IPU_OUTPUT_MODE_HDMI
	debug("setup_hdmi xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r\n");
	setup_hdmi(IPU_NUM, DI_NUM);
#endif

#ifdef IPU_OUTPUT_MODE_LVDS
	debug("setup_lvds xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r\n");
	setup_lvds(IPU_NUM, DI_NUM);
#endif

#ifdef IPU_OUTPUT_MODE_LCD
	debug("setup_lcd  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r\n");
	setup_lcdif(IPU_NUM, DI_NUM);
#endif
	return 0;
}

