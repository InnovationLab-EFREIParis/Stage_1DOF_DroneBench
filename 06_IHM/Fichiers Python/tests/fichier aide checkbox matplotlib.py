# -*- coding: utf-8 -*-
"""
Created on Tue Jan 31 16:22:04 2023

@author: panni
"""

import matplotlib
matplotlib.use('TkAgg')
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib.backend_bases import key_press_handler
from matplotlib.figure import Figure
from tkinter import *
import tkinter as Tk
  
root = Tk.Tk()
root.wm_title("Embedding in TK")
 
nbvar=2
 
#figure1
  
f1 = Figure(figsize=(5,4), dpi=100)
a1 = f1.add_subplot(111)
 
x = [0, 1, 2]
y = [0, 1, 3] 
 
lines=[]
lines.append(a1.plot(x, y, lw=5, visible=True))
lines.append(a1.plot(y, x, lw=5, visible=True))
 
a1.set_xlabel('time (s)')
a2 = a1.twinx()
  
  
canvas = FigureCanvasTkAgg(f1, master=root)
canvas.draw()
 
canvas.get_tk_widget().pack(side=Tk.TOP, fill=Tk.BOTH, expand=1)
 
  
toolbar = NavigationToolbar2Tk( canvas, root )
#toolbar.update()
canvas._tkcanvas.pack(side=Tk.TOP, fill=Tk.BOTH, expand=1)
 
 
#figure2
 
f2 = Figure(figsize=(5,4), dpi=100)
a1 = f2.add_subplot(111)
 
x = [0, 1, 2]
y = [0, -1, -3] 
lines2 = a1.plot(x, y, lw=5, visible=True)
  
  
canvas2 = FigureCanvasTkAgg(f2, master=root)
canvas2.draw()
canvas2.get_tk_widget().pack(side=Tk.TOP, fill=Tk.BOTH, expand=1)
  
toolbar = NavigationToolbar2Tk( canvas2, root )
toolbar.update()
canvas2._tkcanvas.pack(side=Tk.TOP, fill=Tk.BOTH, expand=1)
  
 
def on_key_event(event):
    print('you pressed %s'%event.key)
    key_press_handler(event, canvas, toolbar)
  
canvas.mpl_connect('key_press_event', on_key_event)
def _quit():
    root.quit()
    root.destroy()
  
def hideline():
    for i in range(0,nbvar) :
        if(varhide[i].get()==1):
            lines[i][0].set_visible(False)
        if(varhide[i].get()==0):
            lines[i][0].set_visible(True)
    canvas.draw()
 
button = Tk.Button(master=root, text='Quit', command=_quit)
button.pack(side=Tk.BOTTOM)
 
option = Frame(root)
option.pack()
 
var = IntVar()
var2 = IntVar()
 
label=[]
label.append('courbe1')
label.append('courbe2')
 
varhide= []
for i in range(0,nbvar) :
    varhide.append(IntVar())
 
for i in range(0,nbvar) :
    c = Checkbutton(option, text=label[i], variable=varhide[i], command=hideline)
    c.pack()
  
Tk.mainloop()