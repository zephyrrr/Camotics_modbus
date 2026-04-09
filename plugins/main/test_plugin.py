# test_plugin.py
#
# 此文件在 QtWin2 构造时自动加载
# window = QtWin2 主窗口
#
# 功能：拦截 actionRun.triggered 信号，实现 Python 端的 RunGCode 覆盖

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
        print(f"[Python] test_plugin.py will not load")
        return False

    print(f"[Python] Version check passed: {current_version}")
    return True


# 版本检测
if not check_version():
    # 版本不匹配，不加载后续代码
    print("[Python] test_plugin.py skipped due to version mismatch")
    result = False
else:
    # =====================================================
    # 正常加载逻辑
    # =====================================================

    # 全局变量
    _main_window = None
    _child_window_overrides = {}  # 存储各子窗口的自定义 RunGCode


    def register_child_override(child_name, handler_func):
        """
        注册子窗口的 RunGCode 覆盖函数

        Args:
            child_name: 子窗口的 objectName（如 "WeiZhiYiDongForm"）
            handler_func: 自定义 RunGCode 函数，签名: func(child_window, call_original)
                          - child_window: 子窗口实例
                          - call_original: 函数，调用原始 C++ RunGCode
        """
        global _child_window_overrides
        _child_window_overrides[child_name] = handler_func
        print(f"[Python] Registered RunGCode override for: {child_name}")


    def unregister_child_override(child_name):
        """取消注册子窗口的覆盖函数"""
        global _child_window_overrides
        if child_name in _child_window_overrides:
            del _child_window_overrides[child_name]
            print(f"[Python] Unregistered RunGCode override for: {child_name}")


    def call_original_run_gcode(child_window):
        """
        调用原始的 C++ RunGCode

        Args:
            child_window: 子窗口实例
        """
        print(f"[Python] Calling original C++ RunGCode for {child_window.objectName}")

        # 隐藏键盘
        if hasattr(child_window, 'HideKeyboard'):
            child_window.HideKeyboard()

        # 调用原始 C++ RunGCode
        if hasattr(child_window, 'RunGCode'):
            child_window.RunGCode()

        # 更新 UI 状态
        if _main_window:
            action_run = _main_window.actionRun
            if action_run:
                action_run.setEnabled(False)

            lbl_result = _main_window.findChild(QLabel, "lblResultMessage")
            if lbl_result:
                lbl_result.setText("运行中...")


    def on_run_triggered():
        """
        自定义的运行按钮处理函数
        根据当前子窗口决定执行覆盖函数还是原始逻辑
        """
        global _main_window, _child_window_overrides

        print("[Python] on_run_triggered called")

        if not _main_window:
            return

        # 获取当前子窗口
        stacked_widget = _main_window.findChild(QStackedWidget, "stackedChildWidget")
        if not stacked_widget:
            print("[Python] Error: stackedChildWidget not found")
            return

        current_widget = stacked_widget.currentWidget()
        if not current_widget:
            print("[Python] Error: no current widget")
            return

        child_name = current_widget.objectName
        print(f"[Python] Current child window: {child_name}")

        # 检查是否有覆盖函数
        if child_name in _child_window_overrides:
            print(f"[Python] Using custom RunGCode for {child_name}")
            handler = _child_window_overrides[child_name]
            # 传入子窗口和原始调用函数
            handler(current_widget, call_original_run_gcode)
        else:
            # 没有覆盖函数，直接调用原始逻辑
            call_original_run_gcode(current_widget)


    def setup_interception():
        """设置信号拦截"""
        global _main_window

        if not window:
            print("[Python] Error: window is None")
            return

        _main_window = window
        print(f"[Python] Main window: {window.objectName}")

        # 获取 actionRun
        action_run = window.actionRun
        if not action_run:
            print("[Python] Error: actionRun not found")
            return

        # 断开原有连接
        try:
            action_run.triggered.disconnect()
            print("[Python] Disconnected original actionRun.triggered")
        except Exception as e:
            print(f"[Python] Disconnect warning: {e}")

        # 连接新的处理函数
        action_run.triggered.connect(on_run_triggered)
        print("[Python] Connected custom handler to actionRun.triggered")


    # =====================================================
    # 初始化
    # =====================================================

    print("[Python] test_plugin.py loading...")

    # 设置拦截
    setup_interception()

    # 将注册函数暴露到全局，供各子窗口的 init.py 调用
    import __main__
    __main__.register_child_override = register_child_override
    __main__.unregister_child_override = unregister_child_override
    __main__.call_original_run_gcode = call_original_run_gcode

    print("[Python] test_plugin.py loaded")
    print("[Python] Usage in child window init.py:")
    print("  register_child_override('WeiZhiYiDongForm', custom_run_gcode)")
    print("")
    print("[Python] Handler function signature:")
    print("  def custom_run_gcode(child_window, call_original):")
    print("      if some_condition:")
    print("          call_original(child_window)  # 调用原始 C++ RunGCode")
    print("      else:")
    print("          # 自定义逻辑")