# init.py - WeiZhiYiDongForm 的自定义 RunGCode
#
# 此文件在窗口首次显示时由 BaseChildWindow::showEvent 自动加载
# window = WeiZhiYiDongForm 实例

from PythonQt.QtGui import *
from PythonQt.QtCore import *

# =====================================================
# 版本检测
# =====================================================
REQUIRED_VERSION = "1.3.3"


def check_version():
    """检查主程序版本是否匹配"""
    app = QApplication.instance()
    if not app:
        print("[Python] Warning: QApplication not found")
        return False

    current_version = app.applicationVersion
    if current_version != REQUIRED_VERSION:
        print(f"[Python] Version mismatch: required={REQUIRED_VERSION}, current={current_version}")
        print(f"[Python] init.py for {window.objectName if window else 'unknown'} will not load")
        return False

    print(f"[Python] Version check passed: {current_version}")
    return True


# 版本检测
if not check_version():
    # 版本不匹配，不加载后续代码
    print("[Python] init.py skipped due to version mismatch")
else:
    # =====================================================
    # 正常加载逻辑
    # =====================================================

    def _clear_inputs(child_window):
        """清空输入控件"""
        for name in ["inX", "inY", "inZ", "inU"]:
            edit = child_window.findChild(QLineEdit, name)
            if edit:
                edit.setText("")


    def custom_run_gcode(child_window, call_original):
        """
        自定义 RunGCode 实现

        Args:
            child_window: 当前子窗口（WeiZhiYiDongForm 实例）
            call_original: 函数，调用可执行原始 C++ RunGCode
        """
        print(f"[Python] custom_run_gcode for {child_window.objectName}")

        # =====================================================
        # 条件判断：何时使用自定义逻辑，何时调用原始
        # =====================================================

        # 示例：设置 use_original = True 则调用原始，False 则使用自定义
        use_original = False  # 改为 True 可调用原始 C++ RunGCode

        if use_original:
            print("[Python] Calling original C++ RunGCode")
            call_original(child_window)
            return

        # =====================================================
        # 自定义逻辑
        # =====================================================

        # 获取窗口控件
        inX = child_window.findChild(QLineEdit, "inX")
        inY = child_window.findChild(QLineEdit, "inY")
        inZ = child_window.findChild(QLineEdit, "inZ")
        inU = child_window.findChild(QLineEdit, "inU")
        inCoor = child_window.findChild(QLineEdit, "inCoor")
        inAbsolute = child_window.findChild(QRadioButton, "inAbsolute")
        tabWidget = child_window.findChild(QTabWidget, "tabWidget")
        inTouchSense = child_window.findChild(QCheckBox, "inTouchSense")

        gcode = ""

        # 获取当前 tab
        if tabWidget:
            current_tab = tabWidget.currentIndex
            print(f"[Python] Current tab: {current_tab}")

            if current_tab == 0:  # 定位移动
                gcode += "G27\nG09\n"

                # 绝对/相对坐标
                if inAbsolute and inAbsolute.checked:
                    gcode += "G90\n"
                else:
                    gcode += "G91\n"

                # 坐标系
                if inCoor and inCoor.text:
                    gcode += f"G{inCoor.text}\n"

                # 接触感知
                if inTouchSense and not inTouchSense.checked:
                    gcode += "M05"

                # 位置
                pos = ""
                if inX and inX.text:
                    pos += f"X{inX.text}"
                if inY and inY.text:
                    pos += f"Y{inY.text}"
                if inZ and inZ.text:
                    pos += f"Z{inZ.text}"
                if inU and inU.text:
                    pos += f"U{inU.text}"

                if pos:
                    gcode += f"G00{pos}\n"

            elif current_tab == 1:  # 第二个 tab（G82）
                gcode += "G90\n"

                inCoor_2 = child_window.findChild(QLineEdit, "inCoor_2")
                if inCoor_2 and inCoor_2.text:
                    gcode += f"G{inCoor_2.text}\n"

                inTouchSense_2 = child_window.findChild(QCheckBox, "inTouchSense_2")
                if inTouchSense_2 and not inTouchSense_2.checked:
                    gcode += "M05"

                btnX_2 = child_window.findChild(QPushButton, "btnX_2")
                btnY_2 = child_window.findChild(QPushButton, "btnY_2")
                btnZ_2 = child_window.findChild(QPushButton, "btnZ_2")
                btnU_2 = child_window.findChild(QPushButton, "btnU_2")

                pos = ""
                if btnX_2 and btnX_2.checked:
                    pos += "X2 "
                if btnY_2 and btnY_2.checked:
                    pos += "Y2 "
                if btnZ_2 and btnZ_2.checked:
                    pos += "Z2 "
                if btnU_2 and btnU_2.checked:
                    pos += "U2 "

                if pos:
                    gcode += f"G82 {pos}\n"

            elif current_tab == 2:  # 回零
                btnX_3_P = child_window.findChild(QPushButton, "btnX_3_P")
                btnX_3_N = child_window.findChild(QPushButton, "btnX_3_N")
                btnY_3_P = child_window.findChild(QPushButton, "btnY_3_P")
                btnY_3_N = child_window.findChild(QPushButton, "btnY_3_N")
                btnZ_3_P = child_window.findChild(QPushButton, "btnZ_3_P")
                btnZ_3_N = child_window.findChild(QPushButton, "btnZ_3_N")
                btnU_3_P = child_window.findChild(QPushButton, "btnU_3_P")
                btnU_3_N = child_window.findChild(QPushButton, "btnU_3_N")

                selected_axis = -1

                if btnX_3_P and btnX_3_P.checked:
                    gcode = "do_backtozero X\n"
                    selected_axis = 0
                elif btnX_3_N and btnX_3_N.checked:
                    gcode = "do_backtozeron X\n"
                    selected_axis = 0
                elif btnY_3_P and btnY_3_P.checked:
                    gcode = "do_backtozero Y\n"
                    selected_axis = 1
                elif btnY_3_N and btnY_3_N.checked:
                    gcode = "do_backtozeron Y\n"
                    selected_axis = 1
                elif btnZ_3_P and btnZ_3_P.checked:
                    gcode = "do_backtozero Z\n"
                    selected_axis = 2
                elif btnZ_3_N and btnZ_3_N.checked:
                    gcode = "do_backtozeron Z\n"
                    selected_axis = 2
                elif btnU_3_P and btnU_3_P.checked:
                    gcode = "do_backtozero U\n"
                    selected_axis = 3
                elif btnU_3_N and btnU_3_N.checked:
                    gcode = "do_backtozeron U\n"
                    selected_axis = 3

                # 回零确认
                if gcode and selected_axis != -1:
                    gcode += "do_message_yn to_clear_machine_axis\n"
                    axis_names = ['X', 'Y', 'Z', 'U']
                    gcode += f"o101 if[#<message_ret> EQ 1]\n"
                    gcode += f"do_zeroaxis {axis_names[selected_axis]}\n"
                    gcode += "o101 endif\n"

        print(f"[Python] Generated GCode:\n{gcode}")

        # 获取 NCMachine 并运行 GCode
        nc_machine = child_window.GetMachine()
        if nc_machine and gcode:
            nc_machine.RunGCode(gcode)
            print("[Python] GCode sent to machine")

        # 清空输入
        _clear_inputs(child_window)


    # =====================================================
    # 注册自定义 RunGCode
    # =====================================================

    print(f"[Python] init.py loading for {window.objectName}")

    try:
        register_child_override(window.objectName, custom_run_gcode)
        print(f"[Python] Registered custom RunGCode for {window.objectName}")
    except NameError:
        print("[Python] Warning: register_child_override not found")
        print("[Python] Make sure plugins/main/test_plugin.py is loaded first")