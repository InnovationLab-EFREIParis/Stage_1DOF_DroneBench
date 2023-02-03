# -*- coding: utf-8 -*-
"""
Created on Tue Jan 31 16:22:04 2023

@author: panni
"""

import matplotlib
matplotlib.use('TkAgg')
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
# from matplotlib.backend_bases import key_press_handler
from matplotlib.figure import Figure
from tkinter import *
import tkinter as Tk
  
root = Tk.Tk()
root.geometry("500x500")
root.title("Embedding in TK")
 
nbvar=2
  
f1 = Figure(figsize=(4,3), dpi=100)
a1 = f1.add_subplot(111)
 
x = [0, 1, 2]
y2 = [0, 1, 4]
y = [0, 1, 3] 
 
lines=[]
lines.append(a1.plot(x, y, lw=5, visible=True))
lines.append(a1.plot(x, y2, lw=5, visible=True))
print(lines)
 
a1.set_xlabel('time (s)')
a2 = a1.twinx()
  
  
canvas = FigureCanvasTkAgg(f1, master=root)
canvas.draw()
 
canvas.get_tk_widget().pack(side=Tk.TOP, fill=Tk.BOTH, expand=1)
 
  
toolbar = NavigationToolbar2Tk( canvas, root )
#toolbar.update()
canvas._tkcanvas.pack(side=Tk.TOP, fill=Tk.BOTH, expand=1)
 
# def on_key_event(event):
#     print('you pressed %s'%event.key)
#     key_press_handler(event, canvas, toolbar)
  
# canvas.mpl_connect('key_press_event', on_key_event)
  
def hideline():
    for i in range(0,nbvar) :
        print(lines[i][0])
        if(varhide[i].get()==1):
            print(varhide[i])
            lines[i][0].set_visible(False)
        if(varhide[i].get()==0):
            print(varhide[i])
            lines[i][0].set_visible(True)
    canvas.draw()

option = Frame(root)
option.pack()
 
label=[]
label.append('courbe1')
label.append('courbe2')
 
varhide= []
for i in range(0,nbvar) :
    varhide.append(IntVar())
 
for i in range(0,nbvar) :
    c = Checkbutton(
        option, text=label[i], variable=varhide[i], 
        command=hideline)
    c.pack()
  
Tk.mainloop()