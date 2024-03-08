/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#define     DEBUG   1

#include "mbed.h"

/* Inputs / Outputs definitions */
BufferedSerial      usb_pc(USBTX, USBRX);

/* Global Variables */
char    data;
char    data2[5] = {'a','0','b','5','!'};

/* MAIN PROGRAM */
int main()
{
    // Initialization of Serial communication port
    usb_pc.set_baud(115200);
    usb_pc.set_format(8, SerialBase::None, 1);

    while (true)
    {
        data = 'a';
        usb_pc.write(&data, 1);
        thread_sleep_for(1000);
        usb_pc.write(data2, 5);
        thread_sleep_for(1000);
    }
}
