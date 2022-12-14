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

        self.msg = []

    def ClearData(self):
        """
        Method to clear the data
        """
        self.RowMsg = ""
        self.msg = []