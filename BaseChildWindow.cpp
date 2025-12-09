#include "BaseChildWindow.h"
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "utils/nfile.h"
#include <QDir>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include "BaseMainWindow.h"
#include "modbus/NCMachine.h"

#include "utils/pluginutils.h"

#define BOOST_ALL_NO_LIB
#include <cbang/boost/StartInclude.h>
#include <boost/version.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <cbang/boost/EndInclude.h>

BaseChildWindow::BaseChildWindow(QWidget* parent, bool useNumericKeyboard)
	:SoftKeyboardWidget(parent, useNumericKeyboard), m_ncMachine(NULL), m_toClearInputWidgets(QList<QWidget *>())
{
	//this->setFocusPolicy(Qt::ClickFocus);
	this->setMaximumHeight(700);
}

BaseChildWindow::~BaseChildWindow()
{
}

void BaseChildWindow::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);

	this->setFocus();


	bool isRunning = m_ncMachine->IsGCodeRunning() || m_ncMachine->GetNowOperating() > 0;
	if (isRunning)
		return;

	if (!m_initialized) {
		m_initialized = true;

#ifdef PYTHON_SUPPORT
		QMap<QString, QString> pyFiles = PluginUtils::loadPythonScripts(this->objectName());
		for (const auto& pyFileName : pyFiles.keys()) {
			if (pyFileName == "init") {
				QString value = pyFiles.value(pyFileName);
				PluginUtils::RunFile(value, this);
				break;
			}
		}
#endif
	}


	// 有时候数据要刷新，所以每次显示窗口时都重新加载数据
	//if (!m_dataLoaded) {
		//m_dataLoaded = true;
		LoadData();
	//}
}

void BaseChildWindow::hideEvent(QHideEvent* event)
{
	QWidget::hideEvent(event);

	QList<QLineEdit*> edits = this->findChildren<QLineEdit*>();
	for (QLineEdit* edit : edits)
	{
		if (edit->hasFocus()) {
			edit->clearFocus();
		}
	}

	bool isRunning = m_ncMachine->IsGCodeRunning() || m_ncMachine->GetNowOperating() > 0;
	if (isRunning)
		return;

	SaveData();
	HideKeyboard();
}

//void BaseChildWindow::focusInEvent(QFocusEvent* event)
//{
//	if (event->reason() == Qt::MouseFocusReason) {
//		HideKeyboard();
//	}
//	QWidget::focusInEvent(event);
//}

//void BaseChildWindow::keyPressEvent(QKeyEvent* event)
//{
//	QWidget::keyPressEvent(event);
//}

void BaseChildWindow::LoadData(QString filePath)
{
	BaseChildWindow::LoadData(this, filePath, &m_fileContent);
}

void BaseChildWindow::SaveData(QString filePath)
{
	BaseChildWindow::SaveData(this, filePath, &m_fileContent);
}

void BaseChildWindow::LoadData(QWidget* parent, QString filePath, QHash<QString, QString>* alreadyFileContent)
{
	// 从文件中读取字符串
	if (filePath == NULL) {
		filePath = GetDataFilePath(parent, SystemSettings::instance().GetProjectDir());
	}

	QFile file(filePath);

	if (!file.open(QIODevice::ReadOnly)) {
		return;
	}
	QString jsonString = file.readAll();
	if (alreadyFileContent != NULL) {
		(*alreadyFileContent)[filePath] = jsonString;
	}
	file.close();

	// 将字符串转换为JSON对象
	QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
	QJsonObject json = doc.object();

	// 查找所有的QLineEdit部件
	QList<QLineEdit*> lineEdits = parent->findChildren<QLineEdit*>();
	for (QLineEdit* lineEdit : lineEdits) {
		if (json.contains(lineEdit->objectName())) {
			QString s = json.value(lineEdit->objectName()).toString();
			if (!s.isEmpty()) {
				lineEdit->setText(s);
				lineEdit->setReadOnly(false);
				lineEdit->setStyle(lineEdit->style());
			}
		}
	}

	// 查找所有的QPushButton部件
	QList<QPushButton*> buttons = parent->findChildren<QPushButton*>();
	for (QPushButton* button : buttons) {
		if (!button->isEnabled())
			continue;
		if (!button->isCheckable())
			continue;
		if (json.contains(button->objectName()) && json.value(button->objectName()).toBool()) {
			if (!button->isChecked()) {
				//bool b = button->isChecked();
				button->click();
				//QString x = button->objectName();
				//b = button->isChecked();
			}
		}
	}

	QList<QComboBox*> comboBoxs = parent->findChildren<QComboBox*>();
	for (QComboBox* lineEdit : comboBoxs) {
		if (json.contains(lineEdit->objectName())) {
			QString s = json.value(lineEdit->objectName()).toString();
			if (!s.isEmpty()) {
				lineEdit->setCurrentText(s);
				lineEdit->setEnabled(true);
				lineEdit->setStyle(lineEdit->style());
			}
		}
	}
}

void BaseChildWindow::SaveData(QWidget* parent, QString filePath, QHash<QString, QString>* alreadyFileContent)
{
	if (filePath == NULL) {
		filePath = GetDataFilePath(parent, SystemSettings::instance().GetProjectDir());
	}

	QJsonObject json;
	// 查找所有的QLineEdit部件
	QList<QLineEdit*> lineEdits = parent->findChildren<QLineEdit*>();
	for (QLineEdit* lineEdit : lineEdits) {
		QString key = lineEdit->objectName();
		if (!key.isEmpty()) {
			json.insert(key, QJsonValue(lineEdit->text()));
		}
	}
	// 查找所有的QPushButton部件
	QList<QPushButton*> buttons = parent->findChildren<QPushButton*>();
	for (QPushButton* button : buttons) {
		if (button->isEnabled() && button->isCheckable()) {
			QString key = button->objectName();
			if (!key.isEmpty()) {
				json.insert(key, QJsonValue(button->isChecked()));
			}
		}
	}

	QList<QComboBox*> comboBoxs = parent->findChildren<QComboBox*>();
	for (QComboBox* lineEdit : comboBoxs) {
		QString key = lineEdit->objectName();
		if (!key.isEmpty()) {
			json.insert(key, QJsonValue(lineEdit->currentText()));
		}
	}

	// 将JSON对象转换为字符串
	QJsonDocument doc(json);
	QString jsonString = doc.toJson(QJsonDocument::Indented);

	// 将字符串写入文件
	if (alreadyFileContent != NULL) {
		if (alreadyFileContent->contains(filePath) && jsonString == (*alreadyFileContent)[filePath])
			return;
		(*alreadyFileContent)[filePath] = jsonString;
	}

	NFile file(filePath);
	if (file.open(QIODevice::WriteOnly)) {
		file.write(jsonString.toUtf8());
		file.close();
	}

}

void BaseChildWindow::ClearInputWidgets(QWidget* parent)
{
	if (parent == NULL) {
		parent = this;
	}
	QList<QLineEdit*> edits = parent->findChildren<QLineEdit*>();
	for (QLineEdit* edit: edits)
	{
		edit->clearFocus();
		if (m_toClearInputWidgets.contains(edit)) {
			edit->clear();
		}
	}
	QList<QPushButton*> btns = parent->findChildren<QPushButton*>();
	for (QPushButton* btn : btns)
	{
		if (m_toClearInputWidgets.contains(btn)) {
			btn->setChecked(false);
		}
	}
}

BaseChildWindow* BaseChildWindow::GetChildWindow(QWidget* widget)
{
	return GetParentOfThis<BaseChildWindow>(widget);

	//QWidget* window = nullptr;
	//QWidget* parent = widget->parentWidget();

	//while (parent) {
	//	window = parent;
	//	BaseChildWindow* child = qobject_cast<BaseChildWindow*>(window);
	//	if (child) {
	//		return child;
	//	}
	//	parent = parent->parentWidget();
	//}

	//return nullptr;
}

BaseMainWindow* BaseChildWindow::GetMainWindow()
{
	BaseMainWindow* mainWindow = nullptr;
	foreach(QWidget * widget, QApplication::topLevelWidgets()) {
		mainWindow = qobject_cast<BaseMainWindow*>(widget);
		if (mainWindow)
			break;
	}
	return mainWindow;
}

QString BaseChildWindow::GetProjectDir()
{
	return SystemSettings::instance().GetProjectDir();
}

QString BaseChildWindow::GetDataFilePath(QObject* obj, QString projectDir)
{
	return SystemSettings::GetDataFilePath(obj->objectName(), projectDir);
}

SoftKeyboardWidget::SoftKeyboardWidget(QWidget* parent, bool useNumericKeyboard)
	:QWidget(parent), m_keyboardNumeric(NULL), m_keyboardFull(NULL), m_bAutoHideKeyboard(true), m_useNumericKeyboard(useNumericKeyboard)
{
}
SoftKeyboardWidget::~SoftKeyboardWidget()
{
	if (m_keyboardNumeric) {
		delete m_keyboardNumeric;
	}
	if (m_keyboardFull) {
		delete m_keyboardFull;
	}
}
void SoftKeyboardWidget::addVirtualKeyboard(QWidget* widget)
{
	widget->installEventFilter(this);
}

void SoftKeyboardWidget::addVirtualKeyboardAll(QWidget* parent)
{
	if (!parent)
		return;

	const auto children = parent->findChildren<QLineEdit*>(QString(), Qt::FindDirectChildrenOnly);
	for (QLineEdit* lineEdit : children) {
		if (qobject_cast<QLineEdit*>(lineEdit) == nullptr) {
			continue;
		}
		//QLineEdit4Keyboard* newLineEdit = new QLineEdit4Keyboard(parent->parentWidget());
		//newLineEdit->setText(lineEdit->text());
		//newLineEdit->setGeometry(lineEdit->geometry());

		//delete lineEdit;
		addVirtualKeyboard(lineEdit);
	}
}

bool SoftKeyboardWidget::eventFilter(QObject* obj, QEvent* ev)
{
	if (ev->type() == QEvent::FocusIn) {
		// Handle the focusIn event for editable widgets
		QWidget* widget = qobject_cast<QWidget*>(obj);
		if (widget) {
			//auto w = BaseChildWindow::GetChildWindow(widget);
			//if (w) {
			//	w->ShowKeyboard(widget);
			//}
			this->ShowKeyboard(widget);
		}
	}
	else if (ev->type() == QEvent::FocusOut) {
		QWidget* widget = qobject_cast<QWidget*>(obj);
		QFocusEvent* event = dynamic_cast<QFocusEvent*>(ev);
		if (widget && event) {
			if (event->reason() == Qt::MouseFocusReason) {
				//auto w = BaseChildWindow::GetChildWindow(widget);
				//if (w) {
				//	w->HideKeyboard();
				//}
				this->HideKeyboard();
			}
		}
	}
	return QWidget::eventFilter(obj, ev);
}


void SoftKeyboardWidget::ShowKeyboard(QWidget* receiver, bool autoHide, int keyboardType)
{
	m_bAutoHideKeyboard = autoHide;

	if (keyboardType == 0) {
		keyboardType = m_useNumericKeyboard ? 1 : 2;
	}

	if (keyboardType == 1) {
		if (m_keyboardNumeric == NULL)
		{
			m_keyboardNumeric = new QNumericPad(this);
		}
		QObject* oldReceiver = m_keyboardNumeric->getReceiver();
		if (oldReceiver != receiver) {
			//if (oldReceiver) {
			//	disconnect(oldReceiver, &QWidget::focusOutEvent, this, &BaseChildWindow::hideVirtualKeyboard);
			//}
			if (receiver) {
				m_keyboardNumeric->setReceiver(receiver);
				//m_keyboardNumeric->setFocusProxy(receiver);
				receiver->setFocus(Qt::MouseFocusReason);
			}
			else {
			}
		}

		// 屏幕右下角
		//auto screenGeometry = QApplication::desktop()->availableGeometry();
		//auto screenGeo = screenGeometry.bottomRight();
		//auto msgGeo = m_keyboardNumeric->frameGeometry();
		//msgGeo.moveBottomRight(screenGeo);
		//m_keyboardNumeric->move(msgGeo.topLeft());

		// 屏幕右上角
		//QPoint mainWindowPos = this->pos();
		//QSize mainWindowSize = this->size();
		//int x = mainWindowPos.x() + mainWindowSize.width() - m_keyboardNumeric->width();
		//int y = mainWindowPos.y();
		//m_keyboardNumeric->move(x, y);
		if (receiver) {
			auto x = receiver->geometry().topLeft();
			QPoint pos = receiver->parentWidget()->mapToGlobal(x);
			//QPoint pos2 = this->mapFromGlobal(pos);

			//QPoint newPos = pos + QPoint(80, -10 - m_keyboardNumeric->height());
			//if (newPos.x() + m_keyboardNumeric->width() > QApplication::desktop()->availableGeometry().width())
			//	newPos.setX(QApplication::desktop()->availableGeometry().width() - m_keyboardNumeric->width());
			//if (newPos.y() < 0)
			//	newPos.setY(0);

			//pos = QPoint(870, 150);
			//newPos = this->mapToGlobal(pos);
			// in some window like fangdianform, there is child window which host softkeyboard

			QSize mainWindowSize = this->window()->size();
			// some window, like 放电参数、高挑 in top menu, window position is not like childwindow, so set it absolutely
			QPoint newPos = QPoint(mainWindowSize.width() * 875 / 1920, 600);
			if (pos.y() > 550 && (pos.x() > 850 && pos.x() < 1260))
				newPos = QPoint(400, 600);
			m_keyboardNumeric->move(newPos);
			m_keyboardNumeric->show();
		}
	}
	else if (keyboardType == 2) {
		if (m_keyboardFull == NULL)
		{
			m_keyboardFull = new WidgetKeyboard(this);
		}
		//virtualKeyBoard->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
		QObject* oldReceiver = m_keyboardFull->getReceiver();
		if (oldReceiver != receiver) {
			if (receiver) {
				m_keyboardFull->setReceiver(receiver);
				//m_keyboardFull->setFocusProxy(receiver);
				receiver->setFocus(Qt::MouseFocusReason);
			}
		}
		if (receiver) {
			// 获取主窗口的位置和大小
			QPoint mainWindowPos = this->pos();
			QSize mainWindowSize = this->size();

			// 计算 virtualKeyBoard 的新位置
			int x = mainWindowPos.x() + mainWindowSize.width() - m_keyboardFull->width();
			int y = mainWindowPos.y();
			QPoint pos = this->mapToGlobal(QPoint(mainWindowPos.x() + mainWindowSize.width(), 0));
			y = pos.y() - m_keyboardFull->height() - 10;

			if (x < 0)
				x = 0;
			if (y < 0)
				y = 0;

			// 移动 virtualKeyBoard 到新位置
			m_keyboardFull->move(x, y);
			m_keyboardFull->show();
		}
	}
}

void SoftKeyboardWidget::HideKeyboard()
{
	if (m_keyboardNumeric != NULL) {
		m_keyboardNumeric->hide();
		m_keyboardNumeric->setReceiver(NULL);
	}
	if (m_keyboardFull != NULL) {
		m_keyboardFull->hide();
		m_keyboardFull->setReceiver(NULL);
	}
}

bool SoftKeyboardWidget::IsVirtualKeyboardVisible()
{
	bool b = false;
	if (m_keyboardNumeric != NULL) {
		b = b || m_keyboardNumeric->isVisible();
	}
	if (m_keyboardFull != NULL) {
		b = b || m_keyboardFull->isVisible();
	}
	return b;
}

void SoftKeyboardWidget::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);

	if (m_bAutoHideKeyboard) {
		QWidget* focusWidget = this->focusWidget();
		if (focusWidget) {
			focusWidget->clearFocus();
			HideKeyboard();
		}
	}
}

//SoftKeyboardWidget* SoftKeyboardWidget::GetParentOfThis(QWidget* widget)
//{
//	QWidget* window = nullptr;
//	QWidget* parent = widget->parentWidget();
//
//	while (parent) {
//		window = parent;
//		SoftKeyboardWidget* child = qobject_cast<SoftKeyboardWidget*>(window);
//		if (child) {
//			return child;
//		}
//		parent = parent->parentWidget();
//	}
//
//	return nullptr;
//}

BaseChildWindowWithTools::BaseChildWindowWithTools(QWidget* parent, int toolsType)
	: BaseChildWindow(parent), m_toolsType(toolsType), m_tool4ZeroAndHalfAxis(NULL)
{
}

BaseChildWindowWithTools::~BaseChildWindowWithTools()
{
}

void BaseChildWindowWithTools::showEvent(QShowEvent* event)
{
	if (m_tool4ZeroAndHalfAxis == NULL) {
		int x = 0, y = 0;
#ifdef _DEBUG
		QSize childWindowSize = this->size();
		x = childWindowSize.width() / 4 * 3 / 3 * 2 + 30;
		y = 50;
#endif // _DEBUG

		m_tool4ZeroAndHalfAxis = new Tool4All(m_ncMachine, this, m_toolsType);
		Tool4All::ShowIt(m_tool4ZeroAndHalfAxis, 0, 0, x, y);
	}
	//auto x = this->size();

	BaseChildWindow::showEvent(event);
}

void BaseChildWindowWithTools::UpdateState()
{
	if (m_tool4ZeroAndHalfAxis != NULL) {
		m_tool4ZeroAndHalfAxis->UpdateState();
	}
	bool isRunning = true;
	if (m_ncMachine != NULL) {
		isRunning = m_ncMachine->IsGCodeRunning() || m_ncMachine->GetNowOperating() > 0;
	}
	m_tool4ZeroAndHalfAxis->setEnabled(!isRunning);
}
