/*
 * Clock Initialization for board based on EXYNOS4210
 *
 * Copyright (C) 2013 Samsung Electronics
 * Rajeshwari Shinde <rajeshwari.s@samsung.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <config.h>
#include <asm/io.h>
#include <asm/arch/cpu.h>
#include <asm/arch/clk.h>
#include <asm/arch/clock.h>
#include "common_setup.h"
#ifdef CONFIG_TARGET_ITOP4412
#include "exynos4412_setup.h"
#else
#include "exynos4_setup.h"
#endif

/*
 * system_clock_init: Initialize core clock and bus clock.
 * void system_clock_init(void)
 */
#ifndef CONFIG_TARGET_ITOP4412
void system_clock_init(void)
{
	struct exynos4_clock *clk =
			(struct exynos4_clock *)samsung_get_base_clock();

	writel(CLK_SRC_CPU_VAL, &clk->src_cpu);

	sdelay(0x10000);

	writel(CLK_SRC_TOP0_VAL, &clk->src_top0);
	writel(CLK_SRC_TOP1_VAL, &clk->src_top1);
	writel(CLK_SRC_DMC_VAL, &clk->src_dmc);
	writel(CLK_SRC_LEFTBUS_VAL, &clk->src_leftbus);
	writel(CLK_SRC_RIGHTBUS_VAL, &clk->src_rightbus);
	writel(CLK_SRC_FSYS_VAL, &clk->src_fsys);
	writel(CLK_SRC_PERIL0_VAL, &clk->src_peril0);
	writel(CLK_SRC_CAM_VAL, &clk->src_cam);
	writel(CLK_SRC_MFC_VAL, &clk->src_mfc);
	writel(CLK_SRC_G3D_VAL, &clk->src_g3d);
	writel(CLK_SRC_LCD0_VAL, &clk->src_lcd0);

	sdelay(0x10000);

	writel(CLK_DIV_CPU0_VAL, &clk->div_cpu0);
	writel(CLK_DIV_CPU1_VAL, &clk->div_cpu1);
	writel(CLK_DIV_DMC0_VAL, &clk->div_dmc0);
	writel(CLK_DIV_DMC1_VAL, &clk->div_dmc1);
	writel(CLK_DIV_LEFTBUS_VAL, &clk->div_leftbus);
	writel(CLK_DIV_RIGHTBUS_VAL, &clk->div_rightbus);
	writel(CLK_DIV_TOP_VAL, &clk->div_top);
	writel(CLK_DIV_FSYS1_VAL, &clk->div_fsys1);
	writel(CLK_DIV_FSYS2_VAL, &clk->div_fsys2);
	writel(CLK_DIV_FSYS3_VAL, &clk->div_fsys3);
	writel(CLK_DIV_PERIL0_VAL, &clk->div_peril0);
	writel(CLK_DIV_CAM_VAL, &clk->div_cam);
	writel(CLK_DIV_MFC_VAL, &clk->div_mfc);
	writel(CLK_DIV_G3D_VAL, &clk->div_g3d);
	writel(CLK_DIV_LCD0_VAL, &clk->div_lcd0);

	/* Set PLL locktime */
	writel(PLL_LOCKTIME, &clk->apll_lock);
	writel(PLL_LOCKTIME, &clk->mpll_lock);
	writel(PLL_LOCKTIME, &clk->epll_lock);
	writel(PLL_LOCKTIME, &clk->vpll_lock);

	writel(APLL_CON1_VAL, &clk->apll_con1);
	writel(APLL_CON0_VAL, &clk->apll_con0);
	writel(MPLL_CON1_VAL, &clk->mpll_con1);
	writel(MPLL_CON0_VAL, &clk->mpll_con0);
	writel(EPLL_CON1_VAL, &clk->epll_con1);
	writel(EPLL_CON0_VAL, &clk->epll_con0);
	writel(VPLL_CON1_VAL, &clk->vpll_con1);
	writel(VPLL_CON0_VAL, &clk->vpll_con0);

	sdelay(0x30000);
}
#else /* !CONFIG_TARGET_ITOP4412 */
static inline void wait_mux_state(unsigned int status, void *addr)
{
    unsigned int res = 0;

    do {
        res = readl(addr);
    }while(status != res);
}

static inline void wait_pll_lock(void *addr)
{
    unsigned int res = 0;

    do {
        res = readl(addr);
        res >>= 29;
    }while(!(res & 0x1));
}

void system_clock_init(void)
{
    struct exynos4x12_clock *clk =
        (struct exynos4x12_clock *)samsung_get_base_clock();

    /*
     * bypass PLLs and reset clock source of cpu
     * MUX_MPLL_CTRL_USER_C:    select FINPLL:0
     * MUX_HPM:                 select MOUTAPLL:0
     * MUX_CORE:                select MOUTAPLL:0
     * MUX_APLL:                select FINPLL:0
     *
     */
    writel(CLK_SRC_CPU_RESET, &clk->src_cpu);
    wait_mux_state(CLK_MUX_STAT_CPU_RESET, &clk->mux_stat_cpu);

    /*
     * setup divisor of DMC clocks
     *
     * DIV_DMC0:
     * DOUTDMC(SCLK_DMC) = MOUTDMC_BUS / (DMC_RATIO + 1) : DMC_RATIO:1
     * DOUTDMCD(ACLK_DMCD) = DOUTDMC / (DMCD_RATIO + 1) : DMCD_RATIO:1
     * DOUTDMCP(ACLK_DMCP) = DOUTDMCD/ (DMCP_RATIO + 1) : DMCP_RATIO:1
     * DOUTACP(ACLK_ACP) = MOUTDMC_BUS / (ACP_RATIO + 1) : ACP_RATIO:3
     * DOUTACP_PCLK(PCLK_ACP) = DOUTACP / (ACP_PCLK_RATIO + 1) : ACP_PCLK_RATIO:1
     *
     * For example:
     * if MOUTDMC_BUS = 800MHz, then
     *      SCLK_DMC = 400MHz
     *      ACLK_DMCD = 200MHz
     *      ACLK_DMCP = 100MHz
     *      ACLK_ACP = 200MHz
     *      PCLK_ACP = 100MHz
     * fi
     */
    writel(CLK_DIV_DMC0_VAL, &clk->div_dmc0);
    writel(CLK_DIV_DMC1_VAL, &clk->div_dmc1);

	writel(CLK_SRC_TOP0_RESET, &clk->src_top0);
    wait_mux_state(CLK_MUX_STAT_TOP0_RESET, &clk->mux_stat_top0);
	writel(CLK_SRC_TOP1_RESET, &clk->src_top1);
    wait_mux_state(CLK_MUX_STAT_TOP1_RESET, &clk->mux_stat_top1);
    writel(CLK_DIV_TOP_VAL, &clk->div_top);

    writel(CLK_SRC_LEFTBUS_VAL, &clk->src_leftbus);
    wait_mux_state(CLK_MUX_STAT_LEFTBUS_VAL, &clk->mux_stat_leftbus);
    writel(CLK_DIV_LEFTBUS_VAL, &clk->div_leftbus);

    writel(CLK_SRC_RIGHTBUS_VAL, &clk->src_rightbus);
    wait_mux_state(CLK_MUX_STAT_RIGHTBUS_VAL, &clk->mux_stat_rightbus);
    writel(CLK_DIV_RIGHTBUS_VAL, &clk->div_rightbus);

    writel(APLL_LOCK_VAL, &clk->apll_lock);
    writel(MPLL_LOCK_VAL, &clk->mpll_lock);
    writel(EPLL_LOCK_VAL, &clk->epll_lock);
    writel(VPLL_LOCK_VAL, &clk->vpll_lock);

    /*
     * DIV_CPU0:
     * DOUTCORE = MOUTCORE/(CORE_RATIO + 1)
     * DOUTCORE2 = DOUTCORE/(CORE2_RATE + 1)
     * DOUTCOREM0 = DOUTCORE2/(COREM0_RATIO + 1)
     * DOUTCOREM1 = DOUTCORE2/(COREM1_RATIO + 1)
     * DOUTPERIPH = DOUTCORE2/(PERIPH_RATIO + 1)
     * SCLK_APLL = MOUTAPLL/(APLL_RATIO + 1)
     * DOUTATB = MOUTCORE/(ATB_RATIO + 1)
     * DOUTPCLK_DBG = DOUTATB/PCLK_DBG_RATIO + 1)
     *
     * ---------------------------------------
     *  MOUTAPLL        1000MHz     800MHz
     * ---------------------------------------
     *  ARMCLK          1000        8000
     *  ACLK_COREM0     250         200
     *  ACLK_CORES      250         200
     *  ACLK_COREM1     125         100
     *  PERIPHCLK       250         200
     *  ATCLK           250         200
     *  PCLK_DBG        125         100
     * --------------------------------------
     */
    writel(CLK_DIV_CPU0_VAL, &clk->div_cpu0);

    /*
     * DIV_CPU1:
     * DOUTCOPY = MOUTHPM/(COPY_RATIO + 1)
     * DOUTHPM = DOUTCOPY/(HPM_RATIO + 1)
     *
     * -------------------------------------
     *  MOUTHPM         1000MHz     800MHz
     * -------------------------------------
     *  DOUTCOPY        333         266
     *  SCLK_HPM        165         133
     * -------------------------------------
     */
    writel(CLK_DIV_CPU1_VAL, &clk->div_cpu1);

    /*
     * set APLL to 1000MHz or 800MHz
     */
    writel(APLL_CON1_VAL, &clk->apll_con1);
    writel(APLL_CON0_VAL, &clk->apll_con0);

    /*
     * always set MPLL to 800MHZ
     */
    writel(MPLL_CON1_VAL, &clk->mpll_con1);
    writel(MPLL_CON0_VAL, &clk->mpll_con0);

    /*
     * always set EPLL to 192MHz
     */
    writel(EPLL_CON2_VAL, &clk->epll_con2);
    writel(EPLL_CON1_VAL, &clk->epll_con1);
    writel(EPLL_CON0_VAL, &clk->epll_con0);

    /*
     * always set VPLL to 350MHz
     */
    writel(VPLL_CON2_VAL, &clk->vpll_con2);
    writel(VPLL_CON1_VAL, &clk->vpll_con1);
    writel(VPLL_CON0_VAL, &clk->vpll_con0);

    wait_pll_lock(&clk->apll_con0);
    wait_pll_lock(&clk->mpll_con0);
    wait_pll_lock(&clk->epll_con0);
    wait_pll_lock(&clk->vpll_con0);

    /*
     * select clock source for CPU_BLK
     * MUX_MPLL_CTRL_USER_C:    select SCLK_MPLL:1
     * MUX_HPM:                 select MOUTAPLL:0
     * MUX_CORE:                select MOUTAPLL:0
     * MUX_APLL:                select FOUTAPLL:1
     */
    writel(CLK_SRC_CPU_VAL, &clk->src_cpu);
    wait_mux_state(CLK_MUX_STAT_CPU_VAL, &clk->mux_stat_cpu);

    /*
     * select clock source for DMC_BLK
     *
     */
    writel(CLK_SRC_DMC_VAL, &clk->src_dmc);
    wait_mux_state(CLK_MUX_STAT_DMC_VAL, &clk->mux_stat_dmc);
    writel(CLK_SRC_TOP0_VAL, &clk->src_top0);
    wait_mux_state(CLK_MUX_STAT_TOP0_VAL, &clk->mux_stat_top0);
    writel(CLK_SRC_TOP1_VAL, &clk->src_top1);
    wait_mux_state(CLK_MUX_STAT_TOP1_VAL, &clk->mux_stat_top1);

    /* set UART clock for early debug
     *
     * UART0-3 = 100MHz
     *
     */
    writel(CLK_SRC_PERIL0_VAL, &clk->src_peril0);
    writel(CLK_DIV_PERIL0_VAL, &clk->div_peril0);

    /* MMC clock
     *
     * MMC2, MMC4 = 20MHz
     *
     */
    writel(CLK_SRC_FSYS_VAL, &clk->src_fsys);
    writel(CLK_DIV_FSYS2_VAL, &clk->div_fsys2);
    writel(CLK_DIV_FSYS3_VAL, &clk->div_fsys3);
}
#endif
