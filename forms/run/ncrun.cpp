#include "ncrun.h"
#include "ui_ncrun.h"
#include <QLabel>
#include <QButtonGroup>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include "camotics/qt/Highlighter.h"
#include "camotics/qt/GCodeHighlighter.h"
#include "utils/LineLogger.h"
#include "camotics/project/File.h"
#include "../modbus/NCMachine.h"
#include "../manual/mdiform.h"
#include "BaseMainWindow.h"
#include "utils/nfile.h"
#include <utils/dataform.h>

using namespace CAMotics;

NcRunForm::NcRunForm(QWidget* parent) :
	BaseChildWindow(parent),
	ui(new Ui::NcRunForm)
{
	ui->setupUi(this);
	this->editor = MdiForm::createNCEdit(ui->verticalLayout);
	this->editor->setReadOnly(true);
	//editor->setEnabled(false);
	editor->setTextInteractionFlags(Qt::NoTextInteraction);
}

NcRunForm::~NcRunForm()
{
    delete ui;
}

void NcRunForm::RunGCode()
{
	QString gcode = editor->toPlainText();

	/*if (gcode.contains("G01")) {
		
		int beepTime = DataForms::getInstance()->getDataForm("xitongshezhi5")->getValue("FDWCHMJSJ").toInt() * 1000;
		QString insertCode = QString(R"(
do_beep %1;
do_light 2;
)").arg(beepTime);

		QStringList lines = gcode.split('\n');
		int m02Index = -1;
		for (int i = 0; i < lines.size(); ++i) {
			if (lines.at(i).contains("M02")) {
				m02Index = i;
				break;
			}
		}
		if (m02Index != -1) {
			lines.insert(m02Index, insertCode);
		}
		gcode = lines.join('\n');

		editor->setPlainText(gcode);
	}*/
	m_ncMachine->GetRealtimeJsonMachine()->setFunctionDoPerLineBefore([this](cb::JSON::ValuePtr sink, int line) {
		line--;
		if (line >= 0)
		{
			QMetaObject::invokeMethod(this, [this, line]() {
				try {
					QTextCursor cursor = editor->textCursor();
					cursor.movePosition(QTextCursor::Start);
					cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line);
					cursor.select(QTextCursor::LineUnderCursor);
					editor->setTextCursor(cursor);
					cursor.movePosition(QTextCursor::StartOfLine);
				}
				catch (const std::exception& e) {
					LOG_WARNING("EDM: exception: " << e.what());
				}
				}, Qt::QueuedConnection);
		}
		});
	m_ncMachine->RunGCode(gcode, [this]() {
			this->setHValues();

			// 不行
			//m_ncMachine->RunGCodeSync("do_beep 200");

			if (m_ncMachine->GetRealtimeJsonMachine()->isTaskToContinue()) {
				int beepTime = DataForms::getInstance()->getDataForm("xitongshezhi5")->getValue("FDWCHMJSJ").toInt() * 1000;
				m_ncMachine->enterSetPriority(0);
				m_ncMachine->Beep(1, beepTime);
				m_ncMachine->exitSetPriority();
			}

			QMetaObject::invokeMethod(this, [this]() {
				QTimer::singleShot(500, this, [this] {
					BaseMainWindow* mainWindow = GetMainWindow();
					mainWindow->showChildWindow(QString("NC"));
					});
			}, Qt::QueuedConnection);
		});
}

void NcRunForm::setHValues() {
	QString gcode = editor->toPlainText();
	QRegularExpression regex("H(\\d+)\\s+=\\s+[\\+\\-]?([\\d\\.]+)");

	QRegularExpressionMatchIterator i = regex.globalMatch(gcode);
	while (i.hasNext()) {
		QRegularExpressionMatch match = i.next();
		int h = match.captured(1).toInt();
		double v = this->m_ncMachine->GetController()->get("_h" + std::to_string(h));

		QString formatted;
		if (v >= 0) {
			//formatted = QString("+%1").arg((int)v, 8, 10, QChar('0'));
			formatted = QString("+") + QString("%1").arg(v, 0, 'f', 3, ' ').rightJustified(10, '0');
		}
		else {
			//formatted = QString("-%1").arg(-(int)v, 8, 10, QChar('0'));
			formatted = QString("-") + QString("%1").arg(-v, 0, 'f', 3, ' ').rightJustified(10, '0');
		}
		QString replacement = QString("H%1   = %2").arg(match.captured(1)).arg(formatted);

		gcode = gcode.replace(match.captured(0), replacement);
	}
	if (!srcFilePath.isEmpty()) {
		NFile file(srcFilePath);
		if (file.open(QIODevice::WriteOnly)) {
			file.write(gcode.toUtf8());
			file.close();
		}
	}

	//QMetaObject::invokeMethod(this, [this, gcode]() {
	//	editor->setPlainText(gcode);
	//}, Qt::QueuedConnection);
}


void NcRunForm::LoadData(QString filePath)
{
	if (filePath == NULL) {
		filePath = GetProjectDir() + QDir::separator() + "ncrun.txt";
	}
	srcFilePath = "";
	{
		QFile file(filePath);
		if (file.open(QIODevice::ReadOnly)) {
			srcFilePath = QString::fromUtf8(file.readAll());
			file.close();
		}
		QFile::remove(filePath);
	}

	if (srcFilePath.isEmpty()) {
		return;
	}
	{
		bool hasCode = false;
		QFile file(srcFilePath);
		if (file.open(QIODevice::ReadOnly)) {
			QString text = QString::fromUtf8(file.readAll());
			file.close();
			this->editor->setPlainText(text);
			hasCode = !text.isEmpty();
		}

		if (hasCode) {
			RunGCode();
		}
	}
}

void NcRunForm::SaveData(QString filePath)
{
}

