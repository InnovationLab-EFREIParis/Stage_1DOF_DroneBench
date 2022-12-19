# -*- coding: utf-8 -*-
"""
Created on Fri Dec  2 17:47:54 2022

@author: Julien
"""
# Class to setup the communication data
class DataMaster():
    def __init__(self):
        """
        Initializing the main variables
        """
        self.ipt0 = "0"
        self.iptENTER = "\r"
        self.ipt2 = "2"
        self.ipt3 = "3"
        self.iptx = "x"
        self.ipty = "y"
        self.iptz = "z"
        self.iptw = "w"  
        self.iptr = "r"
        self.iptMINUS = "-"
        self.iptPLUS = "+"
        

        self.msg = []
        self.record = []
        self.gas_value = 0

    def DecodeMsg(self):
        temp = self.RowMsg.decode('utf8')
        if len(temp) > 0:
            if "Position : " in temp:
                self.msg = temp.split("Position : ")
                # print(f"Before removing index :{self.msg}")
                del self.msg[0]
                # print(f"After removing index :{self.msg}")
                self.record.append([self.gas_value,(self.msg[0]).removesuffix('\n')])
                
            if "Gaz Term : " in temp:
                self.msg = temp.split("Gaz Term : ")
                # print(f"Before removing index :{self.msg}")
                del self.msg[0]
                #print(f"After removing index :{self.msg}")
                self.gas_value = (self.msg[0]).removesuffix('\n')


    def ClearData(self):
        """
        Method to clear the data
        """
        self.RowMsg = ""
        self.msg = []
        self.record = []