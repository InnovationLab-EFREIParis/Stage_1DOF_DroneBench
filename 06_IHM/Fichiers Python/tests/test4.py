# -*- coding: utf-8 -*-
"""
Created on Tue Nov 22 10:16:12 2022

@author: Julien
"""
import serial

port = "COM3"

ser = serial.Serial(port,115200,timeout=1)

ser.write(b'0')

while True:
    data = ser.readline()
    print(data)