/*
 * mv_switch_setup.h
 *
 *  Created on: May 14, 2020
 *      Author: tusimple
 */

#ifndef MV_SWITCH_SETUP_H_
#define MV_SWITCH_SETUP_H_

#include "xemacps_example.h"
#include "xil_exception.h"
#include "xscugic.h"


#define INTC		XScuGic
#define EMACPS_DEVICE_ID	XPAR_PSU_ETHERNET_2_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define EMACPS_IRPT_INTR	XPS_GEM2_INT_ID



LONG EmacPsDmaIntrExample(INTC *IntcInstancePtr,
			  XEmacPs *EmacPsInstancePtr,
			  u16 EmacPsDeviceId, u16 EmacPsIntrId);

#endif /* MV_SWITCH_SETUP_H_ */
