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

        tkinter.Button(
            master,
            text="Save Trim",
            command=self.app.saveTrim).grid(
            row=3,
            column=0)
        tkinter.Button(
            master,
            text="Test Trim",
            command=self.app.testTrim).grid(
            row=4,
            column=0)
        tkinter.Button(
            master,
            text="Reset Trim",
            command=self.app.resetTrim).grid(
            row=5,
            column=0)
        tkinter.Button(
            master,
            text="Get OttO-LC Info",
            command=self.app.getInfo).grid(
            row=0,
            column=1)



    def servos(self, _w = None):
    	if self.cbvar.get():
    		self.app._sendcmd("! servoson\n")
    	else:
    		self.app._sendcmd("! servosoff\n")
