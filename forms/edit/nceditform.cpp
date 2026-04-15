#include "nceditform.h"
#include "ui_nceditform.h"
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
#include "BaseMainWindow.h"
#include "widgets/filedialog.h"
#include "utils/nfile.h"
#include "utils/formutils.h"

using namespace CAMotics;

NcEditForm::NcEditForm(QWidget* parent) :
	BaseChildWindow(parent, false),
	ui(new Ui::NcEditForm), findAndReplaceDialog(this, true)
{
	ui->setupUi(this);
	//createNCEdit(ui->horizontalLayout);

	// Simulation and Tool View tabs are not closeable
	ui->fileTabManager->setTabsClosable(false);
	QTabBar* tabBar = ui->fileTabManager->findChild<QTabBar*>();
	for (int i = 0; i < tabBar->count(); i++) {
		tabBar->setTabButton(i, QTabBar::RightSide, 0);
		tabBar->setTabButton(i, QTabBar::LeftSide, 0);
	}

	// Init GUI

	//connect(ui->fileTabManager, SIGNAL(find()), &findAndReplaceDialog,
	//	SLOT(show()));
	connect(ui->fileTabManager, &FileTabManager::find, [this] {
		if (ui->fileTabManager->currentIndex() < 0)
			return;
		auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());
		editor->clearFocus();
		findAndReplaceDialog.show();
	});
	connect(ui->fileTabManager, SIGNAL(findNext()), &findAndReplaceDialog,
		SLOT(find()));
	connect(ui->fileTabManager, SIGNAL(findResult(bool)), &findAndReplaceDialog,
		SLOT(findResult(bool)));
	connect(&findAndReplaceDialog, SIGNAL(find(QString, bool, int)),
		ui->fileTabManager, SLOT(on_find(QString, bool, int)));
	connect(&findAndReplaceDialog, SIGNAL(replace(QString, QString, bool, int, bool)),
		ui->fileTabManager, SLOT(on_replace(QString, QString, bool, int, bool)));

	//connect(ui->fileTabManager, &FileTabManager::currentChanged, [this](int) {
	//	ShowKeyboard(this);
	//});

	QDir dir;
	m_defaultDirName = SystemSettings::instance().GetUserDataDir() + "/nc";
	if (!dir.exists(m_defaultDirName))
	{
		dir.mkdir(m_defaultDirName);
	}

	QNumericPad::setButtonDownModeWhenInTouch(this);

	SoftKeyboardWidget::addVirtualKeyboardAll(&findAndReplaceDialog);
	findAndReplaceDialog.setModal(false);

	connect(ui->fileTabManager, &FileTabManager::editorClicked, [this](QString, int) {
		ShowKeyboard(this);
		if (ui->fileTabManager->currentIndex() >= 0) {
			auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());
			editor->setFocus();
		}
		if (findAndReplaceDialog.isVisible()) {
			// reset findAndReplaceDialog's button state
			findAndReplaceDialog.show();
		}
	});

	
}

NcEditForm::~NcEditForm()
{
    delete ui;
}

void NcEditForm::showEvent(QShowEvent* event)
{
	BaseChildWindow::showEvent(event);

	//ShowKeyboard(this, false, 2);
}

void NcEditForm::RunGCode()
{
	if (ui->fileTabManager->currentIndex() < 0)
		return;

	on_btnSave_clicked();
	SaveData();

	auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());
	QString gcode = editor->toPlainText();

	try {
		gcode = NCMachine::ParseCCode(gcode);
		gcode = NCMachine::ParseHCode(gcode);
		GCodeTool::ParseGCode(gcode);
	}
	catch (const cb::Exception& e) {
		LOG_ERROR("EDM: " << e.what());
		LineLogger::instance().append("ERROR:" + e.getMessage());
		return;
	}
	catch (const std::exception& e) {
		LOG_ERROR("EDM: " << e.what());
		LineLogger::instance().append(tr("EDMZYCW，QXGZ"));
		return;
	}

	QString srcFilePath;
	if (editor->getFile()->getPath().empty()) {
		//std::string filename = "data/nc/temp.nc";
		//if (!filename.empty()) {
		//	editor->getFile()->setPath(filename);
		//	ui->fileTabManager->saveAs();
		//}
		//srcFilePath = QString::fromStdString(filename);
		return;
	}
	else {
		srcFilePath = QString::fromStdString(editor->getFile()->getPath());

		SystemSettings::instance().LastRunNCFileName = QString::fromStdString(editor->getFile()->getBasename());
	}

	QString destFilePath = GetProjectDir() + QDir::separator() + "ncrun.txt";
	if (QFile::exists(destFilePath)) {
		QFile::remove(destFilePath);
	}
	NFile file(destFilePath);
	if (file.open(QIODevice::WriteOnly)) {
		file.write(srcFilePath.toUtf8());
		file.close();
	}
	//QFile::copy(srcFilePath, destFilePath);

	BaseMainWindow* mainWindow = GetMainWindow();
	mainWindow->showChildWindow(tr("NJG"));
	return;

	/*

	ncMachine->RunGCode(gcode, [this]() {
			this->setHValues();
		}, [editor](cb::JSON::ValuePtr sink, int line) {
			line--;
			if (line >= 0)
			{
				try {
					QTextCursor cursor = editor->textCursor();
					cursor.movePosition(QTextCursor::Start);
					cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line);
					cursor.select(QTextCursor::LineUnderCursor);
					editor->setTextCursor(cursor);
				}
				catch (const std::exception& e) {
					LOG_WARNING("EDM: exception: " << e.what());
				}
			}
	});*/
}

void NcEditForm::on_btnNew_clicked()
{
	std::string fileName;
	//if (ui->fileTabManager->count() == 0)
	//	fileName = "nc/default.nc";
	//else
	//	fileName = "nc/default" + std::to_string(ui->fileTabManager->count()) + ".nc";
	ui->fileTabManager->open(new Project::File(fileName));
}

void NcEditForm::OpenFile(std::string filename)
{
	if (!filename.empty()) {
		//auto currentIndex = ui->fileTabManager->currentIndex();
		//if (currentIndex >= 0) {
		//	auto editor = ui->fileTabManager->getEditor(currentIndex);
		//	if (editor->getFile()->getPath().empty()) {
		//		ui->fileTabManager->close(currentIndex);
		//	}
		//}

		for (int i = 0; i < ui->fileTabManager->count(); ++i) {
			std::string filePath2 = ui->fileTabManager->getFile(i)->getPath();
			if (!filePath2.empty() && filePath2 == filename) {
				ui->fileTabManager->close(i);
			}
		}

		ui->fileTabManager->open(new Project::File(filename));
		//auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());
		//SoftKeyboardWidget::addVirtualKeyboard(editor);
	}
}

void NcEditForm::on_btnOpen_clicked()
{
	std::string filename = NFileDialog::getOpenFileName
	(this, tr("Open File"), m_defaultDirName,
		(tr("Supported Files") + QString(" (*.nc);;") +
			tr("All Files") + QString(" (*.*)"))).toUtf8().data();
	OpenFile(filename);
}

void NcEditForm::setHValues() {
	auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());

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
	QMetaObject::invokeMethod(this, [editor, gcode]() {
		editor->setPlainText(gcode);
	}, Qt::QueuedConnection);
}

bool NcEditForm::saveProject(bool saveAs) {
	/*QString filename = project->getFilename().c_str();
	string ext = SystemUtilities::extension(filename.toStdString());

	if (saveAs || filename.isEmpty() || ext != "camotics" ||
		!project->isOnDisk()) {
		if (filename.isEmpty()) filename = "project.camotics";
		else filename = SystemUtilities::swapExtension(filename.toStdString(),
			"camotics").c_str();

		filename = openFile(tr("Save Project"),
			tr("Projects") + QString(" (*.camotics)"),
			filename, true);
		if (filename.isEmpty()) return false;

		string ext = SystemUtilities::extension(filename.toStdString());
		if (ext.empty()) filename.append(".camotics");
		else if (ext != "camotics") {
			warning(tr("Project file must have .camotics extension, not saved!"));
			return false;
		}
	}

	try {
		project->save(filename.toStdString());
		ui->fileTabManager->saveAll();
		showMessage(tr("Saved %1").arg(filename));
		return true;

	}
	catch (const Exception& e) {
		warning(tr("Could not save project: ").append(e.getMessage().c_str()));
	}*/

	return false;
}


void NcEditForm::on_btnSave_clicked()
{
	if (ui->fileTabManager->currentIndex() < 0)
		return;
	try
	{
		auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());
		if (editor->getFile()->getPath().empty()) {
			QString content = editor->toPlainText();
			if (!content.isEmpty()) {
				std::string filename = NFileDialog::getSaveFileName
				(this, tr("Save File"), m_defaultDirName,
					(tr("Supported Files") + QString(" (*.nc);;") +
						tr("All Files") + QString(" (*.*)"))).toUtf8().data();
				if (!filename.empty()) {
					editor->getFile()->setPath(filename);
					ui->fileTabManager->save();
				}
			}
		}
		else {
			ui->fileTabManager->save();
		}
	}
	catch (const std::exception& e)
	{
		LOG_WARNING("EDM: exception: " << e.what());
	}
}

void NcEditForm::on_btnSaveTo_clicked()
{
	if (ui->fileTabManager->currentIndex() < 0)
		return;
	try
	{
		auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());

		std::string filename = NFileDialog::getSaveFileName
		(this, tr("Save File"), m_defaultDirName,
			(tr("Supported Files") + QString(" (*.nc);;") +
				tr("All Files") + QString(" (*.*)"))).toUtf8().data();
		if (!filename.empty()) {
			editor->getFile()->setPath(filename);
			ui->fileTabManager->saveAs();
		}
	}
	catch (const std::exception& e)
	{
		LOG_WARNING("EDM: exception: " << e.what());
	}
}

void NcEditForm::on_btnImport_clicked()
{
	if (ui->fileTabManager->currentIndex() < 0)
		return;
	try
	{
		QString path = NFileDialog::findFirstRemovableDisk(); // "data"
		if (path.isEmpty()) {
			FormUtils::MessageBoxInfo(tr("ZBDUP"));
			return;
		}

		std::string filename = NFileDialog::getOpenFileName
		(this, tr("Open File"), path,
			(tr("Supported Files") + QString(" (*.nc);;") +
				tr("All Files") + QString(" (*.*)"))).toUtf8().data();
		if (!filename.empty()) {

			QString oldFilePath = QString::fromStdString(filename);
			QFileInfo fileInfo(oldFilePath);
			QString newFileName = fileInfo.fileName();
			QDir newDir(m_defaultDirName);
			QString newFilepath = newDir.filePath(newFileName);
			// 直接覆盖，不提示了
			bool b = FormUtils::CopyFileWithOverwrite(oldFilePath, newFilepath, true);

			auto currentIndex = ui->fileTabManager->currentIndex();
			if (currentIndex >= 0) {
				auto editor = ui->fileTabManager->getEditor(currentIndex);
				if (editor->getFile()->getPath().empty()) {
					ui->fileTabManager->close(currentIndex);
				}
			}
			if (b) {
				ui->fileTabManager->open(new Project::File(EUtils::QString2StdString(newFilepath)));
			}
			else {
				ui->fileTabManager->open(new Project::File(filename));
			}
		}
	}
	catch (const std::exception& e)
	{
		LOG_WARNING("EDM: exception: " << e.what());
	}
}

void NcEditForm::on_btnExport_clicked()
{
	if (ui->fileTabManager->currentIndex() < 0)
		return;
	try
	{
		QString path = NFileDialog::findFirstRemovableDisk(); // "data"
		if (path.isEmpty()) {
			FormUtils::MessageBoxInfo(tr("ZBDUP"));
			return;
		}

		auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());

		std::string filename = NFileDialog::getSaveFileName
		(this, tr("Save File"), path,
			(tr("Supported Files") + QString(" (*.nc);;") +
				tr("All Files") + QString(" (*.*)"))).toUtf8().data();
		if (!filename.empty()) {
			auto oldFileName = editor->getFile()->getPath();
			FormUtils::CopyFileWithOverwrite(QString::fromStdString(oldFileName), QString::fromStdString(filename));
			//
			//editor->getFile()->setPath(filename);
			//ui->fileTabManager->saveAs();
			//editor->getFile()->setPath(oldFileName);
		}
	}
	catch (const std::exception& e)
	{
		LOG_WARNING("EDM: exception: " << e.what());
	}
}

void NcEditForm::on_btnClose_clicked()
{
	if (ui->fileTabManager->currentIndex() < 0)
		return;
	on_btnSave_clicked();

	try {
		ui->fileTabManager->close(ui->fileTabManager->currentIndex());
	}
	catch (const std::exception& e)
	{
		LOG_WARNING("EDM: exception: " << e.what());
	}
}

void NcEditForm::on_btnUndo_clicked()
{
	if (ui->fileTabManager->currentIndex() < 0)
		return;
	auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());
	editor->undo();
}

void NcEditForm::on_btnRedo_clicked()
{
	if (ui->fileTabManager->currentIndex() < 0)
		return;
	auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());
	editor->redo();
}

void NcEditForm::on_btnCut_clicked()
{
	if (ui->fileTabManager->currentIndex() < 0)
		return;
	auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());
	editor->cut();
}

void NcEditForm::on_btnCopy_clicked()
{
	if (ui->fileTabManager->currentIndex() < 0)
		return;
	auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());
	editor->copy();
}

void NcEditForm::on_btnPaste_clicked()
{
	if (ui->fileTabManager->currentIndex() < 0)
		return;
	auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());
	editor->paste();
}

void NcEditForm::on_btnDelete_clicked()
{
	if (ui->fileTabManager->currentIndex() < 0)
		return;
	auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());
	editor->textCursor().removeSelectedText();
}

void NcEditForm::on_btnSelectAll_clicked()
{
	if (ui->fileTabManager->currentIndex() < 0)
		return;
	auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());
	editor->selectAll();
}

void NcEditForm::on_btnFind_clicked()
{
	ui->fileTabManager->find();
	//findAndReplaceDialog.find();
	//auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());
	//auto find = new FindDialog(editor, this);
	//find->show();
	
}

void NcEditForm::on_btnFindNext_clicked()
{
	ui->fileTabManager->findNext();
}

void NcEditForm::on_btnKeyboard_clicked()
{
	if (!IsVirtualKeyboardVisible()) { //ui->btnKeyboard->isChecked()) {
		ui->btnKeyboard->setChecked(true);
		ShowKeyboard(this, false, 2);
	}
	else {
		ui->btnKeyboard->setChecked(false);
		HideKeyboard();
	}
}

void NcEditForm::HideKeyboard()
{
	BaseChildWindow::HideKeyboard();
}

void NcEditForm::ShowKeyboard(QWidget* receiver, bool autoHide, int keyboardType)
{
	//if (ui->fileTabManager->currentIndex() < 0)
	//	return;

	//auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());
	//editor->setFocus();

	BaseChildWindow::ShowKeyboard(receiver, autoHide, keyboardType);
}


void NcEditForm::LoadData(QString filePath)
{
	try {
		if (filePath == NULL) {
			filePath = GetProjectDir() + QDir::separator() + "nceditform.json";
		}
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly)) {
			return;
		}
		QString jsonString = file.readAll();
		file.close();
		QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
		QJsonObject jsonObject = doc.object();

		QJsonArray fileList = jsonObject["files"].toArray();

		QHash<QString, QString> alreadyFiles;
		for (int i = 0; i < ui->fileTabManager->count(); ++i) {
			std::string filePath2 = ui->fileTabManager->getFile(i)->getPath();
			if (!filePath2.empty()) {
				alreadyFiles[QString::fromStdString(filePath2)] = "1";
			}
		}

		for (QJsonValue value : fileList) {
			QString fileName = value.toString();
			if (alreadyFiles.contains(fileName))
				continue;
			//setlocale(LC_CTYPE, ".utf8");
			Project::File* file2 = new Project::File(EUtils::QString2StdString(fileName));
			if (file2->exists()) {
				ui->fileTabManager->open(file2);
			}
		}
		int currentIndex = jsonObject["current_index"].toInt();
		if (currentIndex >= 0 && currentIndex < ui->fileTabManager->count()) {
			ui->fileTabManager->setCurrentIndex(currentIndex);

			// reload file content(maybe there is changes in gcode run, for example H*** values)
			auto editor = ui->fileTabManager->getEditor(ui->fileTabManager->currentIndex());
			if (!ui->fileTabManager->isModified(currentIndex)) {
				std::string filePath2 = editor->getFile()->getPath();
				if (!filePath2.empty()) {
					QString srcFilePath = QString::fromStdString(filePath2);
					QFile file2(srcFilePath);
					if (file2.open(QIODevice::ReadOnly)) {
						QByteArray byteData = file2.readAll();
						QString text = QString::fromUtf8(byteData);
						file2.close();
						editor->setPlainText(text);
					}
				}
			}
		}

		//QFile file(filePath);
		//if (file.open(QIODevice::ReadOnly)) {
		//	QTextStream in(&file);
		//	while (!in.atEnd()) {
		//		QString line = in.readLine();
		//		if (alreadyFiles.contains(line))
		//			continue;
		//		alreadyFiles[line] = line;
		//		if (!line.isEmpty()) {
		//			Project::File* file = new Project::File(EUtils::QString2StdString(line));
		//			if (file->exists()) {
		//				ui->fileTabManager->open(file);
		//			}
		//		}
		//	}
		//	file.close();
		//}

		if (ui->fileTabManager->count() == 0) {
			on_btnNew_clicked();
			ui->fileTabManager->setCurrentIndex(0);
		}
	}
	catch (const std::exception& e) {
		LOG_WARNING("EDM: exception: " << e.what());
	}
}

void NcEditForm::SaveData(QString filePath)
{
	if (filePath == NULL) {
		filePath = GetProjectDir() + QDir::separator() + "nceditform.json";
	}

	// do real thing
	QList<QString> fileList;
	for (int i = 0; i < ui->fileTabManager->count(); ++i) {
		std::string p = ui->fileTabManager->getFile(i)->getPath();
		if (!p.empty()) {
			fileList.append(QString::fromStdString(p));
		}
	}
	QJsonObject jsonObject;
	QJsonArray jsonArray;
	for (QString value : fileList) {
		jsonArray.append(QJsonValue(value));
	}
	jsonObject.insert("files", jsonArray);
	jsonObject.insert("current_index", QJsonValue(ui->fileTabManager->currentIndex()));

	QJsonDocument doc(jsonObject);
	QString jsonString = doc.toJson(QJsonDocument::Indented);
	NFile file(filePath);
	if (file.open(QIODevice::WriteOnly)) {
		file.write(jsonString.toUtf8());
		file.close();
	}

	//QFile file(filePath);
	//if (file.open(QIODevice::WriteOnly)) {
	//	QTextStream out(&file);
	//	for (int i = 0; i < ui->fileTabManager->count(); ++i) {
	//		std::string p = ui->fileTabManager->getFile(i)->getPath();
	//		if (!p.empty()) {
	//			out << QString::fromStdString(p) << "\n";
	//		}
	//	}
	//	file.close();
	//}
}

