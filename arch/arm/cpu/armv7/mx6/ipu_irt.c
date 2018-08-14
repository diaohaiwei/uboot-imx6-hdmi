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

/* (SMFC)
0 CSI Fmem VF2 - Bayer; BPP>8; JPEG; MIPI additional channels Generic or Pixel
1 CSI Fmem VF2 - Bayer; BPP>8; JPEG; MIPI additional channels Generic or Pixel
2 CSI Fmem VF2 - Bayer; BPP>8; JPEG; MIPI additional channels Generic or Pixel
3 CSI Fmem VF2 - Bayer; BPP>8; JPEG; MIPI additional channels Generic or Pixel
*/

/*
45 Bmem IRT Rotation for post Encoding task 	Pixel
46 Bmem IRT Rotation for viewfinder task 		Pixel
47 Bmem IRT Rotation for post processing task 	Pixel

48 IRT Bmem Rotation for Encoding task 			Pixel
49 IRT Bmem Rotation for viewfinder task 		Pixel
50 IRT Bmem Rotation for post processing task 	Pixel
*/

/*
05 VDIC Bmem IC 	VF1/VF2 							Pixel
08 Fmem VDIC 		Previous field 						Pixel
09 Fmem VDIC 		Current field 						Pixel
10 Fmem VDIC 		Next field 							Pixel
13 VDIC Fmem 		Recent field from CSI 				Pixel
19 Fmem VDIC 		Transparency (alpha for channel 25) Generic
25 Fmem VDIC 		Plane #1 of the VDIC for combining 	Pixel
26 Fmem VDIC 		Plane #3 of the VDIC for combining 	Pixel
*/


/*
11 Bmem IC 		video plane for post processing task 					Pixel
12 Bmem IC 		video plane for PrP tasks (view finder or encoding) 	Pixel
14 Fmem IC 		graphics plane for PrP task (view finder or encoding) 	Pixel
15 Fmem IC 		graphics plane for post processing task 				Pixel
17 Fmem IC 		Transparency (alpha for channel 14) 					Generic
18 Fmem IC 		Transparency (alpha for channel 15) 					Generic
20 IC Bmem 		Preprocessing data from IC (encoding task) to memory 	Pixel
21 IC Bmem DMFC	Preprocessing data from IC (viewfinder task) to memory	Pixel [#NOTE#]
22 IC Bmem 		Postprocessing data from IC to memory 					Pixel
*/

/*
#NOTE#
This channel can be configured to send the
data directly to the DMFC. This is done by
programming the IC_DMFC_SEL bit.
*/