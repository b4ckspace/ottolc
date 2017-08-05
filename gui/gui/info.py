import tkinter
import gettext
de = gettext.translation('ottolcgui', localedir='locale', languages=['de_DE'])
_ = de.gettext
class Infotab():
    def __init__(self, master, app):
        self.app = app
        tkinter.Button(
            master,
            text=_("Open help page"),
            command=self.app.openhelp).pack()