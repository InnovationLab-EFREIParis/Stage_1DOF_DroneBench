# -*- coding: utf-8 -*-
"""
Created on Thu Dec  1 14:25:05 2022

@author: Julien
"""
import serial.tools.list_ports  # pip install pyserial
import time

# TAG_IHM_008
# Class to secure the UART serial communication with MCU
class SerialCtrl():
    def __init__(self):
        '''
        Initializing the main variables for the serial data
        '''

    def getCOMList(self):
        '''
        Method that get the list of available coms in the system
        '''
        ports = serial.tools.list_ports.comports()
        self.com_list = [com[0] for com in ports]
        self.com_list.insert(0, "...")

    def SerialOpen(self, ComGUI):
        '''
        Method to setup the serial connection
        '''
        try:
            self.ser.is_open
        except:
            PORT = ComGUI.clicked_com.get()
            BAUD = ComGUI.clicked_bd.get()
            self.ser = serial.Serial()
            self.ser.baudrate = BAUD
            self.ser.port = PORT
            self.ser.timeout = 0.1

        try:
            if self.ser.is_open:
                print("Already Open")
                self.ser.status = True
            else:
                PORT = ComGUI.clicked_com.get()
                BAUD = ComGUI.clicked_bd.get()
                self.ser = serial.Serial()
                self.ser.baudrate = BAUD
                self.ser.port = PORT
                self.ser.timeout = 0.01
                self.ser.open()
                self.ser.status = True
        except:
            self.ser.status = False

    def SerialClose(self, ComGUI):
        '''
        Method used to close the UART communication
        '''
        try:
            self.ser.is_open
            self.ser.close()
            self.ser.status = False
        except:
            self.ser.status = False 
    
    def SerialIpt(self, gui, ipt, nb_lines):
        """"
        Method used to communicate with the STM32 by sending a char
        """
        self.ser.write(ipt.encode())
        # Read the printf
        for i in range(nb_lines):
            gui.data.RowMsg = self.ser.readline()
            print(f"RowMsg: {gui.data.RowMsg}") 
            gui.data.DecodeMsg()
            
    def SerialOpt(self, gui, timeout):
        """
        Method used to read the outputs coming from the STM32
        """
        while time.time()<= timeout:
                         # runs this loop forever
            #time.sleep(.001)                    # delay of 1ms
            val = self.ser.readline()                # read complete line from serial output
            while not '\\n'in str(val):         # check if full data is received. 
                # This loop is entered only if serial read value doesn't contain \n
                # which indicates end of a sentence. 
                # str(val) - val is byte where string operation to check `\\n` 
                # can't be performed
                time.sleep(.001)                # delay of 1ms 
                temp = self.ser.readline()           # check for serial output.
                if not not temp.decode():       # if temp is not empty.
                    val = (val.decode()+temp.decode()).encode()
                    # required to decode, sum, then encode because
                    # long values might require multiple passes
            val = val.decode()                  # decoding from bytes
            gui.data.RowMsg = val.strip()                   # stripping leading and trailing spaces.
            print(f"RowMsg: {gui.data.RowMsg}")
            gui.data.DecodeMsg2()
            