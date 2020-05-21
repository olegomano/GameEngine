from PyQt5 import QtWidgets,QtCore,QtGui
import ui.scene_ui as scene

class Scene(QtWidgets.QWidget):
    
    setPos_signal = QtCore.pyqtSignal(int,float,float)
    def __init__(self,parent = None):
        super(QtWidgets.QWidget,self).__init__(parent)
        
        self.setPos_signal.connect(self.setPos_slot) 

        self.window = scene.Ui_Form()
        self.window.setupUi(self)
        self.onClickListener = None 
        self.scene = QtWidgets.QGraphicsScene() 
        self.window.graphicsView.setScene(self.scene)
        self.objectMap = {}
        

        pass 

    def setPos(self,id,x,y):
        #print("Update Object: " + str(id) + "pos( " + str(x) + " " + str(y) + ")")
        self.setPos_signal.emit(id,x,y) 
        pass
    
    def setPos_slot(self,id,x,y):
        #print("Update Object Slot: " + str(id) + "pos( " + str(x) + " " + str(y) + ")")
        if id not in self.objectMap:
          r = self.scene.addRect(0,0,64,64)
          self.objectMap[id] = r
        self.objectMap[id].setRect(x/50,y/50,64,64)
        pass
