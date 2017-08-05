import tkinter
# import tkinter.ttk
from tkinter import ttk
from tkinter import messagebox
import serial
from datetime import datetime, timedelta
from tkinter.filedialog import asksaveasfilename, askopenfilename
import json
import webbrowser
from .anim import Animtab
from .config import Configtab
from .info import Infotab
import serial.tools.list_ports

import re
import gettext
de = gettext.translation('ottolcgui', localedir='locale', languages=['de_DE'])
_ = de.gettext
class App():
    expected_firmware = 2
    expected_api = 3
    gui_patchlevel=100
    def __init__(self, master, serialDevice=None):
        self.master = master
        master.wm_title("OttO-LC V 1.%s.%s.%s"%(self.expected_firmware, self.expected_api, self.gui_patchlevel))


        if not serialDevice:
            serialDevice = self._findSerialPort()
        if serialDevice:
            self.ser = serial.Serial(serialDevice)
            self.use_serial = True
            self._initserial()
        else:
            self.use_serial = False

        tabs = ttk.Notebook(master)

        self.nosend = True
        self.lastsend = datetime.now()

        animframe = ttk.Frame(tabs)
        configframe = ttk.Frame(tabs)
        infoframe = ttk.Frame(tabs)

        tabs.add(animframe, text=_('animation & movement'))
        tabs.add(configframe, text=_('bot configuration'))
        tabs.add(infoframe, text=_('help & info'))
        tabs.pack()

        self.animwidget = Animtab(animframe, self)
        self.configwidget = Configtab(configframe, self)
        self.infowidget = Infotab(infoframe, self)

        self.nosend = False
        self.mov(None)


    def _findSerialPort(self):
        ports = serial.tools.list_ports.comports()
        for i in range(0,len(ports)):
            desc=ports[i].description
            print(desc)
            if desc.find("CH340", 0, len(desc)) >= 0:
                print("FTDI found on %s" % (ports[i].device))
                return ports[i].device
        return False
        

    def _initserial(self):
        self.ser.timeout=0.1
        try:
            while True:
                line = self.ser.readline().strip()
                if len(line)==0:
                    print(".",end="")
                    continue
                print(line)
                if line==b".0 boot_ok":
                    break
        except Exception as e:
            raise
        fwvers, apivers = self.getInfo()
        if (fwvers!= self.expected_firmware)or(apivers!=self.expected_api):
            messagebox.showerror(_("OttO Firmware error (1.%s.%s.%s)"%(self.expected_firmware, self.expected_api, self.gui_patchlevel)), _("Got Firmware version %s and API Version %s, please update the gui and or ottobot")%(fwvers, apivers))
            self.openhelp()
            self.master.destroy()
            raise Exception("Got Firmware version %s and API Version %s"%(fwvers, apivers))



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
        self.lastsend = now
        tempstr = "%d %d %d %d \n" % (
            self.animwidget.getrf(), self.animwidget.getlf(), self.animwidget.getrl(), self.animwidget.getll(), 
            )
        cmdstr = "! setservos %s \n" % (tempstr)
            
        self._sendcmd(cmdstr)


    def _sendcmd(self, cmd, verbose = False):
        if verbose == True:
            print("<<<<<<<<<< New Command >>>>>>>>>>")
        print("_sendcmd > command sent: " + str(cmd))
        if self.use_serial:
            self.ser.write(cmd.encode('utf-8'))
            while True:
                line = self.ser.readline().strip()
                if verbose == True:
                    print("_sendcmd > readline: " + str(line))
                if len(line)<1:
                    continue
                if line[0]!=ord('.'):
                    print(line)
                    continue
                if line[:2] == b'.0':
                    line = line[2:]
                else:
                    raise Exception(line)
                if verbose == True:
                    print("getResponse > final response: %s" % (line))
                print("_sendcmd > END")
                return line.strip()
        else:
            print(cmd[0:-1])
        

    def saveTrim(self, verbose = False):
        verbose=True
        if verbose == True:
            print("saveTrim > called")
        cmdstr = "! gettrims\n"
        response = self._sendcmd(cmdstr, True)
        if verbose == True:
            print("saveTrim > Response from _sendcmd: %s" % (response))
        pos = response.split(b' ')
        posInt = 4 * [0]
        for i in range(0, len(pos)):
            if pos[i] == b'':
                pos[i] = 0
            posInt[i]=int(pos[i])
        posInt[0] += self.animwidget.getrf()
        posInt[1] += self.animwidget.getlf()
        posInt[2] += self.animwidget.getrl()
        posInt[3] += self.animwidget.getll()
        cmdstr = "! settrims %d %d %d %d\n" % (posInt[0], posInt[1], posInt[2], posInt[3])
        response = self._sendcmd(cmdstr)
        self.animwidget.resetServos()
        self.testTrim()
    
    def resetTrim(self):
        cmdstr = "! settrims 0 0 0 0\n"
        self._sendcmd(cmdstr)

    def testTrim(self):
        cmdstr = "! trimtest\n"
        self._sendcmd(cmdstr)

    def getInfo(self):
        cmdstr = "! getinfo\n"
        response = self._sendcmd(cmdstr)
        print(response)
        pos = response.split(b' ')
        #posInt = 2 * [0]
        posInt = []
        #for i in range(0, len(pos)):
        for i in range(0, len(pos)):
            #if pos[i] == b'':
            #    pos[i] = 0
            #posInt[i]=int(pos[i])
            posInt.append(pos[i])
        print("Firmware Version: %s" % (posInt[0]))
        print("API Version: %s" % (posInt[1]))
        print("Compile date: %s" % (posInt[2:]))
        return (int(posInt[0]), int(posInt[1]))
        
    def openhelp(self):
        webbrowser.open_new("http://bckspc.de/otto?fw=%s&api=%s&patchlevel=%s"%(self.expected_firmware, self.expected_api, self.gui_patchlevel))
