/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"

/* Inputs / Outputs definitions */
DigitalOut          led1(LED1);
BufferedSerial      usb_pc(USBTX, USBRX);
char    data;

/* Others functions declarations */
void    usb_pc_ISR(void);

/* MAIN PROGRAM */
int main()
{
    // Initialization of Serial communication port
    usb_pc.set_baud(115200);
    usb_pc.set_format(8, SerialBase::None, 1);
    // Set Interrupt for RX data
    usb_pc.sigio(callback(usb_pc_ISR));

    while (true)
    {
        wait_us(100);
    }
}



void    usb_pc_ISR(void){
    if(usb_pc.readable()){
        usb_pc.read(&data, 1);
        usb_pc.write(&data, 1);	// echo mode
        if(data == 'a') led1 = 1;
        if(data == 'e') led1 = 0;    
    }
}