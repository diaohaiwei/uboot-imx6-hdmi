
//#define CONFIG_VIDEO_LOGO
//#define CONFIG_VIDEO_BMP_LOGO

// Select one of the output mode
#define IPU_OUTPUT_MODE_HDMI
//#define IPU_OUTPUT_MODE_LVDS
//#define IPU_OUTPUT_MODE_LCD

#define CONFIG_FB_BASE	(CONFIG_SYS_TEXT_BASE + 0x1000000)
#define UBOOT_LOGO_BMP_ADDR 0x00B00000

#define CONFIG_IMX_PWM
#define IMX_PWM1_BASE	 PWM1_BASE_ADDR
#define IMX_PWM2_BASE	 PWM2_BASE_ADDR

#ifdef CONFIG_MX6DL
	#define CONFIG_IPUV3_CLK 270000000
#else
	#define CONFIG_IPUV3_CLK 264000000
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef IPU_OUTPUT_MODE_HDMI
//	#define HDMI_RES_1280_720 
	#define HDMI_RES_1920_720
//	#define HDMI_RES_1920_1080
//	#define HDMI_RES_1920_1200

	#define DISPLAY_VSYNC_POLARITY			1
	#define DISPLAY_HSYNC_POLARITY			1
	#define DISPLAY_CLOCK_POLARITY			0
	#define DISPLAY_DATA_POLARITY				0
	#define DISPLAY_DATA_ENABLE_POLARITY		1

	#define IPU_NUM 		1  // 1 for IPU1, 2 for IPU2.
	#define DI_NUM			0  // 0 for DI0, 1 for DI1.
	#define DI_CLOCK_EXTERNAL_MODE  //When clock external mode was defined, the DI clock root will be PLL5, 
					//without this macro, the DI root clock is IPU internal clock.

#ifdef HDMI_RES_1280_720
	// For HDMI, 1280*720 resolution
	#define DISPLAY_WIDTH	1280
	#define DISPLAY_HEIGHT	720
	#define DISPLAY_BPP		32
	#define DISPLAY_IF_BPP	24  // RGB24 interface

	#define DISPLAY_HSYNC_START	220
	#define DISPLAY_HSYNC_END		110
	#define DISPLAY_HSYNC_WIDTH	40

	#define DISPLAY_VSYNC_START	20
	#define DISPLAY_VSYNC_END		5
	#define DISPLAY_VSYNC_WIDTH	5

	#define DISPLAY_PIX_CLOCK	74250000  //(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz)
#endif // (HDMI_RES_1280_720)

#ifdef HDMI_RES_1920_720
	// For HDMI, 1920*720 resolution
	#define DISPLAY_WIDTH	1920
	#define DISPLAY_HEIGHT	720
	#define DISPLAY_BPP		24//32
	#define DISPLAY_IF_BPP	24  // RGB24 interface

	#define DISPLAY_HSYNC_START	148
	#define DISPLAY_HSYNC_END		88
	#define DISPLAY_HSYNC_WIDTH	44

	#define DISPLAY_VSYNC_START	36
	#define DISPLAY_VSYNC_END		4
	#define DISPLAY_VSYNC_WIDTH	5

	// (2200 * 765 * 60)
	#define DISPLAY_PIX_CLOCK	100980000  //(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz)
#endif // (HDMI_RES_1920_720)

#ifdef HDMI_RES_1920_1080
	// For HDMI, 1920*1080 resolution
	#define DISPLAY_WIDTH	1920
	#define DISPLAY_HEIGHT	1080
	#define DISPLAY_BPP		32
	#define DISPLAY_IF_BPP	24  // RGB24 interface

	#define DISPLAY_HSYNC_START	148
	#define DISPLAY_HSYNC_END		88
	#define DISPLAY_HSYNC_WIDTH	44

	#define DISPLAY_VSYNC_START	36
	#define DISPLAY_VSYNC_END		4
	#define DISPLAY_VSYNC_WIDTH	5

	// (2200 * 1125 * 60)
	#define DISPLAY_PIX_CLOCK	148500000  //(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz)
#endif // (HDMI_RES_1920_1080)

#ifdef HDMI_RES_1920_1200
	// For HDMI, 1920*1200 resolution
	#define DISPLAY_WIDTH	1920
	#define DISPLAY_HEIGHT	1200
	#define DISPLAY_BPP		32
	#define DISPLAY_IF_BPP	24  // RGB24 interface

	#define DISPLAY_HSYNC_START	148
	#define DISPLAY_HSYNC_END		88
	#define DISPLAY_HSYNC_WIDTH	44

	#define DISPLAY_VSYNC_START	36
	#define DISPLAY_VSYNC_END		4
	#define DISPLAY_VSYNC_WIDTH	5

	// (2200 * 1245 * 60)
	#define DISPLAY_PIX_CLOCK	164340000  //(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz)
#endif // (HDMI_RES_1920_1200)

	#define CONFIG_IMX_HDMI
#endif // (IPU_OUTPUT_MODE_HDMI)

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef IPU_OUTPUT_MODE_LVDS

//	#define LVDS_RES_1024_768
//	#define LVDS_RES_1920_1080
	#define LVDS_RES_1920_720
//	#define LVDS_RES_1440_540
//	#define LVDS_RES_1920_1200

	#define DISPLAY_VSYNC_POLARITY			0
	#define DISPLAY_HSYNC_POLARITY			0
	#define DISPLAY_CLOCK_POLARITY			0
	#define DISPLAY_DATA_POLARITY				0
	#define DISPLAY_DATA_ENABLE_POLARITY		1

	#define IPU_NUM			1  // 1 for IPU1, 2 for IPU2.
	#define DI_NUM			0  // 0 for DI0, 1 for DI1.
	#define DI_CLOCK_EXTERNAL_MODE  //When clock external mode was defined, the DI clock root will be PLL3 PFD1, 
								//without this macro, the DI root clock is IPU internal clock.

#ifdef LVDS_RES_1440_540
	// For LVDS, 1440*540 resolution
	#define DISPLAY_WIDTH	1440
	#define DISPLAY_HEIGHT	540
	#define DISPLAY_BPP		32
	#define DISPLAY_IF_BPP	24//18  // RGB666 interface

	#define DISPLAY_HSYNC_START	100
	#define DISPLAY_HSYNC_END		50
	#define DISPLAY_HSYNC_WIDTH	50

	#define DISPLAY_VSYNC_START	10
	#define DISPLAY_VSYNC_END		5
	#define DISPLAY_VSYNC_WIDTH	5

	#define DISPLAY_PIX_CLOCK_60HZ	55104000  //(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz)
	#define DISPLAY_PIX_CLOCK_76HZ	69798400  //(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz)
	#define DISPLAY_PIX_CLOCK DISPLAY_PIX_CLOCK_76HZ

	#define LVDS_PORT		0//1  // 0 for LVDS0, 1 for LVDS1.

#endif	// (LVDS_RES_1440_540)

#ifdef LVDS_RES_1920_720
	// For LVDS, 1920*720 resolution, dual channel
	#define DISPLAY_WIDTH	1920
	#define DISPLAY_HEIGHT	720
	#define DISPLAY_BPP		24//32
	#define DISPLAY_IF_BPP	24	// RGB24 interface

	#define DISPLAY_HSYNC_START	100
	#define DISPLAY_HSYNC_END		40
	#define DISPLAY_HSYNC_WIDTH	20

	#define DISPLAY_VSYNC_START	20
	#define DISPLAY_VSYNC_END		10
	#define DISPLAY_VSYNC_WIDTH	10

	#define DISPLAY_PIX_CLOCK		94848000  //(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz)


	#define LVDS_SPLIT_MODE  // For dual channel split mode.
#endif // (LVDS_RES_1920_720)

#ifdef LVDS_RES_1920_1200
	// For LVDS, 1920*1200 resolution, dual channel
	#define DISPLAY_WIDTH	1920
	#define DISPLAY_HEIGHT	1200
	#define DISPLAY_BPP		24//32
	#define DISPLAY_IF_BPP	24	// RGB24 interface

	#define DISPLAY_HSYNC_START	100
	#define DISPLAY_HSYNC_END		40
	#define DISPLAY_HSYNC_WIDTH	20

	#define DISPLAY_VSYNC_START	20
	#define DISPLAY_VSYNC_END		10
	#define DISPLAY_VSYNC_WIDTH	10

	#define DISPLAY_PIX_CLOCK		154752000 //(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz)
							// (160 + 1920) * (40+ 1200) * 60 = 2080 * 1240
							// (160 + 1920) * (40 + 720) * 60

	#define LVDS_SPLIT_MODE  // For dual channel split mode.
#endif // (LVDS_RES_1920_720)


#ifdef LVDS_RES_1024_768
	// For LVDS, 1024*768 resolution
	#define DISPLAY_WIDTH	1024
	#define DISPLAY_HEIGHT	768
	#define DISPLAY_BPP		32
	#define DISPLAY_IF_BPP	18  // RGB666 interface

	#define DISPLAY_HSYNC_START	220
	#define DISPLAY_HSYNC_END		40
	#define DISPLAY_HSYNC_WIDTH	60

	#define DISPLAY_VSYNC_START	21
	#define DISPLAY_VSYNC_END		7
	#define DISPLAY_VSYNC_WIDTH	10

	#define DISPLAY_PIX_CLOCK	64000000  //(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz)
#endif	// (LVDS_RES_1024_768)

#ifdef LVDS_RES_1920_1080
	// For LVDS, 1920*1080 resolution, dual channel
	#define DISPLAY_WIDTH	1920
	#define DISPLAY_HEIGHT	1080
	#define DISPLAY_BPP		32
	#define DISPLAY_IF_BPP	24	// RGB24 interface

	#define DISPLAY_HSYNC_START	100
	#define DISPLAY_HSYNC_END		40
	#define DISPLAY_HSYNC_WIDTH	10

	#define DISPLAY_VSYNC_START	20
	#define DISPLAY_VSYNC_END		3
	#define DISPLAY_VSYNC_WIDTH	2

	#define DISPLAY_PIX_CLOCK		135000000  //(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz)


	#define LVDS_SPLIT_MODE  // For dual channel split mode.
#endif // (LVDS_RES_1920_1080)

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef IPU_OUTPUT_MODE_LCD
	//For LCD, 800*480 resolution
	#define DISPLAY_WIDTH	800
	#define DISPLAY_HEIGHT	480
	#define DISPLAY_BPP		32
	#define DISPLAY_IF_BPP	16  // RGB565 interface

	#define DISPLAY_HSYNC_START	40
	#define DISPLAY_HSYNC_END		60
	#define DISPLAY_HSYNC_WIDTH	20

	#define DISPLAY_VSYNC_START	10
	#define DISPLAY_VSYNC_END		10
	#define DISPLAY_VSYNC_WIDTH	10

	#define DISPLAY_PIX_CLOCK	27000000  //(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz)

	#define DISPLAY_VSYNC_POLARITY			0
	#define DISPLAY_HSYNC_POLARITY			0
	#define DISPLAY_CLOCK_POLARITY			1
	#define DISPLAY_DATA_POLARITY				0
	#define DISPLAY_DATA_ENABLE_POLARITY		1

	#define IPU_NUM			1  // 1 for IPU1, 2 for IPU2.
	#define DI_NUM			0  // 0 for DI0, 1 for DI1.
	#define DI_CLOCK_EXTERNAL_MODE  //When clock external mode was defined, the DI clock root will be PLL5, 
								//without this macro, the DI root clock is IPU internal clock.
#endif


