import tkinter
# import tkinter.ttk
from tkinter import ttk
import serial
from datetime import datetime, timedelta
from tkinter.filedialog import asksaveasfilename, askopenfilename
import json
from .anim import Animtab
from .config import Configtab

class App():
    def __init__(self, master, serial_device=None):
        if serial_device:
            self.ser = serial.Serial(serial_device)
            self.use_serial = True
        else:
            self.use_serial = False

        tabs = ttk.Notebook(master)

        animframe = ttk.Frame(tabs)
        configframe = ttk.Frame(tabs)
        logframe = ttk.Frame(tabs)

        tabs.add(animframe, text='animation & movement')
        tabs.add(configframe, text='bot config')
        tabs.add(logframe, text='log')
        tabs.pack()

        self.animwidget = Animtab(animframe, self)
        self.configwidget = Configtab(configframe, self)

    def _sendcmd(self, cmd):
        if self.use_serial:
            self.ser.write(cmd.encode('utf-8'))
        else:
            print(cmd[0:-1])