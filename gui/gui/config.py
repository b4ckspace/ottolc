import tkinter
class Configtab():
    def __init__(self, master, app):
        self.app = app
        tkinter.Button(
            master,
            text="do stuff",
            command=self.nop).grid(
            row=2,
            column=0)
    def nop(self, _w = None):
    	pass