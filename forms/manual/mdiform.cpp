#include "mdiform.h"
#include "ui_mdiform.h"
#include <QLabel>
#include <QButtonGroup>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include "camotics/qt/Highlighter.h"
#include "camotics/qt/GCodeHighlighter.h"
#include "utils/LineLogger.h"
#include "../modbus/NCMachine.h"
#include "utils/nfile.h"

using namespace CAMotics;

MdiForm::MdiForm(QWidget *parent) :
    BaseChildWindow(parent, false),
    ui(new Ui::MdiForm)
{
    ui->setupUi(this);
    this->editor = createNCEdit(ui->editorLayout);

	deserialize();

	// Install the event filter on the textEdit widget
	KeyPressFilter* filter = new KeyPressFilter(this);
	editor->installEventFilter(filter);

	// Connect the custom signal from the filter to a slot
	connect(filter, &KeyPressFilter::enterKeyPressed, this, &MdiForm::handleEnterKeyPress);
}

void MdiForm::handleEnterKeyPress() {
	auto cursor = editor->textCursor();

	// 2. Get the current block (line)
	QTextBlock block = cursor.block();

	// 3. Get the text from the block
	QString gcode = block.text();

	try {
		gcode = NCMachine::ParseCCode(gcode);
		gcode = NCMachine::ParseHCode(gcode);
		GCodeTool::ParseGCode(gcode);
	}
	catch (const std::exception& e) {
		LOG_WARNING("EDM: exception: " << e.what());
		LineLogger::instance().append(tr("EDMZYCW，QXGZ"));
		return;
	}

	m_ncMachine->RunGCode(gcode);
}

MdiForm::~MdiForm()
{
    delete ui;
}
CAMotics::NCEdit* MdiForm::createNCEdit(QLayout* layout)
{
	SmartPointer<Highlighter> highlighter;
	highlighter = new GCodeHighlighter;

	CAMotics::NCEdit* editor = new NCEdit(NULL, highlighter, NULL);

	editor->loadDarkScheme();
	editor->setWordWrapMode(QTextOption::NoWrap);
	editor->setPlainText("");

	layout->addWidget(editor);

	return editor;
	//{
	//    QDockWidget* dockWidget = new QDockWidget(tr("BJQ"), this);
	//    dockWidget->setObjectName("GCode Editor");
	//    dockWidget->setWidget(editor);
	//    dockWidget->widget()->setMinimumSize(QSize(100, 100));

	//    // Set the allowed areas for docking
	//    dockWidget->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::RightDockWidgetArea);

	//    // Add the dock widget to the main window
	//    addDockWidget(Qt::BottomDockWidgetArea, dockWidget);
	//}
}


void MdiForm::RunGCode()
{
	this->serialize();

	QString gcode = editor->toPlainText();

	try {
		gcode = NCMachine::ParseCCode(gcode);
		gcode = NCMachine::ParseHCode(gcode);
		GCodeTool::ParseGCode(gcode);
	}
	catch (const std::exception& e) {
		LOG_WARNING("EDM: exception: " << e.what());
		LineLogger::instance().append(tr("EDMZYCW，QXGZ"));
		return;
	}
	m_ncMachine->GetRealtimeJsonMachine()->setFunctionDoPerLineBefore([this](cb::JSON::ValuePtr sink, int line) {
		line--;
		if (line >= 0)
		{
			QTextCursor cursor = this->editor->textCursor();
			cursor.movePosition(QTextCursor::Start);
			cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line);
			cursor.select(QTextCursor::LineUnderCursor);
			this->editor->setTextCursor(cursor);
		}
		});

	m_ncMachine->RunGCode(gcode);
}

bool MdiForm::serialize()
{
	QString projectDir = GetProjectDir();
	NFile file(projectDir + QDir::separator() + "mdiform.nc");
	if (file.open(QIODevice::WriteOnly)) {
		QTextStream out(&file);
		out << this->editor->toPlainText();
		file.close();
		return true;
	}
	return false;
}

bool MdiForm::deserialize()
{
	QString projectDir = GetProjectDir();
	QFile file(projectDir + QDir::separator() + "mdiform.nc");
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream in(&file);
		QString text = in.readAll();
		file.close();

		this->editor->setPlainText(text);

		return true;
	}
	return false;
}

void MdiForm::on_btnKeyboard_clicked()
{
	if (!IsVirtualKeyboardVisible()) { //ui->btnKeyboard->isChecked()) {
		ShowKeyboard(this, false);
	}
	else {
		HideKeyboard();
	}
}

void MdiForm::HideKeyboard()
{
	ui->btnKeyboard->setChecked(false);
	BaseChildWindow::HideKeyboard();
}

void MdiForm::ShowKeyboard(QWidget* receiver, bool autoHide, int keyboardType)
{
	ui->btnKeyboard->setChecked(true);
	this->editor->setFocus();

	BaseChildWindow::ShowKeyboard(receiver, autoHide, keyboardType);
}

