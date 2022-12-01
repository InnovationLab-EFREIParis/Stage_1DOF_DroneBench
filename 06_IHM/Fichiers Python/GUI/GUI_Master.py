# -*- coding: utf-8 -*-
"""
Created on Thu Dec  1 10:27:04 2022

@author: Julien
"""

from tkinter import Tk, LabelFrame, Label, StringVar
from tkinter import OptionMenu, Button, messagebox

class RootGUI:
    def __init__(self):
        self.root = Tk() 
        self.root.title("Serial Communication")
        self.root.geometry("660x220")
        self.root.config(bg="white")
             
class ComGUI():
    def __init__(self, root, serial):
        self.root = root
        self.serial = serial
        self.frame = LabelFrame(root, text="Com Manager", 
                                padx=5, pady=5, bg="white")
        self.label_com = Label(
            self.frame, text="Available Port(s): ", 
            bg="white", width=15, anchor="w")
        self.label_bd = Label(
            self.frame, text="Baud Rate: ", 
            bg="white", width=15, anchor="w")
        self.ComOptionMenu()
        self.BaudOptionMenu()
        
        self.btn_refresh = Button(
            self.frame, text="Refresh", width=10,
            command=self.com_refresh)
        self.btn_connect = Button(
            self.frame, text="Connect", width=10, 
            state="disabled", command=self.serial_connect)
        
        self.padx = 20
        self.pady = 5
        self.publish()
    
    def ComOptionMenu(self):
        self.serial.getCOMList()
        self.clicked_com = StringVar()
        self.clicked_com.set(self.serial.com_list[0])
        self.drop_com = OptionMenu(
            self.frame, self.clicked_com, 
            *self.serial.com_list,
            command=self.connect_ctrl)
        self.drop_com.config(width=50)
    
    def BaudOptionMenu(self):
        bds = ["...", "115200"]
        self.clicked_bd = StringVar()
        self.clicked_bd.set(bds[0])
        self.drop_bd = OptionMenu(
            self.frame, self.clicked_bd, *bds,
            command=self.connect_ctrl)
        self.drop_bd.config(width=10)
    
    def publish(self):
        self.frame.grid(row=0, column=0, rowspan=3, 
                        columnspan=3, padx=5, pady=5)
        
        self.label_com.grid(column=1, row=2)
        self.drop_com.grid(column=2, row=2,
                           padx=self.padx, pady=self.pady)
        self.label_bd.grid(column=1, row=3)
        self.drop_bd.grid(column=2, row=3,
                          padx=self.padx, pady=self.pady)
        
        self.btn_refresh.grid(column=3,row=2)
        self.btn_connect.grid(column=3,row=3)
        
    def connect_ctrl(self, other):
        print("Connect ctrl")
        if "..." in self.clicked_com.get() or "..." in self.clicked_bd.get():
            self.btn_connect["state"] = "disable"
        else:
            self.btn_connect["state"] = "active"
        
    def com_refresh(self):
        self.drop_com.destroy()
        self.ComOptionMenu()
        self.drop_com.grid(column=2, row=2,
                           padx=self.padx, pady=self.pady)
        logic = []
        self.connect_ctrl(logic)
        
    def serial_connect(self):
        
        if self.btn_connect["text"] in "Connect":
            self.serial.SerialOpen(self)
            if self.serial.ser.status:
                self.btn_connect["text"] = "Disconnect"
                self.btn_refresh["state"] = "disable"
                self.drop_com["state"] = "disable"
                self.drop_bd["state"] = "disable"
                InfoMsg = f"Successful UART connection using {self.clicked_com.get()} "
                messagebox.showinfo("showinfo", InfoMsg)
            else:
                ErrorMsg = f"Failure to establish UART connection using {self.clicked_com.get()} "
                messagebox.showerror("showerror", ErrorMsg)
        else:
            self.serial.SerialClose()
            InfoMsg = f"UART connection using {self.clicked_com.get()} is now closed"
            messagebox.showwarning("showinfo", InfoMsg)
            self.btn_connect["text"] = "Connect"
            self.btn_refresh["state"] = "active"
            self.drop_com["state"] = "active"
            self.drop_bd["state"] = "active"
        
if __name__ == "__main__":
    RootGUI()
    ComGUI()
    
    