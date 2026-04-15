#include "tool4gcode.h"
#include "widgets\qnumericpad.h"
#include "../modbus/NCMachine.h"
#include "widgets/filedialog.h"
#include "widgets/qnumericpad.h"
#include "utils/nfile.h"
#include "BaseMainWindow.h"
#include "forms/edit/nceditform.h"
#include "utils/gcodeutils.h"

Tool4GCode::Tool4GCode(BaseChildWindow* childWindow, QWidget *parent)
	: QToolWidget(parent), m_childWindow(childWindow)
{
	ui.setupUi(this);

	m_path = SystemSettings::instance().GetUserDataDir() + "/nc";
	QDir directory(m_path);
	if (!directory.exists()) {
		directory.mkpath(m_path);
	}

	QNumericPad::setButtonDownModeWhenInTouch(this);
}

Tool4GCode::~Tool4GCode()
{}

void Tool4GCode::on_btnExport_clicked()
{
	QString s = m_childWindow->GetGCode(false);
	s = GCodeUtils::CleanGCode(s);

	if (!s.isEmpty()) {
		QString filePath = NFileDialog::getSaveFileName(this, tr("BCWJ"), m_path, QString("(*.nc)"));
		if (!filePath.isEmpty()) {

			NFile file(filePath);
			if (file.open(QIODevice::WriteOnly)) {
				file.write(s.toUtf8());
				file.close();

				BaseMainWindow* mainWindow = BaseChildWindow::GetMainWindow();
				mainWindow->showChildWindow(QString("NC"));

				NcEditForm* form = dynamic_cast<NcEditForm*>(mainWindow->getChildWindow(QString("NC")));
				form->OpenFile(filePath.toUtf8().data());
			}
		}
	}
}

