# -*- coding: utf-8 -*-
"""
Created on Tue Nov 22 15:50:25 2022

@author: Julien
"""

from tkinter import *

def connect_menu_init():
    global root
    root = Tk()
    root.title("Serial communication")
    root.geometry("500x500")
    root.config(bg="white")
    
    port_label = Label(root, text = "Available Port(s)", bg="white")
    port_label.grid(column =1, row=3)
    
connect_menu_init()

root.mainloop()