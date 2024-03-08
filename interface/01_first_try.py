#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
LEnsE projects / Institut d'Optique

First try of serial communication between a Nucleo board
and a computer

@see : https://iogs-lense.github.io/acquisition-gui/

Created on 07/Feb/2024

@author: LEnsE / IOGS / Palaiseau
@author: Julien Villemejane
"""
from serial import Serial
import serial.tools.list_ports

if __name__ == "__main__":
    ports = serial.tools.list_ports.comports()
    # To obtain the list of the communication ports
    for port, desc, hwid in sorted(ports):
        print("{}: {}".format(port, desc))
    # To select the port to use
    selectPort = input("Select a COM port : ")    
    print(f"Port Selected : COM{selectPort}")
    # To open the serial communication at a specific baudrate
    serNuc = Serial('COM'+str(selectPort), 115200)  # Under Windows only
    
    # number of bytes to collecting
    nb_bytes = int(input("Enter the number of bytes to collect : "))    
    
    # collecting data byte after byte
    nb_bytes_collected = 0
    while(nb_bytes_collected < nb_bytes):
        # Waiting new data
        while serNuc.inWaiting() == 0:
            pass
        data_rec = serNuc.read(1)  # bytes
        print(f'D{nb_bytes_collected} = {data_rec}')
        nb_bytes_collected += 1
    
    # Close the serial port
    serNuc.close()