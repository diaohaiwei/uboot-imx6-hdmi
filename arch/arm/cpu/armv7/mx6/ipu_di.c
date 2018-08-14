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

// DI counter definitions
#define DI_COUNTER_BASECLK 	0
#define DI_COUNTER_IHSYNC   	1
#define DI_COUNTER_1		   	1
#define DI_COUNTER_HSYNC   		2
#define DI_COUNTER_VSYNC   		3
#define DI_COUNTER_AFIELD    	4
#define DI_COUNTER_ALINE    		5
#define DI_COUNTER_APIXEL   		6
#define DI_COUNTER_7   			7

#define DI_COMMAND_WAVEFORM 0
#define DI_DATAWR_WAVEFORM  1
#define DI_DATARD_WAVEFORM  2
#define DI_SDC_WAVEFORM     3
#define DI_SERIAL_WAVEFORM  4

#define DI_RS_SIGNAL        0
#define DI_WR_SIGNAL        1
#define DI_RD_SIGNAL        1
#define DI_SER_CLK_SIGNAL   1
#define DI_CS_SIGNAL        2
#define DI_NOUSE_SIGNAL     3

#define DI_DEN_SIGNAL       0


extern void ipu_write_field(unsigned int IPU, unsigned int ID_addr, unsigned int ID_mask, unsigned int data);

static void di_sync_config(
                int ipu_num, 
                int di, 
                int pointer,
                int run_value_m1,           
                int run_resolution,         
                int offset_value,           
                int offset_resolution,      
                int cnt_auto_reload,        
                int step_repeat,           
                int cnt_clr_sel,            
                int cnt_polarity_gen_en,    
                int cnt_polarity_trigger_sel,
                int cnt_polarity_clr_sel,   
                int cnt_up,                 
                int cnt_down, 
                int gentime_sel
                )
{
    if(di == 0){
        switch(pointer){
            case  1:{
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_1__DI0_RUN_VALUE_M1_1, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_1__DI0_RUN_RESOLUTION_1, run_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_1__DI0_OFFSET_VALUE_1, offset_value);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_1__DI0_OFFSET_RESOLUTION_1, offset_resolution);                                                                          
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_1__DI0_CNT_POLARITY_GEN_EN_1, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_1__DI0_CNT_AUTO_RELOAD_1, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_1__DI0_CNT_CLR_SEL_1, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_1__DI0_CNT_DOWN_1, cnt_down);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_1__DI0_CNT_POLARITY_TRIGGER_SEL_1, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_1__DI0_CNT_POLARITY_CLR_SEL_1, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_1__DI0_CNT_UP_1, cnt_up);                                                                             
                ipu_write_field(ipu_num, IPU_DI0_STP_REP_1__DI0_STEP_REPEAT_1, step_repeat);
                break;
            }
            case  2:{
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_2__DI0_RUN_VALUE_M1_2, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_2__DI0_RUN_RESOLUTION_2, run_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_2__DI0_OFFSET_VALUE_2, offset_value);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_2__DI0_OFFSET_RESOLUTION_2, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_2__DI0_CNT_POLARITY_GEN_EN_2, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_2__DI0_CNT_AUTO_RELOAD_2, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_2__DI0_CNT_CLR_SEL_2, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_2__DI0_CNT_DOWN_2, cnt_down);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_2__DI0_CNT_POLARITY_TRIGGER_SEL_2, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_2__DI0_CNT_POLARITY_CLR_SEL_2, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_2__DI0_CNT_UP_2, cnt_up);
                ipu_write_field(ipu_num, IPU_DI0_STP_REP_1__DI0_STEP_REPEAT_2, step_repeat);
                break;
            }
            case  3:{
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_3__DI0_RUN_VALUE_M1_3, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_3__DI0_RUN_RESOLUTION_3, run_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_3__DI0_OFFSET_VALUE_3, offset_value);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_3__DI0_OFFSET_RESOLUTION_3, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_3__DI0_CNT_POLARITY_GEN_EN_3, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_3__DI0_CNT_AUTO_RELOAD_3, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_3__DI0_CNT_CLR_SEL_3, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_3__DI0_CNT_DOWN_3, cnt_down);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_3__DI0_CNT_POLARITY_TRIGGER_SEL_3, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_3__DI0_CNT_POLARITY_CLR_SEL_3, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_3__DI0_CNT_UP_3, cnt_up);
                ipu_write_field(ipu_num, IPU_DI0_STP_REP_2__DI0_STEP_REPEAT_3, step_repeat);
                break;
            }
            case  4:{
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_4__DI0_RUN_VALUE_M1_4, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_4__DI0_RUN_RESOLUTION_4, run_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_4__DI0_OFFSET_VALUE_4, offset_value);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_4__DI0_OFFSET_RESOLUTION_4, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_4__DI0_CNT_POLARITY_GEN_EN_4, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_4__DI0_CNT_AUTO_RELOAD_4, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_4__DI0_CNT_CLR_SEL_4, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_4__DI0_CNT_DOWN_4, cnt_down);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_4__DI0_CNT_POLARITY_TRIGGER_SEL_4, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_4__DI0_CNT_POLARITY_CLR_SEL_4, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_4__DI0_CNT_UP_4, cnt_up);
                ipu_write_field(ipu_num, IPU_DI0_STP_REP_2__DI0_STEP_REPEAT_4, step_repeat);
                break;
            }
            case  5:{
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_5__DI0_RUN_VALUE_M1_5, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_5__DI0_RUN_RESOLUTION_5, run_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_5__DI0_OFFSET_VALUE_5, offset_value);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_5__DI0_OFFSET_RESOLUTION_5, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_5__DI0_CNT_POLARITY_GEN_EN_5, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_5__DI0_CNT_AUTO_RELOAD_5, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_5__DI0_CNT_CLR_SEL_5, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_5__DI0_CNT_DOWN_5, cnt_down);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_5__DI0_CNT_POLARITY_TRIGGER_SEL_5, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_5__DI0_CNT_POLARITY_CLR_SEL_5, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_5__DI0_CNT_UP_5, cnt_up);
                ipu_write_field(ipu_num, IPU_DI0_STP_REP_3__DI0_STEP_REPEAT_5, step_repeat);
                break;
            }
            case  6:{
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_6__DI0_RUN_VALUE_M1_6, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_6__DI0_RUN_RESOLUTION_6, run_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_6__DI0_OFFSET_VALUE_6, offset_value);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_6__DI0_OFFSET_RESOLUTION_6, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_6__DI0_CNT_POLARITY_GEN_EN_6, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_6__DI0_CNT_AUTO_RELOAD_6, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_6__DI0_CNT_CLR_SEL_6, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_6__DI0_CNT_DOWN_6, cnt_down);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_6__DI0_CNT_POLARITY_TRIGGER_SEL_6, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_6__DI0_CNT_POLARITY_CLR_SEL_6, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_6__DI0_CNT_UP_6, cnt_up);
                ipu_write_field(ipu_num, IPU_DI0_STP_REP_3__DI0_STEP_REPEAT_6, step_repeat);
                break;
            }
            case  7:{
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_7__DI0_RUN_VALUE_M1_7, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_7__DI0_RUN_RESOLUTION_7, run_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_7__DI0_OFFSET_VALUE_7, offset_value);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_7__DI0_OFFSET_RESOLUTION_7, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_7__DI0_CNT_POLARITY_GEN_EN_7, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_7__DI0_CNT_AUTO_RELOAD_7, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_7__DI0_CNT_CLR_SEL_7, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_7__DI0_CNT_DOWN_7, cnt_down);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_7__DI0_CNT_POLARITY_TRIGGER_SEL_7, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_7__DI0_CNT_POLARITY_CLR_SEL_7, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_7__DI0_CNT_UP_7, cnt_up);
                ipu_write_field(ipu_num, IPU_DI0_STP_REP_4__DI0_STEP_REPEAT_7, step_repeat);
                break;
            }
            case  8:{
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_8__DI0_RUN_VALUE_M1_8, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_8__DI0_RUN_RESOLUTION_8, run_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_8__DI0_OFFSET_VALUE_8, offset_value);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_8__DI0_OFFSET_RESOLUTION_8, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_8__DI0_CNT_POLARITY_GEN_EN_8, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_8__DI0_CNT_AUTO_RELOAD_8, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_8__DI0_CNT_CLR_SEL_8, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_8__DI0_CNT_DOWN_8, cnt_down);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_8__DI0_CNT_POLARITY_TRIGGER_SEL_8, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_8__DI0_CNT_POLARITY_CLR_SEL_8, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_8__DI0_CNT_UP_8, cnt_up);
                ipu_write_field(ipu_num, IPU_DI0_STP_REP_4__DI0_STEP_REPEAT_8, step_repeat);
                break;
            }
            case  9:{
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_9__DI0_RUN_VALUE_M1_9, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_9__DI0_RUN_RESOLUTION_9, run_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_9__DI0_OFFSET_VALUE_9, offset_value);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN0_9__DI0_OFFSET_RESOLUTION_9, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_9__DI0_GENTIME_SEL_9, gentime_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_9__DI0_CNT_AUTO_RELOAD_9, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_9__DI0_CNT_CLR_SEL_9, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_9__DI0_CNT_DOWN_9, cnt_down);
                ipu_write_field(ipu_num, IPU_DI0_SW_GEN1_9__DI0_CNT_UP_9, cnt_up);
                ipu_write_field(ipu_num, IPU_DI0_STP_REP_9__DI0_STEP_REPEAT_9, step_repeat);
                break;
            }
        }
    }
    else
    {
        switch(pointer){
            case  1:{
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_1__DI1_RUN_VALUE_M1_1, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_1__DI1_RUN_RESOLUTION_1, run_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_1__DI1_OFFSET_VALUE_1, offset_value);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_1__DI1_OFFSET_RESOLUTION_1, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_1__DI1_CNT_POLARITY_GEN_EN_1, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_1__DI1_CNT_AUTO_RELOAD_1, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_1__DI1_CNT_CLR_SEL_1, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_1__DI1_CNT_DOWN_1, cnt_down);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_1__DI1_CNT_POLARITY_TRIGGER_SEL_1, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_1__DI1_CNT_POLARITY_CLR_SEL_1, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_1__DI1_CNT_UP_1, cnt_up);
                ipu_write_field(ipu_num, IPU_DI1_STP_REP_1__DI1_STEP_REPEAT_1, step_repeat);
                break;
            }
            case  2:{
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_2__DI1_RUN_VALUE_M1_2, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_2__DI1_RUN_RESOLUTION_2, run_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_2__DI1_OFFSET_VALUE_2, offset_value);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_2__DI1_OFFSET_RESOLUTION_2, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_2__DI1_CNT_POLARITY_GEN_EN_2, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_2__DI1_CNT_AUTO_RELOAD_2, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_2__DI1_CNT_CLR_SEL_2, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_2__DI1_CNT_DOWN_2, cnt_down);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_2__DI1_CNT_POLARITY_TRIGGER_SEL_2, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_2__DI1_CNT_POLARITY_CLR_SEL_2, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_2__DI1_CNT_UP_2, cnt_up);
                ipu_write_field(ipu_num, IPU_DI1_STP_REP_1__DI1_STEP_REPEAT_2, step_repeat);
                break;
            }
            case  3:{
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_3__DI1_RUN_VALUE_M1_3, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_3__DI1_RUN_RESOLUTION_3, run_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_3__DI1_OFFSET_VALUE_3, offset_value);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_3__DI1_OFFSET_RESOLUTION_3, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_3__DI1_CNT_POLARITY_GEN_EN_3, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_3__DI1_CNT_AUTO_RELOAD_3, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_3__DI1_CNT_CLR_SEL_3, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_3__DI1_CNT_DOWN_3, cnt_down);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_3__DI1_CNT_POLARITY_TRIGGER_SEL_3, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_3__DI1_CNT_POLARITY_CLR_SEL_3, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_3__DI1_CNT_UP_3, cnt_up);
                ipu_write_field(ipu_num, IPU_DI1_STP_REP_2__DI1_STEP_REPEAT_3, step_repeat);
                break;
            }
            case  4:{
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_4__DI1_RUN_VALUE_M1_4, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_4__DI1_RUN_RESOLUTION_4, run_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_4__DI1_OFFSET_VALUE_4, offset_value);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_4__DI1_OFFSET_RESOLUTION_4, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_4__DI1_CNT_POLARITY_GEN_EN_4, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_4__DI1_CNT_AUTO_RELOAD_4, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_4__DI1_CNT_CLR_SEL_4, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_4__DI1_CNT_DOWN_4, cnt_down);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_4__DI1_CNT_POLARITY_TRIGGER_SEL_4, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_4__DI1_CNT_POLARITY_CLR_SEL_4, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_4__DI1_CNT_UP_4, cnt_up);
                ipu_write_field(ipu_num, IPU_DI1_STP_REP_2__DI1_STEP_REPEAT_4, step_repeat);
                break;
            }
            case  5:{
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_5__DI1_RUN_VALUE_M1_5, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_5__DI1_RUN_RESOLUTION_5, run_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_5__DI1_OFFSET_VALUE_5, offset_value);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_5__DI1_OFFSET_RESOLUTION_5, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_5__DI1_CNT_POLARITY_GEN_EN_5, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_5__DI1_CNT_AUTO_RELOAD_5, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_5__DI1_CNT_CLR_SEL_5, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_5__DI1_CNT_DOWN_5, cnt_down);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_5__DI1_CNT_POLARITY_TRIGGER_SEL_5, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_5__DI1_CNT_POLARITY_CLR_SEL_5, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_5__DI1_CNT_UP_5, cnt_up);
                ipu_write_field(ipu_num, IPU_DI1_STP_REP_3__DI1_STEP_REPEAT_5, step_repeat);
                break;
            }
            case  6:{
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_6__DI1_RUN_VALUE_M1_6, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_6__DI1_RUN_RESOLUTION_6, run_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_6__DI1_OFFSET_VALUE_6, offset_value);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_6__DI1_OFFSET_RESOLUTION_6, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_6__DI1_CNT_POLARITY_GEN_EN_6, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_6__DI1_CNT_AUTO_RELOAD_6, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_6__DI1_CNT_CLR_SEL_6, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_6__DI1_CNT_DOWN_6, cnt_down);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_6__DI1_CNT_POLARITY_TRIGGER_SEL_6, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_6__DI1_CNT_POLARITY_CLR_SEL_6, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_6__DI1_CNT_UP_6, cnt_up);
                ipu_write_field(ipu_num, IPU_DI1_STP_REP_3__DI1_STEP_REPEAT_6, step_repeat);
                break;
            }
            case  7:{
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_7__DI1_RUN_VALUE_M1_7, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_7__DI1_RUN_RESOLUTION_7, run_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_7__DI1_OFFSET_VALUE_7, offset_value);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_7__DI1_OFFSET_RESOLUTION_7, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_7__DI1_CNT_POLARITY_GEN_EN_7, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_7__DI1_CNT_AUTO_RELOAD_7, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_7__DI1_CNT_CLR_SEL_7, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_7__DI1_CNT_DOWN_7, cnt_down);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_7__DI1_CNT_POLARITY_TRIGGER_SEL_7, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_7__DI1_CNT_POLARITY_CLR_SEL_7, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_7__DI1_CNT_UP_7, cnt_up);
                ipu_write_field(ipu_num, IPU_DI1_STP_REP_4__DI1_STEP_REPEAT_7, step_repeat);
            }
            case  8:{
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_8__DI1_RUN_VALUE_M1_8, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_8__DI1_RUN_RESOLUTION_8, run_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_8__DI1_OFFSET_VALUE_8, offset_value);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_8__DI1_OFFSET_RESOLUTION_8, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_8__DI1_CNT_POLARITY_GEN_EN_8, cnt_polarity_gen_en);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_8__DI1_CNT_AUTO_RELOAD_8, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_8__DI1_CNT_CLR_SEL_8, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_8__DI1_CNT_DOWN_8, cnt_down);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_8__DI1_CNT_POLARITY_TRIGGER_SEL_8, cnt_polarity_trigger_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_8__DI1_CNT_POLARITY_CLR_SEL_8, cnt_polarity_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_8__DI1_CNT_UP_8, cnt_up);
                ipu_write_field(ipu_num, IPU_DI1_STP_REP_4__DI1_STEP_REPEAT_8, step_repeat);
                break;
            }
            case  9:{
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_9__DI1_RUN_VALUE_M1_9, run_value_m1);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_9__DI1_RUN_RESOLUTION_9, run_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_9__DI1_OFFSET_VALUE_9, offset_value);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN0_9__DI1_OFFSET_RESOLUTION_9, offset_resolution);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_9__DI1_GENTIME_SEL_9, gentime_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_9__DI1_CNT_AUTO_RELOAD_9, cnt_auto_reload);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_9__DI1_CNT_CLR_SEL_9, cnt_clr_sel);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_9__DI1_CNT_DOWN_9, cnt_down);
                ipu_write_field(ipu_num, IPU_DI1_SW_GEN1_9__DI1_CNT_UP_9, cnt_up);
                ipu_write_field(ipu_num, IPU_DI1_STP_REP_9__DI1_STEP_REPEAT_9, step_repeat);
                break;
            }
        }
    }
}

static void di_pointer_config(int ipu_num, int di, int pointer, int access, int component, int cst, int pt0, int pt1, int pt2, int pt3, int pt4, int pt5, int pt6)
{
    if(di == 0){
        switch(pointer){
            case  0:{
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_0__DI0_ACCESS_SIZE_0,access);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_0__DI0_COMPONNENT_SIZE_0,component);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_0__DI0_CST_0,cst);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_0__DI0_PT_0_0,pt0);      
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_0__DI0_PT_1_0,pt1);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_0__DI0_PT_2_0,pt2);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_0__DI0_PT_3_0,pt3);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_0__DI0_PT_4_0,pt4);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_0__DI0_PT_5_0,pt5);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_0__DI0_PT_6_0,pt6);
                break;
            }
            case  1:{
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_1__DI0_ACCESS_SIZE_1,access);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_1__DI0_COMPONNENT_SIZE_1,component);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_1__DI0_CST_1,cst);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_1__DI0_PT_0_1,pt0);      
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_1__DI0_PT_1_1,pt1);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_1__DI0_PT_2_1,pt2);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_1__DI0_PT_3_1,pt3);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_1__DI0_PT_4_1,pt4);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_1__DI0_PT_5_1,pt5);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_1__DI0_PT_6_1,pt6);
                break;
            }
            case  2:{
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_2__DI0_ACCESS_SIZE_2,access);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_2__DI0_COMPONNENT_SIZE_2,component);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_2__DI0_CST_2,cst);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_2__DI0_PT_0_2,pt0);      
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_2__DI0_PT_1_2,pt1);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_2__DI0_PT_2_2,pt2);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_2__DI0_PT_3_2,pt3);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_2__DI0_PT_4_2,pt4);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_2__DI0_PT_5_2,pt5);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_2__DI0_PT_6_2,pt6);
                break;
            }
            case  3:{
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_3__DI0_ACCESS_SIZE_3,access);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_3__DI0_COMPONNENT_SIZE_3,component);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_3__DI0_CST_3,cst);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_3__DI0_PT_0_3,pt0);      
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_3__DI0_PT_1_3,pt1);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_3__DI0_PT_2_3,pt2);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_3__DI0_PT_3_3,pt3);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_3__DI0_PT_4_3,pt4);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_3__DI0_PT_5_3,pt5);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_3__DI0_PT_6_3,pt6);
                break;
            }
            case  4:{
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_4__DI0_ACCESS_SIZE_4,access);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_4__DI0_COMPONNENT_SIZE_4,component);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_4__DI0_CST_4,cst);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_4__DI0_PT_0_4,pt0);      
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_4__DI0_PT_1_4,pt1);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_4__DI0_PT_2_4,pt2);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_4__DI0_PT_3_4,pt3);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_4__DI0_PT_4_4,pt4);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_4__DI0_PT_5_4,pt5);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_4__DI0_PT_6_4,pt6);
                break;
            }
            case  5:{
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_5__DI0_ACCESS_SIZE_5,access);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_5__DI0_COMPONNENT_SIZE_5,component);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_5__DI0_CST_5,cst);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_5__DI0_PT_0_5,pt0);      
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_5__DI0_PT_1_5,pt1);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_5__DI0_PT_2_5,pt2);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_5__DI0_PT_3_5,pt3);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_5__DI0_PT_4_5,pt4);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_5__DI0_PT_5_5,pt5);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_5__DI0_PT_6_5,pt6);
                break;
            }
            case  6:{
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_6__DI0_ACCESS_SIZE_6,access);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_6__DI0_COMPONNENT_SIZE_6,component);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_6__DI0_CST_6,cst);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_6__DI0_PT_0_6,pt0);      
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_6__DI0_PT_1_6,pt1);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_6__DI0_PT_2_6,pt2);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_6__DI0_PT_3_6,pt3);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_6__DI0_PT_4_6,pt4);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_6__DI0_PT_5_6,pt5);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_6__DI0_PT_6_6,pt6);
                break;
            }
            case  7:{
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_7__DI0_ACCESS_SIZE_7,access);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_7__DI0_COMPONNENT_SIZE_7,component);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_7__DI0_CST_7,cst);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_7__DI0_PT_0_7,pt0);      
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_7__DI0_PT_1_7,pt1);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_7__DI0_PT_2_7,pt2);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_7__DI0_PT_3_7,pt3);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_7__DI0_PT_4_7,pt4);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_7__DI0_PT_5_7,pt5);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_7__DI0_PT_6_7,pt6);
                break;
            }
            case  8:{
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_8__DI0_ACCESS_SIZE_8,access);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_8__DI0_COMPONNENT_SIZE_8,component);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_8__DI0_CST_8,cst);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_8__DI0_PT_0_8,pt0);      
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_8__DI0_PT_1_8,pt1);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_8__DI0_PT_2_8,pt2);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_8__DI0_PT_3_8,pt3);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_8__DI0_PT_4_8,pt4);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_8__DI0_PT_5_8,pt5);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_8__DI0_PT_6_8,pt6);
                break;
            }
            case  9:{
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_9__DI0_ACCESS_SIZE_9,access);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_9__DI0_COMPONNENT_SIZE_9,component);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_9__DI0_CST_9,cst);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_9__DI0_PT_0_9,pt0);      
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_9__DI0_PT_1_9,pt1);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_9__DI0_PT_2_9,pt2);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_9__DI0_PT_3_9,pt3);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_9__DI0_PT_4_9,pt4);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_9__DI0_PT_5_9,pt5);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_9__DI0_PT_6_9,pt6);
                break;
            }
            case  10:{
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_10__DI0_ACCESS_SIZE_10,access);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_10__DI0_COMPONNENT_SIZE_10,component);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_10__DI0_CST_10,cst);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_10__DI0_PT_0_10,pt0);      
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_10__DI0_PT_1_10,pt1);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_10__DI0_PT_2_10,pt2);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_10__DI0_PT_3_10,pt3);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_10__DI0_PT_4_10,pt4);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_10__DI0_PT_5_10,pt5);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_10__DI0_PT_6_10,pt6);
                break;
            }
            case  11:{
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_11__DI0_ACCESS_SIZE_11,access);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_11__DI0_COMPONNENT_SIZE_11,component);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_11__DI0_CST_11,cst);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_11__DI0_PT_0_11,pt0);      
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_11__DI0_PT_1_11,pt1);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_11__DI0_PT_2_11,pt2);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_11__DI0_PT_3_11,pt3);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_11__DI0_PT_4_11,pt4);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_11__DI0_PT_5_11,pt5);
                ipu_write_field(ipu_num, IPU_DI0_DW_GEN_11__DI0_PT_6_11,pt6);
                break;
            }
        }
    }
    else {
        switch(pointer){
            case  0:{
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_0__DI1_ACCESS_SIZE_0,access);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_0__DI1_COMPONNENT_SIZE_0,component);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_0__DI1_CST_0,cst);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_0__DI1_PT_0_0,pt0);      
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_0__DI1_PT_1_0,pt1);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_0__DI1_PT_2_0,pt2);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_0__DI1_PT_3_0,pt3);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_0__DI1_PT_4_0,pt4);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_0__DI1_PT_5_0,pt5);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_0__DI1_PT_6_0,pt6);
                break;
            }
            case  1:{
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_1__DI1_ACCESS_SIZE_1,access);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_1__DI1_COMPONNENT_SIZE_1,component);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_1__DI1_CST_1,cst);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_1__DI1_PT_0_1,pt0);      
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_1__DI1_PT_1_1,pt1);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_1__DI1_PT_2_1,pt2);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_1__DI1_PT_3_1,pt3);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_1__DI1_PT_4_1,pt4);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_1__DI1_PT_5_1,pt5);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_1__DI1_PT_6_1,pt6);
                break;
            }
            case  2:{
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_2__DI1_ACCESS_SIZE_2,access);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_2__DI1_COMPONNENT_SIZE_2,component);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_2__DI1_CST_2,cst);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_2__DI1_PT_0_2,pt0);      
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_2__DI1_PT_1_2,pt1);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_2__DI1_PT_2_2,pt2);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_2__DI1_PT_3_2,pt3);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_2__DI1_PT_4_2,pt4);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_2__DI1_PT_5_2,pt5);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_2__DI1_PT_6_2,pt6);
                break;
            }
            case  3:{
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_3__DI1_ACCESS_SIZE_3,access);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_3__DI1_COMPONNENT_SIZE_3,component);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_3__DI1_CST_3,cst);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_3__DI1_PT_0_3,pt0);      
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_3__DI1_PT_1_3,pt1);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_3__DI1_PT_2_3,pt2);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_3__DI1_PT_3_3,pt3);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_3__DI1_PT_4_3,pt4);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_3__DI1_PT_5_3,pt5);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_3__DI1_PT_6_3,pt6);
                break;
            }
            case  4:{
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_4__DI1_ACCESS_SIZE_4,access);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_4__DI1_COMPONNENT_SIZE_4,component);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_4__DI1_CST_4,cst);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_4__DI1_PT_0_4,pt0);      
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_4__DI1_PT_1_4,pt1);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_4__DI1_PT_2_4,pt2);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_4__DI1_PT_3_4,pt3);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_4__DI1_PT_4_4,pt4);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_4__DI1_PT_5_4,pt5);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_4__DI1_PT_6_4,pt6);
                break;
            }
            case  5:{
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_5__DI1_ACCESS_SIZE_5,access);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_5__DI1_COMPONNENT_SIZE_5,component);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_5__DI1_CST_5,cst);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_5__DI1_PT_0_5,pt0);      
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_5__DI1_PT_1_5,pt1);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_5__DI1_PT_2_5,pt2);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_5__DI1_PT_3_5,pt3);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_5__DI1_PT_4_5,pt4);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_5__DI1_PT_5_5,pt5);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_5__DI1_PT_6_5,pt6);
                break;
            }
            case  6:{
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_6__DI1_ACCESS_SIZE_6,access);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_6__DI1_COMPONNENT_SIZE_6,component);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_6__DI1_CST_6,cst);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_6__DI1_PT_0_6,pt0);      
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_6__DI1_PT_1_6,pt1);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_6__DI1_PT_2_6,pt2);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_6__DI1_PT_3_6,pt3);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_6__DI1_PT_4_6,pt4);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_6__DI1_PT_5_6,pt5);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_6__DI1_PT_6_6,pt6);
                break;
            }
            case  7:{
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_7__DI1_ACCESS_SIZE_7,access);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_7__DI1_COMPONNENT_SIZE_7,component);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_7__DI1_CST_7,cst);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_7__DI1_PT_0_7,pt0);      
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_7__DI1_PT_1_7,pt1);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_7__DI1_PT_2_7,pt2);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_7__DI1_PT_3_7,pt3);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_7__DI1_PT_4_7,pt4);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_7__DI1_PT_5_7,pt5);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_7__DI1_PT_6_7,pt6);
                break;
            }
            case  8:{
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_8__DI1_ACCESS_SIZE_8,access);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_8__DI1_COMPONNENT_SIZE_8,component);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_8__DI1_CST_8,cst);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_8__DI1_PT_0_8,pt0);      
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_8__DI1_PT_1_8,pt1);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_8__DI1_PT_2_8,pt2);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_8__DI1_PT_3_8,pt3);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_8__DI1_PT_4_8,pt4);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_8__DI1_PT_5_8,pt5);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_8__DI1_PT_6_8,pt6);
                break;
            }
            case  9:{
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_9__DI1_ACCESS_SIZE_9,access);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_9__DI1_COMPONNENT_SIZE_9,component);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_9__DI1_CST_9,cst);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_9__DI1_PT_0_9,pt0);      
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_9__DI1_PT_1_9,pt1);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_9__DI1_PT_2_9,pt2);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_9__DI1_PT_3_9,pt3);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_9__DI1_PT_4_9,pt4);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_9__DI1_PT_5_9,pt5);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_9__DI1_PT_6_9,pt6);
                break;
            }
            case  10:{
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_10__DI1_ACCESS_SIZE_10,access);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_10__DI1_COMPONNENT_SIZE_10,component);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_10__DI1_CST_10,cst);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_10__DI1_PT_0_10,pt0);      
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_10__DI1_PT_1_10,pt1);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_10__DI1_PT_2_10,pt2);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_10__DI1_PT_3_10,pt3);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_10__DI1_PT_4_10,pt4);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_10__DI1_PT_5_10,pt5);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_10__DI1_PT_6_10,pt6);
                break;
            }
            case  11:{
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_11__DI1_ACCESS_SIZE_11,access);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_11__DI1_COMPONNENT_SIZE_11,component);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_11__DI1_CST_11,cst);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_11__DI1_PT_0_11,pt0);      
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_11__DI1_PT_1_11,pt1);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_11__DI1_PT_2_11,pt2);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_11__DI1_PT_3_11,pt3);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_11__DI1_PT_4_11,pt4);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_11__DI1_PT_5_11,pt5);
                ipu_write_field(ipu_num, IPU_DI1_DW_GEN_11__DI1_PT_6_11,pt6);
                break;
            }
        }
    }
}

static void di_up_down_config(int ipu_num, int di, int pointer, int set, int up, int down)
{
    if(di == 0){
        switch(pointer){
        case  0:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_0__DI0_DATA_CNT_UP0_0,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_0__DI0_DATA_CNT_DOWN0_0,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_0__DI0_DATA_CNT_UP1_0,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_0__DI0_DATA_CNT_DOWN1_0,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_0__DI0_DATA_CNT_UP2_0,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_0__DI0_DATA_CNT_DOWN2_0,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_0__DI0_DATA_CNT_UP3_0,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_0__DI0_DATA_CNT_DOWN3_0,down);
                }
                break;}
        case  1:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_1__DI0_DATA_CNT_UP0_1,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_1__DI0_DATA_CNT_DOWN0_1,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_1__DI0_DATA_CNT_UP1_1,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_1__DI0_DATA_CNT_DOWN1_1,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_1__DI0_DATA_CNT_UP2_1,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_1__DI0_DATA_CNT_DOWN2_1,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_1__DI0_DATA_CNT_UP3_1,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_1__DI0_DATA_CNT_DOWN3_1,down);
                }
                break;}
        case  2:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_2__DI0_DATA_CNT_UP0_2,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_2__DI0_DATA_CNT_DOWN0_2,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_2__DI0_DATA_CNT_UP1_2,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_2__DI0_DATA_CNT_DOWN1_2,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_2__DI0_DATA_CNT_UP2_2,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_2__DI0_DATA_CNT_DOWN2_2,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_2__DI0_DATA_CNT_UP3_2,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_2__DI0_DATA_CNT_DOWN3_2,down);
                }
                break;}
        case  3:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_3__DI0_DATA_CNT_UP0_3,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_3__DI0_DATA_CNT_DOWN0_3,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_3__DI0_DATA_CNT_UP1_3,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_3__DI0_DATA_CNT_DOWN1_3,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_3__DI0_DATA_CNT_UP2_3,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_3__DI0_DATA_CNT_DOWN2_3,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_3__DI0_DATA_CNT_UP3_3,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_3__DI0_DATA_CNT_DOWN3_3,down);
                }
                break;}
        case  4:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_4__DI0_DATA_CNT_UP0_4,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_4__DI0_DATA_CNT_DOWN0_4,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_4__DI0_DATA_CNT_UP1_4,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_4__DI0_DATA_CNT_DOWN1_4,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_4__DI0_DATA_CNT_UP2_4,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_4__DI0_DATA_CNT_DOWN2_4,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_4__DI0_DATA_CNT_UP3_4,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_4__DI0_DATA_CNT_DOWN3_4,down);
                }
                break;}
        case  5:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_5__DI0_DATA_CNT_UP0_5,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_5__DI0_DATA_CNT_DOWN0_5,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_5__DI0_DATA_CNT_UP1_5,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_5__DI0_DATA_CNT_DOWN1_5,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_5__DI0_DATA_CNT_UP2_5,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_5__DI0_DATA_CNT_DOWN2_5,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_5__DI0_DATA_CNT_UP3_5,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_5__DI0_DATA_CNT_DOWN3_5,down);
                }
                break;}
        case  6:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_6__DI0_DATA_CNT_UP0_6,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_6__DI0_DATA_CNT_DOWN0_6,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_6__DI0_DATA_CNT_UP1_6,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_6__DI0_DATA_CNT_DOWN1_6,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_6__DI0_DATA_CNT_UP2_6,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_6__DI0_DATA_CNT_DOWN2_6,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_6__DI0_DATA_CNT_UP3_6,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_6__DI0_DATA_CNT_DOWN3_6,down);
                }
                break;}
        case  7:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_7__DI0_DATA_CNT_UP0_7,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_7__DI0_DATA_CNT_DOWN0_7,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_7__DI0_DATA_CNT_UP1_7,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_7__DI0_DATA_CNT_DOWN1_7,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_7__DI0_DATA_CNT_UP2_7,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_7__DI0_DATA_CNT_DOWN2_7,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_7__DI0_DATA_CNT_UP3_7,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_7__DI0_DATA_CNT_DOWN3_7,down);
                }
                break;}
        case  8:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_8__DI0_DATA_CNT_UP0_8,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_8__DI0_DATA_CNT_DOWN0_8,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_8__DI0_DATA_CNT_UP1_8,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_8__DI0_DATA_CNT_DOWN1_8,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_8__DI0_DATA_CNT_UP2_8,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_8__DI0_DATA_CNT_DOWN2_8,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_8__DI0_DATA_CNT_UP3_8,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_8__DI0_DATA_CNT_DOWN3_8,down);
                }
                break;}
        case  9:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_9__DI0_DATA_CNT_UP0_9,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_9__DI0_DATA_CNT_DOWN0_9,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_9__DI0_DATA_CNT_UP1_9,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_9__DI0_DATA_CNT_DOWN1_9,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_9__DI0_DATA_CNT_UP2_9,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_9__DI0_DATA_CNT_DOWN2_9,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_9__DI0_DATA_CNT_UP3_9,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_9__DI0_DATA_CNT_DOWN3_9,down);
                }
                break;}
        case 10:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_10__DI0_DATA_CNT_UP0_10,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_10__DI0_DATA_CNT_DOWN0_10,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_10__DI0_DATA_CNT_UP1_10,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_10__DI0_DATA_CNT_DOWN1_10,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_10__DI0_DATA_CNT_UP2_10,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_10__DI0_DATA_CNT_DOWN2_10,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_10__DI0_DATA_CNT_UP3_10,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_10__DI0_DATA_CNT_DOWN3_10,down);
                }
                break;}
        case 11:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_11__DI0_DATA_CNT_UP0_11,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET0_11__DI0_DATA_CNT_DOWN0_11,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_11__DI0_DATA_CNT_UP1_11,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET1_11__DI0_DATA_CNT_DOWN1_11,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_11__DI0_DATA_CNT_UP2_11,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET2_11__DI0_DATA_CNT_DOWN2_11,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_11__DI0_DATA_CNT_UP3_11,up);
                    ipu_write_field(ipu_num, IPU_DI0_DW_SET3_11__DI0_DATA_CNT_DOWN3_11,down);
                }
                break;}
		default:{
			    break;}	
        }
    }
    else {
        switch(pointer){
        case  0:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_0__DI1_DATA_CNT_UP0_0,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_0__DI1_DATA_CNT_DOWN0_0,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_0__DI1_DATA_CNT_UP1_0,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_0__DI1_DATA_CNT_DOWN1_0,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_0__DI1_DATA_CNT_UP2_0,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_0__DI1_DATA_CNT_DOWN2_0,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_0__DI1_DATA_CNT_UP3_0,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_0__DI1_DATA_CNT_DOWN3_0,down);
                }
                break;}
        case  1:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_1__DI1_DATA_CNT_UP0_1,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_1__DI1_DATA_CNT_DOWN0_1,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_1__DI1_DATA_CNT_UP1_1,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_1__DI1_DATA_CNT_DOWN1_1,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_1__DI1_DATA_CNT_UP2_1,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_1__DI1_DATA_CNT_DOWN2_1,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_1__DI1_DATA_CNT_UP3_1,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_1__DI1_DATA_CNT_DOWN3_1,down);
                }
                break;}
        case  2:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_2__DI1_DATA_CNT_UP0_2,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_2__DI1_DATA_CNT_DOWN0_2,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_2__DI1_DATA_CNT_UP1_2,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_2__DI1_DATA_CNT_DOWN1_2,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_2__DI1_DATA_CNT_UP2_2,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_2__DI1_DATA_CNT_DOWN2_2,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_2__DI1_DATA_CNT_UP3_2,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_2__DI1_DATA_CNT_DOWN3_2,down);
                }
                break;}
        case  3:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_3__DI1_DATA_CNT_UP0_3,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_3__DI1_DATA_CNT_DOWN0_3,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_3__DI1_DATA_CNT_UP1_3,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_3__DI1_DATA_CNT_DOWN1_3,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_3__DI1_DATA_CNT_UP2_3,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_3__DI1_DATA_CNT_DOWN2_3,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_3__DI1_DATA_CNT_UP3_3,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_3__DI1_DATA_CNT_DOWN3_3,down);
                }
                break;}
        case  4:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_4__DI1_DATA_CNT_UP0_4,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_4__DI1_DATA_CNT_DOWN0_4,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_4__DI1_DATA_CNT_UP1_4,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_4__DI1_DATA_CNT_DOWN1_4,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_4__DI1_DATA_CNT_UP2_4,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_4__DI1_DATA_CNT_DOWN2_4,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_4__DI1_DATA_CNT_UP3_4,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_4__DI1_DATA_CNT_DOWN3_4,down);
                }
                break;}
        case  5:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_5__DI1_DATA_CNT_UP0_5,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_5__DI1_DATA_CNT_DOWN0_5,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_5__DI1_DATA_CNT_UP1_5,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_5__DI1_DATA_CNT_DOWN1_5,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_5__DI1_DATA_CNT_UP2_5,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_5__DI1_DATA_CNT_DOWN2_5,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_5__DI1_DATA_CNT_UP3_5,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_5__DI1_DATA_CNT_DOWN3_5,down);
                }
                break;}
        case  6:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_6__DI1_DATA_CNT_UP0_6,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_6__DI1_DATA_CNT_DOWN0_6,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_6__DI1_DATA_CNT_UP1_6,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_6__DI1_DATA_CNT_DOWN1_6,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_6__DI1_DATA_CNT_UP2_6,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_6__DI1_DATA_CNT_DOWN2_6,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_6__DI1_DATA_CNT_UP3_6,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_6__DI1_DATA_CNT_DOWN3_6,down);
                }
                break;}
        case  7:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_7__DI1_DATA_CNT_UP0_7,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_7__DI1_DATA_CNT_DOWN0_7,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_7__DI1_DATA_CNT_UP1_7,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_7__DI1_DATA_CNT_DOWN1_7,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_7__DI1_DATA_CNT_UP2_7,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_7__DI1_DATA_CNT_DOWN2_7,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_7__DI1_DATA_CNT_UP3_7,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_7__DI1_DATA_CNT_DOWN3_7,down);
                }
                break;}
        case  8:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_8__DI1_DATA_CNT_UP0_8,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_8__DI1_DATA_CNT_DOWN0_8,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_8__DI1_DATA_CNT_UP1_8,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_8__DI1_DATA_CNT_DOWN1_8,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_8__DI1_DATA_CNT_UP2_8,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_8__DI1_DATA_CNT_DOWN2_8,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_8__DI1_DATA_CNT_UP3_8,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_8__DI1_DATA_CNT_DOWN3_8,down);
                }
                break;}
        case  9:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_9__DI1_DATA_CNT_UP0_9,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_9__DI1_DATA_CNT_DOWN0_9,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_9__DI1_DATA_CNT_UP1_9,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_9__DI1_DATA_CNT_DOWN1_9,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_9__DI1_DATA_CNT_UP2_9,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_9__DI1_DATA_CNT_DOWN2_9,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_9__DI1_DATA_CNT_UP3_9,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_9__DI1_DATA_CNT_DOWN3_9,down);
                }
                break;}
        case 10:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_10__DI1_DATA_CNT_UP0_10,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_10__DI1_DATA_CNT_DOWN0_10,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_10__DI1_DATA_CNT_UP1_10,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_10__DI1_DATA_CNT_DOWN1_10,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_10__DI1_DATA_CNT_UP2_10,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_10__DI1_DATA_CNT_DOWN2_10,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_10__DI1_DATA_CNT_UP3_10,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_10__DI1_DATA_CNT_DOWN3_10,down);
                }
                break;}
        case 11:{if (set == 0){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_11__DI1_DATA_CNT_UP0_11,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET0_11__DI1_DATA_CNT_DOWN0_11,down);
                }
                if (set == 1){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_11__DI1_DATA_CNT_UP1_11,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET1_11__DI1_DATA_CNT_DOWN1_11,down);
                }
                if (set == 2){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_11__DI1_DATA_CNT_UP2_11,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET2_11__DI1_DATA_CNT_DOWN2_11,down);
                }
                if (set == 3){
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_11__DI1_DATA_CNT_UP3_11,up);
                    ipu_write_field(ipu_num, IPU_DI1_DW_SET3_11__DI1_DATA_CNT_DOWN3_11,down);
                }
                break;}
		default:{
			    break;}	
        }
    }
}


#ifdef DISPLAY_INTERLACED
void di_config_interlaced(unsigned int IPU, unsigned int DI, int div, int extern_clock,
	int clock_polarity, int hsync_polarity, int vsync_polarity, int data_polarity, int data_polarity_enable,
	int hSyncStartWidth, int hSyncWidth, int hSyncEndWidth, int hDisp,
	int vSyncStartWidth, int vSyncWidth, int vSyncEndWidth, int vDisp)
{
	/*********************************************************************
	*	DI configuration:
	**********************************************************************/

	int clkUp, clkDown;

	// config (DRDY signal) DI_PIN15
	//di_pointer_config(IPU, di, pointer, access, componnent, cst, pt0, pt1, pt2, pt3, pt4, pt5, pt6);
	di_pointer_config(IPU, DI, DI_SDC_WAVEFORM, div - 1, div - 1, DI_NOUSE_SIGNAL, DI_NOUSE_SIGNAL, DI_NOUSE_SIGNAL, DI_NOUSE_SIGNAL, DI_NOUSE_SIGNAL, DI_DEN_SIGNAL, DI_NOUSE_SIGNAL, DI_NOUSE_SIGNAL);		

	//di_up_down_config(IPU, di, pointer, set, up, down)
	di_up_down_config(IPU, DI, DI_SDC_WAVEFORM, DI_DEN_SIGNAL, 0, div * 2);
	di_up_down_config(IPU, DI, DI_SDC_WAVEFORM, DI_NOUSE_SIGNAL, 0, 0);

	// set clk for DI
	// generate base clock for di
	clkUp = 0;
	clkDown = div;
	if(DI == 0)
	{
		ipu_write_field(IPU, IPU_DI0_BS_CLKGEN0__DI0_DISP_CLK_OFFSET, 0);
		ipu_write_field(IPU, IPU_DI0_BS_CLKGEN0__DI0_DISP_CLK_PERIOD, div << 4);
		ipu_write_field(IPU, IPU_DI0_BS_CLKGEN1__DI0_DISP_CLK_DOWN, clkDown);
		ipu_write_field(IPU, IPU_DI0_BS_CLKGEN1__DI0_DISP_CLK_UP, clkUp);

		//DI_SCR, set the screen height for field0
		ipu_write_field(IPU, IPU_DI0_SCR_CONF__DI0_SCREEN_HEIGHT, (vDisp + vSyncStartWidth + vSyncEndWidth + vSyncWidth) / 2 - 1);
	}
	else if(DI == 1)
	{
		ipu_write_field(IPU, IPU_DI1_BS_CLKGEN0__DI1_DISP_CLK_OFFSET, 0);
		ipu_write_field(IPU, IPU_DI1_BS_CLKGEN0__DI1_DISP_CLK_PERIOD, div << 4);
		ipu_write_field(IPU, IPU_DI1_BS_CLKGEN1__DI1_DISP_CLK_DOWN, clkDown);
		ipu_write_field(IPU, IPU_DI1_BS_CLKGEN1__DI1_DISP_CLK_UP, clkUp);

		//DI_SCR, set the screen height for field0
		ipu_write_field(IPU, IPU_DI1_SCR_CONF__DI1_SCREEN_HEIGHT, (vDisp + vSyncStartWidth + vSyncEndWidth + vSyncWidth) / 2 - 1);
	}

	// COUNTER_1: Internal VSYNC for each frame
	di_sync_config(
		IPU, // ipu num
		DI, // di
		DI_COUNTER_1, // pointer
		(vDisp + vSyncStartWidth + vSyncEndWidth + vSyncWidth) * 2 - 1, // run value for auto reload
		3, // run resolution, counter 1 can reference to counter 6,7,8 with run_resolution=2,3,4
		1, // offset value
		3, // offset resolution, 3=counter 7
		1, // auto reload mode
		0, // step repeat
		0, // counter clear source selection
		0, // counter polarity
		0, // counter polarity trigger selection
		0, // counter polarity clear selection
		0, // counter up
		0, // counter down
		0 // gentime select
	);

	//COUNTER_2: HSYNC waveform on DI_PIN02
	di_sync_config(
		IPU, // ipu num
		DI, // di
		DI_COUNTER_HSYNC, // pointer
		hDisp + hSyncStartWidth + hSyncEndWidth + hSyncWidth - 1, // run value for auto reload
		DI_COUNTER_BASECLK + 1, // run resolution
		0, // offset value
		0, // offset resolution
		1, // auto reload mode
		0, // step repeat
		0, // counter clear source selection
		1, // counter polarity
		DI_COUNTER_BASECLK + 1, // counter polarity trigger selection
		0, // counter polarity clear selection
		0, // counter up
		hSyncWidth << 1, // counter down
		0 // gentime select
	);

	//COUNTER_3: VSYNC waveform on DI_PIN03
	di_sync_config(
		IPU, // ipu num
		DI, // di
		DI_COUNTER_VSYNC, // pointer
		vDisp + vSyncStartWidth + vSyncEndWidth + vSyncWidth - 1, // run value for auto reload
		4, // run resolution, counter 3 can reference to counter 7 with run_resolution=4
		1, // offset value
		4, // offset resolution, 4=counter 7
		0, // auto reload mode
		2, // step repeat
		DI_COUNTER_1 + 1, // counter clear source selection
		1, // counter polarity
		4, // counter polarity trigger selection, 4=counter 7
		0, // counter polarity clear selection
		0, // counter up
		vSyncWidth << 1, // counter down
		0 // gentime select
	);

	//COUNTER_4: Active Field
	di_sync_config(
		IPU, // ipu num
		DI, // di
		DI_COUNTER_AFIELD, // pointer
		((vDisp + vSyncStartWidth + vSyncEndWidth + vSyncWidth) / 2 + 1) - 1, // run value for auto reload
		DI_COUNTER_HSYNC + 1, // run resolution
		(hDisp + hSyncStartWidth + hSyncEndWidth + hSyncWidth) / 2, // offset value
		DI_COUNTER_BASECLK + 1, // offset resolution
		0, // auto reload mode
		2, // step repeat
		DI_COUNTER_1 + 1, // counter clear source selection
		0, // counter polarity
		0, // counter polarity trigger selection
		0, // counter polarity clear selection
		0, // counter up
		0, // counter down
		0 // gentime select
	);

	//COUNTER_5: Active Line
	di_sync_config(
		IPU, // ipu num
		DI, // di
		DI_COUNTER_ALINE, // pointer
		0, // run value for auto reload
		DI_COUNTER_HSYNC + 1, // run resolution
		(vSyncStartWidth + vSyncWidth) / 2, // offset value
		DI_COUNTER_HSYNC + 1, // offset resolution
		0, // auto reload mode
		vDisp / 2, // step repeat
		DI_COUNTER_AFIELD + 1, // counter clear source selection
		0, // counter polarity
		0, // counter polarity trigger selection
		0, // counter polarity clear selection
		0, // counter up
		0, // counter down
		0 // gentime select
	);

	//COUNTER_6: Active Pixel
	di_sync_config(
		IPU, // ipu num
		DI, // di
		DI_COUNTER_APIXEL, // pointer
		0, // run value for auto reload
		DI_COUNTER_BASECLK + 1, // run resolution
		hSyncStartWidth + hSyncWidth, // offset value
		DI_COUNTER_BASECLK + 1, // offset resolution
		0, // auto reload mode
		hDisp, // step repeat
		DI_COUNTER_ALINE + 1, // counter clear source selection
		0, // counter polarity
		0, // counter polarity trigger selection
		0, // counter polarity clear selection
		0, // counter up
		0, // counter down
		0 // gentime select
	);

	//COUNTER_7: Half line HSYNC
	di_sync_config(
		IPU, // ipu num
		DI, // di
		DI_COUNTER_7, // pointer
		(hDisp + hSyncStartWidth + hSyncEndWidth + hSyncWidth) / 2 - 1, // run value for auto reload
		DI_COUNTER_BASECLK + 1, // run resolution
		0, // offset value
		0, // offset resolution
		1, // auto reload mode
		0, // step repeat
		0, // counter clear source selection
		0, // counter polarity
		0, // counter polarity trigger selection
		0, // counter polarity clear selection
		0, // counter up
		0, // counter down
		0 // gentime select
	);

	if(DI == 0)
	{
		ipu_write_field(IPU, IPU_DI0_SYNC_AS_GEN__DI0_SYNC_START, 0);  //0 lines predictions
		ipu_write_field(IPU, IPU_DI0_SYNC_AS_GEN__DI0_VSYNC_SEL, DI_COUNTER_VSYNC - 1); //pin3/counter 3 as VSYNC
		if(extern_clock){
			ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_CLK_EXT, 1); // select external generated clock 
			ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_VSYNC_EXT, 1); // select external VSYNC 
			ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_ERM_VSYNC_SEL, 1);		
		}else{
			ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_CLK_EXT, 0); // select internal generated clock 
			ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_VSYNC_EXT, 0); // select internal VSYNC 
			ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_ERM_VSYNC_SEL, 0);		
		}
		ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_POLARITY_DISP_CLK, clock_polarity); 
		ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_POLARITY_2, hsync_polarity);
		ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_POLARITY_3, vsync_polarity);
		ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_DISP_Y_SEL, DI_COUNTER_HSYNC - 1);  //pin2/counter 2 as HSYNC

		ipu_write_field(IPU, IPU_DI0_POL__DI0_DRDY_DATA_POLARITY, data_polarity);
		ipu_write_field(IPU, IPU_DI0_POL__DI0_DRDY_POLARITY_15, data_polarity_enable);

		//release ipu DI counter
		ipu_write_field(IPU, IPU_IPU_DISP_GEN__DI0_COUNTER_RELEASE, 1);
	}
	else if(DI == 1)
	{
		ipu_write_field(IPU, IPU_DI1_SYNC_AS_GEN__DI1_SYNC_START, 0);  //0 lines predictions
		ipu_write_field(IPU, IPU_DI1_SYNC_AS_GEN__DI1_VSYNC_SEL, DI_COUNTER_VSYNC - 1); //pin3/counter 3 as VSYNC
		if(extern_clock){
			ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_CLK_EXT, 1); // select external generated clock 
			ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_VSYNC_EXT, 1); // select external VSYNC 
			ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_ERM_VSYNC_SEL, 1);
		}else{
			ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_CLK_EXT, 0); // select internal generated clock 
			ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_VSYNC_EXT, 0); // select internal VSYNC 
			ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_ERM_VSYNC_SEL, 0);
		}
		ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_POLARITY_DISP_CLK, clock_polarity); 
		ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_POLARITY_2, hsync_polarity);
		ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_POLARITY_3, vsync_polarity);
		ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_DISP_Y_SEL, DI_COUNTER_HSYNC - 1);  //pin2/counter 2 as HSYNC

		ipu_write_field(IPU, IPU_DI1_POL__DI1_DRDY_DATA_POLARITY, data_polarity);
		ipu_write_field(IPU, IPU_DI1_POL__DI1_DRDY_POLARITY_15, data_polarity_enable);

		//release ipu DI counter
		ipu_write_field(IPU, IPU_IPU_DISP_GEN__DI1_COUNTER_RELEASE, 1);
	}
}

#else
	
void di_config_none_interlaced(unsigned int IPU, unsigned int DI, int div, int extern_clock,
	int clock_polarity, int hsync_polarity, int vsync_polarity, int data_polarity, int data_polarity_enable,
	int hSyncStartWidth, int hSyncWidth, int hSyncEndWidth, int hDisp,
	int vSyncStartWidth, int vSyncWidth, int vSyncEndWidth, int vDisp)
{
	/*********************************************************************
	*	DI configuration:
	**********************************************************************/

	int clkUp, clkDown;

	// config (DRDY signal) DI_PIN15
	//di_pointer_config(IPU, di, pointer, access, componnent, cst, pt0, pt1, pt2, pt3, pt4, pt5, pt6);
	di_pointer_config(IPU, DI, DI_SDC_WAVEFORM, div - 1, div - 1, DI_NOUSE_SIGNAL, DI_NOUSE_SIGNAL, DI_NOUSE_SIGNAL, DI_NOUSE_SIGNAL, DI_NOUSE_SIGNAL, DI_DEN_SIGNAL, DI_NOUSE_SIGNAL, DI_NOUSE_SIGNAL);		

	//di_up_down_config(IPU, di, pointer, set, up, down)
	di_up_down_config(IPU, DI, DI_SDC_WAVEFORM, DI_DEN_SIGNAL, 0, div * 2);
	di_up_down_config(IPU, DI, DI_SDC_WAVEFORM, DI_NOUSE_SIGNAL, 0, 0);

	// set clk for DI
	// generate base clock for di
	clkUp = 0;
	clkDown = div;
	if(DI == 0)
	{
		ipu_write_field(IPU, IPU_DI0_BS_CLKGEN0__DI0_DISP_CLK_OFFSET, 0);
		ipu_write_field(IPU, IPU_DI0_BS_CLKGEN0__DI0_DISP_CLK_PERIOD, div << 4);
		ipu_write_field(IPU, IPU_DI0_BS_CLKGEN1__DI0_DISP_CLK_DOWN, clkDown);
		ipu_write_field(IPU, IPU_DI0_BS_CLKGEN1__DI0_DISP_CLK_UP, clkUp);

		//DI_SCR, set the screen height   
		ipu_write_field(IPU, IPU_DI0_SCR_CONF__DI0_SCREEN_HEIGHT, vDisp + vSyncStartWidth + vSyncEndWidth + vSyncWidth - 1);
	}
	else if(DI == 1)
	{
		ipu_write_field(IPU, IPU_DI1_BS_CLKGEN0__DI1_DISP_CLK_OFFSET, 0);
		ipu_write_field(IPU, IPU_DI1_BS_CLKGEN0__DI1_DISP_CLK_PERIOD, div << 4);
		ipu_write_field(IPU, IPU_DI1_BS_CLKGEN1__DI1_DISP_CLK_DOWN, clkDown);
		ipu_write_field(IPU, IPU_DI1_BS_CLKGEN1__DI1_DISP_CLK_UP, clkUp);

		//DI_SCR, set the screen height   
		ipu_write_field(IPU, IPU_DI1_SCR_CONF__DI1_SCREEN_HEIGHT, vDisp + vSyncStartWidth + vSyncEndWidth + vSyncWidth - 1);
	}

	di_sync_config(
		IPU, // ipu num
		DI, // di
		DI_COUNTER_IHSYNC, // pointer
		hDisp + hSyncStartWidth + hSyncEndWidth + hSyncWidth - 1, // run value for auto reload
		DI_COUNTER_BASECLK + 1, // run resolution
		0, // offset value
		0, // offset resolution
		1, // auto reload mode
		0, // step repeat
		0, // counter clear source selection
		0, // counter polarity
		0, // counter polarity trigger selection
		0, // counter polarity clear selection
		0, // counter up
		0, // counter down
		0 // gentime select
	);

	//Output HSYNC
	di_sync_config(
		IPU, // ipu num
		DI, // di
		DI_COUNTER_HSYNC, // pointer
		hDisp + hSyncStartWidth + hSyncEndWidth + hSyncWidth - 1, // run value for auto reload
		DI_COUNTER_BASECLK + 1, // run resolution
		0, // offset value
		0, // offset resolution
		1, // auto reload mode
		0, // step repeat
		0, // counter clear source selection
		1, // counter polarity
		DI_COUNTER_BASECLK + 1, // counter polarity trigger selection
		0, // counter polarity clear selection
		0, // counter up
		hSyncWidth << 1, // counter down
		0 // gentime select
	);

	//Output VSYNC
	di_sync_config(
		IPU, // ipu num
		DI, // di
		DI_COUNTER_VSYNC, // pointer
		vDisp + vSyncStartWidth + vSyncEndWidth + vSyncWidth - 1, // run value for auto reload
		DI_COUNTER_HSYNC + 1, // run resolution
		0, // offset value
		0, // offset resolution
		1, // auto reload mode
		0, // step repeat
		0, // counter clear source selection
		1, // counter polarity
		DI_COUNTER_HSYNC + 1, // counter polarity trigger selection
		0, // counter polarity clear selection
		0, // counter up
		vSyncWidth << 1, // counter down
		0 // gentime select
	);

	//Active Lines start points
	di_sync_config(
		IPU, // ipu num
		DI, // di
		DI_COUNTER_ALINE, // pointer
		0, // run value for auto reload	
		DI_COUNTER_HSYNC + 1, // run resolution
		vSyncStartWidth + vSyncWidth, // offset value
		DI_COUNTER_HSYNC + 1, // offset resolution
		0, // auto reload mode
		vDisp, // step repeat
		DI_COUNTER_VSYNC + 1, // counter clear source selection
		0, // counter polarity
		0, // counter polarity trigger selection
		0, // counter polarity clear selection
		0, // counter up
		0, // counter down
		0 // gentime select
	);

	//Active clock start points
	di_sync_config(
		IPU, // ipu num
		DI, // di
		DI_COUNTER_APIXEL, // pointer
		0, // run value for auto reload
		DI_COUNTER_BASECLK + 1, // run resolution
		hSyncStartWidth + hSyncWidth, // offset value
		DI_COUNTER_BASECLK + 1, // offset resolution
		0, // auto reload mode
		hDisp, // step repeat
		DI_COUNTER_ALINE + 1, // counter clear source selection
		0, // counter polarity
		0, // counter polarity trigger selection
		0, // counter polarity clear selection
		0, // counter up
		0, // counter down
		0 // gentime select
	);

	if(DI == 0)
	{
		ipu_write_field(IPU, IPU_DI0_SYNC_AS_GEN__DI0_SYNC_START, 2);  // 2 lines predictions
		ipu_write_field(IPU, IPU_DI0_SYNC_AS_GEN__DI0_VSYNC_SEL, DI_COUNTER_VSYNC - 1);  //select PIN3 as VSYNC
		if(extern_clock){
			ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_CLK_EXT, 1);  //select external generated clock 
			ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_VSYNC_EXT, 1);  //select external VSYNC 
			ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_ERM_VSYNC_SEL, 1);	
		}else{
			ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_CLK_EXT, 0);  //select internal generated clock 
			ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_VSYNC_EXT, 0);  //select internal VSYNC 
			ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_ERM_VSYNC_SEL, 0);
		}

		ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_POLARITY_DISP_CLK, clock_polarity); 
		ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_POLARITY_2, hsync_polarity);  //HSYNC polarity
		ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_POLARITY_3, vsync_polarity);  //VSYNC polarity
		ipu_write_field(IPU, IPU_DI0_GENERAL__DI0_DISP_Y_SEL, DI_COUNTER_HSYNC - 1);  //pin2/counter 2 as HSYNC

		ipu_write_field(IPU, IPU_DI0_POL__DI0_DRDY_DATA_POLARITY, data_polarity);
		ipu_write_field(IPU, IPU_DI0_POL__DI0_DRDY_POLARITY_15, data_polarity_enable);  //VIDEO_DATA_EN POLARITY

		//release ipu DI counter
		ipu_write_field(IPU, IPU_IPU_DISP_GEN__DI0_COUNTER_RELEASE, 1);
	}
	else if(DI == 1)
	{
		ipu_write_field(IPU, IPU_DI1_SYNC_AS_GEN__DI1_SYNC_START, 2);  // 2 lines predictions
		ipu_write_field(IPU, IPU_DI1_SYNC_AS_GEN__DI1_VSYNC_SEL, DI_COUNTER_VSYNC - 1);  //select PIN3 as VSYNC
		if(extern_clock){
			ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_CLK_EXT, 1);	//select external generated clock 
			ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_VSYNC_EXT, 1);  //select external VSYNC 
			ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_ERM_VSYNC_SEL, 1);
		}else{
			ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_CLK_EXT, 0);	//select internal generated clock 
			ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_VSYNC_EXT, 0);  //select internal VSYNC 
			ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_ERM_VSYNC_SEL, 0);
		}
		ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_POLARITY_DISP_CLK, clock_polarity); 
		ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_POLARITY_2, hsync_polarity);	//HSYNC polarity
		ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_POLARITY_3, vsync_polarity);	//VSYNC polarity
		ipu_write_field(IPU, IPU_DI1_GENERAL__DI1_DISP_Y_SEL, DI_COUNTER_HSYNC - 1);  //pin2/counter 2 as HSYNC

		ipu_write_field(IPU, IPU_DI1_POL__DI1_DRDY_DATA_POLARITY, data_polarity);
		ipu_write_field(IPU, IPU_DI1_POL__DI1_DRDY_POLARITY_15, data_polarity_enable);	//VIDEO_DATA_EN POLARITY

		//release ipu DI counter
		ipu_write_field(IPU, IPU_IPU_DISP_GEN__DI1_COUNTER_RELEASE, 1);
	}
}
#endif
