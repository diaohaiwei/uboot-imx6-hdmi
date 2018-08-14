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

#define DI_SDC_WAVEFORM     3
#define DI_COUNTER_APIXEL   		6

#define DC_CHANNEL_READ					0
#define DC_CHANNEL_DC_SYNC_OR_ASYNC	1
#define DC_CHANNEL_DC_ASYNC				2
#define DC_CHANNEL_COMMAND_1			3
#define DC_CHANNEL_COMMAND_2			4
#define DC_CHANNEL_DP_PRIMARY			5
#define DC_CHANNEL_DP_SECONDARY			6

extern void ipu_write_field(unsigned int IPU, unsigned int ID_addr, unsigned int ID_mask, unsigned int data);

/*
28 Fmem=>DC DC channel for both sync and async flows Pixel

40 DC=>Fmem DC read channel Generic
41 Fmem=>DC DC async flow Generic
42 Fmem=>DC DC command stream Generic
43 Fmem=>DC DC command stream Generic
44 Fmem=>DC DC output mask Generic
*/

static void dc_config_common(unsigned int IPU, unsigned width)
{
	//GENERAL 
	ipu_write_field(IPU, IPU_DC_GEN__SYNC_PRIORITY_5, 1);  //sets the priority of channel #5 to high.
	ipu_write_field(IPU, IPU_DC_GEN__SYNC_PRIORITY_1, 1);  //sets the priority of channel #1 to high.
	ipu_write_field(IPU, IPU_DC_GEN__MASK4CHAN_5, 0);  // mask channel is associated to the sync flow via DC (without DP)
	ipu_write_field(IPU, IPU_DC_GEN__MASK_EN, 0);  // mask channel is disabled.(mask channel can associated with different IDMAC channels)
	ipu_write_field(IPU, IPU_DC_GEN__DC_CH5_TYPE, 0);  // alternate sync or asyn flow
	ipu_write_field(IPU, IPU_DC_GEN__SYNC_1_6, 2);  //DC channel #1 handles sync flow

	//DISP_CONF  
	ipu_write_field(IPU, IPU_DC_DISP_CONF1_1__ADDR_INCREMENT_1, 0);  //automatical address increase by 1
	ipu_write_field(IPU, IPU_DC_DISP_CONF1_1__DISP_TYP_1, 2);  //paralel display without byte enable
	ipu_write_field(IPU, IPU_DC_DISP_CONF2_1__SL_1, width);  //stride line

	//DC_UGDE 
	ipu_write_field(IPU, IPU_DC_UGDE1_0__NF_NL_1, 0);  //NL->0,NF->1,NFIELD->2
	ipu_write_field(IPU, IPU_DC_UGDE1_0__AUTORESTART_1, 0);  //no autorestart
	ipu_write_field(IPU, IPU_DC_UGDE1_0__ODD_EN_1, 0);  // disable 'odd'
	ipu_write_field(IPU, IPU_DC_UGDE1_0__COD_ODD_START_1, 1);  //words 1 1st part
	ipu_write_field(IPU, IPU_DC_UGDE1_0__COD_EV_START_1, 5);  //word 2 2nd part
	ipu_write_field(IPU, IPU_DC_UGDE1_0__COD_EV_PRIORITY_1,  1);  //enabled. all others are disabled.
#ifdef IPU_USE_DC_CHANNEL
	ipu_write_field(IPU, IPU_DC_UGDE1_0__ID_CODED_1, 0x1);  //DC_CHANNEL_DC_SYNC_OR_ASYNC
#else
	ipu_write_field(IPU, IPU_DC_UGDE1_0__ID_CODED_1, 0x3);  //DC_CHANNEL_DP_PRIMARY
#endif
	ipu_write_field(IPU, IPU_DC_UGDE1_1__STEP_1, 0);  //every data
	ipu_write_field(IPU, IPU_DC_UGDE1_2__OFFSET_DT_1, 0);  //no offset
	ipu_write_field(IPU, IPU_DC_UGDE1_3__STEP_REPEAT_1, (width - 1));

	//DC_MAP, same as _ipu_init_dc_mappings() in kernel
	// config DC mapping point 1, IPU_PIX_FMT_RGB24
	ipu_write_field(IPU, IPU_DC_MAP_CONF_15__MD_OFFSET_0, 7);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_15__MD_MASK_0, 0xFF);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_15__MD_OFFSET_1, 15);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_15__MD_MASK_1, 0xFF);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_16__MD_OFFSET_2, 23);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_16__MD_MASK_2, 0xFF);

	ipu_write_field(IPU, IPU_DC_MAP_CONF_0__MAPPING_PNTR_BYTE0_0, 0);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_0__MAPPING_PNTR_BYTE1_0, 1);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_0__MAPPING_PNTR_BYTE2_0, 2);

	// config DC mapping point 2, IPU_PIX_FMT_RGB666
	ipu_write_field(IPU, IPU_DC_MAP_CONF_16__MD_OFFSET_3, 5);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_16__MD_MASK_3, 0xFC);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_17__MD_OFFSET_4, 11);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_17__MD_MASK_4, 0xFC);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_17__MD_OFFSET_5, 17);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_17__MD_MASK_5, 0xFC);

	ipu_write_field(IPU, IPU_DC_MAP_CONF_0__MAPPING_PNTR_BYTE0_1, 3);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_0__MAPPING_PNTR_BYTE1_1, 4);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_0__MAPPING_PNTR_BYTE2_1, 5);

	// config DC mapping point 3, IPU_PIX_FMT_YUV444
	ipu_write_field(IPU, IPU_DC_MAP_CONF_18__MD_OFFSET_6, 15);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_18__MD_MASK_6, 0xFF);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_18__MD_OFFSET_7, 23);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_18__MD_MASK_7, 0xFF);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_19__MD_OFFSET_8, 7);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_19__MD_MASK_8, 0xFF);

	ipu_write_field(IPU, IPU_DC_MAP_CONF_1__MAPPING_PNTR_BYTE0_2, 6);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_1__MAPPING_PNTR_BYTE1_2, 7);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_1__MAPPING_PNTR_BYTE2_2, 8);

	// config DC mapping point 4, IPU_PIX_FMT_RGB565
	ipu_write_field(IPU, IPU_DC_MAP_CONF_19__MD_OFFSET_9, 4);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_19__MD_MASK_9, 0xF8);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_20__MD_OFFSET_10, 10);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_20__MD_MASK_10, 0xFC);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_20__MD_OFFSET_11, 15);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_20__MD_MASK_11, 0xF8);

	ipu_write_field(IPU, IPU_DC_MAP_CONF_1__MAPPING_PNTR_BYTE0_3, 9);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_1__MAPPING_PNTR_BYTE1_3, 10);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_1__MAPPING_PNTR_BYTE2_3, 11);

	// config DC mapping point 5, IPU_PIX_FMT_LVDS666
	ipu_write_field(IPU, IPU_DC_MAP_CONF_21__MD_OFFSET_12, 5);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_21__MD_MASK_12, 0xFC);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_21__MD_OFFSET_13, 13);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_21__MD_MASK_13, 0xFC);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_22__MD_OFFSET_14, 21);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_22__MD_MASK_14, 0xFC);

	ipu_write_field(IPU, IPU_DC_MAP_CONF_2__MAPPING_PNTR_BYTE0_4, 12);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_2__MAPPING_PNTR_BYTE1_4, 13);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_2__MAPPING_PNTR_BYTE2_4, 14);

	// config DC mapping point 6,7, IPU_PIX_FMT_VYUY
	ipu_write_field(IPU, IPU_DC_MAP_CONF_22__MD_OFFSET_15, 7);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_22__MD_MASK_15, 0xFF);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_23__MD_OFFSET_16, 0);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_23__MD_MASK_16, 0x00);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_23__MD_OFFSET_17, 15);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_23__MD_MASK_17, 0xFF);

	ipu_write_field(IPU, IPU_DC_MAP_CONF_2__MAPPING_PNTR_BYTE0_5, 15);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_2__MAPPING_PNTR_BYTE1_5, 16);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_2__MAPPING_PNTR_BYTE2_5, 17);

	ipu_write_field(IPU, IPU_DC_MAP_CONF_24__MD_OFFSET_18, 0);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_24__MD_MASK_18, 0x00);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_24__MD_OFFSET_19, 7);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_24__MD_MASK_19, 0xFF);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_25__MD_OFFSET_20, 15);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_25__MD_MASK_20, 0xFF);

	ipu_write_field(IPU, IPU_DC_MAP_CONF_3__MAPPING_PNTR_BYTE0_6, 18);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_3__MAPPING_PNTR_BYTE1_6, 19);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_3__MAPPING_PNTR_BYTE2_6, 20);

	// config DC mapping point 8,9, IPU_PIX_FMT_UYVY
	ipu_write_field(IPU, IPU_DC_MAP_CONF_3__MAPPING_PNTR_BYTE0_7, 18);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_3__MAPPING_PNTR_BYTE1_7, 19);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_3__MAPPING_PNTR_BYTE2_7, 20);

	ipu_write_field(IPU, IPU_DC_MAP_CONF_4__MAPPING_PNTR_BYTE0_8, 15);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_4__MAPPING_PNTR_BYTE1_8, 16);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_4__MAPPING_PNTR_BYTE2_8, 17);

	// config DC mapping point 10,11, IPU_PIX_FMT_YUYV
	ipu_write_field(IPU, IPU_DC_MAP_CONF_4__MAPPING_PNTR_BYTE0_9, 17);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_4__MAPPING_PNTR_BYTE1_9, 16);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_4__MAPPING_PNTR_BYTE2_9, 15);

	ipu_write_field(IPU, IPU_DC_MAP_CONF_5__MAPPING_PNTR_BYTE0_10, 16);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_5__MAPPING_PNTR_BYTE1_10, 17);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_5__MAPPING_PNTR_BYTE2_10, 15);

	// config DC mapping point 12,13, IPU_PIX_FMT_YVYU
	ipu_write_field(IPU, IPU_DC_MAP_CONF_5__MAPPING_PNTR_BYTE0_11, 16);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_5__MAPPING_PNTR_BYTE1_11, 17);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_5__MAPPING_PNTR_BYTE2_11, 15);

	ipu_write_field(IPU, IPU_DC_MAP_CONF_6__MAPPING_PNTR_BYTE0_12, 17);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_6__MAPPING_PNTR_BYTE1_12, 16);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_6__MAPPING_PNTR_BYTE2_12, 15);

	// config DC mapping point 14, IPU_PIX_FMT_GBR24, IPU_PIX_FMT_VYU444
	ipu_write_field(IPU, IPU_DC_MAP_CONF_6__MAPPING_PNTR_BYTE0_13, 2);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_6__MAPPING_PNTR_BYTE1_13, 0);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_6__MAPPING_PNTR_BYTE2_13, 1);

	// config DC mapping point 15, IPU_PIX_FMT_BGR24
	ipu_write_field(IPU, IPU_DC_MAP_CONF_7__MAPPING_PNTR_BYTE0_14, 2);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_7__MAPPING_PNTR_BYTE1_14, 1);
	ipu_write_field(IPU, IPU_DC_MAP_CONF_7__MAPPING_PNTR_BYTE2_14, 0);
}



static void microcode_config (int ipu_num, int word, int stop, char opcode[10], int lf, int af, int operand, int mapping, int waveform, int gluelogic, int sync)
{
	unsigned int IPU_BASE_ADDRE = IPU1_ARB_BASE_ADDR;
	unsigned int LowWord = 0;
	unsigned int HighWord = 0;
	unsigned int opcode_fixed;

	if  (ipu_num==2)
	   IPU_BASE_ADDRE=IPU2_ARB_BASE_ADDR;

//=========================================================================================================	
	//HLG - HOLD WORD GENERIC:  hold operand in register for next operating, without display access 
	if (!strcmp(opcode, "HLG")){
							    //[4:0] = 15'b0
		LowWord = LowWord | (operand << 5);         //[31:5]

		HighWord = HighWord | (operand >> 27);      //[36:32]
		opcode_fixed = 0x0;            		    //0-0
		HighWord = HighWord | (opcode_fixed << 5);  //[40:37]
		HighWord = HighWord | (stop << 9);          //[41]
    	}
//=========================================================================================================	
	//WRITE DATA to display
	if (!strcmp(opcode, "WROD")){
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11]
		LowWord = LowWord | (mapping   << 15);      //[19:15]
	//Generic data, this data is attached with OR to 16 MSB of mapped address	
		LowWord = LowWord | (operand << 20);        //[31:20]

		HighWord = HighWord | (operand >> 12);      //[35:32]
		opcode_fixed = 0x18 | (lf << 1);            //1-1-0-lf-0
		HighWord = HighWord | (opcode_fixed << 4);  //[40:36]
		HighWord = HighWord | (stop << 9);          //[41]
    	}
//=========================================================================================================	
	//ATTACH AND HOLD ADDRESS in REGISTER, Adding Mapped Address to held data and hold in register
	if (!strcmp(opcode, "HLOAR")){
		sync = 0;//fixed
		gluelogic = 0;//fixed
		waveform = 0;//fixed
		operand = 0;//fixed
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11]
		LowWord = LowWord | (mapping   << 15);      //[19:15]
		LowWord = LowWord | (operand << 20);        //[31:20]

		HighWord = HighWord | (operand >> 12);      //[32]
		//1 0 0 0 1 1 1 AF:  AF - Address shift flag is defined by user:
		//0: 24bit LSB operating or no operating,
		//1: 8 bit right shift, 24 MSB operating,
		opcode_fixed = 0x8E | (af << 0);            //1-0-0-0-1-1-1-AF
		HighWord = HighWord | (opcode_fixed << 1);  //[40:33]
		HighWord = HighWord | (stop << 9);          //[41]
    	}
//=========================================================================================================	
	//ATTACH AND WRITE ADDRESS to DISPLAY, Adding Mapped Address to held data and write to display
	if (!strcmp(opcode, "WROAR")){
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11]
		LowWord = LowWord | (mapping   << 15);      //[19:15]
		LowWord = LowWord | (operand << 20);        //[31:20]

		HighWord = HighWord | (operand >> 12);      //[32]
		//1 1 0 0 1 1 1 AF:  AF - Address shift flag is defined by user:
		//0: 24bit LSB operating or no operating,
		//1: 8 bit right shift, 24 MSB operating,
		opcode_fixed = 0xCE | (af << 0);            //1-1-0-0-1-1-1-AF
		HighWord = HighWord | (opcode_fixed << 1);  //[40:33]
		HighWord = HighWord | (stop << 9);          //[41]
    	}
//=========================================================================================================	
	//ATTACH AND HOLD DATA in REGISTER, Adding Mapped Address to held data and hold in register
	if (!strcmp(opcode, "HLODR")){
		sync = 0;//fixed
		gluelogic = 0;//fixed
		waveform = 0;//fixed
		operand = 0;//fixed
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11]
		LowWord = LowWord | (mapping   << 15);      //[19:15]
		LowWord = LowWord | (operand << 20);        //[31:20]

		HighWord = HighWord | (operand >> 12);      //[32]
		opcode_fixed = 0x8C;                         //
		HighWord = HighWord | (opcode_fixed << 1);  //[40:33]
		HighWord = HighWord | (stop << 9);          //[41]
    	}
//=========================================================================================================	
	//WRODR -  WRITE_DATA_OPCODE - ATTACH AND WRITE DATA to DISPLAY, 
	//Adding Mapped Data to hold data in internal register and write to display
	if (!strcmp(opcode, "WRODR")){
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11]
		LowWord = LowWord | (mapping   << 15);      //[19:15]
		LowWord = LowWord | (operand << 20);        //[31:20]

		HighWord = HighWord | (operand >> 12);      //[32]
		opcode_fixed = 0xCC;                         //
		HighWord = HighWord | (opcode_fixed << 1);  //[40:33]
		HighWord = HighWord | (stop << 9);          //[41]
    	}
//=========================================================================================================	
	//WRITE BIT CHANNEL. merging 1bit mask from IDMAC mask channel with data and write to display
	if (!strcmp(opcode, "WRBC")){
		operand = 0;//fixed
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11]
		LowWord = LowWord | (mapping   << 15);      //[19:15]
		LowWord = LowWord | (operand << 20);        //[31:20]

		opcode_fixed = 0x19B;                       //
		HighWord = HighWord | (opcode_fixed << 0);  //[40:32]
		HighWord = HighWord | (stop << 9);          //[41]
    	}
//=========================================================================================================	
	//WAIT CLOCK - Waiting N clocks
	if (!strcmp(opcode, "WCLK")){
		sync = 0;//fixed
		gluelogic = 0;//fixed
		waveform = 0;//fixed
		mapping = 0;//fixed
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11]
		LowWord = LowWord | (mapping   << 15);      //[19:15]
		LowWord = LowWord | (operand << 20);        //[31:20]

		HighWord = HighWord | (operand >> 12);      //[32]
		opcode_fixed = 0xC9;                         //
		HighWord = HighWord | (opcode_fixed << 1);  //[40:33]
		HighWord = HighWord | (stop << 9);          //[41]
    	}
//=========================================================================================================	
	//WAIT STATUS - 3 microcodes command loop for  checking display status by POLLING READ. THE WSTS_II has to be used immediately after WSTS _III
	if (!strcmp(opcode, "WSTS_III")){
		stop = 0;//fixed
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11]
		LowWord = LowWord | (mapping   << 15);      //[19:15]
		//number of IPU's clock to latch data from DI. after WSTS cycle start, defined by user
		LowWord = LowWord | (operand << 20);        //[31:20]

		HighWord = HighWord | (operand >> 12);      //[32]
		opcode_fixed = 0x8B;                         //
		HighWord = HighWord | (opcode_fixed << 1);  //[40:33]
		HighWord = HighWord | (stop << 9);          //[41]
    	}
//=========================================================================================================	
	//WAIT STATUS - second command in  3 microcode commands loop or first command in 2 microcode commands loop for  checking display status by POLLING READ. THE WSTS_I has to be used immediately after WSTS_II
	if (!strcmp(opcode, "WSTS_II")){
		stop = 0;//fixed
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11]
		LowWord = LowWord | (mapping   << 15);      //[19:15]
		//number of IPU's clock to latch data from DI. after WSTS cycle start, defined by user
		LowWord = LowWord | (operand << 20);        //[31:20]

		HighWord = HighWord | (operand >> 12);      //[32]
		opcode_fixed = 0x8A;                        //
		HighWord = HighWord | (opcode_fixed << 1);  //[40:33]
		HighWord = HighWord | (stop << 9);          //[41]
    	}
//=========================================================================================================	
	//WAIT STATUS - third command in  3 microcode commands loop or second command in 2 microcode commands loop or first command in one command loop for  checking display status by POLLING READ.
	if (!strcmp(opcode, "WSTS_I")){
		stop = 0;//fixed
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11]
		LowWord = LowWord | (mapping   << 15);      //[19:15]
		//number of IPU's clock to latch data from DI. after WSTS cycle start, defined by user
		LowWord = LowWord | (operand << 20);        //[31:20]

		HighWord = HighWord | (operand >> 12);      //[32]
		opcode_fixed = 0x89;                         //
		HighWord = HighWord | (opcode_fixed << 1);  //[40:33]
		HighWord = HighWord | (stop << 9);          //[41]
    	}
//=========================================================================================================	
	//HOLD ADDRESS in REGISTER: display's address which is calculated by IPU, is stored in register
	if (!strcmp(opcode, "HLOA")){
		sync = 0;//fixed
		gluelogic = 0;//fixed
		waveform = 0;//fixed
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11]
		LowWord = LowWord | (mapping   << 15);      //[19:15]
	//Generic data, this data is attached with OR to 16 MSB of mapped address	
		LowWord = LowWord | (operand << 20);        //[31:20]

		HighWord = HighWord | (operand >> 12);      //[35:32]
		//1 0 1 0 AF:  AF - Address shift flag is defined by user: 
		//0: 24bit LSB operating or no operating, 
		//1: 8 bit right shift, 24 MSB operating,
		opcode_fixed = 0x14 | (af << 0);            //1-0-1-0-AF
		HighWord = HighWord | (opcode_fixed << 4);  //[40:36]
		HighWord = HighWord | (stop << 9);          //[41]
    	}
//=========================================================================================================	
	//HOLD ADDRESS in REGISTER: display's address which is calculated by IPU, is stored in register
	if (!strcmp(opcode, "WROA")){
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11]
		LowWord = LowWord | (mapping   << 15);      //[19:15]
	//Generic data, this data is attached with OR to 16 MSB of mapped address	
		LowWord = LowWord | (operand << 20);        //[31:20]

		HighWord = HighWord | (operand >> 12);      //[35:32]
		//1 0 1 0 AF:  AF - Address shift flag is defined by user: 
		//0: 24bit LSB operating or no operating, 
		//1: 8 bit right shift, 24 MSB operating,
		opcode_fixed = 0x1C | (af << 0);            //1-1-1-0-AF
		HighWord = HighWord | (opcode_fixed << 4);  //[40:36]
		HighWord = HighWord | (stop << 9);          //[41]
    	}
//=========================================================================================================	
	//HOLD DATA in REGISTER
	if (!strcmp(opcode, "HLOD")){
		sync = 0;//fixed
		gluelogic=0;//fixed
		waveform=0;//fixed
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11]
		LowWord = LowWord | (mapping   << 15);      //[19:15]
	//Generic data, this data is attached with OR to 16 MSB of mapped address	
		LowWord = LowWord | (operand << 20);        //[31:20]

		HighWord = HighWord | (operand >> 12);      //[35:32]
		opcode_fixed = 0x10;                        //1-0-0-0-0
		HighWord = HighWord | (opcode_fixed << 4);  //[40:36]
		HighWord = HighWord | (stop << 9);          //[41]
    	}
//=========================================================================================================	
	//WRG - Write 24bit word to DI and Hold the word in register
	//WRITE WORD GENERIC
	//This opcode is used for sending "a user's code", which is stored in microcode memory  to  a display
	//NO MAPPING
	if (!strcmp(opcode, "WRG")){
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11]
		LowWord = LowWord | (operand << 15);        //[31:15]

		HighWord = HighWord | (operand >> 17);      //[38:32]
		opcode_fixed = 1;                           //0-1
		HighWord = HighWord | (opcode_fixed << 7);  //[40:39]
		HighWord = HighWord | (stop << 9);          //[41]		
	}
//=========================================================================================================	
        //RD - Read command parameters:
	if (!strcmp(opcode, "RD")){
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11] = synchronization
		LowWord = LowWord | (mapping   << 15);      //[19:15]
		//operand - means delay value in DI_CLK for display's data latching by DI, defined by user
		//number of IPU's clock to latch data from DI. after WSTS cycle start
		LowWord = LowWord | (operand << 20);        //[31:20]

		HighWord = HighWord | (operand >> 12);      //[32]
		opcode_fixed = 0x88;                        //
		HighWord = HighWord | (opcode_fixed << 1);  //[40:33]
		HighWord = HighWord | (stop << 9);          //[41]

	}
//=========================================================================================================	
        //WAIT FOR ACKNOWLEDGE
	if (!strcmp(opcode, "WACK")){
		mapping = 0;//fixed
		operand = 0;//fixed
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11] = synchronization
		LowWord = LowWord | (mapping   << 15);      //[19:15]
		//operand - means delay value in DI_CLK for display's data latching by DI, defined by user
		//number of IPU's clock to latch data from DI. after WSTS cycle start
		LowWord = LowWord | (operand << 20);        //[31:20]

		opcode_fixed = 0x11A;                       //
		HighWord = HighWord | (opcode_fixed << 0);  //[40:32]
		HighWord = HighWord | (stop << 9);          //[41]

	}
//=========================================================================================================	
        //HMA  - HOLD_MICROCODE_ADDRESS - hold operand in special Microcode address register
	if (!strcmp(opcode, "HMA")){
		                                            //[4:0]
		//Microcode address defined by user
		LowWord = LowWord | (operand << 5);         //[12:5]
		opcode_fixed = 0x2;                         //
		HighWord = HighWord | (opcode_fixed << 5);  //[40:37]
		HighWord = HighWord | (stop << 9);          //[41]

	}
//========================================================================================================= 
		//HMA1  - HOLD_MICROCODE_ADDRESS - hold operand in special Microcode address register
	if (!strcmp(opcode, "HMA1")){
													//[4:0]
		//Microcode address defined by user
		LowWord = LowWord | (operand << 5); 		//[12:5]
		opcode_fixed = 0x1; 						//
		HighWord = HighWord | (opcode_fixed << 5);	//[40:37]
		HighWord = HighWord | (stop << 9);			//[41]

	}
//=========================================================================================================	
        //BMA  - BRANCH_MICROCODE_ADDRESS jump to Microcode address which is stored at 
	//special Microcode address register
	if (!strcmp(opcode, "BMA")){
		                                            //[36:0] ==0
		LowWord = LowWord | sync; 		//[2:0]
		LowWord = LowWord | (operand << 5); 		//[12:5]
		opcode_fixed = 0x3;                         //
		HighWord = HighWord | (af << 3);	//[35]
		HighWord = HighWord | (lf << 4);	//[36]
		HighWord = HighWord | (opcode_fixed << 5);  //[40:37]
		HighWord = HighWord | (stop << 9);          //[41]

	}
//=========================================================================================================	
        //Additional information for event masking
	if (!strcmp(opcode, "MSK")){
		sync = 0;//fixed
		gluelogic = 0;//fixed
		waveform = 0;//fixed
		stop = 0;//fixed 
		//(no mapping)
		LowWord = LowWord | sync;                   //[3:0]
		LowWord = LowWord | (gluelogic << 4);       //[10:4]
		LowWord = LowWord | (waveform  << 11);      //[14:11] = synchronization
		//[32-28] - 0
                //[27] e0m- event 0 mask, defined by user
                //[26] e1m - event 1 mask, defined by user
                //[25] e2m - event 2 mask, defined by user
                //[24] e3m - event 3 mask, defined by user
                //[23] nfm- new frame mask , defined by user
                //[22] nlm- new line mask , defined by user
                //[21] nfldm- new field mask , defined by user
                //[20] eofm- end of frame mask, defined by user
                //[19] eolm- end of line mask, defined by user
                //[18] eofldm-  end of field mask, defined by user
                //[17] nadm- new address mask, defined by user
                //[16] ncm- new channel mask, defined by user
                //[15] dm - data mask, defined by user 
		LowWord = LowWord | (operand   << 15);      //[19:15]

		HighWord = HighWord | (operand >> 17);      //[32]
		opcode_fixed = 0xC8;                        //
		HighWord = HighWord | (opcode_fixed << 1);  //[40:33]
		HighWord = HighWord | (stop << 9);          //[41]

	}
//=========================================================================================================	
		
	writel(LowWord, IPU_BASE_ADDRE + IPU_MEM_DC_MICROCODE_BASE_ADDR + word * 8);
	writel(HighWord, IPU_BASE_ADDRE + IPU_MEM_DC_MICROCODE_BASE_ADDR + word * 8 + 4);
}

static void microcode_event(int ipu_num, int channel, char event[8], int priority, int address)
{
   if(channel == 0){

     if(!strcmp(event, "NL")){
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_0__COD_NL_START_CHAN_0, address);
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_0__COD_NL_PRIORITY_CHAN_0, priority);
     }//NL
     if(!strcmp(event, "NF")){
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_0__COD_NF_START_CHAN_0, address);
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_0__COD_NF_PRIORITY_CHAN_0, priority);
     }//NF
     if(!strcmp(event, "NFIELD")){
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_0__COD_NFIELD_START_CHAN_0, address);
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_0__COD_NFIELD_PRIORITY_CHAN_0, priority);
     }//NFIELD
      if(!strcmp(event, "EOF")){
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_0__COD_EOF_START_CHAN_0, address);
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_0__COD_EOF_PRIORITY_CHAN_0, priority);
     }//EOF
      if(!strcmp(event, "EOFIELD")){
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_0__COD_EOFIELD_START_CHAN_0, address);
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_0__COD_EOFIELD_PRIORITY_CHAN_0, priority);
     }//EOFIELD
      if(!strcmp(event, "EOL")){
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_0__COD_EOL_START_CHAN_0, address);
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_0__COD_EOL_PRIORITY_CHAN_0, priority);
     }//EOL
      if(!strcmp(event, "NEW_CHAN")){
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_0__COD_NEW_CHAN_START_CHAN_0, address);
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_0__COD_NEW_CHAN_PRIORITY_CHAN_0, priority);
     }//NEW_CHAN
      if(!strcmp(event, "NEW_ADDR")){
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_0__COD_NEW_ADDR_START_CHAN_0, address);
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_0__COD_NEW_ADDR_PRIORITY_CHAN_0, priority);
     }//NEW_ADDR
      if(!strcmp(event, "NEW_DATA")){
       ipu_write_field(ipu_num, IPU_DC_RL4_CH_0__COD_NEW_DATA_START_CHAN_0, address);
       ipu_write_field(ipu_num, IPU_DC_RL4_CH_0__COD_NEW_DATA_PRIORITY_CHAN_0, priority);
     }//NEW_DATA
   }//channel 0 ******************************************************************************

//*************************************************************************************   
   if(channel == 1){

     if(!strcmp(event, "NL")){
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_1__COD_NL_START_CHAN_1, address);
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_1__COD_NL_PRIORITY_CHAN_1, priority);
     }//NL
     if(!strcmp(event, "NF")){
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_1__COD_NF_START_CHAN_1, address);
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_1__COD_NF_PRIORITY_CHAN_1, priority);
     }//NF
     if(!strcmp(event, "NFIELD")){
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_1__COD_NFIELD_START_CHAN_1, address);
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_1__COD_NFIELD_PRIORITY_CHAN_1, priority);
     }//NFIELD
      if(!strcmp(event, "EOF")){
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_1__COD_EOF_START_CHAN_1, address);
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_1__COD_EOF_PRIORITY_CHAN_1, priority);
     }//EOF
      if(!strcmp(event, "EOFIELD")){
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_1__COD_EOFIELD_START_CHAN_1, address);
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_1__COD_EOFIELD_PRIORITY_CHAN_1, priority);
     }//EOFIELD
      if(!strcmp(event, "EOL")){
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_1__COD_EOL_START_CHAN_1, address);
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_1__COD_EOL_PRIORITY_CHAN_1, priority);
     }//EOL
      if(!strcmp(event, "NEW_CHAN")){
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_1__COD_NEW_CHAN_START_CHAN_1, address);
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_1__COD_NEW_CHAN_PRIORITY_CHAN_1, priority);
     }//NEW_CHAN
      if(!strcmp(event, "NEW_ADDR")){
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_1__COD_NEW_ADDR_START_CHAN_1, address);
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_1__COD_NEW_ADDR_PRIORITY_CHAN_1, priority);
     }//NEW_ADDR
      if(!strcmp(event, "NEW_DATA")){
       ipu_write_field(ipu_num, IPU_DC_RL4_CH_1__COD_NEW_DATA_START_CHAN_1, address);
       ipu_write_field(ipu_num, IPU_DC_RL4_CH_1__COD_NEW_DATA_PRIORITY_CHAN_1, priority);
     }//NEW_DATA
   }//channel 1 ****************************************************************************************

//*************************************************************************************   
   if(channel == 2){

     if(!strcmp(event, "NL")){
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_2__COD_NL_START_CHAN_2, address);
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_2__COD_NL_PRIORITY_CHAN_2, priority);
     }//NL
     if(!strcmp(event, "NF")){
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_2__COD_NF_START_CHAN_2, address);
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_2__COD_NF_PRIORITY_CHAN_2, priority);
     }//NF
     if(!strcmp(event, "NFIELD")){
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_2__COD_NFIELD_START_CHAN_2, address);
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_2__COD_NFIELD_PRIORITY_CHAN_2, priority);
     }//NFIELD
      if(!strcmp(event, "EOF")){
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_2__COD_EOF_START_CHAN_2, address);
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_2__COD_EOF_PRIORITY_CHAN_2, priority);
     }//EOF
      if(!strcmp(event, "EOFIELD")){
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_2__COD_EOFIELD_START_CHAN_2, address);
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_2__COD_EOFIELD_PRIORITY_CHAN_2, priority);
     }//EOFIELD
      if(!strcmp(event, "EOL")){
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_2__COD_EOL_START_CHAN_2, address);
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_2__COD_EOL_PRIORITY_CHAN_2, priority);
     }//EOL
      if(!strcmp(event, "NEW_CHAN")){
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_2__COD_NEW_CHAN_START_CHAN_2, address);
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_2__COD_NEW_CHAN_PRIORITY_CHAN_2, priority);
     }//NEW_CHAN
      if(!strcmp(event, "NEW_ADDR")){
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_2__COD_NEW_ADDR_START_CHAN_2, address);
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_2__COD_NEW_ADDR_PRIORITY_CHAN_2, priority);
     }//NEW_ADDR
      if(!strcmp(event, "NEW_DATA")){
       ipu_write_field(ipu_num, IPU_DC_RL4_CH_2__COD_NEW_DATA_START_CHAN_2, address);
       ipu_write_field(ipu_num, IPU_DC_RL4_CH_2__COD_NEW_DATA_PRIORITY_CHAN_2, priority);
     }//NEW_DATA
   }//channel 2 ****************************************************************************************
   
//*************************************************************************************   
   if(channel == 5){

     if(!strcmp(event, "NL")){
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_5__COD_NL_START_CHAN_5, address);
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_5__COD_NL_PRIORITY_CHAN_5, priority);
     }//NL
     if(!strcmp(event, "NF")){
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_5__COD_NF_START_CHAN_5, address);
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_5__COD_NF_PRIORITY_CHAN_5, priority);
     }//NF
     if(!strcmp(event, "NFIELD")){
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_5__COD_NFIELD_START_CHAN_5, address);
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_5__COD_NFIELD_PRIORITY_CHAN_5, priority);
     }//NFIELD
      if(!strcmp(event, "EOF")){
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_5__COD_EOF_START_CHAN_5, address);
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_5__COD_EOF_PRIORITY_CHAN_5, priority);
     }//EOF
      if(!strcmp(event, "EOFIELD")){
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_5__COD_EOFIELD_START_CHAN_5, address);
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_5__COD_EOFIELD_PRIORITY_CHAN_5, priority);
     }//EOFIELD
      if(!strcmp(event, "EOL")){
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_5__COD_EOL_START_CHAN_5, address);
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_5__COD_EOL_PRIORITY_CHAN_5, priority);
     }//EOL
      if(!strcmp(event, "NEW_CHAN")){
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_5__COD_NEW_CHAN_START_CHAN_5, address);
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_5__COD_NEW_CHAN_PRIORITY_CHAN_5, priority);
     }//NEW_CHAN
      if(!strcmp(event, "NEW_ADDR")){
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_5__COD_NEW_ADDR_START_CHAN_5, address);
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_5__COD_NEW_ADDR_PRIORITY_CHAN_5, priority);
     }//NEW_ADDR
      if(!strcmp(event, "NEW_DATA")){
       ipu_write_field(ipu_num, IPU_DC_RL4_CH_5__COD_NEW_DATA_START_CHAN_5, address);
       ipu_write_field(ipu_num, IPU_DC_RL4_CH_5__COD_NEW_DATA_PRIORITY_CHAN_5, priority);
     }//NEW_DATA
   }//channel 5 ****************************************************************************************

//*************************************************************************************   
   if(channel == 6){

     if(!strcmp(event, "NL")){
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_6__COD_NL_START_CHAN_6, address);
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_6__COD_NL_PRIORITY_CHAN_6, priority);
     }//NL
     if(!strcmp(event, "NF")){
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_6__COD_NF_START_CHAN_6, address);
       ipu_write_field(ipu_num, IPU_DC_RL0_CH_6__COD_NF_PRIORITY_CHAN_6, priority);
     }//NF
     if(!strcmp(event, "NFIELD")){
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_6__COD_NFIELD_START_CHAN_6, address);
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_6__COD_NFIELD_PRIORITY_CHAN_6, priority);
     }//NFIELD
      if(!strcmp(event, "EOF")){
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_6__COD_EOF_START_CHAN_6, address);
       ipu_write_field(ipu_num, IPU_DC_RL1_CH_6__COD_EOF_PRIORITY_CHAN_6, priority);
     }//EOF
      if(!strcmp(event, "EOFIELD")){
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_6__COD_EOFIELD_START_CHAN_6, address);
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_6__COD_EOFIELD_PRIORITY_CHAN_6, priority);
     }//EOFIELD
      if(!strcmp(event, "EOL")){
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_6__COD_EOL_START_CHAN_6, address);
       ipu_write_field(ipu_num, IPU_DC_RL2_CH_6__COD_EOL_PRIORITY_CHAN_6, priority);
     }//EOL
      if(!strcmp(event, "NEW_CHAN")){
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_6__COD_NEW_CHAN_START_CHAN_6, address);
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_6__COD_NEW_CHAN_PRIORITY_CHAN_6, priority);
     }//NEW_CHAN
      if(!strcmp(event, "NEW_ADDR")){
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_6__COD_NEW_ADDR_START_CHAN_6, address);
       ipu_write_field(ipu_num, IPU_DC_RL3_CH_6__COD_NEW_ADDR_PRIORITY_CHAN_6, priority);
     }//NEW_ADDR
      if(!strcmp(event, "NEW_DATA")){
       ipu_write_field(ipu_num, IPU_DC_RL4_CH_6__COD_NEW_DATA_START_CHAN_6, address);
       ipu_write_field(ipu_num, IPU_DC_RL4_CH_6__COD_NEW_DATA_PRIORITY_CHAN_6, priority);
     }//NEW_DATA
   }//channel 6 ****************************************************************************************

   
}


#ifdef DISPLAY_INTERLACED
void dc_config_interlaced(unsigned int IPU, unsigned int DI, unsigned int width, int mapping)
{
	unsigned int microCodeAddr_DATA;

	printf("dc_config_interlaced (IPU=%d) (DI=%d) (mapping=%d)\r\n", IPU, DI, mapping);

	microCodeAddr_DATA = 1;

	//DATA
	microcode_config(
	                 IPU, 
	                 microCodeAddr_DATA,//word address
	                 1,//stop
	                 "WROD",//OPCODE
	                 0, //LF
	                 0, //AF
	                 0, //OPERAND
	                 mapping, //MAPPING
	                 DI_SDC_WAVEFORM + 1, //WAVEFORM
	                 0, //GLUELOGIC
	                 DI_COUNTER_APIXEL);//SYNC

#ifdef IPU_USE_DC_CHANNEL
	/*********************************************************************
	* link events to routines
	**********************************************************************/
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "NL",			3,	microCodeAddr_DATA);  //prior=3
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "NF",			0,	0);
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "NFIELD",		0,	0);
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "EOF",		 	0,	0);
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "EOFIELD",		0,	0);
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "EOL",			2,	microCodeAddr_DATA);
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "NEW_CHAN",	0,	0);
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "NEW_ADDR",	0,	0);
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "NEW_DATA",	1,	microCodeAddr_DATA); 

	//WR_CH_CONF_1  
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__PROG_START_TIME_1, 0);  //no anti-tearing elimination used
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__CHAN_MASK_DEFAULT_1, 0);  //only the highest priority event will be served
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__PROG_CHAN_TYP_1, 4);  //normal mode without anti-tearing.DO NOT CHANGE!!
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__PROG_DISP_ID_1, 1);  //select dc_display 1 to link channel #1
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__PROG_DI_ID_1, DI);  //DC channel 1 associated to current DI
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__PROG_DI_ID_5, 1 - DI);  //DC channel 5 associated to another DI
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__W_SIZE_1, 2);  //component size access to DC set to 24bit MSB.
#ifdef DISPLAY_INTERLACED
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__FIELD_MODE_1, 1);  //field mode enable
#else
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__FIELD_MODE_1, 0);  //frame mode enable
#endif
	ipu_write_field(IPU, IPU_DC_WR_CH_ADDR_1__ST_ADDR_1, 0);  //START ADDRESS OF CHANNEL

#else

	/*********************************************************************
	* link events to routines
	**********************************************************************/
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "NL",			3,	microCodeAddr_DATA);  //prior=3
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "NF",			0,	0);
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "NFIELD",		0,	0);
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "EOF",		 	0,	0);
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "EOFIELD",		0,	0);
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "EOL",			2,	microCodeAddr_DATA);
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "NEW_CHAN",	0,	0);
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "NEW_ADDR",	0,	0);
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "NEW_DATA",	1,	microCodeAddr_DATA); 

	//WR_CH_CONF_5  
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__PROG_START_TIME_5, 0);  //no anti-tearing elimination used
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__CHAN_MASK_DEFAULT_5, 0);  //only the highest priority event will be served
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__PROG_CHAN_TYP_5, 4);  //normal mode without anti-tearing.DO NOT CHANGE!!
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__PROG_DISP_ID_5, 1);  //select dc_display 1 to link channel #5
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__PROG_DI_ID_5, DI);  //DC channel 5 associated to current DI
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__PROG_DI_ID_1, 1 - DI);  //DC channel 1 associated to another DI
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__W_SIZE_5, 2);  //component size access to DC set to 24bit MSB.
#ifdef DISPLAY_INTERLACED
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__FIELD_MODE_5, 1);  //field mode enable
#else
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__FIELD_MODE_5, 0);  //frame mode enable
#endif
	ipu_write_field(IPU, IPU_DC_WR_CH_ADDR_5__ST_ADDR_5, 0);  //START ADDRESS OF CHANNEL
#endif

	//COMMON
	dc_config_common(IPU, width);
}
#else

void dc_config_none_interlaced(unsigned int IPU, unsigned int DI, unsigned int width, int mapping)
{
	unsigned int microCodeAddr_NL, microCodeAddr_EOL, microCodeAddr_DATA;
	debug("dc_config (IPU=%d) (DI=%d) (mapping=%d)\r\n", IPU, DI, mapping);

	microCodeAddr_NL = 2;
	microCodeAddr_EOL = 3;
	microCodeAddr_DATA = 4;

	//MICROCODE 
	microcode_config(
	                 IPU, 
	                 1,//word address
	                 1,//stop
	                 "WROD",//OPCODE
	                 0, //LF
	                 0, //AF
	                 0, //OPERAND
	                 mapping, //MAPPING
	                 DI_SDC_WAVEFORM + 1, //WAVEFORM
	                 0, //GLUELOGIC
	                 DI_COUNTER_APIXEL);//SYNC

	microcode_config(
			 IPU, 
			 5,//word address
			 1,//stop
			 "WROD",//OPCODE
			 0, //LF
			 0, //AF
			 0, //OPERAND
			 mapping, //MAPPING
			 DI_SDC_WAVEFORM + 1, //WAVEFORM
			 0, //GLUELOGIC
			 DI_COUNTER_APIXEL);//SYNC
 
	//NL                 
	microcode_config(
	                 IPU, 
	                 microCodeAddr_NL,//word address
	                 1,//stop
	                 "WROD",//OPCODE
	                 0, //LF
	                 0, //AF
	                 0, //OPERAND
	                 mapping, //MAPPING
	                 DI_SDC_WAVEFORM + 1, //WAVEFORM
	                 0, //GLUELOGIC
	                 DI_COUNTER_APIXEL);//SYNC

	//EOL		 
	microcode_config(
	                 IPU, 
	                 microCodeAddr_EOL,//word address
	                 1,//stop
	                 "WROD",//OPCODE
	                 0, //LF
	                 0, //AF
	                 0, //OPERAND
	                 mapping, //MAPPING
	                 DI_SDC_WAVEFORM + 1, //WAVEFORM
	                 0, //GLUELOGIC
	                 DI_COUNTER_APIXEL);//SYNC

	//DATA
	microcode_config(
	                 IPU, 
	                 microCodeAddr_DATA,//word address
	                 1,//stop
	                 "WROD",//OPCODE
	                 0, //LF
	                 0, //AF
	                 0, //OPERAND
	                 mapping, //MAPPING
	                 DI_SDC_WAVEFORM + 1, //WAVEFORM
	                 0, //GLUELOGIC
	                 DI_COUNTER_APIXEL);//SYNC

#ifdef IPU_USE_DC_CHANNEL
	/*********************************************************************
	* link events to routines
	**********************************************************************/
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "NL",			3,	microCodeAddr_NL);
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "NF",			0,	0);
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "NFIELD",		0,	0);
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "EOF",		 	0,	0);
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "EOFIELD",		0,	0);
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "EOL",			2,	microCodeAddr_EOL);
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "NEW_CHAN",	0,	0);
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "NEW_ADDR",	0,	0);
	microcode_event(IPU, DC_CHANNEL_DC_SYNC_OR_ASYNC, "NEW_DATA",	1,	microCodeAddr_DATA); 

	//WR_CH_CONF_1  
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__PROG_START_TIME_1, 0);  //no anti-tearing elimination used
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__CHAN_MASK_DEFAULT_1, 0);  //only the highest priority event will be served
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__PROG_CHAN_TYP_1, 4);  //normal mode without anti-tearing.DO NOT CHANGE!!
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__PROG_DISP_ID_1, 1);  //select dc_display 1 to link channel #1
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__PROG_DI_ID_1, DI);  //DC channel 1 associated to current DI
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__PROG_DI_ID_5, 1 - DI);  //DC channel 5 associated to another DI
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__W_SIZE_1, 2);  //component size access to DC set to 24bit MSB.
#ifdef DISPLAY_INTERLACED
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__FIELD_MODE_1, 1);  //field mode enable
#else
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__FIELD_MODE_1, 0);  //frame mode enable
#endif
	ipu_write_field(IPU, IPU_DC_WR_CH_ADDR_1__ST_ADDR_1, 0);  //START ADDRESS OF CHANNEL

#else

	/*********************************************************************
	* link events to routines
	**********************************************************************/
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "NL",			3,	microCodeAddr_NL);
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "NF",			0,	0);
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "NFIELD",		0,	0);
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "EOF",		 	0,	0);
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "EOFIELD",		0,	0);
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "EOL",			2,	microCodeAddr_EOL);
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "NEW_CHAN",	0,	0);
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "NEW_ADDR",	0,	0);
	microcode_event(IPU, DC_CHANNEL_DP_PRIMARY, "NEW_DATA",	1,	microCodeAddr_DATA); 

	//WR_CH_CONF_5  
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__PROG_START_TIME_5, 0);  //no anti-tearing elimination used
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__CHAN_MASK_DEFAULT_5, 0);  //only the highest priority event will be served
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__PROG_CHAN_TYP_5, 4);  //normal mode without anti-tearing.DO NOT CHANGE!!
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__PROG_DISP_ID_5, 1);  //select dc_display 1 to link channel #5
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__PROG_DI_ID_5, DI);  //DC channel 5 associated to current DI
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_1__PROG_DI_ID_1, 1 - DI);  //DC channel 1 associated to another DI
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__W_SIZE_5, 2);  //component size access to DC set to 24bit MSB.
#ifdef DISPLAY_INTERLACED
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__FIELD_MODE_5, 1);  //field mode enable
#else
	ipu_write_field(IPU, IPU_DC_WR_CH_CONF_5__FIELD_MODE_5, 0);  //frame mode enable
#endif
	ipu_write_field(IPU, IPU_DC_WR_CH_ADDR_5__ST_ADDR_5, 0);  //START ADDRESS OF CHANNEL
#endif

	//COMMON
	dc_config_common(IPU, width);
}
#endif
