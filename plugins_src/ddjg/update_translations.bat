@echo off
setlocal

set QT_DIR=C:\Qt\5.15.2\msvc2019
set UI_FILE=pointgeneratorbygrid.ui

echo [1/2] Generate .ts from .ui ...
"%QT_DIR%\bin\lupdate.exe" "%UI_FILE%" -ts ddjg_zh_CN.ts ddjg_en.ts
if %ERRORLEVEL% NEQ 0 (
    echo FAILED: lupdate
    pause
    exit /b %ERRORLEVEL%
)

echo [2/2] Compile .ts to .qm ...
"%QT_DIR%\bin\lrelease.exe" ddjg_zh_CN.ts -qm ddjg_zh_CN.qm
if %ERRORLEVEL% NEQ 0 (
    echo FAILED: lrelease zh_CN
    pause
    exit /b %ERRORLEVEL%
)
"%QT_DIR%\bin\lrelease.exe" ddjg_en.ts -qm ddjg_en.qm
if %ERRORLEVEL% NEQ 0 (
    echo FAILED: lrelease en
    pause
    exit /b %ERRORLEVEL%
)

echo Done: ddjg_zh_CN.qm + ddjg_en.qm
pause