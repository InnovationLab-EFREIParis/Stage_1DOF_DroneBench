# -*- coding: utf-8 -*-
"""
Created on Mon Nov 21 17:20:31 2022

@author: Julien
"""

import serial
#import time

with serial.Serial() as SerialObj:
    SerialObj.port = 'COM3' 
    SerialObj.baudrate = 115200
    SerialObj.bytesize = 8
    SerialObj.parity = 'N'
    SerialObj.stopbits = 1
    

    SerialObj.open()
    SerialObj.isOpen
    SerialObj.write(b'0')
    
    #ReceivedString = SerialObj.read_until('\r')
    #print(ReceivedString)
    
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    
    SerialObj.write(b'2')
    
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    ReceivedString = SerialObj.readline()
    print(ReceivedString)
    
    
#time.sleep(3)

#SerialObj.write(str(0).encode('ascii'))
#SerialObj.write(b'0')

#ReceivedString = SerialObj.readline()
#print(ReceivedString)

#SerialObj.close()