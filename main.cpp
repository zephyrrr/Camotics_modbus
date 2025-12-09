#include "forms_v1/mainwindow.h"
#include "forms/mainwindow2.h"

#include <random>
#include <ctime>

#include <QApplication>
#include <QStyleFactory>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDir>
#include <QFile>
#include <QString>
#include <QDesktopWidget>
#include <QScreen>
#include <QStatusBar>
#include <QFileSystemWatcher>
#include <QMessageBox>
#include <QtPlugin>


#include <signal.h>
#include <windows.h>
#include <DbgHelp.h>
#include <tchar.h>
#include "StackWalker/StackWalker.h"
#include <QProcess>
#include <cbang/os/SystemUtilities.h>
#include <cbang/time/Time.h>
#include <cbang/String.h>
#include "version.h"

#include <string>

#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif


#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSlider>
#include "utils/EventLogger.h"


//#define STYLE_SHEET_PATH 
#define APPLICATION_LOG_PATH "./logs/application.log"


void realRefreshStyle(QFileSystemWatcher* fileWatcher)
{
#ifdef QT_DEBUG
    bool b = fileWatcher->addPath("./resources/myQss.qss");
    if (!b)
    {
        //QMessageBox::warning(NULL, "Warning", "No stylesheet file");
        return;
    }
    QObject::connect(fileWatcher, &QFileSystemWatcher::fileChanged, [](const QString& path) {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly))
        {
            qobject_cast<QApplication*>(QApplication::instance())->setStyleSheet(file.readAll());
            file.close();
        }
        });
#else
    //QFile file(STYLE_SHEET_PATH);
    //if (file.open(QIODevice::ReadOnly))
    //{
    //    qobject_cast<QApplication*>(QApplication::instance())->setStyleSheet(file.readAll());
    //    file.close();
    //}
#endif
}

class MyStackWalker : public StackWalker {
public:
    MyStackWalker() : StackWalker() {}

    void OnOutput(LPCSTR szText) override {
        m_stackTrace += szText;
        //m_stackTrace += "\n";
    }

    const QString& getStackTrace() const {
        return m_stackTrace;
    }

private:
    QString m_stackTrace;
};


void logTo(QString logMessage)
{
    std::string header = "";
    uint64_t now = cb::Time::now(); // Must be the same time for both
    header += Time(now).toString("%Y-%m-%d:");
    header += Time(now).toString("%H:%M:%S:");

    QFile logFile(QDir::currentPath() + APPLICATION_LOG_PATH);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        logFile.write(header.c_str());
        logFile.write(logMessage.toLocal8Bit());
        logFile.close();
    }
}

//#define UNHANDLED_EXCEPTION_TEST
//#define EXCEPTION_FILTER_TEST

// For more info about "PreventSetUnhandledExceptionFilter" see:
// "SetUnhandledExceptionFilter" and VC8
// http://blog.kalmbachnet.de/?postid=75
// and
// Unhandled exceptions in VC8 and above?for x86 and x64
// http://blog.kalmbach-software.de/2008/04/02/unhandled-exceptions-in-vc8-and-above-for-x86-and-x64/
// Even better: http://blog.kalmbach-software.de/2013/05/23/improvedpreventsetunhandledexceptionfilter/

#if defined(_M_X64) || defined(_M_IX86) || defined(_M_ARM64)
static BOOL PreventSetUnhandledExceptionFilter()
{
    HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
    if (hKernel32 == NULL)
        return FALSE;
    void* pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
    if (pOrgEntry == NULL)
        return FALSE;

#ifdef _M_IX86
    // Code for x86:
    // 33 C0                xor         eax,eax
    // C2 04 00             ret         4
    unsigned char szExecute[] = { 0x33, 0xC0, 0xC2, 0x04, 0x00 };
#elif _M_X64
    // 33 C0                xor         eax,eax
    // C3                   ret
    unsigned char szExecute[] = { 0x33, 0xC0, 0xC3 };
#elif _M_ARM64  
    unsigned char szExecute[] = {
        0x00, 0x00, 0x80, 0x52, // mov     w0, #0
        0xC0, 0x03, 0x5F, 0xD6 // ret       
    };
#else
#error "The following code only works for x86 and x64!"
#endif

    DWORD dwOldProtect = 0;
    BOOL  bProt = VirtualProtect(pOrgEntry, sizeof(szExecute), PAGE_EXECUTE_READWRITE, &dwOldProtect);

    SIZE_T bytesWritten = 0;
    BOOL   bRet = WriteProcessMemory(GetCurrentProcess(), pOrgEntry, szExecute, sizeof(szExecute),
        &bytesWritten);

    if ((bProt != FALSE) && (dwOldProtect != PAGE_EXECUTE_READWRITE))
    {
        DWORD dwBuf;
        VirtualProtect(pOrgEntry, sizeof(szExecute), dwOldProtect, &dwBuf);
    }
    return bRet;
}
#else
#pragma message("This code works only for x86, x64 and arm64!")
#endif

const LPCWSTR PIPE_NAME = L"\\\\.\\pipe\\MyAppCrashDumpPipe";

void LaunchDumper() {
    // Replace "dumper.exe" with the actual path to your compiled dumper application
    _spawnl(_P_NOWAIT, "dumper.exe", "dumper.exe", nullptr);
    std::cout << "Launched crash dumper." << std::endl;
    // Consider adding error checking for _spawnl
}

bool SendCrashInfoToDumper(DWORD pid) {
    HANDLE hPipe = CreateFileW(PIPE_NAME, GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "CreateFile (pipe client) failed with error " << GetLastError() << std::endl;
        return false;
    }

    DWORD bytesWritten;
    BOOL success = WriteFile(hPipe, &pid, sizeof(pid), &bytesWritten, nullptr);
    CloseHandle(hPipe);
    return success && bytesWritten == sizeof(pid);
}

void TriggerCrash() {
    std::cout << "Simulating a crash..." << std::endl;
    int* p = nullptr;
    *p = 10; // This will cause a segmentation fault
}
void exhaustMemoryMalloc() {
    std::cout << "Simulating memory exhaustion (new with handler)...\n";
    //try {
        size_t allocationSize = 1024; // Start with 1KB
        while (true) {
            // Attempt to allocate memory
            char* ptr = new char[allocationSize];
            if (ptr == nullptr) {
                std::cerr << "new returned nullptr, allocation failed.\n";
                break; // Exit loop on allocation failure
            }
            // Use the memory (optional, but prevents compiler optimization)
            for (size_t i = 0; i < allocationSize; ++i)
                ptr[i] = 'a';

            std::cout << "Allocated " << allocationSize / 1024 << " KB\n";
            allocationSize = std::min(allocationSize * 2, (size_t)std::numeric_limits<size_t>::max() / 2); //prevent overflow
            delete[] ptr; // Clean up the allocated memory
        }
    //}
    //catch (const std::bad_alloc& e) {
    //    std::cerr << "Caught std::bad_alloc: " << e.what() << "\n";
    //    std::cerr << "Memory exhaustion simulation ended.\n";
    //    throw;
    //}
    std::cout << "exhaustMemoryNew() finished.\n";
}

void create_minidump(struct _EXCEPTION_POINTERS* pExceptionPointers)
{
    //DWORD crashingProcessId = GetCurrentProcessId();
    //if (SendCrashInfoToDumper(crashingProcessId)) {
    //}

    HANDLE  hDumpFile = ::CreateFile(_T("logs/core.dmp"), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDumpFile != INVALID_HANDLE_VALUE) {
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pExceptionPointers;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;

        MINIDUMP_TYPE dumpType = MiniDumpNormal;
        //dumpType = (MINIDUMP_TYPE)(MiniDumpWithFullMemoryInfo |
        //    MiniDumpWithThreadInfo |
        //    MiniDumpWithHandleData |
        //    MiniDumpWithUnloadedModules);

        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, dumpType, &dumpInfo, NULL, NULL);
        CloseHandle(hDumpFile);
    }
}

static BOOL  s_bUnhandledExeptionFilterSet = FALSE;
static LONG __stdcall CrashHandlerExceptionFilter(EXCEPTION_POINTERS* pExp)
{
#ifdef _M_IX86
    if (pExp->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW)
    {
        static char MyStack[1024 * 128]; // be sure that we have enough space...
        // it assumes that DS and SS are the same!!! (this is the case for Win32)
        // change the stack only if the selectors are the same (this is the case for Win32)
        //__asm push offset MyStack[1024*128];
        //__asm pop esp;
        __asm mov eax, offset MyStack[1024 * 128];
        __asm mov esp, eax;
    }
#endif

	create_minidump(pExp);

    MyStackWalker sw; // output to console
    sw.ShowCallstack(GetCurrentThread(), pExp->ContextRecord);
    logTo("Unhandled Exception :" + sw.getStackTrace());

    TCHAR lString[500];
//    _stprintf_s(lString,
//        _T("*** Unhandled Exception! See application.log for more infos!\n")
//        _T("   ExpCode: 0x%8.8X\n")
//        _T("   ExpFlags: %d\n")
//#if _MSC_VER >= 1900
//        _T("   ExpAddress: 0x%8.8p\n")
//#else
//        _T("   ExpAddress: 0x%8.8X\n")
//#endif
//        _T("   Please report!"),
//        pExPtrs->ExceptionRecord->ExceptionCode, pExPtrs->ExceptionRecord->ExceptionFlags,
//        pExPtrs->ExceptionRecord->ExceptionAddress);
	_stprintf_s(lString, _T("程序出现异常，即将退出！"));
    FatalAppExit(-1, lString);
    return EXCEPTION_CONTINUE_SEARCH;
}

static void InitUnhandledExceptionFilter()
{
    if (s_bUnhandledExeptionFilterSet == FALSE)
    {
        // set global exception handler (for handling all unhandled exceptions)
        SetUnhandledExceptionFilter(CrashHandlerExceptionFilter);
#if defined _M_X64 || defined _M_IX86 || defined _M_ARM64
        PreventSetUnhandledExceptionFilter();
#endif
        s_bUnhandledExeptionFilterSet = TRUE;
    }
}


LONG WINAPI ExpFilter(EXCEPTION_POINTERS* pExp, DWORD dwExpCode)
{
    create_minidump(pExp);

    MyStackWalker sw; // output to the console
    sw.ShowCallstack(GetCurrentThread(), pExp->ContextRecord);
    logTo(QString("ExpFilter: %1").arg(sw.getStackTrace()));
    return EXCEPTION_EXECUTE_HANDLER;
}

#ifdef EXCEPTION_FILTER_TEST
void ExpTest5()
{
    char* p = NULL;
    p[0] = 0;
    printf(p);
}
void ExpTest4()
{
    ExpTest5();
}
void ExpTest3()
{
    ExpTest4();
}
void ExpTest2()
{
    ExpTest3();
}
void ExpTest1()
{
    ExpTest2();
}
void TestExceptionWalking()
{
    __try
    {
        ExpTest1();
    }
    __except (ExpFilter(GetExceptionInformation(), GetExceptionCode()))
    {
        printf("\n\nException-Handler called\n\n\n");
    }
}

#endif // EXCEPTION_FILTER_TEST

void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    QByteArray logMessage;

    QDateTime now = QDateTime::currentDateTime();
    logMessage = now.toString("yy-MM-ddThh:mm:ss:").toLocal8Bit();

    switch (type) {
    case QtDebugMsg:
        logMessage += "Debug: ";
        break;
    case QtInfoMsg:
        logMessage += "Info: ";
        break;
    case QtWarningMsg:
        logMessage += "Warning: ";
        break;
    case QtCriticalMsg:
        logMessage += "Critical: ";
        break;
    case QtFatalMsg:
        logMessage += "Fatal: ";
        break;
    }

    logMessage += message.toLocal8Bit();
    logMessage += "\n";

    // Print the stack trace if the message type is QtFatalMsg or QtCriticalMsg
    if (type == QtFatalMsg || type == QtCriticalMsg || type == QtWarningMsg) {  //  || type == QtWarningMsg
        logMessage += "Stack trace:\n";
        MyStackWalker walker;
        walker.ShowCallstack();
        logMessage += walker.getStackTrace().toLocal8Bit();
    }
    logTo(logMessage);
}

void signalHandler(int signal)
{
    return;

    // Raise a custom Qt message to be handled by the message handler
    //qFatal("Segmentation fault caught (signal %d)", signal);
    TCHAR lString[500];
    _stprintf_s(lString, _T("程序出现异常，即将退出！"));
    FatalAppExit(-1, lString);
}



int mainQt(int argc, char* argv[])
{
    //if (false) {
    //    NCMachine* ncMachine = new NCMachine(NULL, NULL);
    //    ncMachine->enterSetPriority(1);
    //    ncMachine->enterSetPriority(1);
    //    ncMachine->enterSetPriority(2);
    //    ncMachine->enterSetPriority(3);
    //    ncMachine->enterSetPriority(2);
    //    ncMachine->exitSetPriority();
    //    ncMachine->exitSetPriority();
    //    ncMachine->exitSetPriority();
    //    ncMachine->exitSetPriority();
    //    ncMachine->exitSetPriority();
    //    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //    std::random_device rd; // Create ONE random_device
    //    std::seed_seq seed_gen{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() }; // Use seed_seq for better initial seeding
    //    std::vector<std::mt19937> generators(1000);
    //    for (auto& gen : generators)
    //        gen.seed(seed_gen);
    //    std::thread* threads[1000];
    //    for (int i = 0; i < 1000; ++i) {
    //        std::thread* t1 = new std::thread([ncMachine, generator = generators[i]]() mutable {
    //            std::uniform_int_distribution<int> distribution(1, 3);
    //            int random_number = distribution(generator);
    //            EUtils::sleep(random_number * 1000);
    //            ncMachine->enterSetPriority(random_number - 1);
    //            random_number = distribution(generator);
    //            EUtils::sleep(random_number * 1000);
    //            ncMachine->exitSetPriority();
    //            random_number = distribution(generator);
    //            EUtils::sleep(random_number * 1000);
    //            });
    //        threads[i] = t1;
    //    }
    //    for (int i = 0; i < 1000; ++i) {
    //        threads[i]->join(); // Wait for t1 to complete
    //        delete threads[i];
    //    }
    //    std::cout << "Main thread finished." << std::endl;
    //    return 0;
    //}

    //MessageBox(
    //    NULL, // Handle to the owner window (NULL for no owner)
    //    L"This is the message text.", // Text to display in the message box
    //    L"Message Box Title", // Title of the message box
    //    MB_OK | MB_ICONINFORMATION // Style of the message box (OK button and information icon)
    //);
    
    //printf("\n\n\nCatch unhandled exceptions and show the callstack:\n\n\n");
    // This will only work, if the program is *not* started under a debugger
    // If the program is running under a debugger, the debugger will catch this exception ;)
    //f(0);  // endless recursion

    //char* szTemp = (char*)1;
    //strcpy_s(szTemp, 1000, "A");


    if (argc == 3) {
        if (std::string(argv[1]) == "--parse") {
            // 检查--parse后面是否有参数
                // 检查参数是否是abc.nc
            QString fileName = QString(argv[2]);
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                return 0;
            }
            QTextStream in(&file);
            QString gcode = in.readAll();
            file.close();
            QString gcodeNew;
            try {
                gcode = NCMachine::ParseCCode(gcode);
                gcode = NCMachine::ParseHCode(gcode);
                gcodeNew = GCodeTool::ParseGCode(gcode);
            }
			catch (const std::exception& e) {
                gcodeNew = QString("Exception: %1").arg(e.what());
			}
			catch (...) {
                gcodeNew = QString("Exception: %1").arg("unknown");
			}
			fileName = fileName + ".new";
            QFile file2(fileName);
            if (!file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
                return 0;
            }

            QTextStream out(&file2);
            out << gcodeNew;
            file2.close();
            return 0;
        }
        return 1;
    }

    // https://doc.qt.io/qt-6/qlocale.html#toString-21
    //int n = QLocale::FloatingPointShortest;

    QFile logFile(QDir::currentPath() + APPLICATION_LOG_PATH);
  //  QFileInfo fileInfo(logFile.fileName());
  //  if (fileInfo.exists()) {
		//// rename the existing log file
		//QString newFileName = fileInfo.baseName() + "-" + QDateTime::currentDateTime().toString("yyMMdd-hhmmss") + "." + fileInfo.completeSuffix();
		//logFile.rename(QDir::currentPath() + "/logs/" + newFileName);
  //  }
    //qint64 fileSize = fileInfo.size();
    //if (fileSize > 100000000L)
    //{
    //    logFile.remove();
    //}
    try {
        cb::SystemUtilities::rotate(
            EUtils::QString2StdString(logFile.fileName()), "logs", 30);
    } CATCH_ERROR;

    logTo("EDM Start...\n");

    //QTextStream(stdout) << (*((int*)0)) << endl;

    /*auto s = QStyleFactory::keys();
    QApplication::setStyle(QStyleFactory::create("windows"));*/

    QApplication qtApp(argc, argv);

    EventLogger* logger = new EventLogger(&qtApp);
    qtApp.installEventFilter(logger);

    QApplication::setWindowIcon(QIcon(":/resources/image/status_icon.png"));

	QApplication::setOrganizationName(COMPANY_NAME);
    QApplication::setApplicationName(PRODUCT_NAME);
	QApplication::setApplicationVersion(PRODUCT_VERSION);

    QLocale curLocale(QLocale(QLocale::Language::Chinese, QLocale::Country::China));
    QLocale::setDefault(curLocale);

    QTranslator qtTran;

    //qtTran.load(QLocale::system(), QStringLiteral("qtbase_"));
    //qtApp.installTranslator(&qtTran);

    auto translationsPath = "./translations"; // QLibraryInfo::location(QLibraryInfo::TranslationsPath);
    bool b = qtTran.load("qt_zh_CN.qm", translationsPath);
    b = qtApp.installTranslator(&qtTran);

    QTranslator qtTran2;
    b = qtTran2.load("qtbase_zh_CN.qm", translationsPath);
    qtApp.installTranslator(&qtTran2);

    QTranslator qtTran3;
    b = qtTran3.load("Translation_zh_CN.qm", translationsPath);
    qtApp.installTranslator(&qtTran3);

    //auto fileName = QDir::currentPath() + "/styles/ElegantDark.qss";
    auto fileName = ":/resources/myQss.qss";// ":/qt/form/stylesheet.qss";
    QFile file(fileName);
    if (file.exists())
    {
        file.open(QFile::ReadOnly);
        qtApp.setStyleSheet(file.readAll());
        file.close();
    }
    QFileSystemWatcher fileWatcher;
    realRefreshStyle(&fileWatcher);

    QString dirName = "data";
    QDir dir;
    if (!dir.exists(dirName))
    {
        dir.mkdir(dirName);
    }

    //auto x = QApplication::translate("NCMachine", "to_clear_machine_axis");

    if (false)
    {
        //QMainWindow w;
        //w.setCentralWidget(new EditableTextWidget(QStringLiteral("1. 坐标移动到 X%x:10%，Y%y:9%")));
        //w.setWindowState(w.windowState() | Qt::WindowMaximized);
        //w.show();
        //int n = qtApp.exec();
        //return n;
    }
    else {
        QtWin2 w;
        //QtWin3 w;
        //QtWin w;
#ifdef _DEBUG
        w.setMaximumSize(QSize(1920 * 2, 1080 * 2));
#endif // _DEBUG

        //w.showMaximized();
        w.setWindowState(w.windowState());
        w.show();

        //QDesktopWidget* desktop = QApplication::desktop();
        //QRect screenSize2 = desktop->availableGeometry(&w);
        //QList<QScreen*> screens = QApplication::screens();
        //QScreen* screen = screens.at(screens.count() - 1);
        //QRect screenSize = screen->availableGeometry();
        ////w.setGeometry(screenSize);
        //w.setFixedSize(QSize(screenSize.width(), screenSize.height() - w.statusBar()->height()));
        //w.move(0, 0);

        int n = qtApp.exec();
        logTo("EDM End\n");
        return n;
    }
}

// 编译选项里，给Release增加debug info：1. C++ -> 常规 -> 调试信息 2.链接器 -> 调试 -> 生成调试信息
int main(int argc, char* argv[])
{
    HANDLE mutex = CreateMutex(NULL, TRUE, TEXT("UniqueAppMutexName4EDM"));
    if (mutex == NULL) {
        std::cout << "CreateMutexW failed: %d\n" << GetLastError() << std::endl;
        return 1;
    }
    // Check if the mutex was created or if it already exists
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        // Another instance is running, handle the situation
        std::cout << "Another instance is already running." << std::endl;
        CloseHandle(mutex);
        return 1;
    }

    //Q_IMPORT_PLUGIN(PointGenratorByGrid)

    //qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    //qputenv("QT_VIRTUALKEYBOARD_OVERLAY", QByteArray("1"));

    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

    qInstallMessageHandler(myMessageOutput); // Install the handler
    ////signal(SIGSEGV, signalHandler);

    //printf("\n\n\nShow a the callstack from inside an exception-handler:\n\n\n");
    //TestExceptionWalking();

    InitUnhandledExceptionFilter();

    //QCoreApplication watchdogApp(argc, argv);

    //// Path to the main application executable
    //QString appPath = "path/to/your/main/application/executable";

    //// Create a QProcess to launch the main application
    //QProcess* process = new QProcess(&watchdogApp);
    //process->setProgram(appPath);

    //// Connect the finished signal to restart the main application
    //QObject::connect(process, &QProcess::finished, [process](const int exitCode) {
    //    process->start();
    //    });

    //// Start the main application for the first time
    //process->start();

    //return watchdogApp.exec();

    int currentExitCode = 0;
    do {
        //LaunchDumper();
        //std::this_thread::sleep_for(std::chrono::seconds(1));

        if (true) {
            currentExitCode = mainQt(argc, argv);
        }
        else {
            __try
            {
                //TriggerCrash();
                //exhaustMemoryMalloc();
                currentExitCode = mainQt(argc, argv);
            }
            __except (ExpFilter(GetExceptionInformation(), GetExceptionCode()))
            {
                TCHAR lString[500];
                _stprintf_s(lString, _T("程序出现异常，即将退出！"));

                DWORD exceptionCode = GetExceptionCode();
                TCHAR hexBuffer[16]; // Enough for 8 hex digits + null terminator
                _stprintf_s(hexBuffer, _countof(hexBuffer), _T("0x%X\n"), exceptionCode);
                _tcscat_s(lString, _countof(lString), _T(" 异常代码: "));
                _tcscat_s(lString, _countof(lString), hexBuffer);

                HMODULE hNtDll = LoadLibrary(_T("NTDLL.DLL"));
                LPTSTR messageBuffer = nullptr;
                DWORD result = FormatMessage(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
                    hNtDll,
                    exceptionCode,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                    (LPTSTR)&messageBuffer,
                    0,
                    nullptr
                );
                FreeLibrary(hNtDll);

                if (result != 0) {
                    LocalFree(messageBuffer);
                    _tcscat_s(lString, _countof(lString), messageBuffer);
                }
                else {
                    DWORD lastError = GetLastError();
                    _tcscat_s(lString, _countof(lString), _T("Unknown Exception Code"));
                }
                FatalAppExit(-1, lString);
            }
        }
        //std::cout << "Current exit code: " << currentExitCode << std::endl;
    } while (currentExitCode == 1000);

    // Release the mutex
    ReleaseMutex(mutex);
    CloseHandle(mutex);

    return currentExitCode;
}


////////////////////////////
//class MainWindow : public QWidget {
//public:
//    MainWindow(QWidget* parent = nullptr) : QWidget(parent) {
//        // 设置背景渐变
//        setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1c1c2b, stop:1 #3c3c5b);");
//
//        // 创建顶部按钮
//        QPushButton* moveButton = new QPushButton("移动");
//        QPushButton* halfMoveButton = new QPushButton("半程移动");
//        QPushButton* limitMoveButton = new QPushButton("限位移动");
//
//        // 设置按钮样式
//        QString buttonStyle = "QPushButton { background-color: #4CAF50; color: white; border: none; padding: 10px; }"
//            "QPushButton:hover { background-color: #45a049; }";
//        moveButton->setStyleSheet(buttonStyle);
//        halfMoveButton->setStyleSheet(buttonStyle);
//        limitMoveButton->setStyleSheet(buttonStyle);
//
//        // 创建输入框
//        QLineEdit* xInput = new QLineEdit("365.000");
//        QLineEdit* yInput = new QLineEdit();
//        QLineEdit* zInput = new QLineEdit();
//        QLineEdit* uInput = new QLineEdit();
//
//        // 设置输入框样式
//        QString inputStyle = "QLineEdit { background-color: white; border: 1px solid #ccc; padding: 5px; }";
//        xInput->setStyleSheet(inputStyle);
//        yInput->setStyleSheet(inputStyle);
//        zInput->setStyleSheet(inputStyle);
//        uInput->setStyleSheet(inputStyle);
//
//        // 创建坐标系选择
//        QComboBox* coordinateSystem = new QComboBox();
//        coordinateSystem->addItem("绝对");
//        coordinateSystem->addItem("相对");
//
//        // 创建移动方式选择
//        QComboBox* moveMode = new QComboBox();
//        moveMode->addItem("线性");
//        moveMode->addItem("旋转");
//
//        // 创建接触感知开关
//        QCheckBox* contactSensing = new QCheckBox("接触感知");
//        contactSensing->setStyleSheet("QCheckBox { color: white; }");
//
//        // 创建布局
//        QVBoxLayout* mainLayout = new QVBoxLayout();
//        QHBoxLayout* buttonLayout = new QHBoxLayout();
//        QFormLayout* inputLayout = new QFormLayout();
//        QHBoxLayout* settingsLayout = new QHBoxLayout();
//
//        // 添加按钮到布局
//        buttonLayout->addWidget(moveButton);
//        buttonLayout->addWidget(halfMoveButton);
//        buttonLayout->addWidget(limitMoveButton);
//
//        // 添加输入框到布局
//        inputLayout->addRow("X:", xInput);
//        inputLayout->addRow("Y:", yInput);
//        inputLayout->addRow("Z:", zInput);
//        inputLayout->addRow("U:", uInput);
//
//        // 添加设置到布局
//        settingsLayout->addWidget(coordinateSystem);
//        settingsLayout->addWidget(moveMode);
//        settingsLayout->addWidget(contactSensing);
//
//        // 添加所有布局到主布局
//        mainLayout->addLayout(buttonLayout);
//        mainLayout->addLayout(inputLayout);
//        mainLayout->addLayout(settingsLayout);
//
//        // 设置主布局
//        setLayout(mainLayout);
//    }
//};
//
//int main_test(int argc, char* argv[]) {
//    QApplication app(argc, argv);
//
//    MainWindow window;
//    window.show();
//
//    return app.exec();
//}