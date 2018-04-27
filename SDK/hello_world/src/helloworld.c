/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "xparameters.h"
#include "platform.h"
#include "xintc.h"
#include "xio.h"
#include "xil_exception.h"

XIntc Intc;

void my_timer_interrupt_handler(void * baseaddr_p) {
	xil_printf("\n\rtimer timeout.");
}

int main()
{
    init_platform();
    XStatus Status;
    Xuint32 value1, value2, value3;

    xil_printf("Interrupt example\n\r");

   // XIo_Out32(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);
    //initialize interrupt controller
    Status = XIntc_Initialize (&Intc, XPAR_INTC_0_DEVICE_ID);
    if (Status != XST_SUCCESS) xil_printf ("\r\nInterrupt controller initialization failure");
    else xil_printf("\r\nInterrupt controller initialized");

    // Connect my_timer_interrupt_handler
    Status = XIntc_Connect (&Intc, XPAR_INTC_SINGLE_DEVICE_ID,
                             (XInterruptHandler) my_timer_interrupt_handler,
                             (void *)0);

    if (Status != XST_SUCCESS) xil_printf ("\r\nRegistering MY_TIMER Interrupt Failed");
    else xil_printf("\r\nMY_TIMER Interrupt registered");

    //start the interrupt controller in real mode
    Status = XIntc_Start(&Intc, XIN_REAL_MODE);

    //enable interrupt controller
    XIntc_Enable  (&Intc, XPAR_INTC_SINGLE_DEVICE_ID);

	microblaze_enable_interrupts();

    while (1){
    	/*value3 = XIo_In32(XPAR_MY_TIMER_0_BASEADDR + 0x0);
    	value1 = XIo_In32(XPAR_MY_TIMER_0_BASEADDR + 0x4);
    	value2 = XIo_In32(XPAR_MY_TIMER_0_BASEADDR + 0x8);
        xil_printf("\n\rvalue1 = %x, value2 = %x, value3 = %x.", value1, value2, value3);*/
    }
    cleanup_platform();

    return 0;
}
