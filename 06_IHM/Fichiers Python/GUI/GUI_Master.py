# -*- coding: utf-8 -*-
"""
Created on Thu Dec  1 10:27:04 2022

@author: Julien
"""

import tkinter as tk
from tkinter import Tk, LabelFrame, Label, Button, Entry
from tkinter import messagebox, StringVar, OptionMenu, filedialog
import time
import pandas as pd
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
import re

# TAG_IHM_001
# Class to setup the main window
class RootGUI():
    def __init__(self, serial, data):
        '''
        Initializing the root GUI and other comps of the program
        '''
        self.root = Tk()
        self.root.resizable(width=False, height=False)
        self.root.title("Serial communication")
        self.root.geometry("360x110")
        self.root.config(bg="white")
        self.serial = serial
        self.data = data

        self.root.protocol("WM_DELETE_WINDOW", self.close_window)

    def close_window(self):
        """
        Method used if clicking on the window's red cross
        """
        print("Closing the window and exit")
        self.root.destroy()
        # Landing Mode called in case the engine is still functionning (if we click on the red cross before the 'STOP!' button)
        self.serial.SerialIpt(self, self.data.iptSPACE, 30)
        # Closing Serial COM
        self.serial.SerialClose(self)
        

# TAG_IHM_002
# Class to setup and create the communication manager with MCU
class ComGui():
    def __init__(self, root, serial, data):
        '''
        Initialize the connexion GUI and initialize the main widgets 
        '''
        # Initializing the Widgets
        self.root = root
        self.serial = serial
        self.data = data
        self.frame = LabelFrame(root, text="Com Manager",
                                padx=5, pady=5, bg="white")
        self.label_com = Label(
            self.frame, text="Available Port(s): ", bg="white", width=15, 
            anchor="w")
        self.label_bd = Label(
            self.frame, text="Baud Rate: ", bg="white", width=15, anchor="w")

        # Setup the Drop option menu
        self.baudOptionMenu()
        self.ComOptionMenu()

        # Add the control buttons for refreshing the COMs & Connect
        self.btn_refresh = Button(self.frame, text="Refresh",
                                  width=10,  command=self.com_refresh)
        self.btn_connect = Button(self.frame, text="Connect",
                                  width=10, state="disabled",  
                                  command=self.serial_connect)

        # Optional Graphic parameters
        self.padx = 20
        self.pady = 5

        # Put on the grid all the elements
        self.publish()

    def publish(self):
        '''
         Method to display all the widgets of the main frame
        '''
        self.frame.grid(row=0, column=0, rowspan=3,
                        columnspan=3, padx=5, pady=5)
        self.label_com.grid(column=1, row=2)
        self.label_bd.grid(column=1, row=3)

        self.drop_baud.grid(column=2, row=3, padx=self.padx, pady=self.pady)
        self.drop_com.grid(column=2, row=2, padx=self.padx)

        self.btn_refresh.grid(column=3, row=2)
        self.btn_connect.grid(column=3, row=3)

    def ComOptionMenu(self):
        '''
         Method to get the available COMs connected to the PC
         and list them into the drop menu
        '''
        # Generate the list of available coms

        self.serial.getCOMList()

        self.clicked_com = StringVar()
        self.clicked_com.set(self.serial.com_list[0])
        self.drop_com = OptionMenu(
            self.frame, self.clicked_com, *self.serial.com_list, 
            command=self.connect_ctrl)

        self.drop_com.config(width=10)

    def baudOptionMenu(self):
        '''
         Method to list all the baud rates in a drop menu
        '''
        self.clicked_bd = StringVar()
        bds = ["...", "115200"]
        self.clicked_bd .set(bds[0])
        self.drop_baud = OptionMenu(
            self.frame, self.clicked_bd, *bds, command=self.connect_ctrl)
        self.drop_baud.config(width=10)

    def connect_ctrl(self, widget):
        '''
        Method to keep the connect button disabled if all the 
        conditions are not cleared
        '''
        # Checking the logic consistency to keep the connection btn
        if "..." in self.clicked_bd.get() or "..." in self.clicked_com.get():
            self.btn_connect["state"] = "disabled"
        else:
            self.btn_connect["state"] = "active"

    def com_refresh(self):
        """
        Method to erase the current selected COM
        """
        print("Refresh")
        # Get the Widget destroyed
        self.drop_com.destroy()

        # Refresh the list of available Coms
        self.ComOptionMenu()

        # Publish the this new droplet
        self.drop_com.grid(column=2, row=2, padx=self.padx)

        # Just in case to secure the connect logic
        logic = []
        self.connect_ctrl(logic)

    def serial_connect(self):
        '''
        Method that updates the GUI during connect / disconnect status
        Manage serials and data flows during connect / disconnect status
        '''
        if self.btn_connect["text"] in "Connect":
            # Start the serial communication
            self.serial.SerialOpen(self)
            # If connection established move on
            if self.serial.ser.status:
                # Update the COM manager
                self.btn_connect["text"] = "Disconnect"
                self.btn_refresh["state"] = "disable"
                self.drop_baud["state"] = "disable"
                self.drop_com["state"] = "disable"
                InfoMsg = f"Successful UART connection using {self.clicked_com.get()}"
                messagebox.showinfo("showinfo", InfoMsg)
                # Display the Motor Ready manager
                self.motor_ready = MotorReadyGUI(self.root, self.serial, self.data)
                # Access to Init UC
                self.serial.SerialIpt(self, self.data.iptENTER, 5)
                # Access to Motor Ready
                self.serial.SerialIpt(self, self.data.iptSPACE, 20)
            else:
                ErrorMsg = f"Failure to estabish UART connection using {self.clicked_com.get()} "
                messagebox.showerror("showerror", ErrorMsg)
        else:
            
            # Landing mode called in case the motor is still functionning
            self.serial.SerialIpt(self, self.data.iptSPACE, 30)
            # Closing the Serial COM
            self.serial.SerialClose(self)
            # Closing the Motor Ready Manager
            self.motor_ready.MotorReadyClose()
            
            self.data.ClearData()

            InfoMsg = f"UART connection using {self.clicked_com.get()} is now closed"
            messagebox.showwarning("showinfo", InfoMsg)
            self.btn_connect["text"] = "Connect"
            self.btn_refresh["state"] = "active"
            self.drop_baud["state"] = "active"
            self.drop_com["state"] = "active"

# TAG_IHM_003
class MotorReadyGUI():
    def __init__(self, root, serial, data):
        '''
        Initialize main widgets for Motor Ready GUI
        '''
        self.root = root
        self.serial = serial
        self.data = data

        # Build MotorReadyGUI Static Elements
        self.frame = LabelFrame(root, text="Motor Ready",
                                padx=5, pady=5, bg="white", width=60)
        ## Button to start the Manual Mode Term
        self.btn_mode2 = Button(self.frame, text="Manual Mode Term", width=15, 
                                command=self.ModeChoice2)
        ## Button to start the Auto Mode
        self.btn_mode3 = Button(self.frame, text="Auto Mode", width=15,
                                command=self.ModeChoice3)
        ## Button to start the Calibration Mode
        self.btn_mode4 = Button(self.frame, text="Calibration Mode", width=15,
                                command=self.ModeChoice4)
        ## Button to start the Trip Mode
        self.btn_mode5 = Button(self.frame, text="Trip Mode", width=15,
                                command=self.ModeChoice5)

        # Extending the GUI
        self.MotorReadyOpen()
        
    def MotorReadyOpen(self):
        '''
        Method to display all the widgets 
        '''
        self.root.geometry("360x220")
        self.frame.grid(row=4, column=0, rowspan=3, columnspan=5,
                        padx=5, pady=5)
        self.btn_mode2.grid(row=0, column=1, 
                                  padx=5, pady=5)
        self.btn_mode3.grid(row=0, column=2, 
                                  padx=5, pady=5)
        self.btn_mode4.grid(row=1, column=1, 
                                  padx=5, pady=5)
        self.btn_mode5.grid(row=1, column=2, 
                                  padx=5, pady=5)
        
    
    def MotorReadyClose(self):
        '''
        Method to close the Motor Ready GUI and destroy the widgets
        '''
        # Must destroy all the elements so that they are not kept in memory
        for widget in self.frame.winfo_children():
            widget.destroy()
        self.frame.destroy()
        self.root.geometry("360x110")
        
    def ModeChoice2(self):
        """
        Method to redirect on manual term mode
        """
        self.manual_term_mode = ModeTermGUI(self.root, self.serial, self.data)
        self.serial.SerialIpt(self, self.data.ipt2, 30)
    
    def ModeChoice3(self):
        """
        Method to redirect on auto mode
        """
        self.auto_mode = AutoModeGUI(self.root, self.serial, self.data)
        self.serial.SerialIpt(self, self.data.ipt3, 30)
        
    def ModeChoice4(self):
        """
        Method to redirect on calibration mode
        """
        self.calibration_mode = CalibrationGUI(self.root, self.serial, self.data)
        # It permits to initialize the Gyro
        self.serial.SerialIpt(self, self.data.ipt3, 30)
        self.serial.SerialIpt(self, self.data.iptSPACE, 30)
        # Use the manual term mode (gas values)
        self.serial.SerialIpt(self, self.data.ipt2, 30) 
        
    def ModeChoice5(self):
        """
        Method to redirect on trip mode
        """
        self.trip_mode = TripModeGUI(self.root, self.serial, self.data)
        self.serial.SerialIpt(self, self.data.ipt3, 30)

# TAG_IHM_004            
class ModeTermGUI():
    def __init__(self, root, serial, data):
        '''
        Initialize main widgets for Manual Mode Term GUI
        '''        
        self.root = root
        self.serial = serial
        self.data = data

        # Build AutoModeGUI Static Elements
        self.frame = LabelFrame(root, text="Manual Mode Term",
                                padx=5, pady=5, bg="white", width=60)
        ## Angle consigne
        self.label_consigne = Label(
            self.frame, text="Gas (%): ", bg="white", width=15, anchor="w")
        self.label_screen = Label(self.frame, text="0", bg="white", width=5)
        self.consigne_box = Entry(self.frame, width=5)
        ## Button to start the engine
        self.btn_go_consigne = Button(self.frame, text="GO!", width=10,
                                      command=self.start_manual_term_mode)
        ## Button to stop the engine by landing
        self.btn_stop_landing = Button(self.frame, text="STOP!", width=10,
                                       state="disabled",
                                      command=self.stop_manual_term_mode) 
        self.btn_plus = Button(self.frame, text="+", width=5,
                               command=self.plus_gas_value)
        self.btn_minus = Button(self.frame, text="-", width=5,
                                state="disabled",
                               command=self.minus_gas_value)
        self.change_mode = Button(self.frame, text="Change Mode", width=15,
                                  command=self.ModeTermClose)

        # Extending the GUI
        self.ModeTermOpen()

    def ModeTermOpen(self):
        '''
        Method to display all the widgets 
        '''
        self.root.geometry("960x120")
        self.frame.grid(row=1, column=5, rowspan=3,
                        columnspan=5, padx=5, pady=5)
        self.label_consigne.grid(row=1, column=1)
        self.label_screen.grid(row=1, column=2, 
                               padx=5, pady=5)
        self.consigne_box.grid(row=1, column=3, 
                               padx=5, pady=5)
        self.btn_go_consigne.grid(row=1, column=5, 
                                  padx=5, pady=5)
        self.btn_stop_landing.grid(row=1, column=6, 
                                  padx=5, pady=5)
        self.change_mode.grid(row=1, column=7, 
                                  padx=5, pady=5)
        self.btn_minus.grid(row=2, column=3, 
                                  padx=5, pady=5)
        self.btn_plus.grid(row=2, column=4, 
                                  padx=5, pady=5)
        
    def ModeTermClose(self):
        '''
        Method to close the Mode Term GUI and destroy the widgets
        '''
        self.serial.SerialIpt(self, self.data.iptSPACE, 30)
        # Must destroy all the elements so that they are not kept in memory
        for widget in self.frame.winfo_children():
            widget.destroy()
        self.frame.destroy()
        
        self.motor_ready = MotorReadyGUI(self.root, self.serial, self.data)
        
    def start_manual_term_mode(self):
        """
        Method to start the engine after entering the gas value, 
        clicking on the button "GO!" and checking if it is a correct value
        """ 
        # Take the gas value in the "Entry" widget
        gas_value = self.consigne_box.get()
        self.consigne_box.delete(0,"end")
        
        if gas_value.isdigit():
            gas_value = int(gas_value)
            if (gas_value >= 0) and (gas_value <= 10):
                self.btn_stop_landing["state"] = "active"
                self.serial.ser.write(str(gas_value).encode())
                self.serial.SerialIpt(self, self.data.iptENTER, 30)
                self.label_screen["text"] = gas_value
            else:
                ErrorMsg = "Please enter a number between 0 and 10. If you want a number over 10, please use the '+' symbol."
                messagebox.showerror("showerror", ErrorMsg)
        else:
            ErrorMsg = "Please enter a positive integer."
            messagebox.showerror("showerror", ErrorMsg)
            
        if self.label_screen["text"] == "0":
            self.btn_minus["state"] = "disabled"
        else:
            self.btn_minus["state"] = "active"
        
    def minus_gas_value(self):
        """
        Method to remove 1% of gas value
        """
        self.consigne_box.delete(0,"end")
        
        gas_entry = int(self.label_screen["text"])
        gas_entry -=1
        self.label_screen["text"] = str(gas_entry)
        self.serial.SerialIpt(self, self.data.iptMINUS, 30)
        
        if gas_entry == 0:
            self.btn_stop_landing["state"] = "disabled"
            
        if self.label_screen["text"] == "0":
            self.btn_minus["state"] = "disabled"
        else:
            self.btn_minus["state"] = "active"
    
    def plus_gas_value(self):
        """
        Method to add 1% of gas value
        """
        self.consigne_box.delete(0,"end")
        
        gas_entry = int(self.label_screen["text"])
        gas_entry +=1
        self.label_screen["text"] = str(gas_entry)
        self.serial.SerialIpt(self, self.data.iptPLUS, 30)
        
        if self.label_screen["text"] == "0":
            self.btn_minus["state"] = "disabled"
        else:
            self.btn_minus["state"] = "active"
            self.btn_stop_landing["state"] = "active"
        
    def stop_manual_term_mode(self):
        """
        Method to stop the engine and landing after clicking on the button "STOP!"
        """        
        self.label_screen["text"] = "0"
        # Landing & Return to Motor Ready
        self.serial.SerialIpt(self, self.data.iptSPACE, 30)
        # Go to Manual Term Mode
        self.serial.SerialIpt(self, self.data.ipt2, 30)
        
        self.btn_stop_landing["state"] = "disabled"
        self.btn_minus["state"] = "disabled"
            
# TAG_IHM_005   
class AutoModeGUI():
    def __init__(self, root, serial, data):
        '''
        Initialize main widgets for Auto Mode GUI
        '''
        self.root = root
        self.serial = serial
        self.data = data

        # Build AutoModeGUI Static Elements
        self.frame = LabelFrame(root, text="Auto Mode",
                                padx=5, pady=5, bg="white", width=60)
        self.label_screen = Label(self.frame, text="0", bg="white", width=5)
        ## Angle consigne
        self.label_consigne = Label(
            self.frame, text="Angle (°): ", bg="white", width=15, anchor="w")
        self.consigne_box = Entry(self.frame, width=5)
        ## Button to start the engine
        self.btn_go_consigne = Button(self.frame, text="GO!", width=10,
                                      command=self.start_auto_mode)
        ## Button to stop the engine by landing
        self.btn_stop_landing = Button(self.frame, text="STOP!", width=10,
                                       state="disabled",
                                      command=self.stop_auto_mode)
        ## kp
        self.label_kp = Label(
            self.frame, text="kp:\t\t0,", bg="white", width=15, anchor="w")
        self.kp_box = Entry(self.frame, width=5)
        ### Default value
        self.kp_box.insert(0, "001")
        
        ## ki
        self.label_ki = Label(
            self.frame, text="ki:\t\t0,", bg="white", width=15, anchor="w")
        self.ki_box = Entry(self.frame, width=5)
        ### Default value
        self.ki_box.insert(0, "018")
        
        ## kd
        self.label_kd = Label(
            self.frame, text="kd:\t\t0,", bg="white", width=15, anchor="w")
        self.kd_box = Entry(self.frame, width=5)
        ### Default value
        self.kd_box.insert(0, "1")
        
        ## Button to come back to default set of kp/ki/kd values
        self.btn_default_set_k = Button(self.frame, text="Default", width=10,
                                      command=self.default_k_values)
        
        self.change_mode = Button(self.frame, text="Change Mode", width=15,
                                  command=self.AutoModeClose)
        
        # Extending the GUI
        self.AutoModeOpen()
        
    def AutoModeOpen(self):
        '''
        Method to display all the widgets 
        '''
        self.root.geometry("950x170")
        self.frame.grid(row=1, column=5, rowspan=3,
                        columnspan=5, padx=5, pady=5)
        self.label_screen.grid(row=1, column=2)
        self.label_consigne.grid(row=1, column=1)
        self.consigne_box.grid(row=1, column=3, 
                               padx=5, pady=5)
        self.btn_go_consigne.grid(row=1, column=4, 
                                  padx=5, pady=5)
        self.btn_stop_landing.grid(row=1, column=5, 
                                  padx=5, pady=5)
        self.change_mode.grid(row=1, column=6, 
                               padx=5, pady=5)
        self.label_kp.grid(row=2, column=1)
        self.kp_box.grid(row=2, column=2, 
                               padx=5, pady=5)
        self.label_ki.grid(row=3, column=1)
        self.ki_box.grid(row=3, column=2, 
                               padx=5, pady=5)
        self.label_kd.grid(row=4, column=1)
        self.kd_box.grid(row=4, column=2, 
                               padx=5, pady=5)
        self.btn_default_set_k.grid(row=3, column=3, 
                               padx=5, pady=5)

    def AutoModeClose(self):
        '''
        Method to close the Auto Mode GUI and destroy the widgets
        '''
        self.serial.SerialIpt(self, self.data.iptSPACE, 30)
        # Must destroy all the elements so that they are not kept in memory
        for widget in self.frame.winfo_children():
            widget.destroy()
        self.frame.destroy()
        
        self.motor_ready = MotorReadyGUI(self.root, self.serial, self.data)
        
    def isAngle(self):
        """
        Method to check if the entry is a correct angle value
        """
        angle_entry = self.consigne_box.get()
        
        
        if angle_entry.isdigit():
            angle_entry = int(angle_entry)
            if (angle_entry >= 0) and (angle_entry <= 90):
                self.btn_go_consigne["state"] = "active"
            else:
                self.btn_go_consigne["state"] = "disabled"
                ErrorMsg = "Please enter a number between 0 and 90."
                messagebox.showerror("showerror", ErrorMsg)
                self.consigne_box.delete(0,"end")
        else:
            self.btn_go_consigne["state"] = "disabled"
            ErrorMsg = "Please enter a positive integer."
            messagebox.showerror("showerror", ErrorMsg)
            self.consigne_box.delete(0,"end")
        
    def start_auto_mode(self):
        """
        Method to start the engine after entering the angle value,
        clicking on the button "GO!" and checking if it is a correct value
        """
        # Take the angle value in the "Entry" widget
        angle_value = self.consigne_box.get()
        self.consigne_box.delete(0,"end")
        
        if angle_value.isdigit():
            angle_value = int(angle_value)
            if (angle_value >= 0) and (angle_value <= 90):
                self.btn_stop_landing["state"] = "active"
                self.btn_default_set_k["state"] = "disabled"
                if self.kp_box["state"] == "disabled":
                    self.serial.SerialIpt(self, self.data.iptw, 30)
                    self.serial.ser.write(str(angle_value).encode())
                    self.serial.SerialIpt(self, self.data.iptENTER, 30)
                    self.label_screen["text"] = angle_value
                elif self.kp_box["state"] == "normal":
                    # Take the kp value in the "Entry" widget
                    kp_value = self.kp_box.get()
                    # Take the ki value in the "Entry" widget
                    ki_value = self.ki_box.get()
                    # Take the kd value in the "Entry" widget
                    kd_value =self.kd_box.get()
                    # Communicate the values of the Entry widgets
                    ## Angle
                    self.serial.ser.write(str(angle_value).encode())
                    self.serial.SerialIpt(self, self.data.iptENTER, 30)
                    self.label_screen["text"] = angle_value
                    ## kp
                    self.serial.SerialIpt(self, self.data.iptp, 30)
                    self.serial.ser.write(kp_value.encode())
                    self.serial.SerialIpt(self, self.data.iptENTER, 30)
                    ## ki
                    self.serial.SerialIpt(self, self.data.ipti, 30)
                    self.serial.ser.write(ki_value.encode())
                    self.serial.SerialIpt(self, self.data.iptENTER, 30)
                    ## kd
                    self.serial.SerialIpt(self, self.data.iptd, 30)
                    self.serial.ser.write(kd_value.encode())
                    self.serial.SerialIpt(self, self.data.iptENTER, 30)
                    
                    self.kp_box["state"] = "disabled"
                    self.ki_box["state"] = "disabled"
                    self.kd_box["state"] = "disabled"
            else:
                ErrorMsg = "Please enter a number between 0 and 90."
                messagebox.showerror("showerror", ErrorMsg)
        else:
            ErrorMsg = "Please enter a positive integer."
            messagebox.showerror("showerror", ErrorMsg)
                
    def stop_auto_mode(self):
        """
        Method to stop the engine and landing after clicking on the button "STOP!"
        """
        self.label_screen["text"] = "0"
        # Landing & Return to Motor Ready
        self.serial.SerialIpt(self, self.data.iptSPACE, 30)
        # Go to Auto Mode (for now, we need this line too because we don't have a menu for modes, we directly go to Auto Mode)
        self.serial.SerialIpt(self, self.data.ipt3, 30)
        
        self.kp_box["state"] = "normal"
        self.ki_box["state"] = "normal"
        self.kd_box["state"] = "normal"
        
        self.btn_stop_landing["state"] = "disabled"
        self.btn_default_set_k["state"] = "active"
            
    def default_k_values(self):
        """
        Method to reset the kp/i/d values to default ones
        """
        self.kp_box.delete(0,"end")
        self.kp_box.insert(0, "001")
        self.ki_box.delete(0,"end")
        self.ki_box.insert(0, "018")
        self.kd_box.delete(0,"end")
        self.kd_box.insert(0, "1")
        
# TAG_IHM_006
class CalibrationGUI():
    def __init__(self, root, serial, data):
        '''
        Initialize main widgets for Calibration GUI
        '''
        self.root = root
        self.serial = serial
        self.data = data

        # Build AutoModeGUI Static Elements
        self.frame = LabelFrame(root, text="Calibration Mode",
                                padx=5, pady=5, bg="white", width=60)
        
        self.btn_browse_file = Button(self.frame, text="Select File", width=10,
                                      command=self.browseFile)
        
        self.change_mode = Button(self.frame, text="Change Mode", width=15,
                                  command=self.CalibrationModeClose)
        
        self.btn_show_graph = Button(self.frame, text="Show Graph", width=10,
                                     state="disabled",
                                      command=self.showgraph)
        self.graph_type = 0
        self.gyro_issue = 0
        
        # Extending the GUI
        self.CalibrationModeOpen()
        
    def CalibrationModeOpen(self):
        '''
        Method to display all the widgets 
        '''
        self.root.geometry("600x110")
        self.frame.grid(row=1, column=5, rowspan=3,
                        columnspan=5, padx=5, pady=5)
        self.btn_browse_file.grid(row=1, column=1, 
                                  padx=5, pady=5)
        self.change_mode.grid(row=1, column=2, 
                                  padx=5, pady=5)
        self.btn_show_graph.grid(row=2, column=1, 
                                  padx=5, pady=5)
        
    def CalibrationModeClose(self):
        '''
        Method to close the Calibration Mode GUI and destroy the widgets
        '''
        self.serial.SerialIpt(self, self.data.iptSPACE, 30)
        # Must destroy all the elements so that they are not kept in memory
        for widget in self.frame.winfo_children():
            widget.destroy()
        self.frame.destroy()
        
        self.motor_ready = MotorReadyGUI(self.root, self.serial, self.data)
        self.data.ClearData()
        
    def browseFile(self):
        """
        Method to search for txt files and do the calibration protocol
        """
        self.data.ClearData()
        self.graph_type = 0
        self.btn_show_graph["state"] = "disabled"
        self.root.filename = filedialog.askopenfilename(
            initialdir="/", 
            title="File Explorer",
            filetypes=(
                ("Text files","*.txt"),
                       ("All files","*.*")
                       )
            )
        
        myfile = open(self.root.filename, "rt")
        content = myfile.readlines()
        myfile.close()
        new_content=[]
        
        # We have lines with 2 values, separated by \t
        # 1st one is the gas value
        # 2nd one is the time we want to spend on this value
        # so here is the pattern : 2 integers and \t between them (nothing before, nothing after)
        # other lines will be ignored and considered as errors or comments
        pattern = re.compile("^[0-9]+\t[0-9]+$")
        for i in range(len(content)):
            if pattern.match(content[i]):
                new_content.append(content[i].split("\t"))   

        for i in range(len(new_content)):
            new_content[i][1] = int(new_content[i][1])
            self.serial.ser.write(new_content[i][0].encode()) 
            self.serial.SerialIpt(self, self.data.iptENTER, 30)  
            # You can choose or not to add a delay before recording the position values
            #time.sleep(1)
            self.serial.ser.write(self.data.iptr.encode())
            timeout = time.time() + new_content[i][1]
            self.serial.SerialOpt(self, timeout)
            self.serial.ser.write(self.data.ipts.encode())
             
            time.sleep(.1)
    
        self.serial.SerialIpt(self, self.data.iptSPACE, 30)
        
        title_file = time.strftime("%Y-%m-%d-%Hh%M-Calibration_mode_Results")        
        self.df = pd.DataFrame(self.data.record, 
                          columns = ['Gas', 'PositionX', 'PositionY', 
                                     "Ax Raw", "Ay Raw", "Az Raw",
                                     "Gx Raw", "Gy Raw", "Gz Raw"])
        saving_path = filedialog.asksaveasfile(initialfile=title_file,
                                               mode='w',
                                               defaultextension='.csv',
                                               filetypes=(
                                                   ("CSV files","*.csv"),
                                                          ("All files","*.*")
                                                          ))
        self.df.to_csv(saving_path, sep=';', decimal=',', encoding='utf-8', index=False, line_terminator='\n')
        print(self.data.record)
        self.btn_show_graph["state"] = "active"
        
        self.serial.SerialIpt(self, self.data.ipt2, 30)
        
        if len(new_content) == 1:
            self.graph_type = 1
        
    def showgraph(self):
        """
        Method to show the graphic of the simulation
        """
        if self.graph_type == 1:
            list_time = []
           
            for i in range(len(self.df)):
                list_time.append(i*20)
            df2 = self.df.assign(Time=list_time) 
            
            fig = df2.plot.scatter(
                x=df2.columns[-1],
                y=df2.columns[1]).get_figure()
            
            new_window = Tk()
            new_window.geometry("500x400")
            new_window.title("Calibration Graphic - Dynamic")
            
            canvas= FigureCanvasTkAgg(fig, new_window)
            canvas.draw()
            
            toolbar = NavigationToolbar2Tk(
                canvas, new_window)
            toolbar.update()
            
            canvas.get_tk_widget().pack(
                side=tk.BOTTOM, fill=tk.BOTH, expand=True)
            
            new_window.mainloop()
        else:    
            fig = self.df.plot.scatter(
                x=self.df.columns[0],
                y=self.df.columns[1]).get_figure()
            
            new_window = Tk()
            new_window.geometry("500x400")
            new_window.title("Calibration Graphic - Static")
            
            canvas= FigureCanvasTkAgg(fig, new_window)
            canvas.draw()
            
            toolbar = NavigationToolbar2Tk(
                canvas, new_window)
            toolbar.update()
            
            canvas.get_tk_widget().pack(
                side=tk.BOTTOM, fill=tk.BOTH, expand=True)
            
            new_window.mainloop()
            
        
# TAG_IHM_007
class TripModeGUI():
    def __init__(self, root, serial, data):
        '''
        Initialize main widgets for Trip Mode GUI
        '''
        self.root = root
        self.serial = serial
        self.data = data

        # Build AutoModeGUI Static Elements
        self.frame = LabelFrame(root, text="Trip Mode",
                                padx=5, pady=5, bg="white", width=60)
        
        self.btn_browse_file = Button(self.frame, text="Select File", width=10,
                                      command=self.browseFile)
        self.change_mode = Button(self.frame, text="Change Mode", width=15,
                                  command=self.TripModeClose)
        
        # Extending the GUI
        self.TripModeOpen()
        
    def TripModeOpen(self):
        '''
        Method to display all the widgets 
        '''
        self.root.geometry("600x110")
        self.frame.grid(row=1, column=5, rowspan=3,
                        columnspan=5, padx=5, pady=5)
        self.btn_browse_file.grid(row=1, column=1, 
                                  padx=5, pady=5)
        self.change_mode.grid(row=1, column=2, 
                                  padx=5, pady=5)
        
    def TripModeClose(self):
        '''
        Method to close the Trip Mode GUI and destroy the widgets
        '''
        self.serial.SerialIpt(self, self.data.iptSPACE, 30)
        self.serial.SerialIpt(self, self.data.iptENTER, 30)
        # Must destroy all the elements so that they are not kept in memory
        for widget in self.frame.winfo_children():
            widget.destroy()
        self.frame.destroy()
        
        self.motor_ready = MotorReadyGUI(self.root, self.serial, self.data)
        
    def browseFile(self):
        """
        Method to search for txt files and do the "séquence de vol"/trip
        """
        self.data.ClearData()
        
        self.root.filename = filedialog.askopenfilename(
            initialdir="/", 
            title="File Explorer",
            filetypes=(
                ("Text files","*.txt"),
                       ("All files","*.*")
                       )
            )

        myfile = open(self.root.filename, "rt")
        content = myfile.readlines()
        myfile.close()
        new_content=[]
        # 1st loop change the raw elements in something we will use in the 2nd loop
        # 'content' holds lists of 2 elements : 1st one is a str which corresponds to the angle value we will assign to Auto Mode
        # 2nd element is an int, the time (in seconds) we want to wait on this position before next position
        pattern = re.compile("^[0-9]+\t[0-9]+$")
        for i in range(len(content)):
            if pattern.match(content[i]):
                new_content.append(content[i].split("\t"))  
            
        for i in range(len(new_content)):
            new_content[i][1] = int(new_content[i][1])
            self.serial.ser.write(new_content[i][0].encode())
            self.serial.SerialIpt(self, self.data.iptENTER, 30)
            self.serial.ser.write(self.data.iptr.encode())
            
            timeout = time.time() + new_content[i][1]
            self.serial.SerialOpt(self, timeout)
            self.serial.ser.write(self.data.ipts.encode())
            time.sleep(.1)
            self.serial.SerialIpt(self, self.data.iptw, 30)
        
        self.serial.SerialIpt(self, self.data.iptSPACE, 30)
        
        title_file = time.strftime("%Y-%m-%d-%Hh%M-Trip_mode_Results")        
        self.df = pd.DataFrame(self.data.record, 
                          columns = ['Angle', 'PositionX', 'PositionY', 
                                     "Ax Raw", "Ay Raw", "Az Raw",
                                     "Gx Raw", "Gy Raw", "Gz Raw", 
                                     "Erreur P", "Erreur I", "Erreur D"])
        saving_path = filedialog.asksaveasfile(initialfile=title_file,
                                               mode='w',
                                               defaultextension='.csv',
                                               filetypes=(
                                                   ("CSV files","*.csv"),
                                                          ("All files","*.*")
                                                          ))
        self.df.to_csv(saving_path, sep=';', decimal=',', encoding='utf-8', index=False, line_terminator='\n')
        print(self.data.record)
        

if __name__ == "__main__":
    RootGUI()
    ComGui()
    MotorReadyGUI()
    ModeTermGUI()
    AutoModeGUI()
    CalibrationGUI()
    TripModeGUI()