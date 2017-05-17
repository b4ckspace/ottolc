import tkinter
import serial
from datetime import datetime, timedelta
from tkinter.filedialog import asksaveasfilename, askopenfilename
import json
from .anim import Animtab

class App():
    def __init__(self, master, serial_device=None):
        if serial_device:
            self.ser = serial.Serial(serial_device)
            self.use_serial = True
        else:
            self.use_serial = False

        self.animwidget = Animtab(master, self)

    def _sendcmd(self, cmd):
        if self.use_serial:
            self.ser.write(cmd.encode('utf-8'))
        else:
            print(cmd[0:-1])