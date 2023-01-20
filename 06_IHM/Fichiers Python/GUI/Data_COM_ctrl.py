# -*- coding: utf-8 -*-
"""
Created on Fri Dec  2 17:47:54 2022

@author: Julien
"""

from tkinter import messagebox

# TAG_IHM_009
# Class to setup the communication data
class DataMaster():
    def __init__(self):
        """
        Initializing the main variables
        """
        self.iptSPACE = " "
        self.iptENTER = "\r"
        self.ipt2 = "2"
        self.ipt3 = "3"
        self.iptp = "p"
        self.ipti = "i"
        self.iptd = "d"
        self.iptw = "w"  
        self.iptr = "r"
        self.ipts = "s"
        self.iptMINUS = "-"
        self.iptPLUS = "+"
        
        self.msg = []
        self.record = []
        self.gas_value = 0
                
    def DecodeMsg(self):
        temp = self.RowMsg
        if len(temp) > 0:
            if "Gaz Term : " in temp:
                self.msg = temp.split("Gaz Term : ")
                del self.msg[0]
                self.gas_value = (self.msg[0]).removesuffix('\n')
            if "Data:" in temp:
                self.msg = temp.split("Data:")
                self.msg = self.msg[1].split(";")
                if len(self.msg) == 8:
                    self.record.append([int(self.gas_value),
                                        float(self.msg[0]),
                                        float(self.msg[1]),
                                        int(self.msg[2]),
                                        int(self.msg[3]),
                                        int(self.msg[4]),
                                        int(self.msg[5]),
                                        int(self.msg[6]),
                                        int((self.msg[7]).removesuffix('\n'))])
            if "Gyro MPU6050 is not working" in temp:
                print("Error\n")
                ErrorMsg = "Gyro MPU6050 is not working. Please check if it is well connected."
                messagebox.showerror("showerror", ErrorMsg)   

    def ClearData(self):
        """
        Method to clear the data
        """
        self.RowMsg = ""
        self.msg = []
        self.record = []