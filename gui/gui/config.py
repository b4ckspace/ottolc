import tkinter
import gettext
de = gettext.translation('ottolcgui', localedir='locale', languages=['de_DE'])
_ = de.gettext
class Configtab():
    def __init__(self, master, app):
        self.app = app
        self.cbvar = tkinter.IntVar()
        self.servocb = tkinter.Checkbutton(
            master,
            variable=self.cbvar,
            text=_("enable servos"),
            command=self.servos)
        self.servocb.grid(row=0,column=0)
        self.servocb.select()

        tkinter.Button(
            master,
            text=_("save trim values"),
            command=self.app.saveTrim).grid(
            row=3,
            column=0)
        tkinter.Button(
            master,
            text=_("test trim values"),
            command=self.app.testTrim).grid(
            row=4,
            column=0)
        tkinter.Button(
            master,
            text=_("reset trim values"),
            command=self.app.resetTrim).grid(
            row=5,
            column=0)
        tkinter.Button(
            master,
            text=_("get ottolc info"),
            command=self.app.getInfo).grid(
            row=0,
            column=1)



    def servos(self, _w = None):
    	if self.cbvar.get():
    		self.app._sendcmd("! servoson\n")
    	else:
    		self.app._sendcmd("! servosoff\n")
