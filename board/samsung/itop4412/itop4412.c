// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/arch/cpu.h>
#include <asm/arch/mmc.h>
#include <asm/arch/periph.h>
#include <asm/arch/pinmux.h>
#include <usb.h>
#include <asm/arch/power.h>
#include <asm/arch/clock.h>
#include <dm.h>
#include <power/pmic.h>
#include <power/regulator.h>
#include <power/max77686_pmic.h>
// #include <error.h>
#include <mmc.h>
#include <usb/dwc2_udc.h>
#include <samsung/misc.h>



#ifndef CONFIG_SPL_BUILD
u32 get_board_rev(void)
{
	return 0;
}


static void board_gpio_init(void)
{
       ;
}

 int exynos_init(void)
 {
    board_gpio_init();

    /**
     * 配置USB驱动
     */
    gpio_request(EXYNOS4X12_GPIO_M33, "USB3503A Connect");
    gpio_request(EXYNOS4X12_GPIO_M24, "USB3503A Reset");
    // gpio_request(EXYNOS4X12_GPIO_C01, "DM9621");

    return 0;
}

#ifdef CONFIG_USB_GADGET
static int s5pc210_phy_control(int on)
{
    return 0;
}
 
struct dwc2_plat_otg_data s5pc210_otg_data = {
    .phy_control    = s5pc210_phy_control,
    .regs_phy       = EXYNOS4X12_USBPHY_BASE,
    .regs_otg       = EXYNOS4X12_USBOTG_BASE,
    .usb_phy_ctrl   = EXYNOS4X12_USBPHY_CONTROL,
    .usb_flags      = PHY0_SLEEP,
};
#endif

int board_usb_init(int index, enum usb_init_type init)
{

    debug("USB_udc_probe\n");
	return dwc2_udc_probe(&s5pc210_otg_data);
    // gpio_direction_output(EXYNOS4X12_GPIO_M33, 0);
    // gpio_direction_output(EXYNOS4X12_GPIO_C01, 0);
    // gpio_direction_output(EXYNOS4X12_GPIO_M24, 0);
    // gpio_direction_output(EXYNOS4X12_GPIO_M24, 1);
    // gpio_direction_output(EXYNOS4X12_GPIO_C01, 1);
    // gpio_direction_output(EXYNOS4X12_GPIO_M33, 1);
    return 0;
}

#ifdef CONFIG_BOARD_EARLY_INIT_F
int exynos_early_init_f(void)
{
	return 0;
}
#endif

#else /* !CONFIG_SPL_BUILD */
void exynos_led_set_on_early(unsigned int led_num)
{
#define GPL2CON (0x11000100)
#define GPL2DAT (0x11000104)
#define GPK1CON (0x11000060)
#define GPK1DAT (0x11000064)

        uint8_t val;

        /* LED2 */
        val = led_num & 0x01;
        if (val) {
                clrsetbits_le32(GPL2CON, 0xf << 0, 0x01 << 0);
                setbits_8(GPL2DAT, 1);
        }

        /* LED3 */
        val = led_num & 0x02;
        if (val) {
                clrsetbits_le32(GPK1CON, 0xf << 4, 0x01 << 4);
                setbits_8(GPK1DAT, 1 << 1);
        }
}
#endif
