/*
 * Copyright (C) 2012-2014 Freescale Semiconductor, Inc.
 *
 * Author: Fabio Estevam <fabio.estevam@freescale.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/arch/clock.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/crm_regs.h>
#include <asm/arch/iomux.h>
#include <asm/arch/mx6-pins.h>
#include <asm/errno.h>
#include <asm/gpio.h>
#include <mmc.h>

#include <asm/imx-common/iomux-v3.h>
#include <asm/arch/mxc_hdmi.h>
#include <linux/fb.h>
#include <ipu_pixfmt.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>

#include "mx6sabresd-main.h"


//#undef CONFIG_UBOOT_LOGO_ENABLE

#ifdef CONFIG_UBOOT_LOGO_ENABLE
#include <asm/imx-common/imx_pwm.h>
#include <asm/imx-common/mxc_ipu.h>
#endif


#if defined(CONFIG_VIDEO_IPUV3)
struct display_info_t {
	int	bus;
	int	addr;
	int	pixfmt;
	int	(*detect)(struct display_info_t const *dev);
	void	(*enable)(struct display_info_t const *dev);
	struct	fb_videomode mode;
};

static void disable_lvds(struct display_info_t const *dev)
{
	struct iomuxc *iomux = (struct iomuxc *)IOMUXC_BASE_ADDR;

	int reg = readl(&iomux->gpr[2]);

	reg &= ~(IOMUXC_GPR2_LVDS_CH0_MODE_MASK |
		 IOMUXC_GPR2_LVDS_CH1_MODE_MASK);

	writel(reg, &iomux->gpr[2]);
}

static void do_enable_hdmi(struct display_info_t const *dev)
{
	disable_lvds(dev);
	imx_enable_hdmi_phy();
}

static struct display_info_t const displays[] = {{
	.bus	= -1,
	.addr	= 0,
	.pixfmt	= IPU_PIX_FMT_RGB666,
	.detect	= NULL,
	.enable	= NULL,
	.mode	= {
		.name           = "Hannstar-XGA",
		.refresh        = 60,
		.xres           = 1024,
		.yres           = 768,
		.pixclock       = 15385,
		.left_margin    = 220,
		.right_margin   = 40,
		.upper_margin   = 21,
		.lower_margin   = 7,
		.hsync_len      = 60,
		.vsync_len      = 10,
		.sync           = FB_SYNC_EXT,
		.vmode          = FB_VMODE_NONINTERLACED
} }, {
	.bus	= -1,
	.addr	= 0,
	.pixfmt	= IPU_PIX_FMT_RGB24,
	.detect	= NULL,
	.enable	= do_enable_hdmi,
	.mode	= {
		.name           = "HDMI",
		.refresh        = 60,
		.xres           = 640,
		.yres           = 480,
		.pixclock       = 39721,
		.left_margin    = 48,
		.right_margin   = 16,
		.upper_margin   = 33,
		.lower_margin   = 10,
		.hsync_len      = 96,
		.vsync_len      = 2,
		.sync           = 0,
		.vmode          = FB_VMODE_NONINTERLACED
} } };

int board_video_skip(void)
{
	int i;
	int ret;
	char const *panel = getenv("panel");
	if (!panel) {
		for (i = 0; i < ARRAY_SIZE(displays); i++) {
			struct display_info_t const *dev = displays+i;
			if (dev->detect && dev->detect(dev)) {
				panel = dev->mode.name;
				printf("auto-detected panel %s\n", panel);
				break;
			}
		}
		if (!panel) {
			panel = displays[0].mode.name;
			printf("No panel detected: default to %s\n", panel);
			i = 0;
		}
	} else {
		for (i = 0; i < ARRAY_SIZE(displays); i++) {
			if (!strcmp(panel, displays[i].mode.name))
				break;
		}
	}
	if (i < ARRAY_SIZE(displays)) {
		ret = ipuv3_fb_init(&displays[i].mode, 0,
				    displays[i].pixfmt);
		if (!ret) {
			if (displays[i].enable)
				displays[i].enable(displays+i);
			printf("Display: %s (%ux%u)\n",
			       displays[i].mode.name,
			       displays[i].mode.xres,
			       displays[i].mode.yres);
		} else
			printf("LCD %s cannot be configured: %d\n",
			       displays[i].mode.name, ret);
	} else {
		printf("unsupported panel %s\n", panel);
		return -EINVAL;
	}

	return 0;
}

static void setup_display(void)
{
	struct mxc_ccm_reg *mxc_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;
	struct iomuxc *iomux = (struct iomuxc *)IOMUXC_BASE_ADDR;
	int reg;

	/* Setup HSYNC, VSYNC, DISP_CLK for debugging purposes */
	imx_iomux_v3_setup_multiple_pads(di0_pads, ARRAY_SIZE(di0_pads));

	enable_ipu_clock();
	imx_setup_hdmi();

	/* Turn on LDB0, LDB1, IPU,IPU DI0 clocks */
	reg = readl(&mxc_ccm->CCGR3);
	reg |=  MXC_CCM_CCGR3_LDB_DI0_MASK | MXC_CCM_CCGR3_LDB_DI1_MASK;
	writel(reg, &mxc_ccm->CCGR3);

	/* set LDB0, LDB1 clk select to 011/011 */
	reg = readl(&mxc_ccm->cs2cdr);
	reg &= ~(MXC_CCM_CS2CDR_LDB_DI0_CLK_SEL_MASK
		 | MXC_CCM_CS2CDR_LDB_DI1_CLK_SEL_MASK);
	reg |= (3 << MXC_CCM_CS2CDR_LDB_DI0_CLK_SEL_OFFSET)
	      | (3 << MXC_CCM_CS2CDR_LDB_DI1_CLK_SEL_OFFSET);
	writel(reg, &mxc_ccm->cs2cdr);

	reg = readl(&mxc_ccm->cscmr2);
	reg |= MXC_CCM_CSCMR2_LDB_DI0_IPU_DIV | MXC_CCM_CSCMR2_LDB_DI1_IPU_DIV;
	writel(reg, &mxc_ccm->cscmr2);

	reg = readl(&mxc_ccm->chsccdr);
	reg |= (CHSCCDR_CLK_SEL_LDB_DI0
		<< MXC_CCM_CHSCCDR_IPU1_DI0_CLK_SEL_OFFSET);
	reg |= (CHSCCDR_CLK_SEL_LDB_DI0
		<< MXC_CCM_CHSCCDR_IPU1_DI1_CLK_SEL_OFFSET);
	writel(reg, &mxc_ccm->chsccdr);

	reg = IOMUXC_GPR2_BGREF_RRMODE_EXTERNAL_RES
	     | IOMUXC_GPR2_DI1_VS_POLARITY_ACTIVE_LOW
	     | IOMUXC_GPR2_DI0_VS_POLARITY_ACTIVE_LOW
	     | IOMUXC_GPR2_BIT_MAPPING_CH1_SPWG
	     | IOMUXC_GPR2_DATA_WIDTH_CH1_18BIT
	     | IOMUXC_GPR2_BIT_MAPPING_CH0_SPWG
	     | IOMUXC_GPR2_DATA_WIDTH_CH0_18BIT
	     | IOMUXC_GPR2_LVDS_CH0_MODE_DISABLED
	     | IOMUXC_GPR2_LVDS_CH1_MODE_ENABLED_DI0;
	writel(reg, &iomux->gpr[2]);

	reg = readl(&iomux->gpr[3]);
	reg = (reg & ~(IOMUXC_GPR3_LVDS1_MUX_CTL_MASK
			| IOMUXC_GPR3_HDMI_MUX_CTL_MASK))
	    | (IOMUXC_GPR3_MUX_SRC_IPU1_DI0
	       << IOMUXC_GPR3_LVDS1_MUX_CTL_OFFSET);
	writel(reg, &iomux->gpr[3]);
}
#endif /* CONFIG_VIDEO_IPUV3 */

#ifdef CONFIG_UBOOT_LOGO_ENABLE
#ifdef IPU_OUTPUT_MODE_LCD
static void ipu_iomux_config(void)
{
	iomux_v3_cfg_t display_pads[] = {
		MX6_PAD_DI0_DISP_CLK__IPU1_DI0_DISP_CLK | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DI0_PIN15__IPU1_DI0_PIN15 | MUX_PAD_CTRL(NO_PAD_CTRL), 	/* DE */
		MX6_PAD_DI0_PIN2__IPU1_DI0_PIN02 | MUX_PAD_CTRL(NO_PAD_CTRL),		/* HSync */
		MX6_PAD_DI0_PIN3__IPU1_DI0_PIN03 | MUX_PAD_CTRL(NO_PAD_CTRL),		/* VSync */

		MX6_PAD_DISP0_DAT0__IPU1_DISP0_DATA00 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT1__IPU1_DISP0_DATA01 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT2__IPU1_DISP0_DATA02 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT3__IPU1_DISP0_DATA03 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT4__IPU1_DISP0_DATA04 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT5__IPU1_DISP0_DATA05 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT6__IPU1_DISP0_DATA06 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT7__IPU1_DISP0_DATA07 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT8__IPU1_DISP0_DATA08 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT9__IPU1_DISP0_DATA09 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT10__IPU1_DISP0_DATA10 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT11__IPU1_DISP0_DATA11 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT12__IPU1_DISP0_DATA12 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT13__IPU1_DISP0_DATA13 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT14__IPU1_DISP0_DATA14 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT15__IPU1_DISP0_DATA15 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT16__IPU1_DISP0_DATA16 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT17__IPU1_DISP0_DATA17 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT18__IPU1_DISP0_DATA18 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT19__IPU1_DISP0_DATA19 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT20__IPU1_DISP0_DATA20 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT21__IPU1_DISP0_DATA21 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT22__IPU1_DISP0_DATA22 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_DISP0_DAT23__IPU1_DISP0_DATA23 | MUX_PAD_CTRL(NO_PAD_CTRL),
	};

	imx_iomux_v3_setup_multiple_pads(display_pads,
			ARRAY_SIZE(display_pads));
}
#endif

#if 0 // from QNX
void mx6q_init_lcd_panel(void)
{
	/* IPU1 Display Interface 0 clock */
	pinmux_set_swmux(SWMUX_DI0_DISP_CLK, MUX_CTL_MUX_MODE_ALT0);

	/* LCD EN */
	pinmux_set_swmux(SWMUX_DI0_PIN15, MUX_CTL_MUX_MODE_ALT0);

	/* LCD HSYNC */
	pinmux_set_swmux(SWMUX_DI0_PIN2, MUX_CTL_MUX_MODE_ALT0);

	/* LCD VSYNC */
	pinmux_set_swmux(SWMUX_DI0_PIN3, MUX_CTL_MUX_MODE_ALT0);

	/* Data Lines */
	pinmux_set_swmux(SWMUX_DISP0_DAT0, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT1, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT2, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT3, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT4, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT5, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT6, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT7, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT8, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT9, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT10, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT11, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT12, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT13, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT14, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT15, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT16, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT17, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT18, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT19, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT20, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT21, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT22, MUX_CTL_MUX_MODE_ALT0);
	pinmux_set_swmux(SWMUX_DISP0_DAT23, MUX_CTL_MUX_MODE_ALT0);

	/* Configure pin as GPIO1_30 (Power Enable)
	 * Force LCD_EN (ENET_TXD0) HIGH to enable LCD */
	pinmux_set_swmux(SWMUX_ENET_TXD0, MUX_CTL_MUX_MODE_ALT5);
	mx6x_set_gpio_output(MX6X_GPIO1_BASE, 30, GPIO_HIGH);

	/* Note: the touchscreen is configured by [mx6q_init_sensors] */
}
#endif

#if 0 // FROM QNX
void mx6q_init_lvds(void)
{
	kprintf("enter %s\n", __FUNCTION__);

	/* Enable PWM - Used to configure backlight brightness (GPIO1[18]) */
	pinmux_set_swmux(SWMUX_SD1_CMD, MUX_CTL_MUX_MODE_ALT5);

	/*
	 * Configure GPIO controlling PWM as an output and drive the GPIO high.  In this case PWM is always high, meaning
	 * a 100% duty cycle, a lower duty cycle could be used to decrease the brightness of the display.
	 */
	mx6x_set_gpio_output(MX6X_GPIO1_BASE, 18, GPIO_HIGH);




	pinmux_set_swmux(SWMUX_EIM_A17, MUX_CTL_MUX_MODE_ALT5);
	mx6x_set_gpio_output(MX6X_GPIO2_BASE, 21, GPIO_HIGH);
	uint32_t value = 0;
	kprintf("gpio2.21: %d\n", value);

	mx6x_get_gpio_value(MX6X_GPIO2_BASE, 21, &value);
	kprintf("gpio2.21: %d\n", value);

	uint32_t lcdblock = 0;
	lcdblock = in32(MX6X_IOMUXC_BASE + MX6X_IOMUX_GPR2);
	kprintf("MX6X_IOMUXC_GPR2 (LCD BLOCK): %d\n", lcdblock);
	//lcdblock = lcdblock & 0xFFFFFF9F ^ 0x60;
	lcdblock = lcdblock & 0xFFFFFF9F ^ 0x20;
	kprintf("MX6X_IOMUXC_GPR2 (LCD BLOCK): %d\n", lcdblock);
	out32(MX6X_IOMUXC_BASE + MX6X_IOMUX_GPR2, lcdblock);

}

#endif

//int imx_pwm_config(struct pwm_device pwm, int duty_ns, int period_ns)

#ifdef IPU_OUTPUT_MODE_LVDS
	struct pwm_device pwm = {
		.pwm_id = 0,
		.pwmo_invert = 0,
	};

static void setup_lvds_iomux(void)
{
	imx_pwm_disable(pwm);
	debug("setup_lvds_iomux\n");

	/* DISP0_PWR_EN */
#if defined(CONFIG_SYS_JYD)
	imx_iomux_v3_setup_pad(MX6_PAD_ENET_TXD0__GPIO1_IO30 | MUX_PAD_CTRL(NO_PAD_CTRL));
	gpio_direction_output(IMX_GPIO_NR(1, 30), 1);
#endif

#if defined(CONFIG_SYS_HEZHONG)
	imx_iomux_v3_setup_pad(MX6_PAD_GPIO_17__GPIO7_IO12 | MUX_PAD_CTRL(NO_PAD_CTRL));
	gpio_direction_output(IMX_GPIO_NR(7, 12), 1);
#endif

	/* GPIO backlight */
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_DAT3__PWM1_OUT | MUX_PAD_CTRL(NO_PAD_CTRL));
	/* LVDS panel CABC_EN */
	imx_iomux_v3_setup_pad(MX6_PAD_NANDF_CS2__GPIO6_IO15 | MUX_PAD_CTRL(NO_PAD_CTRL));
	imx_iomux_v3_setup_pad(MX6_PAD_NANDF_CS3__GPIO6_IO16 | MUX_PAD_CTRL(NO_PAD_CTRL));

	/*
	 * Set LVDS panel CABC_EN to low to disable
	 * CABC function. This function will turn backlight
	 * automatically according to display content, so
	 * simply disable it to get rid of annoying unstable
	 * backlight phenomena.
	 */
	gpio_direction_output(IMX_GPIO_NR(6, 15), 0);
	gpio_direction_output(IMX_GPIO_NR(6, 16), 0);

	//gpio_direction_output(IMX_GPIO_NR(6, 15), 1);
	//gpio_direction_output(IMX_GPIO_NR(6, 16), 1);

	imx_pwm_config(pwm, 25000, 50000);
	imx_pwm_enable(pwm);

	// LCD_RST
	imx_iomux_v3_setup_pad(MX6_PAD_EIM_A17__GPIO2_IO21  | MUX_PAD_CTRL(NO_PAD_CTRL));
	gpio_direction_output(IMX_GPIO_NR(2, 21), 1);
	uint32_t value = 0;
	debug("gpio2.21: %d\n", value);

	value = gpio_get_value(IMX_GPIO_NR(2, 21));
	debug("gpio2.21: %d\n", value);

#if 0
	//
	struct iomuxc *iomux = (struct iomuxc *)IOMUXC_BASE_ADDR;//IOMUXC_GPR_BASE_ADDR;
	uint32_t lcdblock = 0;
	lcdblock = __raw_readl(&iomux->grp[2]);
	printf("MX6X_IOMUXC_GPR2 (LCD BLOCK): %d\n", lcdblock);
	//lcdblock = lcdblock & 0xFFFFFF9F ^ 0x60;
	lcdblock = lcdblock & 0xFFFFFF9F ^ 0x20;
	printf("MX6X_IOMUXC_GPR2 (LCD BLOCK): %d\n", lcdblock);
	__raw_writel(&iomux->grp[2], lcdblock);
#endif


}
#endif

#ifdef IPU_OUTPUT_MODE_LCD
static void setup_lcd_iomux(void)
{
	unsigned int reg;
	struct pwm_device pwm = {
		.pwm_id = 0,
		.pwmo_invert = 0,
	};

	imx_pwm_config(pwm, 25000, 50000);
	imx_pwm_enable(pwm);

	/* LCD Power */
	imx_iomux_v3_setup_pad(MX6_PAD_ENET_TXD0__GPIO1_IO30 | MUX_PAD_CTRL(NO_PAD_CTRL));
	/* LCD reset */
	imx_iomux_v3_setup_pad(MX6_PAD_EIM_DA8__GPIO3_IO08 | MUX_PAD_CTRL(NO_PAD_CTRL));
	/* Backlight */
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_DAT3__PWM1_OUT | MUX_PAD_CTRL(NO_PAD_CTRL));

	/* Set LCD Power to high. */
	gpio_direction_output(IMX_GPIO_NR(1, 30), 1);

	/* Set LCD reset to high. */
	gpio_direction_output(IMX_GPIO_NR(3, 8), 1);
}
#endif

#ifdef IPU_OUTPUT_MODE_HDMI
static void setup_hdmi_iomux(void)
{
	iomux_v3_cfg_t hdmi_i2c_pads[] = {
		MX6_PAD_KEY_COL3__HDMI_TX_DDC_SCL | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_KEY_ROW3__HDMI_TX_DDC_SDA | MUX_PAD_CTRL(NO_PAD_CTRL),
	};

	imx_iomux_v3_setup_multiple_pads(hdmi_i2c_pads,
			ARRAY_SIZE(hdmi_i2c_pads));
}
#endif

#endif  //CONFIG_UBOOT_LOGO_ENABLE

#ifdef CONFIG_UBOOT_LOGO_ENABLE

static void display_dummy(void)
{
	unsigned char * pData;
	pData = (unsigned char *)CONFIG_FB_BASE;
	memset(pData, 0, DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_BPP / 8));

#ifndef CONFIG_SYS_DCACHE_OFF
	//printf("board_late_init(flush_dcache_range)\n");
	flush_dcache_range((u32)pData, (u32)(pData + DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_BPP / 8)));
#endif


}

static void display_custmor_logo(void)
{
	unsigned char * pData;
	int bmpReady = 0;
	
	//printf("enter board_late_init\n");
	pData = (unsigned char *)CONFIG_FB_BASE;

//	bmpReady = load_image_from_mmc(pData);
	bmpReady = load_image_from_fat(pData);
	if (bmpReady == 0) {
		printf("not bitmap in sdcard\r\n");
		//print_logo_info();
		//printf("bmp_logo_bitmap addr %d \r\n", bmp_logo_bitmap);
		createColorBar(pData);
		//memcpy(pData, bmp_logo_bitmap, 1000);

	}

#ifndef CONFIG_SYS_DCACHE_OFF
	//printf("board_late_init(flush_dcache_range)\n");
	flush_dcache_range((u32)pData, (u32)(pData + DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_BPP / 8)));
#endif

}


void display_logo(void)
{
	display_dummy();

#ifdef IPU_OUTPUT_MODE_LVDS
	//printf("board_late_init(setup_lvds_iomux)\n");
	imx_pwm_enable(pwm);
	setup_lvds_iomux();
#endif

#ifdef IPU_OUTPUT_MODE_LCD
	//printf("board_late_init(setup_lcd_iomux)\n");
	ipu_iomux_config();
	setup_lcd_iomux();
#endif

#ifdef IPU_OUTPUT_MODE_HDMI
	printf("board_late_init(setup_hdmi_iomux)\n");
	setup_hdmi_iomux();
#endif

	//printf("board_late_init(ipu_display_setup)\n");
	ipu_display_setup();

//	display_dummy();
	display_custmor_logo();
}

#endif