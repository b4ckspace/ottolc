import tkinter
import serial


class App():
	def __init__(self, master):
		self.ser = serial.Serial('/dev/ttyUSB0')
		frame = tkinter.Frame(master)
		
		self.lf = tkinter.Scale(master, from_=0, to=180, resolution=1, command=self.cmd, orient=tkinter.HORIZONTAL)
		self.lf.set(90)
		self.lf.pack()		
		
		self.rf = tkinter.Scale(master, from_=0, to=180, resolution=1, command=self.cmd, orient=tkinter.HORIZONTAL)
		self.rf.set(90)
		self.rf.pack()

		self.rl = tkinter.Scale(master, from_=0, to=180, resolution=1, command=self.cmd, orient=tkinter.VERTICAL)
		self.rl.set(90)
		self.rl.pack()
		self.ll = tkinter.Scale(master, from_=0, to=180, resolution=1, command=self.cmd, orient=tkinter.VERTICAL)
		self.ll.set(90)
		self.ll.pack()


	def cmd(self, _w):
		# print(self.s1.get())
		# print(self.s2.get())
		self.ser.write(("! setservos %d %d %d %d \n"%(self.rf.get(), self.lf.get(),  self.rl.get(),  self.ll.get())).encode('utf-8'))
		

root = tkinter.Tk()
app = App(root)
root.mainloop()
