/*
 * Copyright (C) 2012-2014 Freescale Semiconductor, Inc.
 *
 * Author: Fabio Estevam <fabio.estevam@freescale.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/arch/mx6-pins.h>
#include <mmc.h>

extern int mmc_get_env_devno(void);

void createColorBar(unsigned char * pData)
{
	int i = 0;
		// Fill RGB frame buffer
		// Red
		for (i = 0; i < (DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_BPP / 8) / 3); i += (DISPLAY_BPP / 8)) {
#if (DISPLAY_BPP == 16)
			pData[i + 0] = 0x00;
			pData[i + 1] = 0xF8;
#else
			pData[i + 0] = 0x00;
			pData[i + 1] = 0x00;
			pData[i + 2] = 0xFF;
#endif
		}

		// Green
		for (; i < (DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_BPP / 8) / 3) * 2; i += (DISPLAY_BPP / 8)) {
#if (DISPLAY_BPP == 16)
			pData[i + 0] = 0xE0;
			pData[i + 1] = 0x07;
#else
			pData[i + 0] = 0x00;
			pData[i + 1] = 0xFF;
			pData[i + 2] = 0x00;
#endif
		}

		// Blue
		for (; i < DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_BPP / 8); i += (DISPLAY_BPP / 8)) {
#if (DISPLAY_BPP == 16)
			pData[i + 0] = 0x1F;
			pData[i + 1] = 0x00;
#else
			pData[i + 0] = 0xFF;
			pData[i + 1] = 0x00;
			pData[i + 2] = 0x00;
#endif
		}
}

#if 1

//static int print_logo_info(void)
//{
//	return 0;
//}

#else

#include <bmp_logo_data.h>
#include <bmp_logo.h>

#include <asm/global_data.h>
#include <asm-generic/sections.h>


//extern unsigned int __logo_start[];
//extern unsigned int __logo_end[];
char __logo_start[0] __attribute__((section(".__logo_start")));
char __logo_end[0] __attribute__((section(".__logo_end")));

static int print_logo_info(void)
{
	volatile char *p = (volatile char *)(__logo_start);
  
	int i;
    	unsigned int len = 16;


    	//len = (unsigned int)&__logo_end - (unsigned int)&__logo_start;

   	printf("__image_copy_start = %p\n", __image_copy_start);
   	printf("__image_copy_end = %p\n", __image_copy_end);
   	printf("__logo_start = %p\n", __logo_start);
   	printf("__logo_end = %p\n", __logo_end);
	printf("logo size is %x\n", len);
	printf("gd->relocaddr, %lu\r\n", gd->relocaddr);

 	printf("=======================\r\n");

   for (i = 0; i < len; i++)
    {
        if(i % 16 == 0)
        {
            printf("\n");
        }

        printf("%3x ", *p++&0xff);
    }
    printf("\n");

	printf("=======================\r\n");

    return 0;

}

#endif

int load_image_from_mmc(unsigned char * pData)
{
	unsigned int size = DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_BPP / 8);
	unsigned int start, count;

	int mmc_dev = mmc_get_env_devno();
	struct mmc *mmc = find_mmc_device(mmc_dev);
	if (mmc)	{
		if (mmc_init(mmc) == 0) {
			start = ALIGN(UBOOT_LOGO_BMP_ADDR, mmc->read_bl_len) / mmc->read_bl_len;
			count = ALIGN(size, mmc->read_bl_len) / mmc->read_bl_len;
			mmc->block_dev.block_read(mmc_dev, start, count, pData);
			return 1;
		}

	}
	return 0;
}

#include <fs.h>

static int read_file_from_fat(unsigned char* pData, const char *filename,unsigned long pos, unsigned long bytes) {

        unsigned long addr;
 
        int len_read;
        unsigned long time;

 
        if (fs_set_blk_dev("mmc", "2:1", FS_TYPE_FAT)){
		printf("no fat filesystem at mmc 2:1\r\n");
		return 1;
	}

	addr = (unsigned long) pData;
        time = get_timer(0);
        len_read = fs_read(filename, addr, pos, bytes);
        time = get_timer(time);
        if (len_read <= 0){
		printf("fail to read image file\r\n");
		return 1;
	}

        printf("%d bytes read in %lu ms\r\n", len_read, time);
 
        return 0;
}

/*
typedef struct tagBITMAPFILEHEADER {

	uint16_t  bfType;  
	uint32_t bfSize; 
	uint16_t  bfReserved1; 
	uint16_t  bfReserved2; 
	uint32_t bfOffBits;

} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {

	uint32_t biSize; 
	uint32_t biWidth; 
	uint32_t biHeight; 
	uint16_t biPlanes; 
	uint16_t biBitCount; 
	uint32_t biCompression; 
	uint32_t biSizeImage; 
	uint32_t biXPelsPerMeter; 
	uint32_t biYPelsPerMeter; 
	uint32_t biClrUsed; 
	uint32_t biClrImportant;

} BITMAPINFOHEADER;
*/

int load_image_from_fat_old(unsigned char * pData) {
	unsigned int size = DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_BPP / 8);
	//unsigned int start = 54;
	//unsigned int test = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    	//printf("bitmap offset %d\r\n", test);

	//if(read_file_from_fat(pData, "screen.bmp", start, size))

#if defined(HDMI_RES_1920_720) || defined(LVDS_RES_1920_720)
	if(read_file_from_fat(pData, "ept.logo", 0, size))
		return 0;
#endif
#if defined(HDMI_RES_1920_1080) || defined(LVDS_RES_1920_1080)
	if(read_file_from_fat(pData, "ept_1920_1080.logo", 0, size))
		return 0;
	
#endif
	return size;
}

#define BITMAP_HEIGHT 	540
#define BITMAP_WIDTH	540
#define BITMAP_BPP		DISPLAY_BPP
#define BITMAP_SIZE 	(BITMAP_HEIGHT*BITMAP_WIDTH*BITMAP_BPP/8)

#define BITMAP_OFFSET	56

#define START_ROW 		((DISPLAY_HEIGHT - BITMAP_HEIGHT)/2)
#define START_COL 		((DISPLAY_WIDTH - BITMAP_WIDTH)/2)

int load_image_from_fat(unsigned char * pData) {
	unsigned char* pBitmap = pData + (DISPLAY_HEIGHT*DISPLAY_WIDTH*DISPLAY_BPP/8);
	unsigned char* pSrcData = pBitmap;
	int i;
		
	if(read_file_from_fat(pBitmap, "logo.bmp", 0, BITMAP_OFFSET+BITMAP_SIZE)){
		printf("read logo error \n");
		return 0;
	}
	
		printf("read logo ok \n");
	{
		pSrcData += BITMAP_OFFSET;
		pData += DISPLAY_WIDTH * (DISPLAY_BPP / 8) * START_ROW;
		pData += START_COL* (DISPLAY_BPP / 8);
		
		for(i=0; i < BITMAP_HEIGHT; i++){
			memcpy(pData, pSrcData, BITMAP_WIDTH*BITMAP_BPP/8);
			pSrcData += BITMAP_WIDTH*(BITMAP_BPP / 8);
			pData += DISPLAY_WIDTH*(DISPLAY_BPP / 8);
		}
		
	}

	return 1;
}

