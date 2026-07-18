# -*- coding: utf-8 -*-
# wolong_h3.py - 卧龙H3 多电机控制页面
# 放置在"设置"菜单下，提供卧龙H3 电机的使能/失能、正反转/停止、转速与加速时间设置功能。
# 当前支持 2 路电机 (n=2)，各用一个 QGroupBox 包裹，横向排列；多电机靠 Modbus 从机地址区分。
#
# 协议参考: cc_h3/*.txt (H3定制485指令)
#   - Modbus RTU, 同一总线, 从机地址依次为 1、2 ... (F.31=1, 多电机依次往上加)
#   - 默认波特率 115200 (F.30=6), 速度模式 (A01=1)
#   - 通过 GCode 的 do_writereg / do_readreg 指令读写寄存器, 用 #<_slave> 指定从机地址
#
# 寄存器说明 (十进制地址, 各电机相同):
#   0x403B = 16443  写 0x0055  获取指令执行权限 (上电时由系统自动发送, 插件不再重复)
#   0x3654 = 13908  长度2     写 0x00008001 使能 / 0x00008000 失能
#   0x4040 = 16448  长度4     [转速, 加速时间, 减速时间, 转矩] (4个寄存器必须一起发送)
#                             转速: 正数正转, 负数反转, 0 停止
#   0x4001 = 16385  长度1 读  报警编号, 0=正常
#
# 通信方式 (异步, 不阻塞 UI):
#   - 写/读均用 machine.RunGCode(gcode) (异步), 不用 RunGCodeSync。
#   - C++ 侧 do_writereg/do_readreg 完成后把 ret 存入控制器变量 _reg_<conn>_<slave>_ret
#     (ret == -1 表示失败; 读成功时数据存于 _reg_<conn>_<slave>_<addr>)。
#   - on_show 启动 1000ms 定时器, on_hide 停止: 每个周期收集所有电机上一次操作的
#     ret+数据更新状态, 并轮询发起一路电机的报警读取。
#   - 读优先走 C++ AddReadRegTask (直接挂 Modbus task, 优先级 TASK_TIMER_PRIORITY=2,
#     不计入 IsGCodeRunning), 避免和按钮写命令互相阻塞; C++ 未加时回退 RunGCode。
#
# 多语言: QCoreApplication.translate("WolongH3Plugin", sourceText), .ts context = WolongH3Plugin

import PythonQt
from PythonQt.QtGui import *
from PythonQt.QtCore import *
from PythonQt.QtUiTools import QUiLoader
from PythonQt.QtGui import QLineEdit4Keyboard
import os


# =====================================================
# 卧龙H3 寄存器映射与参数 (来自 cc_h3 协议)
# =====================================================
CONN_IDX = 1                  # Modbus 连接索引 (同一总线共用)
TASK_TIMER_PRIORITY = 2       # 读任务优先级: 不计入 IsGCodeRunning (其只查 0/1 队列), 读不挡写
WRITE_TASK_PRIORITY = 0       # 写任务优先级 (与 C++ AddWriteRegTask 默认一致); 写比读优先级高, 响应更快

REG_PERMISSION = 16443        # 0x403B (上电由系统自动发送, 此处仅作常量保留)
VAL_PERMISSION = [0x0055,]       # 85

REG_ENABLE = 13908            # 0x3654, 长度2
VAL_ENABLE = [0x0000, 0x8001] # 使能: 0x00008001
VAL_DISABLE = [0x0000, 0x8000]# 失能: 0x00008000

REG_SPEED = 16448             # 0x4040, 长度4: [转速, 加速时间, 减速时间, 转矩]
REG_ALARM = 16385             # 0x4001, 长度1, 读

DEFAULT_SPEED = 1000          # 默认转速 RPM
DEFAULT_ACCEL = 100           # 默认加速/减速时间 ms (减速时间与加速相同)
DEFAULT_TORQUE = 50           # 默认转矩 (协议默认值, 不在界面输入)

ALARM_POLL_MS = 1000           # 状态查询周期 ms (on_show 启动, on_hide 停止)
ENABLE_ALARM_POLL = True      # 是否定时查询报警/通信状态并显示

# 电机列表: (编号, 从机地址)。编号对应 .ui 中控件 objectName 的后缀 (如 btnEnable1 -> 1)
MOTORS = [(1, 1), (2, 2)]
#MOTORS = [(1, 1),]

# =====================================================
# 模块级状态
# =====================================================

__all__ = ['create_widget', 'EdmUiLoader']

class EdmUiLoader(QUiLoader):
    def __init__(self, parent=None):
        super().__init__(parent)

    def createWidget(self, class_name, parent=None, name=''):
        if class_name == "QLineEdit4Keyboard":
            widget = QLineEdit4Keyboard(parent)
            widget.setObjectName(name)
            return widget
        return QUiLoader.createWidget(self, class_name, parent, name)


def tr(source_text, context="WolongH3Plugin"):
    return QCoreApplication.translate(context, source_text)

PLUGIN_NAME = "卧龙H3"
PLUGIN_VERSION = "1.6.0"

def create_widget(window):
    """创建并返回插件控件."""
    # 此处window = QtWin2
    #print(window)

    # 载入翻译
    translator = QTranslator()
    current_dir = os.path.dirname(os.path.abspath(__file__))
    translator.load(QLocale(), "wolong_h3", "_", current_dir)
    QApplication.instance().installTranslator(translator)

    # 载入UI文件
    ui_file_name = os.path.join(current_dir, "wolong_h3.ui")
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

    # 为每路电机收集控件引用、设置默认值、连接信号
    motors = []                # 每路电机状态: {num, slave, group, lbl_status, edit_speed, edit_accel, last_status}

    for num, slave in MOTORS:
        s = str(num)
        m = {
            "num": num,
            "slave": slave,
            "group": widget.findChild(QGroupBox, f"groupBoxMotor{s}"),
            "lbl_status": widget.findChild(QLabel, f"lblStatus{s}"),
            "edit_speed": widget.findChild(QLineEdit4Keyboard, f"editSpeed{s}"),
            "edit_accel": widget.findChild(QLineEdit4Keyboard, f"editAccel{s}"),
            "last_status": (tr("正常"), "#888888"),
        }
        if m["edit_speed"]:
            m["edit_speed"].setText(str(DEFAULT_SPEED))
        if m["edit_accel"]:
            m["edit_accel"].setText(str(DEFAULT_ACCEL))
        set_motor_status(m, tr("正常"), "#888888")

        # 连接按钮 (用 _bind 返回 0 参回调, 见 _bind 说明)
        _connect(widget, f"btnEnable{s}",  _bind(on_enable, m, window))
        _connect(widget, f"btnDisable{s}", _bind(on_disable, m, window))
        _connect(widget, f"btnForward{s}", _bind(on_forward, m, window))
        _connect(widget, f"btnReverse{s}", _bind(on_reverse, m, window))
        _connect(widget, f"btnStop{s}",    _bind(on_stop, m, window))

        motors.append(m)

    widget.setProperty('motors', motors)

    # 探测 C++ 直挂 task 的 slot 是否可用 (不可用则回退 RunGCode)
    _probe_slots(window)

    # 状态查询定时器
    if ENABLE_ALARM_POLL:
        timer = QTimer(widget)
        timer.setInterval(ALARM_POLL_MS)
        timer.timeout.connect(lambda w=widget: _poll_alarms(w))
        print(f"[WolongH3] QTimer created: {timer}, interval={timer.interval}")

        # 安装事件过滤器监听 show/hide 事件
        #event_filter = _VisibilityEventFilter(timer)
        #widget.installEventFilter(event_filter)
        #print(f"[WolongH3] Event filter installed")

        # 存储定时器和事件过滤器防止被垃圾回收
        widget.setProperty('timer', timer)

    print("[WolongH3 Plugin] Widget loaded")

    return widget


def _connect(widget, object_name, handler):
    """按 objectName 查找按钮并连接 clicked 信号."""
    btn = widget.findChild(QPushButton, object_name)
    if btn:
        btn.clicked.connect(handler)


def _bind(fn, m, window):
    """构造 0 参回调, 供 QPushButton.clicked 连接。

    不能用 ``lambda m=m: fn(m, window)``: clicked 会发射 clicked(bool), PythonQt
    见该 lambda 接受 1 个参数, 便把该 bool 当作 m 传入覆盖默认值。
    这里返回 0 参 lambda, PythonQt 连接到无参的 clicked(), m 经闭包按次绑定。
    """
    return lambda: fn(m, window)


# =====================================================
# 状态显示
# =====================================================
def set_motor_status(m, text, color="#333333"):
    """设置命令状态: 更新显示并记录为最近命令状态 (报警/通信恢复时回到此状态)."""
    m["last_status"] = (text, color)
    _show_status(m, text, color)


def _show_transient(m, text, color):
    """显示瞬时状态 (通信失败/报警), 不改写最近命令状态."""
    _show_status(m, text, color)


def _show_status(m, text, color):
    lbl = m["lbl_status"]
    if lbl:
        lbl.setText(text)
        lbl.setStyleSheet(f"font-weight: bold; color: {color};")


def restore_last_status(m):
    """恢复到最近一次命令状态 (无报警/通信正常时使用)."""
    text, color = m["last_status"]
    _show_status(m, text, color)


# =====================================================
# 底层通信 (异步)
# =====================================================
def _ret_key(m):
    """该电机读写结果 (ret) 的控制器变量名."""
    return f"_reg_{CONN_IDX}_{m['slave']}_ret"


def _data_key(m, addr):
    """该电机某寄存器读数据的控制器变量名."""
    return f"_reg_{CONN_IDX}_{m['slave']}_{addr}"


def _build_write_gcode(m, start_addr, values):
    gcode = f"#<_conn_idx> = {CONN_IDX}\n"
    gcode += f"#<_slave> = {m['slave']}\n"
    gcode += f"#<_reg_cnt> = {len(values)}\n"
    for i, v in enumerate(values):
        gcode += f"#<_reg_{start_addr + i}> = {v}\n"
    gcode += f"do_writereg {start_addr}\n"
    return gcode


def _build_read_gcode(m, addr, quantity=1):
    gcode = f"#<_conn_idx> = {CONN_IDX}\n"
    gcode += f"#<_slave> = {m['slave']}\n"
    gcode += f"#<_reg_cnt> = {quantity}\n"
    gcode += f"do_readreg {addr}\n"
    return gcode


def _run_gcode_async(window, gcode):
    """异步执行 GCode (不阻塞 UI)。返回 True 表示已入队, False 表示引擎忙/未连接."""
    machine = window.GetMachine()
    if not machine:
        return False
    return machine.RunGCode(gcode)


def _probe_slots(window):
    """打印 C++ 直挂 task 的 slot 对 Python 是否可见 (仅诊断)。"""
    machine = window.GetMachine()
    if not machine:
        print("[WolongH3] GetMachine() 在探测时为 None")
        return
    print(f"[WolongH3] hasattr AddReadRegTask: {hasattr(machine, 'AddReadRegTask')}")
    print(f"[WolongH3] hasattr AddWriteRegTask: {hasattr(machine, 'AddWriteRegTask')}")


def _issue_write(m, window, addr, values):
    """异步写寄存器。优先用 AddWriteRegTask 直挂 task (不经 RunGCode/IsGCodeRunning),
    否则回退 RunGCode。返回 True 表示已入队; 实际成败 (ret<0) 由轮询采集后显示通信失败。

    C++ 签名: AddWriteRegTask(addr, quantity, values, connIdx, slave, priority) -> void
    """
    machine = window.GetMachine()
    if not machine:
        return False

    print('_issue_write')
    try:
        machine.AddWriteRegTask(addr, len(values), values, CONN_IDX, m["slave"], WRITE_TASK_PRIORITY)
        return True
    except Exception as e:
        print(e)
        pass
    return _run_gcode_async(window, _build_write_gcode(m, addr, values))


def _get_param(window, key):
    """读取控制器变量 (double -> int)。失败返回 None."""
    machine = window.GetMachine()
    if not machine:
        return None
    try:
        return int(machine.GetControllerParam(key))
    except Exception:
        return None


# =====================================================
# 按钮处理 (异步发命令, 即时更新命令状态)
# =====================================================
def on_enable(m, window):
    """电机使能: 0x3654 <- 0x00008001."""
    if _issue_write(m, window, REG_ENABLE, VAL_ENABLE):
        set_motor_status(m, tr("已使能(待机)"), "#27ae60")
    else:
        _show_transient(m, tr("系统忙碌"), "#aaaaaa")


def on_disable(m, window):
    """电机失能: 0x3654 <- 0x00008000."""
    if _issue_write(m, window, REG_ENABLE, VAL_DISABLE):
        set_motor_status(m, tr("电机失能"), "#888888")
    else:
        _show_transient(m, tr("系统忙碌"), "#aaaaaa")


def on_forward(m, window):
    """正转: 0x4040 <- [转速+, 加速, 减速=加速, 转矩]."""
    sp = read_speed_params(m)
    if not sp:
        return
    speed, accel = sp
    if _issue_write(m, window, REG_SPEED, [speed, accel, accel, DEFAULT_TORQUE]):
        set_motor_status(m, tr("正转运行中") + " " + str(speed) + " rpm", "#2980b9")
    else:
        _show_transient(m, tr("系统忙碌"), "#aaaaaa")


def on_reverse(m, window):
    """反转: 0x4040 <- [转速-, 加速, 减速=加速, 转矩]."""
    sp = read_speed_params(m)
    if not sp:
        return
    speed, accel = sp
    if _issue_write(m, window, REG_SPEED, [-speed, accel, accel, DEFAULT_TORQUE]):
        set_motor_status(m, tr("反转运行中") + " " + str(speed) + " rpm", "#8e44ad")
    else:
        _show_transient(m, tr("系统忙碌"), "#aaaaaa")


def on_stop(m, window):
    """停止: 0x4040 <- [0, 加速, 减速=加速, 转矩]."""
    sp = read_speed_params(m)
    if not sp:
        return
    _, accel = sp
    if _issue_write(m, window, REG_SPEED, [0, accel, accel, DEFAULT_TORQUE]):
        set_motor_status(m, tr("已停止"), "#e67e22")
    else:
        _show_transient(m, tr("系统忙碌"), "#aaaaaa")


def read_speed_params(m):
    """从该电机输入框读取转速与加速时间, 校验后返回 (speed, accel); 非法时返回 None."""
    speed = DEFAULT_SPEED
    accel = DEFAULT_ACCEL
    if m["edit_speed"]:
        t = m["edit_speed"].text.strip()
        if t:
            try:
                speed = int(t)
            except ValueError:
                QMessageBox.warning(m["group"], tr("错误"), tr("转速必须为整数"))
                return None
    if m["edit_accel"]:
        t = m["edit_accel"].text.strip()
        if t:
            try:
                accel = int(t)
            except ValueError:
                QMessageBox.warning(m["group"], tr("错误"), tr("加速时间必须为整数"))
                return None
    return (speed, accel)


# =====================================================
# 报警/通信状态轮询 (异步读 0x4001, 用 ret 判成败)
# =====================================================
def _poll_alarms(widget):
    """100ms 周期: 先收集所有电机上一次操作的 ret+数据更新状态, 再轮询发起一路电机的读。

    每个周期只发起一路电机的读 (round-robin), 避免同周期内连续两次 RunGCode
    因引擎忙而后者被拒 (否则只有第一路电机能被读到)。

    ret 取自 _reg_<conn>_<slave>_ret (-1=失败); 报警数据取自 _reg_<conn>_<slave>_<addr>。
    由于写/读共用同一 ret 键, 读到的 ret 反映该电机最近一次完成的操作 (读或写):
      - ret == -1 -> 通信失败 (写失败也会在此体现)
      - ret 正常 -> 取报警数据: 非0显示报警, 0 恢复最近命令状态
    """
    window = widget.parentWidget()
    if not window:
        return

    motors = widget.motors
    # 1. 收集所有电机状态 (仅控制器变量查询, 不触发 Modbus, 开销很小)
    for m in motors:
        _collect_motor(m, window)

    # 2. 轮询发起一路电机的报警读取 (优先用直接 task 方式, 绕开 GCode 引擎)
    for m in motors:
        _issue_alarm_read(m, window)


def _issue_alarm_read(m, window):
    """发起一次报警读取。

    优先调用 C++ 的 AddReadRegTask; 失败/不可用则回退异步 RunGCode 读。
    结果仍写入相同的 _reg_<conn>_<slave>_ret / _reg_<conn>_<slave>_<addr>, 收集逻辑不变。
    """
    machine = window.GetMachine()
    if not machine:
        return
    try:
        machine.AddReadRegTask(REG_ALARM, 1, CONN_IDX, m["slave"], TASK_TIMER_PRIORITY)
        return
    except Exception as e:
        print(e)
        pass
    _run_gcode_async(window, _build_read_gcode(m, REG_ALARM))



def _collect_motor(m, window):
    """根据上一次操作的 ret+数据更新该电机状态显示.

    失败判定用 ret < 0:
      - 读失败: ret == -1 (doTask 未抹平)
      - 写失败: ret == -9 (Release 普通模式, doTask 把写失败改成 -9)
    注意: Debug 构建 / Release+WriteErrorAsWarning=True 时, doTask 把写失败抹成 ret=1,
    此时写失败无法通过 ret 检测 (会显示为成功); 但读轮询仍会给出 ret=-1 -> 通信失败。
    """
    ret = _get_param(window, _ret_key(m))
    if ret is None:
        return
    if ret < 0:
        _show_transient(m, tr("通信失败"), "#e74c3c")
        return
    alarm = _get_param(window, _data_key(m, REG_ALARM))
    if alarm is not None and alarm != 0:
        _show_transient(m, tr("报警：") + str(alarm), "#e74c3c")
    else:
        restore_last_status(m)


def on_show(window):
    """Called when the widget is shown."""
    print("[WolongH3] Widget shown")
    widget = window.findChild(QWidget, "python_plugin_widget")
    motors = widget.motors
    for m in motors:
        _issue_write(m, window, REG_PERMISSION, VAL_PERMISSION)  # 发送获取指令执行权限 
    widget.timer.start()

def on_hide(window):
    """Called when the widget is hidden."""
    print("[WolongH3] Widget hidden")
    widget = window.findChild(QWidget, "python_plugin_widget")
    widget.timer.stop()
