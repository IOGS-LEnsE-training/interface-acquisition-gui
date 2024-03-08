.. Acquisition GUI documentation master file, created by
   sphinx-quickstart on Fri Dec 15 11:17:25 2023.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Acquisition GUI's documentation
===============================

.. caution::
   This application and its documentation website are still works in progress

**Acquisition GUI** is a complete tutorial to develop a **graphical user interface** based on **PyQt6** in Python to collect data using an RS232 communication protocol from an acquisition home-made device.

In our case, the acquisition device is a *Nucleo L476RG* board from *STMicroelectronics*. The STM32 microcontroller is programmed using **Arm Keil Studio** (and MBED-OS library). 

The GitHub repository of this project : `Acquisition GUI <https://github.com/IOGS-LEnsE/acquisition-gui>`_

|

This set of tutorials is based on PyQt6 library for the graphical interface and on MBED-OS from Arm for the embedded device. 

You can learn how to develop graphical interface in PyQt6 with tutorials from LEnsE : `base_LEnsE_GUI <https://iogs-lense.github.io/base_LEnsE_GUI/>`_

A set of tutorials for MBED-OS and Nucleo boards are available on the LEnsE website : `Nucleo and MBED-OS Tutorials <http://lense.institutoptique.fr/prototyper-avec-nucleo-et-mbed-tutoriels-a-la-carte/>`_

|

`First Version (from LEnsE Website) <http://lense.institutoptique.fr/mine/python-pyserial-premier-script/>`_

GUI Tutorials
-------------

.. toctree::
   :maxdepth: 1

	Main steps<contents/step_by_step>

.. toctree::
   :maxdepth: 1
   :caption: RS232 protocol
   
	Low-level RS232 protocol<contents/rs232_protocol>
	Application-level protocol<contents/app_protocol>

.. toctree::
   :maxdepth: 1
   :caption: Events and signals



About the LEnsE
---------------

This is a test.

.. raw:: html

	<span class="linkgit"><a href="http://lense.institutoptique.fr/mine/nucleo-bibliotheques-de-fonctions/"><i class="fa fa-github"></i> Bibliothèques MBED 6</a></span>
