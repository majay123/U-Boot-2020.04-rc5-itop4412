/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuration and settings for the TOPEET iTOP4412 development board.
 */

#ifndef __CONFIG_ITOP4412_H
#define __CONFIG_ITOP4412_H

#include <configs/exynos4-common.h>

/* iTOP4412 has 4 bank of DRAM */
#define CONFIG_SYS_SDRAM_BASE		0x40000000
#define PHYS_SDRAM_1			CONFIG_SYS_SDRAM_BASE
#define SDRAM_BANK_SIZE			(256 << 20)	/* 256 MiB */

/* memtest works on */
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0x6000000)
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x3E00000)
#define CONFIG_SYS_INIT_SP_ADDR	        (CONFIG_SYS_LOAD_ADDR - 0x1000000)

#define CONFIG_MACH_TYPE		MACH_TYPE_ITOP4412

/* select serial console configuration */
#define CONFIG_BAUDRATE 115200

/* Console configuration */
#define CONFIG_DEFAULT_CONSOLE		"ttySAC2,115200n8"

#define CONFIG_SYS_MEM_TOP_HIDE	(1 << 20)	/* ram console */

#define CONFIG_SYS_MONITOR_BASE	0x00000000

/* Power Down Modes */
#define S5P_CHECK_SLEEP			0x00000BAD
#define S5P_CHECK_DIDLE			0xBAD00000
#define S5P_CHECK_LPA			0xABAD0000

/* SPL configuration */
#define CONFIG_SPL_MAX_FOOTPRINT        (14 << 10)
#define CONFIG_SPL_STACK                0x02060000

#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x40007000\0" \
	"rdaddr=0x48000000\0" \
	"kerneladdr=0x40007000\0" \
	"ramdiskaddr=0x48000000\0" \
	"console=" CONFIG_DEFAULT_CONSOLE "\0"\
	"mmcdev=0\0" \
	"bootenv=uEnv.txt\0" \
	"loadbootenv=load mmc ${mmcdev} ${loadaddr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from mmc ...; " \
		"env import -t $loadaddr $filesize\0" \
        "loadbootscript=load mmc ${mmcdev} ${loadaddr} boot.scr\0" \
        "bootscript=echo Running bootscript from mmc${mmcdev} ...; " \
                "source ${loadaddr}\0"

#define CONFIG_BOOTCOMMAND \
	"if mmc rescan; then " \
		"echo SD/MMC found on device ${mmcdev};" \
		"if run loadbootenv; then " \
			"echo Loaded environment from ${bootenv};" \
			"run importbootenv;" \
		"fi;" \
		"if test -n $uenvcmd; then " \
			"echo Running uenvcmd ...;" \
			"run uenvcmd;" \
		"fi;" \
		"if run loadbootscript; then " \
			"run bootscript; " \
		"fi; " \
	"fi;" \
	"load mmc ${mmcdev} ${loadaddr} uImage; bootm ${loadaddr} "

#define CONFIG_CLK_1000_400_200

#define CONFIG_SYS_MMC_ENV_DEV	0
#define CONFIG_ENV_SIZE		(16 << 10)	/* 16 KB */
#define RESERVE_BLOCK_SIZE	(512)
#define FWBL_SIZE       (8 << 10)  /* 8 KiB reserved for fwbl.bin*/
#define SPL_SZIE        (16 << 10)  /* 16KiB reserved for u-boot-spl.bin */
#define UBOOT_SIZE      (0x80000)   /* 512 KiB reserved for u-boot.bin*/
#define TZSW_SIZE       (0x27000)   /* 156 KiB */
#define BL2_START_OFFSET      ((RESERVE_BLOCK_SIZE + FWBL_SIZE \
                                        + SPL_SZIE)/512)
#define BL2_SIZE_BLOC_COUNT   (UBOOT_SIZE/512)
#define TZSW_START_OFFSET       (BL2_START_OFFSET \
                                        + BL2_SIZE_BLOC_COUNT)
#define TZSW_SIZE_BLOC_COUNT    (TZSW_SIZE/512)
#define CONFIG_ENV_OFFSET	(RESERVE_BLOCK_SIZE + FWBL_SIZE \
                                        + SPL_SZIE + UBOOT_SIZE \
                                        + TZSW_SIZE)

#endif	/* __CONFIG_H */
