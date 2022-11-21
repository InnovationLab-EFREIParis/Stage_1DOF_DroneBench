# -*- coding: utf-8 -*-
"""
Created on Mon Nov 21 14:44:03 2022

@author: Julien
"""

import serial
import serial.tools.list_ports
#import time

print("Recherche d'un port série...")

ports = serial.tools.list_ports.comports(include_links=False)

if (len(ports)!=0):
    if (len(ports)>1):
        print(str(len(ports)) + "ports actifs ont ete trouves:")
    else:
        print("1 port actif a ete trouve:")
        
    ligne=1
    
    for port in ports :
        print(str(ligne) + ' : ' + port.device)
        ligne = ligne +1
    portChoisi = input('Ecrivez le numero du port desire:')
    
    print('1: 9600   2: 38400   3: 115200')
    
    baud = input('Ecrivez le baud rate desire:')
    
    if (baud==1):
        baud = 9600
    if (baud==2):
        baud = 38400
    if (baud==3):
        baud = 115200
        
    stm32 = serial.Serial(ports[int(portChoisi) - 1].device, baud, timeout=1)
    
    print('Connexion a ' + stm32.name + ' a un baud rate de ' + str(baud))
    
    while True:
        data = stm32.readline()[:-2]
        if data:
            print(data)
        else:
            print("Aucun port actif n'a ete trouve")