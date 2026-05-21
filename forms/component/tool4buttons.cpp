#include "tool4buttons.h"
#include "widgets\qnumericpad.h"
#include "../modbus/NCMachine.h"
#include "../utils/pluginutils.h"
#include "../BaseChildWindow.h"

Tool4Buttons::Tool4Buttons(QWidget *parent, QStringList btnCaptions)
	: QToolWidget(parent)
{
	ui.setupUi(this);
	horizontalLayout = ui.horizontalLayout;

	for each(QString caption in btnCaptions)
	{
		addButton(caption);
	}
	QNumericPad::setButtonDownModeWhenInTouch(this);
}

Tool4Buttons::~Tool4Buttons()
{}

QPushButton* Tool4Buttons::addButton(const QString & caption)
{
	QPushButton* btn = new QPushButton(caption, this);
	if (horizontalLayout->count() >= 3) {
		//horizontalLayout = new QHBoxLayout();
		//horizontalLayout->setSpacing(5);
		//horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout%01").arg(ui.verticalLayout->count()));

		//ui.verticalLayout->addLayout(horizontalLayout);
		//resize(400, 500 * ui.verticalLayout->count());
	}
	m_buttons.append(btn);
	horizontalLayout->addWidget(btn);
	QNumericPad::setButtonDownModeWhenInTouch(this, btn);
	return btn;
}

QString Tool4Plugins::getPluginButtonName(const QString& pyFileName)
{
	// 键：Python 文件名（不带路径）
	// 值：英文显示名称（可翻译）
	static const QMap<QString, QString> nameMap = {
		{"grid1", QT_TR_NOOP("Plugin Grid1")},
		{"grid2", QT_TR_NOOP("Plugin Grid2")},
	};

	// 找不到就返回原文件名
	return tr(nameMap.value(pyFileName, pyFileName).toUtf8());;
}

QList<Tool4Buttons*> Tool4Plugins::create(BaseChildWindow* childWindow, QWidget* parent)
{
	QList<Tool4Buttons*> ret = QList<Tool4Buttons*>();
#ifdef PYTHON_SUPPORT
	QMap<QString, QString> pyFiles = PluginUtils::loadPythonScripts(childWindow->objectName());
	int cnt = MAX_BUTTONS_IN_TOOL;
	Tool4Buttons* tool = NULL;
	

	for (const auto& pyFileName : pyFiles.keys()) {
		if (!pyFileName.startsWith("tools_")) {
			continue;
		}
		//if (!PluginUtils::isAllEnglish(pyFileName)) {
		//	continue;
		//}
		if (pyFileName == QStringLiteral("tools_格子1") || pyFileName == QStringLiteral("tools_格子2"))
		{
			// obsolete plugins, delte and skip
			QFile::remove(pyFiles.value(pyFileName));
			continue;
		}
		if (cnt >= MAX_BUTTONS_IN_TOOL) {
			cnt = 0;
			tool = new Tool4Buttons(parent);
			ret.append(tool);
		}
		QString btnText = getPluginButtonName(pyFileName.mid(6));
		QPushButton* btn = tool->addButton(btnText);
		cnt++;
		QString value = pyFiles.value(pyFileName);
		QObject::connect(btn, &QPushButton::clicked, [value, childWindow]() {
			PluginUtils::RunFile(value, childWindow);
			});
	}
#endif
	return ret;
}
