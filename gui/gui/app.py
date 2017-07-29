import tkinter
# import tkinter.ttk
from tkinter import ttk
import serial
from datetime import datetime, timedelta
from tkinter.filedialog import asksaveasfilename, askopenfilename
import json
from .anim import Animtab
from .config import Configtab
import serial.tools.list_ports

class App():
    def __init__(self, master, serialDevice=None):
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

        if not serialDevice:
            serialDevice = self._findSerialPort()
        if serialDevice:
            self.ser = serial.Serial(serialDevice)
            self.use_serial = True
            self._initserial()
        else:
            self.use_serial = False

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
                line = self.ser.readline()
                if len(line)==0:
                    break
                print(line)
        except Exception as e:
            raise

    def _sendcmd(self, cmd):
        print("cmd: " + str(cmd))
        response = b''
        if self.use_serial:
            self.ser.write(cmd.encode('utf-8'))
            try:
                while True:
                    line = self.ser.readline()
                    if len(line)==0:
                        break
                    #print(line)
                    response += line
                    if line[0]==ord('.'):
                        #print("***************")
                        response += b'***************'
                        break
                #print(output)
                #return self.getResponse(response, True)
                return self.getResponse(response)
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
        self.lastsend = now
        tempstr = "%d %d %d %d \n" % (
            self.animwidget.getrf(), self.animwidget.getlf(), self.animwidget.getrl(), self.animwidget.getll(), 
            )
        cmdstr = "! setservos %s \n" % (tempstr)
            
        self._sendcmd(cmdstr)

    def getResponse(self, serialResponse, verbose = False):
        response = b''
        responselist = serialResponse.split(b'\n')
        for line in responselist:
            if line[-1:] == b'\r':
                line = line[:-1]
            if verbose == True:
                print("response: %s" % (line))
            if line and line[0] == ord('.'):
                if verbose == True:
                    print ("found valid resonse: %s" % (line))
                splitline = line.split(b' ')
                if len(splitline) > 1:
                    response = splitline[1]
        return response
        

    def saveTrim(self):
        cmdstr = "! gettrims\n"
        response = self._sendcmd(cmdstr)
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
        cmdstr = "trimtest\n" # needs to get an api command!!!
        self._sendcmd(cmdstr)

    def getInfo(self):
        cmdstr = "! getinfo\n" # needs to get an api command!!!
        self._sendcmd(cmdstr)
        
    # def log
