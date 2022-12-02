# -*- coding: utf-8 -*-
"""
Created on Fri Dec  2 17:47:54 2022

@author: panni
"""

class DataMaster():
    def __init__(self):
        self.sync = "\r"
        self.syncb = "0"
    
    def DecodeMsg(self):
        temp = self.RowMsg.decode('utf8')
        if len(temp) >0:
            if "#" in temp:
                self.msg = temp.split("#")
                print(self.msg)
                del self.msg[0]
                print(self.msg)