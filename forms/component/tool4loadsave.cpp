#include "tool4loadsave.h"
#include "widgets\qnumericpad.h"
#include "../modbus/NCMachine.h"
#include "widgets\filedialog.h"
#include "widgets/qnumericpad.h"

Tool4LoadSave::Tool4LoadSave(BaseChildWindow* childWindow, QWidget *parent)
	: QToolWidget(parent), m_childWindow(childWindow)
{
	ui.setupUi(this);

	m_path = m_childWindow->GetProjectDir() + QDir::separator() + QString("%1").arg(m_childWindow->objectName());
	QDir directory(m_path);
	if (!directory.exists()) {
		directory.mkpath(m_path);
	}

	QNumericPad::setButtonDownModeWhenInTouch(this);
}

Tool4LoadSave::~Tool4LoadSave()
{}

void Tool4LoadSave::on_btnLoad_clicked()
{
	QString fileName = NFileDialog::getOpenFileName(this, QStringLiteral("打开文件"), m_path, QStringLiteral("(*.json)"));
	if (!fileName.isEmpty()) {
		m_childWindow->LoadData(fileName);
	}
}

void Tool4LoadSave::on_btnSave_clicked()
{
	QString fileName = NFileDialog::getSaveFileName(this, QStringLiteral("保存文件"), m_path, QStringLiteral("(*.json)"));
	if (!fileName.isEmpty()) {
		m_childWindow->SaveData(fileName);
	}
}

