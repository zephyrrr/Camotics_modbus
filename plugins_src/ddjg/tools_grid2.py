import sys
import os
import PythonQt
from PythonQt.Qt import *
from PythonQt.QtGui import *
from PythonQt.QtGui import QLineEdit4AxisValue, QLineEdit4Int
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
        elif class_name == 'QLineEdit4Int':
            widget = QLineEdit4Int(parent)
            widget.setObjectName(name)
            return widget 

        # 2. Fall back to the base class for all standard Qt widgets
        return QUiLoader.createWidget(self, class_name, parent, name)

def load_stylesheet(filename):
    """
    Opens a file, reads its content, and returns the stylesheet string.
    """
    # Create a QFile object for the stylesheet
    file = QFile(filename)

    # Try to open the file in read-only text mode
    if file.open(QFile.ReadOnly | QFile.Text):
        # Create a QTextStream to read the file content
        stream = QTextStream(file)
        # Read the entire file content
        stylesheet = stream.readAll()
        file.close()
        return stylesheet
    else:
        # Handle the error if the file can't be opened
        print(f"Error: Could not open stylesheet file {filename}")
        return ""

def do():
    current_dir = os.path.dirname(os.path.abspath(__file__))  

    ui_file_name = os.path.join(current_dir, "pointgeneratorbygrid.ui")
    ui_file = QFile(ui_file_name)
    if not ui_file.open(QIODevice.ReadOnly):
        print(f"Cannot open {ui_file_name}: {ui_file.errorString()}")
        return
    loader = EdmUiLoader()
    dlg = loader.load(ui_file)
    ui_file.close()
    dlg.setObjectName('pointgeneratorbygrid2')
    BaseChildWindow.LoadData(dlg)
    if not dlg:
        print(loader.errorString())
        return
    dlg.cancelButton.clicked.connect(lambda: dlg.reject())
    dlg.okButton.clicked.connect(lambda: dlg.accept())
    dlg.setParent(window)
    dlg.setWindowFlags(Qt.Dialog);
    dlg.show()
    dlg.setStyle(QApplication.instance().style());
    #print(QApplication.instance().styleSheet)

    stylesheet = load_stylesheet(os.path.join(current_dir, "pointgeneratorbygrid.qss"))
    dlg.setStyleSheet(stylesheet)
    if dlg.exec() == QDialog.Accepted:
        BaseChildWindow.SaveData(dlg)

        x1 = float(dlg.txtStartX.text);
        x2 = int(dlg.txtCountX.text);
        y1 = float(dlg.txtStartY.text);
        y2 = int(dlg.txtCountY.text);
        dx = float(dlg.txtStepX.text);
        dy = float(dlg.txtStepY.text);

        table3 = window.findChild(QTableWidget, "ddjg_table3")
        n = table3.getDataCount()
        for i in range(n-1, -1, -1):
            table3.deleteRow(i + 1)

        # 2. 外部循环：遍历 Y 轴的每一行
        row = 0
        for i in range(y2):
            # 计算当前行的 Y 坐标
            current_y = y1 + i * dy
            
            # 3. 内部循环：生成当前行的 X 坐标
            
            # 生成 X 坐标的原始顺序列表
            for j in range(x2):
                # 4. 判断是否需要反转 X 轴顺序 (蛇形遍历的关键)
                if True:
                    current_x = x1 + j * dx
                else:
                    current_x = x1 + (x2 - 1) * dx - j * dx
                
                if row >= table3.getDataCount():
                    table3.insertNewRow(row)
                table3.setValue(row, 1, '{:.3f}'.format(current_x));
                table3.setValue(row, 2, '{:.3f}'.format(current_y));
                row += 1

            # 中间行
            if i != y2 - 1:
                current_y = current_y + dy / 2
                for j in range(x2 - 1):
                    current_x = x1 + (x2 - 1) * dx - j * dx - dx / 2
                    if row >= table3.getDataCount():
                        table3.insertNewRow(row)
                    table3.setValue(row, 1, '{:.3f}'.format(current_x));
                    table3.setValue(row, 2, '{:.3f}'.format(current_y));
                    row += 1

        window.HideKeyboard()

if __name__ == "__main__":
    do()