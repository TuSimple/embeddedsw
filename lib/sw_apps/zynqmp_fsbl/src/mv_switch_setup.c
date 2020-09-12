/*
 * mv_switch_setup.c
 *
 *  Created on: May 18, 2020
 *      Author: tusimple
 */

#include "mv_switch_setup.h"
#include "xemacps.h"

#define MACB_MDIO_EN
#define set_bit(val, bit_num) ((val) | (1 << bit_num))
#define clear_bit(val, bit_num) ((val) & ~(1 << bit_num))

#define AXI_GPIO_REG_BASE (0x80000000)
static void ResetMvSwitch(void) {
	// config all gpios as output
	Xil_Out32(AXI_GPIO_REG_BASE+4, 0x0);

	// set and clear
	Xil_Out32(AXI_GPIO_REG_BASE, 0xFFFFFFFF);
	usleep(1000);
	Xil_Out32(AXI_GPIO_REG_BASE, 0x0);
	usleep(1000);
	Xil_Out32(AXI_GPIO_REG_BASE, 0xFFFFFFFF);
}

static void SetPortReg(XEmacPs * EmacPsInstancePtr, uint32_t phy_addr, uint32_t dev_addr, uint32_t reg_addr, uint32_t mask, uint32_t val) {
	// uint16_t reg_addr = 0x0;
	int origin_val = switch_port_read(EmacPsInstancePtr, phy_addr, dev_addr, reg_addr);
	origin_val = (origin_val & ~mask) | (val & mask);
	switch_port_write(EmacPsInstancePtr, phy_addr, dev_addr, reg_addr, origin_val);
	int readback = switch_port_read(EmacPsInstancePtr, phy_addr, dev_addr, reg_addr);
	xil_printf("val: 0x%x, dev_addr: 0x%x, reg_addr: 0x%x\n\r", readback, dev_addr, reg_addr);
}

static void SetupSwitchPort(XEmacPs * EmacPsInstancePtr, uint32_t phy_addr) {
	for (int dev_addr = 0x10; dev_addr < 0x17; dev_addr++) {
//				for (int reg_addr = 0; reg_addr <= 0x1F; reg_addr++) {
			// uint16_t reg_addr = 0x0;
			// SetPortReg(EmacPsInstancePtr, phy_addr, dev_addr, reg_addr, 0xf, 0x7);
			// int val = switch_port_read(EmacPsInstancePtr, phy_addr, dev_addr, reg_addr);
			// val = (val & ~(0xf)) | 0x7;
			// switch_port_write(EmacPsInstancePtr, phy_addr, dev_addr, reg_addr, val);
			// int readback = switch_port_read(EmacPsInstancePtr, phy_addr, dev_addr, reg_addr);
			// xil_printf("val: 0x%x, dev_addr: 0x%x, reg_addr: 0x%x\n\r", readback, dev_addr, reg_addr);

			uint16_t reg_addr = 0x1;
			uint32_t mask = (0x3 << 14);
			SetPortReg(EmacPsInstancePtr, phy_addr, dev_addr, reg_addr, mask, mask);
// 			val = switch_port_read(EmacPsInstancePtr, phy_addr, dev_addr, reg_addr);
// 			val = set_bit(val, 14);
// 			val = set_bit(val, 15);
// 			switch_port_write(EmacPsInstancePtr, phy_addr, dev_addr, reg_addr, val);
// 			readback = switch_port_read(EmacPsInstancePtr, phy_addr, dev_addr, reg_addr);
// //			int val = macb_mdio_read(PHY_ADDRESS, addr);
// 			xil_printf("val: 0x%x, dev_addr: 0x%x, reg_addr: 0x%x\n\r", readback, dev_addr, reg_addr);
//					xil_printf("cmd: %u, addr: %u, val: %d\n\r", cmd, addr, val);
//				}
		usleep(10*1000);
	}
}

/****************************************************************************/
/**
*
* This function demonstrates the usage of the EmacPs driver by sending by
* sending and receiving frames in interrupt driven DMA mode.
*
*
* @param	IntcInstancePtr is a pointer to the instance of the Intc driver.
* @param	EmacPsInstancePtr is a pointer to the instance of the EmacPs
*		driver.
* @param	EmacPsDeviceId is Device ID of the EmacPs Device , typically
*		XPAR_<EMACPS_instance>_DEVICE_ID value from xparameters.h.
* @param	EmacPsIntrId is the Interrupt ID and is typically
*		XPAR_<EMACPS_instance>_INTR value from xparameters.h.
*
* @return	XST_SUCCESS to indicate success, otherwise XST_FAILURE.
*
* @note		None.
*
*****************************************************************************/
LONG EmacPsDmaIntrExample(INTC * IntcInstancePtr,
			  XEmacPs * EmacPsInstancePtr,
			  u16 EmacPsDeviceId,
			  u16 EmacPsIntrId)
{
	LONG Status;
	XEmacPs_Config *Config;

	/*************************************/
	/* Setup device for first-time usage */
	/*************************************/


	/*
	 *  Initialize instance. Should be configured for DMA
	 *  This example calls _CfgInitialize instead of _Initialize due to
	 *  retiring _Initialize. So in _CfgInitialize we use
	 *  XPAR_(IP)_BASEADDRESS to make sure it is not virtual address.
	 */
	Config = XEmacPs_LookupConfig(EmacPsDeviceId);

	Status = XEmacPs_CfgInitialize(EmacPsInstancePtr, Config,
					Config->BaseAddress);

	if (Status != XST_SUCCESS) {
		EmacPsUtilErrorTrap("Error in initialize");
		return XST_FAILURE;
	}

	/*
	 * Set emacps to phy loopback
	 */

		xil_printf("GemVersion 1\n");
		if ((Platform & PLATFORM_MASK) == PLATFORM_VERSALEMU) {
			XEmacPs_SetMdioDivisor(EmacPsInstancePtr, MDC_DIV_8);
		} else {
			XEmacPs_SetMdioDivisor(EmacPsInstancePtr, MDC_DIV_224);
		}

#ifdef MACB_MDIO_EN
//		ResetMvSwitch();
		// waiting for reset 3s

//		while (1) {

			u16 val = 1;
//				uint32_t phy_manag = macb_readl(bp, MAN);
//				macb_writel(bp, MAN, phy_manag | 0xA5);
			// while(1) {
#if 0
#else
					// sleep(15);
				for (int i = 0; i < 32; i++) {
					//int i = 31;
					XEmacPs_PhyRead(EmacPsInstancePtr, i, 0, &val);
					xil_printf("addr: %d, val: %d\n\r", i, val);
					if (val != 0xffff) {
						xil_printf("setup phy-%d\n\r", i, val);
						SetupSwitchPort(EmacPsInstancePtr, i);
					}
					// sleep(15);
				}
#endif
				// sleep(30);
			// }

//		}
#else
		if ((Platform & PLATFORM_MASK) == PLATFORM_SILICON) {
			EmacPsUtilEnterLoopback(EmacPsInstancePtr, EMACPS_LOOPBACK_SPEED_1G);
			XEmacPs_SetOperatingSpeed(EmacPsInstancePtr,EMACPS_LOOPBACK_SPEED_1G);
		} else {
			EmacPsUtilEnterLoopback(EmacPsInstancePtr, EMACPS_LOOPBACK_SPEED);
			XEmacPs_SetOperatingSpeed(EmacPsInstancePtr,EMACPS_LOOPBACK_SPEED);
		}
#endif
//    while(1) {
//    	;
//    }
	return XST_SUCCESS;
}

