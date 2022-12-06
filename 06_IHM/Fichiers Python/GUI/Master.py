# -*- coding: utf-8 -*-
"""
Created on Thu Dec  1 10:26:21 2022

@author: Julien
"""

from GUI_Master import RootGUI, ComGui
from Serial_COM_ctrl import SerialCtrl
from Data_COM_ctrl import DataMaster
# Initiate the Root class that will manage the other classes
MySerial = SerialCtrl()
MyData = DataMaster()
RootMaster = RootGUI(MySerial, MyData)
# Initiate the Communication Master class that will manage all the other GUI classes
ComMaster = ComGui(RootMaster.root, MySerial, MyData)

# Start the Graphic User Interface
RootMaster.root.mainloop()