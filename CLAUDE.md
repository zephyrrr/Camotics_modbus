# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**EDM (Electrical Discharge Machining) Control System** - A Qt-based HMI application for CNC EDM machines with Modbus communication.

- **Product Name**: EDM
- **Version**: 1.4.0
- **Platform**: Windows (MSVC 2019/2022, Visual Studio 2022)
- **Qt Version**: Qt 5.15.2 MSVC 2019
- **Python**: Embedded Python 3.8 for scripting

## Build Commands

### Visual Studio (Recommended)
```bash
# Open in Visual Studio 2022
Camotics_modbus.sln

# Build configurations: Debug/Release | x86/x64
# Primary configuration: Debug|x64 or Release|x64
```

### QMake (Qt Visual Studio Tools)
```bash
# Run qmake to regenerate makefiles
C:\Qt\5.15.2\msvc2019\bin\qmake.exe Camotics_modbus.pro

# Build with nmake/jom
nmake
```

### Translation Updates
```bash
# Extract translatable strings to .ts file
C:\Qt\5.15.2\msvc2019\bin\lupdate.exe . -ts Translation_en.ts
C:\Qt\5.15.2\msvc2019\bin\lupdate.exe . -ts Translation_zh_CN.ts

# Compile .ts to .qm binary files
C:\Qt\5.15.2\msvc2019\bin\lrelease.exe Translation_en.ts
C:\Qt\5.15.2\msvc2019\bin\lrelease.exe Translation_zh_CN.ts
```

### Python Tools
```bash
# Convert XLS to CSV for database generation
pyinstaller -F plugins_src\RunAutoParamForm\tool\convert_xls_to_csv.py

# Run database conversion
plugins_src\RunAutoParamForm\tool\convert_db.bat
```

## Architecture Overview

### Core Layers

```
┌─────────────────────────────────────────────────────────────┐
│                    UI Layer (forms/)                        │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────────────┐   │
│  │ run/        │ │ manual/     │ │ auto/               │   │
│  │ GCode Run   │ │ Manual Ops  │ │ Auto Parameters     │   │
│  └─────────────┘ └─────────────┘ └─────────────────────┘   │
├─────────────────────────────────────────────────────────────┤
│                 Main Window (forms/mainwindow2.cpp)         │
│  - MDI container management                                 │
│  - Plugin loading (IDoPlugin interface)                     │
│  - Menu navigation system                                   │
├─────────────────────────────────────────────────────────────┤
│               Machine Control Layer (modbus/)               │
│  ┌──────────────────┐ ┌──────────────────┐                 │
│  │ NCMachine        │ │ ModbusAdapter    │                 │
│  │ - GCode parsing  │ │ - RTU/TCP comms  │                 │
│  │ - Axis control   │ │ - Register R/W   │                 │
│  │ - Priority mgmt  │ │ - Polling timer  │                 │
│  └──────────────────┘ └──────────────────┘                 │
├─────────────────────────────────────────────────────────────┤
│              GCode Engine (gcode/)                          │
│  ┌──────────────────────────────┐                          │
│  │ RealtimeJsonMachine          │                          │
│  │ - MachineAdapter subclass    │                          │
│  │ - JSON command generation    │                          │
│  │ - Interrupt handling         │                          │
│  └──────────────────────────────┘                          │
├─────────────────────────────────────────────────────────────┤
│              Utilities (utils/)                             │
│  - TaskThread (priority-based task queue)                   │
│  - DatabaseManager (SQLite)                                 │
│  - EventLogger, GCodeTool, PythonQtRuntime                  │
└─────────────────────────────────────────────────────────────┘
```

### Key Components

**NCMachine** (`modbus/NCMachine.h:1`)
- Central machine controller
- Handles G/H code parsing via `ParseCCode()`, `ParseHCode()`
- Manages axis priority for thread-safe operations
- Registers: 67-69 for special functions (lift, retract)

**ModbusAdapter** (`modbus/modbusadapter.h:1`)
- Modbus RTU/TCP communication
- Task-based priority queue (3 levels: 0=normal, 1=delay, 2=priority)
- Poll timer at 200ms default scan rate
- Uses libmodbus library

**QtWin2** (`forms/mainwindow2.h:47`)
- Main application window
- Child window management via `BaseChildWindow` base class
- Plugin system for extensible forms

**RealtimeJsonMachine** (`gcode/RealtimeJsonMachine.h:12`)
- GCode interpreter bridge
- Generates JSON commands for machine actions
- Supports interrupt exceptions

### Modbus Register Map
- Registers 0-999: General data registers
- Register 67: Retract distance setting
- Register 68: Slow lift height
- Register 69: Slow lift speed
- Register 86: Additional function register

### Plugin Architecture

Plugins implement `IDoPlugin` interface (`plugins_src/iplugin.h`):
```cpp
class IDoPlugin : public IWidgetPlugin : public INamePlugin
{
    virtual QString getName() = 0;
    virtual QWidget* getWidget() = 0;
    virtual QVariant doIt() = 0;
};
Q_DECLARE_INTERFACE(IDoPlugin, "com.edm.IDoPlugin")
```

Plugin directories in `plugins_src/`:
- `RunAutoParamForm/` - Auto parameter generation
- `ddjg/` - Single-point machining
- `sdjg/` - Multi-point machining
- `python/` - Python script integration

### Data Flow

1. **GCode Processing**: `main.cpp` CLI `--parse` mode or runtime via `GCodeTool::ParseGCode()`
2. **Machine Commands**: `RealtimeJsonMachine` converts GCode to JSON commands
3. **Modbus Execution**: `ModbusAdapter` sends commands via ModbusTask queue
4. **State Updates**: Polling timer reads registers, updates UI via models

### Configuration Files

- `data/qGlobal.ini` - System settings (language, UI preferences)
- `data/ncmachine.json` - Machine parameters backup
- `data/fangdiancanshu_data.json` - Discharge parameters
- `data/cp.db` / `data/work.db` - SQLite databases for material/parameters
- `data/default/` - Default NC files

### Debugging

- Application log: `logs/application.log`
- Crash dumps: `logs/core.dmp` (MiniDump format)
- Stack tracing via `StackWalker` library
- Qt debug messages logged with stack trace for Warning/Critical/Fatal

### External Dependencies

- **Qt 5.15.2** - UI framework
- **libmodbus** - Modbus protocol (included in `modbus/libmodbus/`)
- **PythonQt** - Python integration (PythonQt-Qt5-Python3.8_d.dll)
- **cbang** - C++ utility library (JSON, Time, SystemUtilities)
- **StackWalker** - Stack trace library
