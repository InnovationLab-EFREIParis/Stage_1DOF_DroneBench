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
        self.ipt3 = "3"
        self.iptx = "x"
        self.ipty = "y"
        self.iptz = "z"
        
        self.msg = []

    def DecodeMsg(self):
        """
        Method to decode the received printf
        (Not necessary for now)
        """
        temp = self.RowMsg.decode('utf8')
        if len(temp) > 0:
            if "\r" in temp:
                self.msg = temp.split("\r")
                #print(f"Before removing index :{self.msg}")
                del self.msg[0]
                #print(f"After removing index :{self.msg}")

    def ClearData(self):
        """
        Method to clear the data
        """
        self.RowMsg = ""
        self.msg = []