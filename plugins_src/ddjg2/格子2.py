# -*- coding: utf-8 -*-
import sys
from PythonQt.Qt import *
from PythonQt.QtGui import *

#ui_file_name = "C:\\OneDrive\\doing\\cnc\\Camotics_modbus\\plugins\\ddjg\\pointgeneratorbygrid.ui"
ui_file_name = "pointgeneratorbygrid.ui"
ui_file = QFile(ui_file_name)
if not ui_file.open(QIODevice.ReadOnly):
	print(f"Cannot open {ui_file_name}: {ui_file.errorString()}")
	sys.exit(-1)
loader = QUiLoader()
dlg = loader.load(ui_file)
ui_file.close()
if not dlg:
    print(loader.errorString())
    sys.exit(-1)
dlg.cancelButton.clicked.connect(lambda: dlg.reject())
dlg.okButton.clicked.connect(lambda: dlg.accept())
dlg.setParent(window)
dlg.setWindowFlags(Qt.Dialog);
dlg.show()
dlg.setStyle(QApplication.instance().style());
#print(QApplication.instance().styleSheet)

app_stylesheet = """
    QLabel {
        font-size: 30px;
        background-color: rgb(128, 128, 128);
        color: white;
        border: none;
        text-align: center;
        vertical-align: middle;
    }
    QPushButton {
        font: bold 30px;
        color: black;
        background-color: rgb(225, 225, 225);
    }
	QDialog#PointGenratorByGrid QLineEdit {
        font: bold 30px;
        background-color: rgb(236, 255, 242);
        text-align: center;
        vertical-align: middle;
    }
"""
dlg.setStyleSheet(app_stylesheet)
if dlg.exec() == QDialog.Accepted:
	rs = []
	x1 = float(dlg.txtStartX.text);
	x2 = float(dlg.txtEndX.text);
	y1 = float(dlg.txtStartY.text);
	y2 = float(dlg.txtEndY.text);
	dx = float(dlg.txtStepX.text);
	dy = float(dlg.txtStepY.text);
	xs = x1
	while xs <= x2:
		ys = y1
		while ys <= y2:
			rs.append(xs);
			rs.append(ys);
			ys += dy;
		xs += dx;
	table3 = window.sddg_table3
	for i in range(len(rs)):
		if i / 2 >= table3.getRowCount():
			table3.insertNewRow(i / 2)
		table3.setValue(i / 2, i % 2, '{:.3f}'.format(rs[i]));