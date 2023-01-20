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

        for i in range(nb_lines):
            gui.data.RowMsg = self.ser.readline()
            print(f"RowMsg: {gui.data.RowMsg}")
            
    def SerialOpt(self, gui, timeout):
        """
        Method used to read the outputs coming from the STM32
        """
        while time.time()<= timeout:
            val = self.ser.readline()
            while not '\\n'in str(val):
                time.sleep(.001)               
                temp = self.ser.readline()         
                if not not temp.decode():       
                    val = (val.decode()+temp.decode()).encode()
            val = val.decode()                
            gui.data.RowMsg = val.strip()             
            print(f"RowMsg: {gui.data.RowMsg}")
            gui.data.DecodeMsg()