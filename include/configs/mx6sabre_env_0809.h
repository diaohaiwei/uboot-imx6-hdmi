#ifdef CONFIG_SYS_BOOT_NAND
#define CONFIG_MFG_NAND_PARTITION "mtdparts=gpmi-nand:64m(boot),16m(kernel),16m(dtb),-(rootfs) "
#else
#define CONFIG_MFG_NAND_PARTITION ""
#endif


#define CONFIG_MFG_ENV_SETTINGS \
	"mfgtool_args=setenv bootargs console=" CONFIG_CONSOLE_DEV ",115200 " \
		"rdinit=/linuxrc " \
		"g_mass_storage.stall=0 g_mass_storage.removable=1 " \
		"g_mass_storage.idVendor=0x066F g_mass_storage.idProduct=0x37FF "\
		"g_mass_storage.iSerialNumber=\"\" "\
		"enable_wait_mode=off "\
		CONFIG_MFG_NAND_PARTITION \
		"\0" \
		"initrd_addr=0x12C00000\0" \
		"initrd_high=0xffffffff\0" \
		"bootcmd_mfg=run mfgtool_args;bootz ${loadaddr} ${initrd_addr} ${fdt_addr};\0" \


#if defined(CONFIG_SYS_BOOT_NAND)
	/*
	 * The dts also enables the WEIN NOR which is mtd0.
	 * So the partions' layout for NAND is:
	 *     mtd1: 16M      (uboot)
	 *     mtd2: 16M      (kernel)
	 *     mtd3: 16M      (dtb)
	 *     mtd4: left     (rootfs)
	 */
#define CONFIG_EXTRA_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS \
	"fdt_addr=0x18000000\0" \
	"fdt_high=0xffffffff\0"	  \
	"bootargs=console=" CONFIG_CONSOLE_DEV ",115200 ubi.mtd=4 "  \
		"root=ubi0:rootfs rootfstype=ubifs "		     \
		"mtdparts=gpmi-nand:64m(boot),16m(kernel),16m(dtb),-(rootfs)\0"\
	"bootcmd=nand read ${loadaddr} 0x4000000 0x800000;"\
		"nand read ${fdt_addr} 0x5000000 0x100000;"\
		"bootz ${loadaddr} - ${fdt_addr}\0"

#elif defined(CONFIG_SYS_BOOT_SATA)

#define CONFIG_EXTRA_ENV_SETTINGS \
		CONFIG_MFG_ENV_SETTINGS \
		"fdt_addr=0x18000000\0" \
		"fdt_high=0xffffffff\0"   \
		"bootargs=console=" CONFIG_CONSOLE_DEV ",115200 \0"\
		"bootargs_sata=setenv bootargs ${bootargs} " \
			"root=/dev/sda1 rootwait rw \0" \
		"bootcmd_sata=run bootargs_sata; sata init; " \
			"sata read ${loadaddr} 0x800  0x4000; " \
			"sata read ${fdt_addr} 0x8000 0x800; " \
			"bootz ${loadaddr} - ${fdt_addr} \0" \
		"bootcmd=run bootcmd_sata \0"

#elif defined(CONFIG_SYS_JYD)

#define CONFIG_UPGRADE_SETTINGS \
		"initrd_high=0xffffffff\0" \

#define CONFIG_EXTRA_ENV_SETTINGS \
	"image=zImage\0" \
	"uptimg=upgrade.image\0" \
	"uptfdt=upgrade.dtb\0" \
	"fdt_file=" CONFIG_DEFAULT_FDT_FILE "\0" \
	"fdt_addr=0x18000000\0" \
	"boot_fdt=try\0" \
	"phase_file=bootflag\0" \
	"phase_addr=0x12B00000\0" \
	"ethaddr=08:00:3e:26:0a:5b\0" \
	"ipaddr=192.168.2.33\0" \
	"serverip=192.168.2.99\0" \
	"console=" CONFIG_CONSOLE_DEV "\0" \
	"fdt_high=0xffffffff\0"	  \
	"initrd=upgrade.cpio.gz.u-boot\0" \
	"initrd_addr=0x12C00000\0" \
	"initrd_high=0xffffffff\0" \
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" \
	"mmcpart=" __stringify(CONFIG_SYS_MMC_IMG_LOAD_PART) "\0" \
	"mmcroot=" CONFIG_MMCROOT " rootwait rw\0" \
	"mmcautodetect=yes\0" \
	"smp=" CONFIG_SYS_NOSMP "\0"\
	"mmcargs_standard=setenv bootargs console=${console},${baudrate} ${smp} " \
		"initcall_debug " \
		"lpj=7905280 " \
		"video=mxcfb0:dev=ldb,1920x720M@60,if=RGB24,bpp=24 video=mxcfb1:off video=mxcfb2:off video=mxcfb3:off " \
		"root=${mmcroot}\0" \
	"mmcargs_lvds=setenv bootargs console=${console},${baudrate} ${smp} " \
		"lpj=7905280 " \
		"video=mxcfb0:dev=ldb,1920x720M@60,if=RGB24,bpp=24 " \
		"video=mxcfb1:off " \
		"video=mxcfb2:off " \
		"video=mxcfb3:off " \
		"root=${mmcroot}\0" \
	"mmcargs_lvds_low=setenv bootargs console=${console},${baudrate} ${smp} " \
		"lpj=7905280 " \
		"video=mxcfb0:dev=ldb,1440x540M@60,if=RGB24,bpp=32 " \
		"video=mxcfb1:off " \
		"video=mxcfb2:off " \
		"video=mxcfb3:off " \
		"root=${mmcroot}\0" \
	"mmcargs_hdmi=setenv bootargs console=${console},${baudrate} ${smp} " \
		"lpj=7905280 " \
		"video=mxcfb0:dev=hdmi,1920x1080M@60,if=RGB24,bpp=32 " \
		"video=mxcfb1:off " \
		"video=mxcfb2:off " \
		"video=mxcfb3:off " \
		"root=${mmcroot}\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} ${smp} " \
		"lpj=7905280 earlyprintk " \
		"video=mxcfb0:dev=hdmi,1920x1080M@60,if=RGB24,bpp=32 " \
		"video=mxcfb1:dev=ldb,1440x540M@60,if=RGB24,bpp=32,ldb=sin1 " \
		"video=mxcfb2:off " \
		"video=mxcfb3:off " \
		"root=${mmcroot}\0" \
	"upgrade_args=setenv bootargs console=tty0,115200 ${smp} " \
		"rdinit=/linuxrc " \
		"enable_wait_mode=off\0" \
	"loadinitrd=fatload mmc ${mmcdev}:${mmcpart} ${initrd_addr} ${initrd}\0" \
	"loaduptimg=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${uptimg}\0" \
	"loaduptfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${uptfdt}\0" \
	"loadphase=fatload mmc ${mmcdev}:${mmcpart} ${phase_addr} ${phase_file}\0" \
	"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"mmcupgrade=" \
		"run upgrade_args; " \
		"if run loaduptfdt; then " \
			"if run loadinitrd; then " \
				"bootz ${loadaddr} ${initrd_addr} ${fdt_addr}; " \
			"else " \
				"echo failed to load initrd;" \
			"fi;" \
		"else " \
			"echo failed to load fdt;" \
		"fi;\0" \
	"mmcboot=" \
		"run mmcargs; " \
		"if run loadfdt; then " \
			"bootz ${loadaddr} - ${fdt_addr}; " \
		"else " \
			"if test ${boot_fdt} = try; then " \
				"bootz; " \
			"else " \
				"echo WARN: Cannot load the DT; " \
			"fi; " \
		"fi; " \

#define CONFIG_BOOTCOMMAND \
	"mmc dev ${mmcdev};" \
	"if mmc rescan; then " \
	    "if run loadphase; then " \
		"if checkupt ${phase_addr}; then " \
			"if run loaduptimg; then " \
				"run mmcupgrade; " \
			"else run netboot; " \
			"fi; " \
		"else " \
			"if run loadimage; then " \
				"run mmcboot; " \
			"else run netboot; " \
			"fi; " \
		"fi;" \
	     "else " \
			"echo WARNING: fail to run loadphase;" \
			"if run loadimage; then " \
				"run mmcboot; " \
			"else run netboot; " \
			"fi; " \
	     "fi;" \
	"else " \
		"run netboot; " \
	"fi"
#elif defined(CONFIG_SYS_HEZHONG)

#define CONFIG_UPGRADE_SETTINGS \
		"initrd_high=0xffffffff\0" \

#define CONFIG_EXTRA_ENV_SETTINGS \
	"image=zImage\0" \
	"uptimg=upgrade.image\0" \
	"uptfdt=upgrade.dtb\0" \
	"fdt_file=" CONFIG_DEFAULT_FDT_FILE "\0" \
	"fdt_addr=0x18000000\0" \
	"boot_fdt=try\0" \
	"phase_file=bootflag\0" \
	"phase_addr=0x12B00000\0" \
	"ethaddr=08:00:3e:26:0a:5b\0" \
	"ipaddr=192.168.2.33\0" \
	"serverip=192.168.2.99\0" \
	"console=" CONFIG_CONSOLE_DEV "\0" \
	"fdt_high=0xffffffff\0"	  \
	"initrd=upgrade.cpio.gz.u-boot\0" \
	"initrd_addr=0x12C00000\0" \
	"initrd_high=0xffffffff\0" \
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" \
	"mmcpart=" __stringify(CONFIG_SYS_MMC_IMG_LOAD_PART) "\0" \
	"mmcroot=" CONFIG_MMCROOT " rootwait rw\0" \
	"mmcautodetect=yes\0" \
	"smp=" CONFIG_SYS_NOSMP "\0"\
	"mmcargs_standard=setenv bootargs console=${console},${baudrate} ${smp} " \
		"initcall_debug " \
		"lpj=7905280 " \
		"video=mxcfb0:dev=ldb,1920x720M@60,if=RGB24,bpp=24 video=mxcfb1:off video=mxcfb2:off video=mxcfb3:off " \
		"root=${mmcroot}\0" \
	"mmcargs_lvds=setenv bootargs console=${console},${baudrate} ${smp} " \
		"lpj=7905280 " \
		"video=mxcfb0:dev=ldb,1920x720M@60,if=RGB24,bpp=24 " \
		"video=mxcfb1:off " \
		"video=mxcfb2:off " \
		"video=mxcfb3:off " \
		"root=${mmcroot}\0" \
	"mmcargs_lvds_high=setenv bootargs console=${console},${baudrate} ${smp} " \
		"lpj=7905280 " \
		"video=mxcfb0:dev=ldb,1920x1200M@60,if=RGB24,bpp=24 " \
		"video=mxcfb1:off " \
		"video=mxcfb2:off " \
		"video=mxcfb3:off " \
		"root=${mmcroot}\0" \
	"mmcargs_lvds_low=setenv bootargs console=${console},${baudrate} ${smp} " \
		"lpj=7905280 " \
		"video=mxcfb0:dev=ldb,1440x540M@60,if=RGB24,bpp=32 " \
		"video=mxcfb1:off " \
		"video=mxcfb2:off " \
		"video=mxcfb3:off " \
		"root=${mmcroot}\0" \
	"mmcargs_hdmi=setenv bootargs console=${console},${baudrate} ${smp} " \
		"lpj=7905280 " \
		"video=mxcfb0:dev=hdmi,1920x1080M@60,if=RGB24,bpp=32 " \
		"video=mxcfb1:off " \
		"video=mxcfb2:off " \
		"video=mxcfb3:off " \
		"root=${mmcroot}\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} ${smp} " \
		"lpj=7905280 earlyprintk " \
		"video=mxcfb0:dev=hdmi,1920x1080M@60,if=RGB24,bpp=32 " \
		"video=mxcfb1:dev=ldb,1440x540M@60,if=RGB24,bpp=32,ldb=sin1 " \
		"video=mxcfb2:off " \
		"video=mxcfb3:off " \
		"root=${mmcroot}\0" \
	"upgrade_args=setenv bootargs console=tty0,115200 ${smp} " \
		"rdinit=/linuxrc " \
		"enable_wait_mode=off\0" \
	"loadinitrd=fatload mmc ${mmcdev}:${mmcpart} ${initrd_addr} ${initrd}\0" \
	"loaduptimg=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${uptimg}\0" \
	"loaduptfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${uptfdt}\0" \
	"loadphase=fatload mmc ${mmcdev}:${mmcpart} ${phase_addr} ${phase_file}\0" \
	"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"mmcupgrade=" \
		"run upgrade_args; " \
		"if run loaduptfdt; then " \
			"if run loadinitrd; then " \
				"bootz ${loadaddr} ${initrd_addr} ${fdt_addr}; " \
			"else " \
				"echo failed to load initrd;" \
			"fi;" \
		"else " \
			"echo failed to load fdt;" \
		"fi;\0" \
	"mmcboot=" \
		"run mmcargs_lvds_high; " \
		"if run loadfdt; then " \
			"bootz ${loadaddr} - ${fdt_addr}; " \
		"else " \
			"if test ${boot_fdt} = try; then " \
				"bootz; " \
			"else " \
				"echo WARN: Cannot load the DT; " \
			"fi; " \
		"fi; " \

#define CONFIG_BOOTCOMMAND \
	"mmc dev ${mmcdev};" \
	"if mmc rescan; then " \
	    "if run loadphase; then " \
		"if checkupt ${phase_addr}; then " \
			"if run loaduptimg; then " \
				"run mmcupgrade; " \
			"else run netboot; " \
			"fi; " \
		"else " \
			"if run loadimage; then " \
				"run mmcboot; " \
			"else run netboot; " \
			"fi; " \
		"fi;" \
	     "else " \
			"echo WARNING: fail to run loadphase;" \
			"if run loadimage; then " \
				"run mmcboot; " \
			"else run netboot; " \
			"fi; " \
	     "fi;" \
	"else " \
		"run netboot; " \
	"fi"


#else
#define CONFIG_EXTRA_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS \
	"script=boot.scr\0" \
	"image=zImage\0" \
	"fdt_file=" CONFIG_DEFAULT_FDT_FILE "\0" \
	"fdt_addr=0x18000000\0" \
	"boot_fdt=try\0" \
	"ip_dyn=yes\0" \
	"ethaddr=08:00:3e:26:0a:5b\0" \
	"ipaddr=192.168.2.33\0" \
	"serverip=192.168.2.99\0" \
	"console=" CONFIG_CONSOLE_DEV "\0" \
	"fdt_high=0xffffffff\0"	  \
	"initrd_high=0xffffffff\0" \
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" \
	"mmcpart=" __stringify(CONFIG_SYS_MMC_IMG_LOAD_PART) "\0" \
	"mmcroot=" CONFIG_MMCROOT " rootwait rw\0" \
	"mmcautodetect=yes\0" \
	"update_sd_firmware=" \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"if mmc dev ${mmcdev}; then "	\
			"if ${get_cmd} ${update_sd_firmware_filename}; then " \
				"setexpr fw_sz ${filesize} / 0x200; " \
				"setexpr fw_sz ${fw_sz} + 1; "	\
				"mmc write ${loadaddr} 0x2 ${fw_sz}; " \
			"fi; "	\
		"fi\0" \
	"smp=" CONFIG_SYS_NOSMP "\0"\
	"mmcargs=setenv bootargs console=${console},${baudrate} ${smp} " \
		"initcall_debug " \
		"lpj=7905280 " \
		"root=${mmcroot}\0" \
	"loadbootscript=" \
		"fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${script};\0" \
	"bootscript=echo Running bootscript from mmc ...; " \
		"source\0" \
	"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"mmcboot=echo E-planet Booting from mmc ...; " \
		"echo ${mmcargs};" \
		"run mmcargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"netargs=setenv bootargs console=${console},${baudrate} ${smp} " \
		"root=/dev/nfs " \
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
	"netboot=echo Booting from net ...; " \
		"run netargs; " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${image}; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if ${get_cmd} ${fdt_addr} ${fdt_file}; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0"


#define CONFIG_BOOTCOMMAND \
	"mmc dev ${mmcdev};" \
	"if mmc rescan; then " \
		"if run loadbootscript; then " \
		"run bootscript; " \
		"else " \
			"if run loadimage; then " \
				"run mmcboot; " \
			"else run netboot; " \
			"fi; " \
		"fi; " \
	"else run netboot; fi"
#endif

