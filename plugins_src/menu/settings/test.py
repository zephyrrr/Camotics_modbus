# test.py - 寄存器测试页面
# 放置在"设置"菜单下，提供寄存器值设置和GCode测试功能

import PythonQt
from PythonQt.QtGui import *
from PythonQt.QtCore import *
from PythonQt.QtUiTools import QUiLoader
from PythonQt.QtGui import QLineEdit4Keyboard
import os


class EdmUiLoader(QUiLoader):
    def __init__(self, parent=None):
        super().__init__(parent)

    def createWidget(self, class_name, parent=None, name=''):
        if class_name == "QLineEdit4Keyboard":
            widget = QLineEdit4Keyboard(parent)
            widget.setObjectName(name)
            return widget
        return QUiLoader.createWidget(self, class_name, parent, name)


def tr(source_text, context="TestPlugin"):
    return QCoreApplication.translate(context, source_text)

PLUGIN_NAME = tr("Test")

def create_widget(window):
    """创建并返回插件控件."""
    # 载入翻译
    translator = QTranslator()
    current_dir = os.path.dirname(os.path.abspath(__file__))
    translator.load(QLocale(), "test", "_", current_dir)
    QApplication.instance().installTranslator(translator)

    # 载入UI文件
    ui_file_name = os.path.join(current_dir, "test.ui")
    ui_file = QFile(ui_file_name)
    if not ui_file.open(QIODevice.ReadOnly):
        print(f"Cannot open {ui_file_name}: {ui_file.errorString()}")
        return None

    loader = EdmUiLoader()
    widget = loader.load(ui_file)
    ui_file.close()

    if not widget:
        print(loader.errorString())
        return None

    # 连接按钮信号
    widget.btnOk.clicked.connect(lambda: on_ok_clicked(widget, window))

    print("[Register Test Plugin] Widget loaded")
    return widget


def on_ok_clicked(widget, window):
    """处理OK按钮点击"""
    # Format GCode with register values
    gcode = "#<_conn_idx> = 1\n"
    gcode = "#<_reg_cnt> = 4\n"
    for i in range(4):
        reg = 82 + i
        edit = widget.findChild(QLineEdit4Keyboard, f"editReg{reg}")
        value = edit.text.strip()
        gcode += f"#<_reg_{reg}> = {value}\n"

    gcode += "do_writereg 82\n"

    # Execute GCode
    machine = window.GetMachine()
    if machine:
        ok = machine.RunGCodeSync(gcode)
        if ok:
            QMessageBox.information(widget, tr("OK"), tr("Sent successfully"))
        else:
            QMessageBox.warning(widget, tr("Error"), tr("Send failed"))
