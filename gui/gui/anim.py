import tkinter
from datetime import datetime, timedelta
from tkinter.filedialog import asksaveasfilename, askopenfilename
import json
from parse import parse

# TODO: add library of animations to add quickly

class Animtab():
    def __init__(self, master, app):
        self.app = app
            
        frame = tkinter.Frame(master)

        self.lf = tkinter.Scale(master, from_=0, to=180,
                                resolution=1, command=self.app.mov,
                                orient=tkinter.HORIZONTAL, label="left foot")
        self.lf.set(90)
        self.lf.grid(row=1, column=0)

        self.rf = tkinter.Scale(master, from_=0, to=180,
                                resolution=1, command=self.app.mov,
                                orient=tkinter.HORIZONTAL, label="right foot")
        self.rf.set(90)
        self.rf.grid(row=1, column=1)

        self.rl = tkinter.Scale(master, from_=0, to=180,
                                resolution=1, command=self.app.mov,
                                orient=tkinter.HORIZONTAL, label="right leg")
        self.rl.set(90)
        self.rl.grid(row=0, column=1)
        self.ll = tkinter.Scale(master, from_=0, to=180,
                                resolution=1, command=self.app.mov,
                                orient=tkinter.HORIZONTAL, label="left leg")
        self.ll.set(90)
        self.ll.grid(row=0, column=0)

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
        self.framelist.bind('<Double-Button-1>', self.loadset)

        #self.nosend = False
        #self.app.mov(None)

    def getrf(self):
        return self.rf.get()
    def getlf(self):
        return self.lf.get()
    def getrl(self):
        return self.rl.get()
    def getll(self):
        return self.ll.get()


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
        self.app._sendcmd("! resetanim\n" )
        for i in animlist:
            self.app._sendcmd("! pushframe %s\n"%(i,))
        self.app._sendcmd("! playanim\n")

    def deleteselection(self):
        for i in reversed(self.framelist.curselection()):
            self.framelist.delete(i)

    def loadset(self, _w=None):
        item = self.framelist.get(tkinter.ACTIVE)
        if len(item)==0:
            return
        res = parse("{:d} {:d} {:d} {:d} {:d}", item)
        self.nosend = True
        self.lf.set(res[1])
        self.rf.set(res[0])
        self.ll.set(res[3])
        self.rl.set(res[2])
        self.ts.set(res[4])
        self.nosend = False
        self.app.mov(None)


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
        self.app.mov(None)
