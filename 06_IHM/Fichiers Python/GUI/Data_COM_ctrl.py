# -*- coding: utf-8 -*-
"""
Created on Fri Dec  2 17:47:54 2022

@author: Julien
"""

class DataMaster():
    def __init__(self):
        self.sync = "\r0"
        self.syncB = "3"
        self.msg = []

    def DecodeMsg(self):
        temp = self.RowMsg.decode('utf8')
        if len(temp) > 0:
            if "\r" in temp:
                self.msg = temp.split("\r")
                #print(f"Before removing index :{self.msg}")
                del self.msg[0]
                #print(f"After removing index :{self.msg}")

    def ClearData(self):
        self.RowMsg = ""
        self.msg = []