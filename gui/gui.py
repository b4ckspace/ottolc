import tkinter
import serial
from datetime import datetime, timedelta
from tkinter.filedialog import asksaveasfilename, askopenfilename
import json
import sys


class App():
    def __init__(self, master, serial_device=None):
        if serial_device:
            self.ser = serial.Serial(serial_device)
            self.use_serial = True
        else:
            self.use_serial = False
        self.nosend = True
            
        frame = tkinter.Frame(master)
        self.lastsend = datetime.now()

        self.lf = tkinter.Scale(master, from_=0, to=180,
                                resolution=1, command=self.mov,
                                orient=tkinter.HORIZONTAL, label="left foot")
        self.lf.set(90)
        self.lf.grid(row=1, column=0)

        self.rf = tkinter.Scale(master, from_=0, to=180,
                                resolution=1, command=self.mov,
                                orient=tkinter.HORIZONTAL, label="right foot")
        self.rf.set(90)
        self.rf.grid(row=1, column=1)

        self.rl = tkinter.Scale(master, from_=0, to=180,
                                resolution=1, command=self.mov,
                                orient=tkinter.HORIZONTAL, label="right leg")
        self.rl.set(90)
        self.rl.grid(row=0, column=0)
        self.ll = tkinter.Scale(master, from_=0, to=180,
                                resolution=1, command=self.mov,
                                orient=tkinter.HORIZONTAL, label="left leg")
        self.ll.set(90)
        self.ll.grid(row=0, column=1)

        self.ts = tkinter.Scale(master, from_=1, to=1000,
                                orient=tkinter.VERTICAL, label="animdur")
        self.ts.set(500)
        self.ts.grid(row=0, column=2, rowspan=2)

        tkinter.Button(
            master,
            text="Reset",
            command=self.resetServos).grid(
            row=2,
            column=0)
        tkinter.Button(
            master,
            text="add frame",
            command=self.addframe).grid(
            row=2,
            column=2)
        tkinter.Button(
            master,
            text="play anim",
            command=self.playanim).grid(
            row=1,
            column=3)
        tkinter.Button(
            master,
            text="play selection",
            command=self.playanimsel).grid(
            row=2,
            column=3)
        tkinter.Button(
            master,
            text="delete selection",
            command=self.deleteselection).grid(
            row=0,
            column=3)

        tkinter.Button(
            master,
            text="save",
            command=self.save).grid(
            row=2,
            column=4)
        tkinter.Button(
            master,
            text="load",
            command=self.load).grid(
            row=3,
            column=4)

        self.framelist = tkinter.Listbox(master, selectmode=tkinter.EXTENDED)
        self.framelist.grid(row=0, column=4, rowspan=2)

        self.nosend = False
        self.mov(None)

    def mov(self, _w):
        if self.nosend:
            return
        now = datetime.now()
        delta = now - self.lastsend
        if (now - self.lastsend) < timedelta(milliseconds=100):
            return
        self.lastsend = now
        cmdstr = "! setservos %d %d %d %d \n" % (
            self.rf.get(), self.lf.get(), self.rl.get(), self.ll.get())
        self._sendcmd(cmdstr)

    def addframe(self):
        cmd = "%d %d %d %d %d" % (
            self.rf.get(), self.lf.get(), self.rl.get(), self.ll.get(), self.ts.get())
        self.framelist.insert(tkinter.END, cmd)

    def playanim(self):
        self._sendandplayanim(self.framelist.get(0, tkinter.END))

    def playanimsel(self):
        if len(self.framelist.curselection())==0:
            return
        self._sendandplayanim([self.framelist.get(0, tkinter.END)[i] for i in self.framelist.curselection()])

    def _sendandplayanim(self, animlist):
        self._sendcmd("! resetanim\n" )
        for i in animlist:
            self._sendcmd("! pushframe %s\n"%(i,))
        self._sendcmd("! playanim\n")

    def deleteselection(self):
        for i in reversed(self.framelist.curselection()):
            self.framelist.delete(i)


    def save(self):
        savepath = asksaveasfilename(defaultextension=".json")
        if not savepath:
            return
        with open(savepath, "w") as f:
            json.dump(self.framelist.get(0, tkinter.END),f)
        
    def load(self):
        loadpath = askopenfilename(filetypes=[("Json file","*.json")])
        if not loadpath:
            return
        with open(loadpath, "r") as f:
            obj = json.load(f)
            self.framelist.delete(0, tkinter.END)
            for e in obj:
                self.framelist.insert(tkinter.END, e)

    def resetServos(self):
        self.nosend = True
        self.lf.set(90)
        self.rf.set(90)
        self.ll.set(90)
        self.rl.set(90)
        self.nosend = False
        self.mov(None)

    def _sendcmd(self, cmd):
        if self.use_serial:
            self.ser.write(cmd.encode('utf-8'))
        else:
            print(cmd[0:-1])

if __name__ == "__main__":
    root = tkinter.Tk()
    root.configure()
    if len(sys.argv)>1:
        app = App(root, sys.argv[1])
    else:
        app = App(root)
    root.mainloop()
