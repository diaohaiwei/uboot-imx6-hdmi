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

void dmfc_config(unsigned int IPU)
{
	unsigned int IPU_BASE_ADDRE = IPU1_ARB_BASE_ADDR;

	if(IPU == 2)
		IPU_BASE_ADDRE = IPU2_ARB_BASE_ADDR;

	// Same as _ipu_dmfc_init() in kernel
	writel(0x202020F6, IPU_BASE_ADDRE + IPU_DMFC_WR_CHAN_DEF__ADDR);
#ifdef IPU_USE_DC_CHANNEL
	writel(0x00000088, IPU_BASE_ADDRE + IPU_DMFC_WR_CHAN__ADDR);
	writel(0x00009694, IPU_BASE_ADDRE + IPU_DMFC_DP_CHAN__ADDR);
#else
	writel(0x00000090, IPU_BASE_ADDRE + IPU_DMFC_WR_CHAN__ADDR);
	writel(0x0000968A, IPU_BASE_ADDRE + IPU_DMFC_DP_CHAN__ADDR);
#endif
	writel(0x2020F6F6, IPU_BASE_ADDRE + IPU_DMFC_DP_CHAN_DEF__ADDR);

	// Set sync refresh channels and CSI->mem channel as high priority, copy from kernel
	writel(0x18800001, IPU_BASE_ADDRE + IPU_IDMAC_CH_PRI_1__ADDR);

	// Set MCU_T to divide MCU access window into 2, copy from kernel
	writel(0x00400000 | (8 << 18), IPU_BASE_ADDRE + IPU_IPU_DISP_GEN__ADDR);
}
