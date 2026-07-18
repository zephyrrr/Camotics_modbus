#include "MyPythonQtScriptingConsole.h"
#include <QApplication>
#include <QClipBoard>
#include <QKeyEvent>
#include <QMimeData>

MyPythonQtScriptingConsole::MyPythonQtScriptingConsole(QWidget* parent, const PythonQtObjectPtr& context, Qt::WindowFlags i)
	: PythonQtScriptingConsole(parent, context, i)
{
	this->setFont(QFont("Consolas", 14));
	this->setMinimumSize(QSize(1024, 768));
}

void MyPythonQtScriptingConsole::keyPressEvent(QKeyEvent* event)
{
	switch (event->key()) {
	case Qt::Key_C:
		if (event->modifiers() == Qt::ControlModifier) {
			// 显式调用复制
			QTextEdit::copy();
			event->accept();
			return;
		}
		break;
	case Qt::Key_V:
		if (event->modifiers() == Qt::ControlModifier) {
			QString clipboardText = QApplication::clipboard()->text();

            QString toCheckCode = "";
			if (clipboardText.contains('\n')) {
                // 3. 按换行符分割文本
                QStringList lines = clipboardText.split('\n');

                // 4. 遍历每一行
                for (int i = 0; i < lines.size(); ++i) {
                    bool isLastLine = (i == lines.size() - 1);

                    if (!lines[i].isEmpty()) {
                        this->insertPlainText(lines[i]);
                    }

                    if (!toCheckCode.isEmpty()) {
                        toCheckCode += "\n"; // 添加换行符，保持多行代码的结构
					}
                    toCheckCode += lines[i];
                    if (CheckSyntax(toCheckCode) || isLastLine || lines[i + 1].isEmpty()) {
                        // 如果当前行是完整的 Python 语句，则执行它
                        executeLine(false);
                        toCheckCode = "";
                    }
                    else {
                        executeLine(true);
                    }
                }
                

                event->accept(); // 拦截事件，不让父类再处理一次
                return;
			}
		}
	}
	PythonQtScriptingConsole::keyPressEvent(event);
}

bool MyPythonQtScriptingConsole::CheckSyntax(QString code)
{
    PythonQtObjectPtr mainContext = PythonQt::self()->getMainModule();

    code = code.replace("\\", "\\\\");  // 转义反斜杠
    code = code.replace("'", "\\'");    // 转义单引号
    code = code.replace("\n", "\\n");   // 转义换行符
    //QString checkScript = QString("import code\n"
    //    "r = code.compile_command('%1') is not None")
    //    .arg(code);

    QString checkScript = QString("import code\n"
		"console=code.InteractiveConsole()\n"
		"r = console.push('%1')\n")
        .arg(code);
    // 注意：这里直接使用基类的 evalScript，因为它返回 QVariant，方便判断
    auto r = mainContext.evalScript(checkScript);
    auto r2 = mainContext.getVariable("r");
    bool isCompleteCode = !r2.toBool();
    return isCompleteCode;
}

void MyPythonQtScriptingConsole::insertFromMimeData(const QMimeData* source)
{
    // 1. 检查剪贴板数据是否包含纯文本
    if (source->hasText()) {
        // 2. 强制使用纯文本内容进行插入
        // 这会丢弃所有的 HTML 标签、字体、颜色等格式信息
        QString plainText = source->text();

        // 3. 调用父类的普通插入函数（或者 QTextCursor 的插入函数）
        // 这样文本就会以控制台当前的默认样式（比如白色字体、黑色背景）显示
        this->insertPlainText(plainText);
    }
}
