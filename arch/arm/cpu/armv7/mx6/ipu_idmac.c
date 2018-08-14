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

typedef struct {
	unsigned int channel;
	unsigned int xv;
	unsigned int yv;
	unsigned int xb;
	unsigned int yb;
	unsigned int nsb_b;
	unsigned int cf;
	unsigned int sx;
	unsigned int sy;
	unsigned int ns;
	unsigned int sdx;
	unsigned int sm;
	unsigned int scc;
	unsigned int sce;
	unsigned int sdy;
	unsigned int sdrx;
	unsigned int sdry;
	unsigned int bpp;
	unsigned int dec_sel;
	unsigned int dim;
	unsigned int so;
	unsigned int bndm;
	unsigned int bm;
	unsigned int rot;
	unsigned int hf;
	unsigned int vf;
	unsigned int the;
	unsigned int cap;
	unsigned int cae;
	unsigned int fw;
	unsigned int fh;
	unsigned int eba0;
	unsigned int eba1;
	unsigned int ilo;
	unsigned int npb;
	unsigned int pfs;
	unsigned int alu;
	unsigned int albm;
	unsigned int id;
	unsigned int th;
	unsigned int sl;
	unsigned int wid0;
	unsigned int wid1;
	unsigned int wid2;
	unsigned int wid3;
	unsigned int ofs0;
	unsigned int ofs1;
	unsigned int ofs2;
	unsigned int ofs3;
	unsigned int ubo;
	unsigned int vbo;
}ipu_channel_parameter_t;


static void config_idmac_interleaved_channel(unsigned int IPU, ipu_channel_parameter_t ipu_channel_params)
{
	unsigned int IPU_BASE_ADDRE = IPU1_ARB_BASE_ADDR;
	int w0_d0=0, w0_d1=0, w0_d2=0, w0_d3=0, w0_d4=0, w1_d0=0, w1_d1=0, w1_d2=0, w1_d3=0, w1_d4=0;

	if (IPU == 2)
		IPU_BASE_ADDRE = IPU2_ARB_BASE_ADDR;
    
	w0_d0 = ipu_channel_params.xb<<19 | ipu_channel_params.yv<<10  |ipu_channel_params.xv;
	w0_d1 = ipu_channel_params.sy<<26 | ipu_channel_params.sx<<14  | ipu_channel_params.cf<<13  | ipu_channel_params.nsb_b<<12 \
		| ipu_channel_params.yb;
	w0_d2 = ipu_channel_params.sm<<22 | ipu_channel_params.sdx<<15 | ipu_channel_params.ns<<5   | ipu_channel_params.sy>>6;
	w0_d3 = ipu_channel_params.fw<<29 | ipu_channel_params.cae<<28 | ipu_channel_params.cap<<27 | ipu_channel_params.the<<26  \
		| ipu_channel_params.vf<<25 | ipu_channel_params.hf<<24 | ipu_channel_params.rot<<23 | ipu_channel_params.bm<<21 \
		| ipu_channel_params.bndm<<18 | ipu_channel_params.so<<17 | ipu_channel_params.dim<<16 | ipu_channel_params.dec_sel<<14 \
		| ipu_channel_params.bpp<<11 | ipu_channel_params.sdry<<10 | ipu_channel_params.sdrx<<9 | ipu_channel_params.sdy<<2 \
		| ipu_channel_params.sce<<1 | ipu_channel_params.scc;
	w0_d4 = ipu_channel_params.fh<<10 | ipu_channel_params.fw>>3;

	w1_d0 = ipu_channel_params.eba1<<29 | ipu_channel_params.eba0;
	w1_d1 = ipu_channel_params.ilo<<26  | ipu_channel_params.eba1>>3;
	w1_d2 = ipu_channel_params.th<<31   | ipu_channel_params.id<<29   | ipu_channel_params.albm<<26 \
		| ipu_channel_params.alu<<25  | ipu_channel_params.pfs<<21 | ipu_channel_params.npb<<14 | ipu_channel_params.ilo>>6;
	w1_d3 = ipu_channel_params.wid3<<29 | ipu_channel_params.wid2<<26 | ipu_channel_params.wid1<<23 | ipu_channel_params.wid0<<20 \
		| ipu_channel_params.sl<<6   | ipu_channel_params.th>>1;
	w1_d4 = ipu_channel_params.ofs3<<15 | ipu_channel_params.ofs2<<10 | ipu_channel_params.ofs1<<5 | ipu_channel_params.ofs0;

	writel(w0_d0, IPU_BASE_ADDRE + CPMEM_WORD0_DATA0_INT__ADDR + (ipu_channel_params.channel<<6));
	writel(w0_d1, IPU_BASE_ADDRE + CPMEM_WORD0_DATA1_INT__ADDR + (ipu_channel_params.channel<<6));
	writel(w0_d2, IPU_BASE_ADDRE + CPMEM_WORD0_DATA2_INT__ADDR + (ipu_channel_params.channel<<6));
	writel(w0_d3, IPU_BASE_ADDRE + CPMEM_WORD0_DATA3_INT__ADDR + (ipu_channel_params.channel<<6));
	writel(w0_d4, IPU_BASE_ADDRE + CPMEM_WORD0_DATA4_INT__ADDR + (ipu_channel_params.channel<<6));

	writel(w1_d0, IPU_BASE_ADDRE + CPMEM_WORD1_DATA0_INT__ADDR + (ipu_channel_params.channel<<6));
	writel(w1_d1, IPU_BASE_ADDRE + CPMEM_WORD1_DATA1_INT__ADDR + (ipu_channel_params.channel<<6));
	writel(w1_d2, IPU_BASE_ADDRE + CPMEM_WORD1_DATA2_INT__ADDR + (ipu_channel_params.channel<<6));
	writel(w1_d3, IPU_BASE_ADDRE + CPMEM_WORD1_DATA3_INT__ADDR + (ipu_channel_params.channel<<6));
	writel(w1_d4, IPU_BASE_ADDRE + CPMEM_WORD1_DATA4_INT__ADDR + (ipu_channel_params.channel<<6));
}

void idmac_config(unsigned int IPU, unsigned int bpp, unsigned int width, unsigned int height, unsigned eba)
{
	ipu_channel_parameter_t ipu_channel_params;

	memset(&ipu_channel_params, 0, sizeof(ipu_channel_parameter_t));

#ifdef IPU_USE_DC_CHANNEL
	ipu_channel_params.channel = 28;
#else
	ipu_channel_params.channel	 = 23;
#endif
	ipu_channel_params.eba0 = eba;
	ipu_channel_params.eba1 = ipu_channel_params.eba0;
	ipu_channel_params.fw = width - 1;  //frame width
	ipu_channel_params.fh = height - 1;  //frame hight
	ipu_channel_params.sl = width * (bpp / 8) - 1;
	ipu_channel_params.npb = 15;  //16 pixels per burst
	ipu_channel_params.pfs = 7;  //7->RGB

	if(bpp==16)
	{
		ipu_channel_params.bpp = 3;  //0->32bpp; 1->24bpp; 3->16bpp
		ipu_channel_params.wid0 = 5 - 1;
		ipu_channel_params.wid1 = 6 - 1;
		ipu_channel_params.wid2 = 5 - 1;
		ipu_channel_params.wid3 = 0;
		ipu_channel_params.ofs0 = 0;
		ipu_channel_params.ofs1 = 5;
		ipu_channel_params.ofs2 = 11;
		ipu_channel_params.ofs3 = 0;
	}
	if(bpp==24)
	{
		ipu_channel_params.bpp = 1;  //0->32bpp; 1->24bpp; 3->16bpp
		ipu_channel_params.wid0 = 8 - 1;
		ipu_channel_params.wid1 = 8 - 1;
		ipu_channel_params.wid2 = 8 - 1;
		ipu_channel_params.wid3 = 0;
		ipu_channel_params.ofs0 = 0;
		ipu_channel_params.ofs1 = 8;
		ipu_channel_params.ofs2 = 16;
		ipu_channel_params.ofs3 = 0;	
	}
	if(bpp==32)
	{
		ipu_channel_params.bpp = 0;  //0->32bpp; 1->24bpp; 3->16bpp
		ipu_channel_params.wid0 = 8 - 1;
		ipu_channel_params.wid1 = 8 - 1;
		ipu_channel_params.wid2 = 8 - 1;
		ipu_channel_params.wid3 = 8 - 1;
		ipu_channel_params.ofs0 = 8;
		ipu_channel_params.ofs1 = 16;
		ipu_channel_params.ofs2 = 24;
		ipu_channel_params.ofs3 = 0;
	}

#ifdef DISPLAY_INTERLACED
	ipu_channel_params.so = 1;	//Scan order is interlaced
	ipu_channel_params.ilo = (ipu_channel_params.sl + 1) / 8;  //Interlace Offset
	ipu_channel_params.sl = (ipu_channel_params.sl + 1) * 2 - 1;  //Stride Line
#endif

	config_idmac_interleaved_channel(IPU, ipu_channel_params);
}
