# -*- coding: utf-8 -*-
"""
Created on Fri Dec  2 17:47:54 2022

@author: Julien
"""
import time

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
        self.msgX = []
        self.msgY = []
        self.msgAx = []
        self.msgAy = []
        self.msgAz = []
        self.msgGx = []
        self.msgGy = []
        self.msgGz = []
        self.record = []
        self.gas_value = 0

    def DecodeMsg1(self):
        temp = self.RowMsg.decode('utf8')
        if len(temp) > 0:
            if "Gaz Term : " in temp:
                self.msg = temp.split("Gaz Term : ")
                # print(f"Before removing index :{self.msg}")
                del self.msg[0]
                #print(f"After removing index :{self.msg}")
                self.gas_value = (self.msg[0]).removesuffix('\n')
            
            if "PositionX : " in temp:
                self.msgX = temp.split("PositionX : ")
                del self.msgX[0]
            if "PositionY : " in temp:    
                self.msgY = temp.split("PositionY : ")
                del self.msgY[0]
            if "Ax : " in temp:    
                self.msgAx = temp.split("Ax : ")
                del self.msgAx[0]
            if "Ay : " in temp:    
                self.msgAy = temp.split("Ay : ")
                del self.msgAy[0]  
            if "Az : " in temp:    
                self.msgAz = temp.split("Az : ")
                del self.msgAz[0]     
            if "Gx : " in temp:    
                self.msgGx = temp.split("Gx : ")
                del self.msgGx[0]
            if "Gy : " in temp:    
                self.msgGy = temp.split("Gy : ")
                del self.msgGy[0]  
            if "Gz : " in temp:    
                self.msgGz = temp.split("Gz : ")
                del self.msgGz[0]       
                self.record.append([int(self.gas_value),
                                    float((self.msgX[0]).removesuffix('\n')),
                                    float((self.msgY[0]).removesuffix('\n')),
                                    int((self.msgAx[0]).removesuffix('\n')),
                                    int((self.msgAy[0]).removesuffix('\n')),
                                    int((self.msgAz[0]).removesuffix('\n')),
                                    int((self.msgGx[0]).removesuffix('\n')),
                                    int((self.msgGy[0]).removesuffix('\n')),
                                    int((self.msgGz[0]).removesuffix('\n'))])
                #self.record.append([self.gas_value,time.time(),(self.msgX[0]).removesuffix('\n')])
                
    def DecodeMsg(self):
        temp = self.RowMsg.decode('utf8')
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
                else:
                    self.record.append([100,
                                        1,
                                        1,
                                        1,
                                        1,
                                        1,
                                        1,
                                        1,
                                        1])
            else:
                self.record.append([100,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0])
        else:
            self.record.append([100,
                                2,
                                2,
                                2,
                                2,
                                2,
                                2,
                                2,
                                2])
                
    def DecodeMsg3(self):
        temp = self.RowMsg.decode('utf8')
        if len(temp) > 0:
            if "Gaz Term : " in temp:
                self.msg = temp.split("Gaz Term : ")
                del self.msg[0]
                self.gas_value = (self.msg[0]).removesuffix('\n')
            if "Data:" in temp:
                self.msg = temp.split("Data:")
                self.msg = self.msg[1].split(";")
                self.posx_value = float(self.msg[0])
                self.posy_value = float(self.msg[1])
                self.Ax_value = int(self.msg[2])
                self.Ay_value = int((self.msg[3]).removesuffix('\n'))
            
                              
            if "Data2:" in temp:
                self.msg = temp.split("Data:")
                self.msg = self.msg[1].split(";")
                self.record.append([int(self.gas_value),
                                    self.posx_value,
                                    self.posy_value,
                                    self.Ax_value,
                                    self.Ay_value,
                                    int(self.msg[0]),
                                    int(self.msg[1]),
                                    int(self.msg[2]),
                                    int((self.msg[3]).removesuffix('\n'))])   


    def ClearData(self):
        """
        Method to clear the data
        """
        self.RowMsg = ""
        self.msg = []
        self.msgX = []
        self.msgY = []
        self.msgAx = []
        self.msgAy = []
        self.msgAz = []
        self.msgGx = []
        self.msgGy = []
        self.msgGz = []
        self.record = []