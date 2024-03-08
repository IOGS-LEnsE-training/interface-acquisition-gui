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
import numpy as np
from matplotlib import pyplot as plt



def main_menu():
    print('Commands list :')
    print('\t(i)nitialization of the board')
    print('\t(m)anual aquisition - one point')
    print('\t(a)utomatic aquisition - N points')
    print('\t(d)ata transmission and display')
    print('\t(p)lot last graphics')
    print('\t(q)uit')
    choice = input('\n\nEnter a command :')
    return choice
    
def receive_data(cmd):
    while serNuc.inWaiting() == 0:
        pass
    data_rec = serNuc.read(6)  # bytes
    print(f'\t\tdata = {data_rec}')
    indice = 0
    val = 0
    good = False
    cmd = int.from_bytes(cmd.encode('utf-8'), "little")
    # converting str to bytes then to int
    if((data_rec[0] == cmd) and (data_rec[1] == cmd)):
        good = True
        indice = data_rec[2]*256 + data_rec[3]
        val = data_rec[4]*256 + data_rec[5]
    print(f'I = {indice} / D = {val}')
    return good, indice, val

def create_frame(cmd, value1=-1, value2=-1):
    # to avoid 2 consecutive bytes with the same value
    if value1 == -1:
        value1 = 256
    if value2 == -1: 
        value2 = 256        
    value1_bytes = value1.to_bytes(2, 'big') 
    value2_bytes = value2.to_bytes(2, 'big')
    cmd = cmd.encode('utf-8')
    frame = b''
    frame += cmd+cmd+value1_bytes+value2_bytes   
    return frame
    
def display_data(data_x, data_y):
    plt.figure()
    plt.plot(data_x, data_y)
    plt.show()
    


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
    
    nb_points = -1
    appOk = 1
    array_data = np.zeros(1)
    time_lin = None

    while(appOk == 1):
        choice = main_menu()
        
        if(choice == 'q'):
            appOk = 0
            break
        elif(choice == 'i'):
            print('\nInitialization')
            frame = create_frame(choice)                
            serNuc.write(frame)
            g, ind, val = receive_data(choice)
        elif(choice == 'm'):
            print('\nManual acquisition')
            frame = create_frame(choice)    
            serNuc.write(frame)
            g, ind, val = receive_data(choice)
        elif(choice == 'a'):
            print('\nAutomatic acquisition')
            nb_points = int(input('\n\tEnter the number of points (100 to 10000):'))
            samp_freq = int(input('\tEnter the sampling frequency (100 to 10000 Hz) :'))
            frame = create_frame(choice, nb_points, samp_freq)      
            serNuc.write(frame)
            g, ind, val = receive_data(choice)
            array_data = np.zeros(nb_points)
        elif(choice == 'd'):
            print('\nCollect data')
            if(nb_points < 0):
                print('\t\tNot possible')
            else:
                frame = create_frame(choice, 1, 1)
                print(f'\tF = {frame}')
                serNuc.write(frame)            
                g, ind, val = receive_data(choice)
                print(f'\t\tIND__ = {ind}')
                
                while(ind == 0):
                    print(f'\t\tIND = {ind}')
                    frame = create_frame(choice, 1, 1)
                    serNuc.write(frame)                
                    g, ind, val = receive_data(choice)
                    
                for index_k in range(nb_points):
                    frame = create_frame(choice, index_k+1, index_k+5)   
                    serNuc.write(frame) 
                    g, ind, val = receive_data(choice)
                    array_data[ind-1] = val
                
                time_lin = np.linspace(0, (nb_points-1)*1/samp_freq, nb_points)
                display_data(time_lin, array_data)
                nb_points = -1
        elif(choice == 'p'):            
            print('\nDisplay last data')
            if(time_lin is None):
                print('\t\tNot possible')                
            else:
                display_data(time_lin, array_data)
        else:
            print('\nNo mode')
            frame = create_frame(choice)     
            serNuc.write(frame)
            g, ind, val = receive_data(choice)
            
       
    # Close the serial port
    serNuc.close()
