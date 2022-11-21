# -*- coding: utf-8 -*-
"""
Created on Mon Nov 21 17:20:31 2022

@author: Julien
"""

import serial
import time

SerialObj = serial.Serial('COM3')
SerialObj.bytesize = 8
SerialObj.parity = 'N'
SerialObj.stopbits = 1

time.sleep(3)

SerialObj.write(str(0).encode('ascii'))
#ReceivedString = SerialObj.readline()
#print(ReceivedString)
SerialObj.close()