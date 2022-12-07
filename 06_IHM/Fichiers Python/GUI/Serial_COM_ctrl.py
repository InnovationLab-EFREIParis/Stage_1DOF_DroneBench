# -*- coding: utf-8 -*-
"""
Created on Thu Dec  1 14:25:05 2022

@author: Julien
"""
import serial.tools.list_ports  # pip install pyserial

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
    
    def SerialIptENTER(self, gui):
        """"
        Method used to communicate with the STM32 and send "\r" which is
        the ENTER to access to Init_UC or the ENTER necessary for consigne
        """
        self.ser.write(gui.data.iptENTER.encode())
        # Read the printf
        for i in range(30):
            gui.data.RowMsg = self.ser.readline()
            print(f"RowMsg: {gui.data.RowMsg}")
    
    def SerialIpt0(self, gui):
        """"
        Method used to communicate with the STM32 and send "0" which is
        the '0' to access to Motor Ready and/or landing 
        """
        self.ser.write(gui.data.ipt0.encode())
        # Read the printf
        for i in range(30):
            gui.data.RowMsg = self.ser.readline()
            print(f"RowMsg: {gui.data.RowMsg}")        
    
    def SerialIpt3(self, gui):
        """"
        Method used to communicate with the STM32 and send "3" which is
        the '3' to access to Auto Mode
        """
        self.ser.write(gui.data.ipt3.encode())
        # Read the printf
        for i in range(30):
            gui.data.RowMsg = self.ser.readline()
            print(f"RowMsg: {gui.data.RowMsg}")
    
    def SerialIptx(self, gui):
        """
        Method used to communicate with the STM32 and send "x" which is
        the 'x' to access to instruct kp 
        """
        self.ser.write(gui.data.iptx.encode())
        # Read the printf
        for i in range(30):
            gui.data.RowMsg = self.ser.readline()
            print(f"RowMsg: {gui.data.RowMsg}")
            
    def SerialIpty(self, gui):
        """
        Method used to communicate with the STM32 and send "y" which is
        the 'y' to access to instruct ki 
        """
        self.ser.write(gui.data.ipty.encode())
        # Read the printf
        for i in range(30):
            gui.data.RowMsg = self.ser.readline()
            print(f"RowMsg: {gui.data.RowMsg}")
            
    def SerialIptz(self, gui):
        """
        Method used to communicate with the STM32 and send "z" which is
        the 'z' to access to instruct kd 
        """
        self.ser.write(gui.data.iptz.encode())
        # Read the printf
        for i in range(30):
            gui.data.RowMsg = self.ser.readline()
            print(f"RowMsg: {gui.data.RowMsg}")
        
        