#include "multibuttonswidget.h"
#include <QHBoxLayout>
#include "buttonedittablewidget.h"
#include "qnumericpad.h"
#include "widgets/filedialog.h"

MultiButtonsWidget::MultiButtonsWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

MultiButtonsWidget::~MultiButtonsWidget()
{}

void MultiButtonsWidget::addButton(QString caption, std::function<void(bool)> btnFunc)
{
	QPushButton* button = new QPushButton(caption, this);
	ui.horizontalLayout->insertWidget(m_buttons.count(), button);
	m_buttons.push_back(button);
	connect(button, &QPushButton::clicked, this, btnFunc);
	QNumericPad::setButtonDownModeWhenInTouch(this, button);
}

void MultiButtonsWidget::addDefaultButtons(ButtonEditTableWidget* table)
{
	this->addButton(tr("CR"), [table](bool) {
		if (table->isReadOnly())
			return;
		table->copyRow(table->currentRow());
		table->insertNewRow(table->currentRow());
		table->pasteRow();
		});
	this->addButton(tr("SC"), [table](bool) {
		if (table->isReadOnly())
			return;
		table->deleteRow();
		});
	this->addButton(tr("FZ"), [table](bool) {
		if (table->isReadOnly())
			return;
		table->copyRow();
		});
	this->addButton(tr("ZT"), [table](bool) {
		if (table->isReadOnly())
			return;
		table->pasteRow();
		});
	this->addButton(tr("DQ"), [table](bool) {
		if (table->isReadOnly())
			return;
		BaseChildWindow* childWindow = BaseChildWindow::GetChildWindow(table);
		QString path = SystemSettings::GetPath(QString("%1/table").arg(childWindow->objectName()), SystemSettings::ProjectFlag);
		QDir directory(path);
		if (!directory.exists()) {
			directory.mkpath(path);
		}
		QString fileName = NFileDialog::getOpenFileName(table, tr("DKWJ"), path, QString("(*.table.json)"));
		if (!fileName.isEmpty())
			table->deserialize(fileName);
		});
	this->addButton(tr("BC"), [table](bool) {
		if (table->isReadOnly())
			return;
		BaseChildWindow* childWindow = BaseChildWindow::GetChildWindow(table);
		QString path = SystemSettings::GetPath(QString("%1/table").arg(childWindow->objectName()), SystemSettings::ProjectFlag);
		QDir directory(path);
		if (!directory.exists()) {
			directory.mkpath(path);
		}
		QString fileName = NFileDialog::getSaveFileName(table, tr("BCWJ"), path, QString("(*.table.json)"));
		if (!fileName.isEmpty())
			table->serialize(fileName);
		});
}

void MultiButtonsWidget::removeButton(int idx)
{
	if (idx >= 0 && idx < m_buttons.count()) {
		QPushButton* button = m_buttons.at(idx);
		ui.horizontalLayout->removeWidget(button);
		m_buttons.removeAt(idx);
		delete button;
	}
}

void MultiButtonsWidget::setEnabled(bool enabled)
{
	for each(QPushButton* btn in m_buttons)
	{
		btn->setEnabled(enabled);
	}
}
