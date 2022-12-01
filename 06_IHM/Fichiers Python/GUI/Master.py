# -*- coding: utf-8 -*-
"""
Created on Thu Dec  1 10:26:21 2022

@author: Julien
"""

from GUI_Master import RootGUI, ComGUI
from Serial_COM_ctrl import SerialCtrl
  
MySerial = SerialCtrl()
RootMaster = RootGUI()

ComMaster = ComGUI(RootMaster.root, MySerial)

RootMaster.root.mainloop()