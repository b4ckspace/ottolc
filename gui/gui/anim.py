import tkinter
from datetime import datetime, timedelta
from tkinter.filedialog import asksaveasfilename, askopenfilename
import json
from parse import parse

# TODO: add library of animations to add quickly

class Animtab():
    def __init__(self, master, app):
        self.app = app

        self.ll = tkinter.Scale(master, from_=-90, to=90,
                                resolution=1, length=200, command=self.app.mov,
                                orient=tkinter.HORIZONTAL, label=_("left leg"))
        self.ll.set(0)
        self.ll.grid(row=0, column=0)

        self.rl = tkinter.Scale(master, from_=-90, to=90,
                                resolution=1, length=200, command=self.app.mov,
                                orient=tkinter.HORIZONTAL, label=_("right leg"))
        self.rl.set(0)
        self.rl.grid(row=0, column=1)
        
        self.lf = tkinter.Scale(master, from_=-90, to=90,
                                resolution=1, length=200, command=self.app.mov,
                                orient=tkinter.HORIZONTAL, label=_("left foot"))
        self.lf.set(0)
        self.lf.grid(row=1, column=0)

        self.rf = tkinter.Scale(master, from_=-90, to=90,
                                resolution=1, length=200, command=self.app.mov,
                                orient=tkinter.HORIZONTAL, label=_("right foot"))
        self.rf.set(0)
        self.rf.grid(row=1, column=1)

        self.ts = tkinter.Scale(master, from_=1, to=1000, length=200,
                                orient=tkinter.VERTICAL, label=_("time"))
        self.ts.set(500)
        self.ts.grid(row=0, column=2, rowspan=2)

        tkinter.Button(
            master,
            text=_("Reset servos"),
            command=self.resetServos).grid(
            row=2,
            column=0)
        tkinter.Button(
            master,
            text=_("add frame"),
            command=self.addframe).grid(
            row=2,
            column=2)
        tkinter.Button(
            master,
            text=_("play animation"),
            command=self.playanim).grid(
            row=1,
            column=3)
        tkinter.Button(
            master,
            text=_("play selection"),
            command=self.playanimsel).grid(
            row=2,
            column=3)
        tkinter.Button(
            master,
            text=_("delete selection"),
            command=self.deleteselection).grid(
            row=0,
            column=3)

        tkinter.Button(
            master,
            text=_("save"),
            command=self.savefile).grid(
            row=2,
            column=4)
        tkinter.Button(
            master,
            text=_("load"),
            command=self.loadfile).grid(
            row=3,
            column=4)
        tkinter.Button(
            master,
            text=_("save to clipboard"),
            command=self.saveclipboard).grid(
            row=4,
            column=4)
        tkinter.Button(
            master,
            text=_("load from clipboard"),
            command=self.loadclipboard).grid(
            row=5,
            column=4)
        rawlist = self.app._sendcmd("! supportedanims\n")
        funlist =[]
        if rawlist:
            funlist = list(map(lambda x: x.decode('utf-8'), rawlist.split(b' ')))
        if len(funlist)>0:
            var = tkinter.StringVar(master)
            var.set(funlist[0])
            self.selected_fun = var
            self.funwidget = tkinter.OptionMenu(master, var, *funlist).grid(row=3, column=2)
            self.funbutton = tkinter.Button(
                master,
                text="add animation as step",
                command=self.addfun).grid(
                row=4,
                column=2)

        
        self.framelist = tkinter.Listbox(master, selectmode=tkinter.EXTENDED)
        self.framelist.grid(row=0, column=4, rowspan=2)
        self.framelist.bind('<Double-Button-1>', self.loadset)


    def getrf(self):
        return self.rf.get()
    def getlf(self):
        return self.lf.get()
    def getrl(self):
        return self.rl.get()
    def getll(self):
        return self.ll.get()

    def addfun(self):
        self.framelist.insert(tkinter.END, self.selected_fun.get())

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
            if i[0].isdigit() or i[0]=='-':
                self.app._sendcmd("! pushframe %s\n" % (i,))
            else:
                self.app._sendcmd("! pushcallback %s\n" % (i,))
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


    def savefile(self):
        savepath = asksaveasfilename(defaultextension=".json")
        if not savepath:
            return
        with open(savepath, "w") as f:
            json.dump(self.framelist.get(0, tkinter.END), f, indent=4)
        
    def loadfile(self):
        loadpath = askopenfilename(filetypes=[("Json file","*.json")])
        if not loadpath:
            return
        with open(loadpath, "r") as f:
            obj = json.load(f)
            self.framelist.delete(0, tkinter.END)
            for e in obj:
                self.framelist.insert(tkinter.END, e)

    def saveclipboard(self):
         self.app.master.clipboard_append( json.dumps(self.framelist.get(0, tkinter.END), indent=4) )

    def loadclipboard(self):
        obj = json.loads(self.app.master.clipboard_get())
        self.framelist.delete(0, tkinter.END)
        for e in obj:
            self.framelist.insert(tkinter.END, e)

    def resetServos(self):
        self.nosend = True
        self.lf.set(0)
        self.rf.set(0)
        self.ll.set(0)
        self.rl.set(0)
        self.nosend = False
        self.app.mov(None)
