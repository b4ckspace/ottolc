import tkinter
import serial
from datetime import datetime, timedelta


class App():
    def __init__(self, master, use_serial):
        self.use_serial = use_serial
        self.nosend = True
        if self.use_serial:
            self.ser = serial.Serial('/dev/ttyUSB0')
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
            command=self.sendanim).grid(
            row=0,
            column=3)
        tkinter.Button(
            master,
            text="play anim",
            command=self.playanim).grid(
            row=1,
            column=3)

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

    def sendanim(self):
        cmdstr = "! pushframe %d %d %d %d %d\n" % (
            self.rf.get(), self.lf.get(), self.rl.get(), self.ll.get(), self.ts.get())
        self._sendcmd(cmdstr)

    def playanim(self):
        self._sendcmd("! playanim\n")

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


root = tkinter.Tk()
root.configure()
app = App(root, True)
root.mainloop()
