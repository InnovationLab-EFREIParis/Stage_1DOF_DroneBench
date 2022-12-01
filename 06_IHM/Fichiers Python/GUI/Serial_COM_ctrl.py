# -*- coding: utf-8 -*-
"""
Created on Thu Dec  1 14:25:05 2022

@author: Julien
"""
import serial.tools.list_ports

class SerialCtrl():
    def __init__(self):
        self.com_list = []
    
    def getCOMList(self):
        ports = serial.tools.list_ports.comports()
        self.com_list = [com[1] for com in ports]
        self.com_list.insert(0, "...")
    
    def SerialOpen(self, ComGUI):
        try:
            self.ser.is_open
        except:
            PORT = ComGUI.clicked_com.get()
            BAUD = ComGUI.clicked_bd.get()
            self.ser = serial.Serial()
            self.ser.port = PORT
            self.ser.baudrate = BAUD
            self.ser.timeout = 0.1
        
        try:
            if self.ser.is_open:
                self.ser.status = True
            else:
                PORT = ComGUI.clicked_com.get()
                BAUD = ComGUI.clicked_bd.get()
                self.ser = serial.Serial()
                self.ser.port = PORT
                self.ser.baudrate = BAUD
                self.ser.timeout = 0.1
                self.ser.open()
                self.ser.status = True
        except:
                self.ser.status = False
                
        def SerialClose(self):
            try:
                self.ser.is_open
                self.ser.close()
                self.ser.status = False
            except:
                self.ser.status = False

if __name__ == "__main__":
    SerialCtrl()