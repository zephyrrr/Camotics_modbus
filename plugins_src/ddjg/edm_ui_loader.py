
import PythonQt
from PythonQt.Qt import *
from PythonQt.QtGui import *
from PythonQt.QtGui import QLineEdit4AxisValue
from PythonQt.QtUiTools import QUiLoader

class EdmUiLoader(QUiLoader):
    def __init__(self, parent=None):
        super().__init__(parent)

    def createWidget(self, class_name, parent=None, name=''):
        """Overrides QUiLoader's widget creation logic."""
        
        # 1. Check for the custom widget name from the .ui file
        if class_name == "QLineEdit4AxisValue":
            #print(class_name, parent, name)
            widget = QLineEdit4AxisValue(parent)
            #widget = QLineEdit(parent)
            widget.setObjectName(name)
            #widget = QUiLoader.createWidget(self, class_name, parent, name)
            return widget 
        
        # 2. Fall back to the base class for all standard Qt widgets
        return QUiLoader.createWidget(self, class_name, parent, name)
		