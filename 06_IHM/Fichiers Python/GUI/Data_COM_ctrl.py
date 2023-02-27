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
        self.ipt4 = "4"
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
        self.consigne_value = 0
        
        self.exceeding_value = False
                
    def DecodeMsg(self):
        temp = self.RowMsg
        if len(temp) > 0:
            if "Gaz Term : " in temp:
                self.msg = temp.split("Gaz Term : ")
                del self.msg[0]
                self.consigne_value = (self.msg[0]).removesuffix('\n')
            if "Angle : " in temp:
                self.msg = temp.split("Angle : ")
                del self.msg[0]
                self.consigne_value = (self.msg[0]).removesuffix('\n')
            if "DataA:" in temp:
                self.msg = temp.split("DataA:")
                self.msg = self.msg[1].split(";")
                if len(self.msg) == 11:
                    self.record.append([int(self.consigne_value),
                                        float(self.msg[0]),
                                        float(self.msg[1]),
                                        float(self.msg[2]),
                                        float(self.msg[3]),
                                        float(self.msg[4]),
                                        float(self.msg[5]),
                                        float(self.msg[6]),
                                        float(self.msg[7]),
                                        float(self.msg[8]),
                                        float(self.msg[9]),
                                        float((self.msg[10]).removesuffix('\n'))])
            if "DataG:" in temp:
                self.msg = temp.split("DataG:")
                self.msg = self.msg[1].split(";")
                if len(self.msg) == 8:
                    self.record.append([int(self.consigne_value),
                                        float(self.msg[0]),
                                        float(self.msg[1]),
                                        float(self.msg[2]),
                                        float(self.msg[3]),
                                        float(self.msg[4]),
                                        float(self.msg[5]),
                                        float(self.msg[6]),
                                        float((self.msg[7]).removesuffix('\n'))])
                    
            if "Gyro MPU6050 is not working" in temp:
                print("Error\n")
                ErrorMsg = "Gyro MPU6050 is not working. Please check if it is well connected."
                messagebox.showerror("showerror", ErrorMsg)   
            
            if "EXCEEDING VALUE" in temp:
                print("Error\n")
                ErrorMsg = "The arm was too close to the maximum value. For safety, landing state has been initiated."
                messagebox.showerror("showerror", ErrorMsg)
                self.exceeding_value = True
                



 



    def ClearData(self):
        """
        Method to clear the data
        """
        self.RowMsg = ""
        self.msg = []
        self.record = []