import tkinter
class Configtab():
    def __init__(self, master, app):
        self.app = app
        self.cbvar = tkinter.IntVar()
        self.servocb = tkinter.Checkbutton(
            master,
            variable=self.cbvar,
            text="enable servos",
            command=self.servos)
        self.servocb.grid(row=0,column=0)
        self.servocb.select()

        """
        self.tll = tkinter.Scale(master, from_=-90, to=90,
                                resolution=1, command=self.app.mov,
                                orient=tkinter.HORIZONTAL, label="trim left leg")
        self.trl = tkinter.Scale(master, from_=-90, to=90,
                                resolution=1, command=self.app.mov,
                                orient=tkinter.HORIZONTAL, label="trim right leg")
        self.tlf = tkinter.Scale(master, from_=-90, to=90,
                                resolution=1, command=self.app.mov,
                                orient=tkinter.HORIZONTAL, label="trim left foot")
        self.trf = tkinter.Scale(master, from_=-90, to=90,
                                resolution=1, command=self.app.mov,
                                orient=tkinter.HORIZONTAL, label="trim right foot")
        self.tll.set(0)
        self.trl.set(0)
        self.tlf.set(0)
        self.trf.set(0)
        self.tll.grid(row=1, column=0)
        self.trl.grid(row=1, column=1)
        self.tlf.grid(row=2, column=0)
        self.trf.grid(row=2, column=1)
        """
        tkinter.Button(
            master,
            text="Save Trim",
            command=self.app.saveTrim).grid(
            row=2,
            column=0)


    def servos(self, _w = None):
    	if self.cbvar.get():
    		self.app._sendcmd("! servoson\n")
    	else:
    		self.app._sendcmd("! servosoff\n")
