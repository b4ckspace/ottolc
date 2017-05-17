import sys
import tkinter
from gui.app import App
if __name__ == "__main__":
    root = tkinter.Tk()
    root.configure()
    if len(sys.argv)>1:
        app = App(root, sys.argv[1])
    else:
        app = App(root)
    root.mainloop()
