from PyQt5 import QtWidgets,QtCore,QtGui
import ui.scene_ui as scene

class Scene(QtWidgets.QWidget):
    def __init__(self,parent = None):
        super(QtWidgets.QWidget,self).__init__(parent)
        self.window = scene.Ui_Form()
        self.window.setupUi(self)
        self.onClickListener = None
        pass 

    def setPos(self,x,y):
        pass
    

