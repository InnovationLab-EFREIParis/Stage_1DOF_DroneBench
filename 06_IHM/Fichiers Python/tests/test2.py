# -*- coding: utf-8 -*-
"""
Created on Mon Nov 21 17:02:31 2022

@author: Julien
"""

import struct

class ReadFromSerial(object):
   
    def __init__(self, port):
        self.port = port
        self.vals = []
 
    def read_one_struct(self):
        self.read = False
        while not self.read:
            myByte = self.port.read(1)
            if myByte == b'S':  # Look for Start indicator 
                data = self.port.read_until(b'Z') # Z indicates Stop
                dat = data[1:len(data)-1]
                # unpack the dat into struct
                try:
                    """ '<' indicates small endian format
                        'f' indicates float type => 4B groups formed
                        3x 'f' indicates there are 3 vars to be 
                        unpacked. => 12 bytes in all.
                        This must match the size of dat.
                        Hence this section is in try-catch block.
                        Normally, it will never face any issue,
                        but there is always a chance of noise.
                    """
                    new_values = struct.unpack('<fff', dat)
                    self.read = True
                    self.vals.append(list(new_values))
                    return True
                except KeyboardInterrupt:
                    return False # Stop Program
                except:
                    return True # To ignore struct error
                
                
import serial

s = serial.Serial('COM3') # Check COM port in device manager
opClass = ReadFromSerial(s)
while opClass.read_one_struct():
   pass
s.close() # Close Serial Port
data = opClass.vals