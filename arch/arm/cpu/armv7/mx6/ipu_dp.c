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

extern void ipu_write_field(unsigned int IPU, unsigned int ID_addr, unsigned int ID_mask, unsigned int data);

/*
23 Fmem=>DP 	DP primary flow - main plane 				Pixel
24 Fmem=>DP 	DP secondary flow - main plane 				Pixel
27 Fmem=>DP 	DP primary flow - auxiliary plane 			Pixel
29 Fmem=>DP 	DP secondary flow - auxiliary plane 		Pixel
31 Fmem=>DP 	Transparency (alpha for channel 27) 		Generic
33 Fmem=>DP 	Transparency (alpha for channel 29) 		Generic
51 Fmem=>DP 	Transparency (alpha for channel 23) 		Generic
52 Fmem=>DP 	Transparency (alpha for channel 24) 		Generic
*/

void dp_config(unsigned int IPU)
{
#ifndef IPU_USE_DC_CHANNEL
	ipu_write_field(IPU, SRM_DP_COM_CONF_SYNC__DP_GAMMA_EN_SYNC, 0);
	ipu_write_field(IPU, SRM_DP_COM_CONF_SYNC__DP_CSC_YUV_SAT_MODE_SYNC, 0); //SAT mode is zero
	ipu_write_field(IPU, SRM_DP_COM_CONF_SYNC__DP_CSC_GAMUT_SAT_EN_SYNC, 0); //GAMUT en (RGB...)
	ipu_write_field(IPU, SRM_DP_COM_CONF_SYNC__DP_CSC_DEF_SYNC, 0); //CSC Disable
	ipu_write_field(IPU, SRM_DP_COM_CONF_SYNC__DP_COC_SYNC, 0); //no cursor
	ipu_write_field(IPU, SRM_DP_COM_CONF_SYNC__DP_GWCKE_SYNC, 0); //color keying disabled
	ipu_write_field(IPU, SRM_DP_COM_CONF_SYNC__DP_GWAM_SYNC, 1); //1=global alpha,0=local alpha
	ipu_write_field(IPU, SRM_DP_COM_CONF_SYNC__DP_GWSEL_SYNC, 1); //1=graphic is FG,0=graphic is BG
#endif
}

void dp_enable(unsigned int IPU)
{
#ifdef IPU_USE_DC_CHANNEL
//	ipu_write_field(IPU, IPU_IPU_CH_DB_MODE_SEL_0__DMA_CH_DB_MODE_SEL_28, 1);
	ipu_write_field(IPU, IPU_IPU_CH_TRB_MODE_SEL_0__DMA_CH_TRB_MODE_SEL_28, 1);
	ipu_write_field(IPU, IPU_IPU_CH_BUF0_RDY0__DMA_CH_BUF0_RDY_28, 1);
	ipu_write_field(IPU, IPU_IDMAC_CH_EN_1__IDMAC_CH_EN_28, 0x1);  //enable channel 28
#else
//	ipu_write_field(IPU, IPU_IPU_CH_DB_MODE_SEL_0__DMA_CH_DB_MODE_SEL_23, 1);
	ipu_write_field(IPU, IPU_IPU_CH_TRB_MODE_SEL_0__DMA_CH_TRB_MODE_SEL_23, 1);
	ipu_write_field(IPU, IPU_IPU_CH_BUF0_RDY0__DMA_CH_BUF0_RDY_23, 1);
	ipu_write_field(IPU, IPU_IDMAC_CH_EN_1__IDMAC_CH_EN_23, 0x1);  //enable channel 23
#endif	
}
