Low-Level RS232 Protocol
========================



Asynchronous protocol
---------------------

RX/TX signals
~~~~~~~~~~~~~

Default frame
~~~~~~~~~~~~~

Baudrate
~~~~~~~~

Serial Terminal
~~~~~~~~~~~~~~~


First communication / Nucleo
----------------------------

Before venturing into the development of a complete graphical interface able to display data graphics and to send orders to the acquisition board, we will learn how to set up a serial link and transfer data in the two directions.


Setting up a Serial communication
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To implement a Serial communication port on a *Nucleo board*, via *MBED-OS*, a **BufferedSerial** object is required, from the *mbed.h* library.

.. code-block:: cpp
	:linenos:
  
	#include "mbed.h"
	
	BufferedSerial      usb_pc(USBTX, USBRX);
	
In this example, the serial link is named **usb_pc** and is based on a virtual RS232 link through the USB cable (the same as for downloading the binary file to the board).

To create a link with other devices (as BlueTooth HC05 module for example), you can use two others serial ports on the L476RG board : 
- Serial 1 : A4 (TX) / A5 (RX)
- Serial 4 : A0 (TX) / A1 (RX)

Differents parameters can be updated on a Serial link. You can set a different data transfer rate, called baudrate, or modify the transmission low-level format of the data.

.. code-block:: cpp
	:linenos:
	
	usb_pc.set_baud(115200);
	
	usb_pc.format(8, SerialBase::None, 1);
	
The first function, **set_baud**, permits to change the data transfer rate in bauds. 

The second function, **set_format**, permits to change the data format. The firts argument is the number of data bits (1 byte is 8 bits). The second argument is to set a parity value (default is None). The third argument is to set a stop bit (default value is 1). 


Sendind data from external device to computer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To send data to the computer, **write** function from the *BufferedSerial* class can be used. Two arguments are required : a pointer to a memory space where data are stored as bytes and the amount of data (in bytes) to send.

.. code-block:: cpp
	:linenos:
	
	char data = 'a';
	usb_pc.write(&data, 1);
	
This first example permits to send one byte at a time (here is the 'a' character) through the RS232 serial port.

.. code-block:: cpp
	:linenos:
	
	char data[5] = {'a','0','b','5','!'};
	usb_pc.write(data, 5);
	
This second example permits to send 5 bytes at a time through the RS232 serial port.

Testing the transmission
~~~~~~~~~~~~~~~~~~~~~~~~

To test the communication between the Nucleo board and the computer, a simple program that send periodically a character or a string of characters can be used.

.. code-block:: cpp
	:linenos:
	
	#include "mbed.h"
	BufferedSerial      usb_pc(USBTX, USBRX);
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

In this example, the 'a' character followed by the {a0b5!} string 1 second later are sent from the Nucleo board at a baudrate of 115200 bauds, in the default format (data are on 8 bits with no parity bit and 1 stop bit).
	
|

But computers are not natively able to read serial communication port. An application that read the hardware buffer of the communication port is required, as for example `Tera Term <https://tera-term.fr.softonic.com/>`_

By setting up a serial terminal correctly (same baudrate...), the result is given in the next figure.

.. figure:: ../_static/rs232_first_test.png
   :alt: RS232 - Nucleo board test
   :align: center
   :width: 90%
   
   First communication test between a computer and a Nucleo board.

The computer receives each second, alternatively, 'a' character and {a0b5!} string.

Python and Serial communication
-------------------------------

Serial terminals are not user-friendly for developing a complete system able to collect data and display them in a professionnal graphics.

Pyserial library
~~~~~~~~~~~~~~~~


Listing communication ports
~~~~~~~~~~~~~~~~~~~~~~~~~~~


Accessing a specific port
~~~~~~~~~~~~~~~~~~~~~~~~~


First console application to collect data
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


.. code-block:: python
	:linenos:

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



Complete communication / Nucleo
-------------------------------

In the previous part, communication was tested **only in one direction** : from the Nucleo board (or acquisition device) to the computer.

In a complete application it's necessary to **also send commands** from the computer to the acquisition board.

Reading data / Polling method
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The RS232 protocol is **asynchronous**, meaning that there is no clock transmitted between the two nodes of the network. But there is also no periodicity in the frame transmission.

When an RS232 signal is received by a node (detected by a start bit sequence on the communication RX port), the data is stored in an hardware buffer.

To know if a byte is received (i.e. stored in the hardware buffer), **readable** function from the *BufferedSerial* class can be used. This function returns true if a byte can be read, false otherwise.

.. code-block:: cpp
	:linenos:
	
	while(true){
		if(usb_pc.readable()){
			// almost one byte is readable
		}
	}

In the previous example


To receive data from the computer, *read* function from the *BufferedSerial* class can be used. Two arguments are required : a pointer to a memory space where data will be stored as bytes and the amount of data (in bytes) to send.



Echo application
~~~~~~~~~~~~~~~~

.. code-block:: cpp
	:linenos:
	
	while(true){
		if(usb_pc.readable()){
			usb_pc.read(&data, 1);
			usb_pc.write(&data, 1);
		}
	}

Read data / Interrupt method
~~~~~~~~~~~~~~~~~~~~~~~~~~~~



Switch On/Off the acquisition device
------------------------------------

Final program
~~~~~~~~~~~~~

.. code-block:: cpp
	:linenos:
  
	#include "mbed.h"
	
	BufferedSerial      usb_pc(USBTX, USBRX);
	
	void usb_pc_ISR(void){
		char rec_data_pc;
		int rec_length = 0;
		if(usb_pc.readable()){
			rec_length = usb_pc.read(&rec_data_pc, 1);
			usb_pc.write(&rec_data_pc, 1);
			rec_data_pc = 'c';
			usb_pc.write(&rec_data_pc, 1);
		}
	}
	
	int main()
	{
		usb_pc.set_baud(115200);
		usb_pc.sigio(callback(usb_pc_ISR));
		while (true){}
	}








More data
---------


