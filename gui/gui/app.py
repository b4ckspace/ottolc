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
            self._initserial()
        else:
            self.use_serial = False

        tabs = ttk.Notebook(master)

        self.nosend = True
        self.lastsend = datetime.now()

        animframe = ttk.Frame(tabs)
        configframe = ttk.Frame(tabs)
        logframe = ttk.Frame(tabs)

        tabs.add(animframe, text='animation & movement')
        tabs.add(configframe, text='bot config')
        tabs.add(logframe, text='log')
        tabs.pack()

        self.animwidget = Animtab(animframe, self)
        self.configwidget = Configtab(configframe, self)

        self.nosend = False
        self.mov(None)

    def _initserial(self):
        self.ser.timeout=0.1
        try:
            while True:
                line = self.ser.readline()
                if len(line)==0:
                    break
                print(line)
        except Exception as e:
            raise

    def _sendcmd(self, cmd):
        print("cmd: " + str(cmd))
        if self.use_serial:
            self.ser.write(cmd.encode('utf-8'))
            try:
                while True:
                    line = self.ser.readline()
                    if len(line)==0:
                        break
                    print(line)
                    if line[0]==ord('.'):
                        print("***************")
                        break
            except Exception as e:
                raise
        else:
            print(cmd[0:-1])


    def calcabspos(self,pos):
        ## depricated function, because absPos is calculated in ottolc arduino code
        if len(pos.split()) == 5:
            l = list(map(lambda x:int(x)+90, pos.split()[0:-1]))
            l.append( int(pos.split()[-1]) )
        elif len(pos.split()) == 4:
            l = list(map(lambda x:int(x)+90, pos.split()[0:]))
        else:
            print("wrong position-string format: %d" % (pos))
        newlist = " ".join(map(str, l))
        return newlist

    def mov(self, _w):
        if self.nosend:
            return
        now = datetime.now()
        delta = now - self.lastsend
        if (now - self.lastsend) < timedelta(milliseconds=100):
            return
        print("mov...")
        self.lastsend = now
        tempstr = "%d %d %d %d \n" % (
            self.animwidget.getrf(), self.animwidget.getlf(), self.animwidget.getrl(), self.animwidget.getll(), 
            )
        cmdstr = "! setservos %s \n" % (tempstr)
            
        self._sendcmd(cmdstr)
        
    # def log
