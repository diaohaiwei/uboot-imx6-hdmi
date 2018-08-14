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




void ipu_write_field(unsigned int IPU, unsigned int ID_addr, unsigned int ID_mask, unsigned int data)
{
	unsigned int rdata; 
	unsigned int IPU_BASE_ADDRE = IPU1_ARB_BASE_ADDR;
	
	if (IPU == 2)
		IPU_BASE_ADDRE = IPU2_ARB_BASE_ADDR;

	ID_addr += IPU_BASE_ADDRE;
	rdata = readl(ID_addr);
	rdata &= ~ID_mask;
	rdata |= (data*(ID_mask & -ID_mask))&ID_mask;
	writel(rdata, ID_addr);
}


