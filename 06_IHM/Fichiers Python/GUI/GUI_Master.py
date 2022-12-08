# -*- coding: utf-8 -*-
"""
Created on Thu Dec  1 10:27:04 2022

@author: Julien
"""

from tkinter import Tk, LabelFrame, Label, Button, Entry
from tkinter import messagebox, StringVar, OptionMenu

# Class to setup the main window
class RootGUI():
    def __init__(self, serial, data):
        '''
        Initializing the root GUI and other comps of the program
        '''
        self.root = Tk()
        self.root.title("Serial communication")
        self.root.geometry("360x120")
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
        self.serial.SerialIpt(self, self.data.ipt0)
        # Closing Serial COM
        self.serial.SerialClose(self)
        

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
            self.frame, text="Available Port(s): ", bg="white", width=15, anchor="w")
        self.label_bd = Label(
            self.frame, text="Baud Rate: ", bg="white", width=15, anchor="w")

        # Setup the Drop option menu
        self.baudOptionMenu()
        self.ComOptionMenu()

        # Add the control buttons for refreshing the COMs & Connect
        self.btn_refresh = Button(self.frame, text="Refresh",
                                  width=10,  command=self.com_refresh)
        self.btn_connect = Button(self.frame, text="Connect",
                                  width=10, state="disabled",  command=self.serial_connect)

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
            self.frame, self.clicked_com, *self.serial.com_list, command=self.connect_ctrl)

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

                # Display the Auto Mode manager
                self.auto_mode = AutoModeGUI(self.root, self.serial, self.data)
                # Access to Init UC
                self.serial.SerialIpt(self, self.data.iptENTER)
                # Access to Motor Ready
                self.serial.SerialIpt(self, self.data.ipt0)
                # Access to Auto Mode
                self.serial.SerialIpt(self, self.data.ipt3)
            else:
                ErrorMsg = f"Failure to estabish UART connection using {self.clicked_com.get()} "
                messagebox.showerror("showerror", ErrorMsg)
        else:
            
            # Landing mode called in case the motor is still functionning (if you click on 'Disconnect' button before 'STOP!' button)
            self.serial.SerialIpt(self, self.data.ipt0)
            # Closing the Serial COM
            self.serial.SerialClose(self)
            # Closing the Auto Mode Manager
            self.auto_mode.AutoModeClose()
            self.data.ClearData()

            InfoMsg = f"UART connection using {self.clicked_com.get()} is now closed"
            messagebox.showwarning("showinfo", InfoMsg)
            self.btn_connect["text"] = "Connect"
            self.btn_refresh["state"] = "active"
            self.drop_baud["state"] = "active"
            self.drop_com["state"] = "active"


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
        
        # Optional Graphic parameters
        self.padx = 20
        self.pady = 15

        # Extending the GUI
        self.AutoModeOpen()

    def AutoModeOpen(self):
        '''
        Method to display all the widgets 
        '''
        self.root.geometry("730x180")
        self.frame.grid(row=0, column=4, rowspan=3,
                        columnspan=5, padx=5, pady=5)
        self.label_consigne.grid(row=1, column=1)
        self.consigne_box.grid(row=1, column=2, 
                               padx=5, pady=5)
        self.btn_go_consigne.grid(row=1, column=3, 
                                  padx=5, pady=5)
        self.btn_stop_landing.grid(row=1, column=4, 
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
        # Must destroy all the elements so that they are not kept in memory
        for widget in self.frame.winfo_children():
            widget.destroy()
        self.frame.destroy()
        self.root.geometry("360x120")
        
    def start_auto_mode(self):
        """
        Method to start the engine after entering the angle value and
        clicking on the button "GO!"
        """
        self.btn_stop_landing["state"] = "active"
        self.btn_default_set_k["state"] = "disabled"
        # Take the angle value in the "Entry" widget
        angle_value = self.consigne_box.get()
        if self.kp_box["state"] == "disabled":
            self.serial.SerialIpt(self, self.data.iptw)
            self.serial.ser.write(angle_value.encode())
            self.serial.SerialIpt(self, self.data.iptENTER) 
        elif self.kp_box["state"] == "normal":
            # Take the kp value in the "Entry" widget
            kp_value = self.kp_box.get()
            # Take the ki value in the "Entry" widget
            ki_value = self.ki_box.get()
            # Take the kd value in the "Entry" widget
            kd_value =self.kd_box.get()
            # Communicate the values of the Entry widgets
            ## Angle
            self.serial.ser.write(angle_value.encode())
            self.serial.SerialIpt(self, self.data.iptENTER)
            ## kp
            self.serial.SerialIpt(self, self.data.iptx)
            self.serial.ser.write(kp_value.encode())
            self.serial.SerialIpt(self, self.data.iptENTER)
            ## ki
            self.serial.SerialIpt(self, self.data.ipty)
            self.serial.ser.write(ki_value.encode())
            self.serial.SerialIpt(self, self.data.iptENTER)
            ## kd
            self.serial.SerialIpt(self, self.data.iptz)
            self.serial.ser.write(kd_value.encode())
            self.serial.SerialIpt(self, self.data.iptENTER)
            
            self.kp_box["state"] = "disabled"
            self.ki_box["state"] = "disabled"
            self.kd_box["state"] = "disabled"
        
    def stop_auto_mode(self):
        """
        Method to stop the engine and landing after clicking on the button "STOP!"
        """
        # Landing & Return to Motor Ready
        self.serial.SerialIpt(self, self.data.ipt0)
        # Go to Auto Mode (for now, we need this line too because we don't have a menu for modes, we directly go to Auto Mode)
        self.serial.SerialIpt(self, self.data.ipt3)
        
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

if __name__ == "__main__":
    RootGUI()
    ComGui()
    AutoModeGUI()