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
	this->addButton(QStringLiteral("插入"), [table](bool) {
		if (table->isReadOnly())
			return;
		table->copyRow(table->currentRow());
		table->insertNewRow(table->currentRow());
		table->pasteRow();
		});
	this->addButton(QStringLiteral("删除"), [table](bool) {
		if (table->isReadOnly())
			return;
		table->deleteRow();
		});
	this->addButton(QStringLiteral("复制"), [table](bool) {
		if (table->isReadOnly())
			return;
		table->copyRow();
		});
	this->addButton(QStringLiteral("粘贴"), [table](bool) {
		if (table->isReadOnly())
			return;
		table->pasteRow();
		});
	this->addButton(QStringLiteral("读取"), [table](bool) {
		if (table->isReadOnly())
			return;
		BaseChildWindow* childWindow = BaseChildWindow::GetChildWindow(table);
		QString path = childWindow->GetProjectDir() + QDir::separator() + QString("%1").arg(childWindow->objectName()) + QDir::separator() + "table";
		QDir directory(path);
		if (!directory.exists()) {
			directory.mkpath(path);
		}
		QString fileName = NFileDialog::getOpenFileName(table, QStringLiteral("打开文件"), path, QStringLiteral("(*.table.json)"));
		if (!fileName.isEmpty())
			table->deserialize(fileName);
		});
	this->addButton(QStringLiteral("保存"), [table](bool) {
		if (table->isReadOnly())
			return;
		BaseChildWindow* childWindow = BaseChildWindow::GetChildWindow(table);
		QString path = childWindow->GetProjectDir() + QDir::separator() + QString("%1").arg(childWindow->objectName()) + QDir::separator() + "table";
		QDir directory(path);
		if (!directory.exists()) {
			directory.mkpath(path);
		}
		QString fileName = NFileDialog::getSaveFileName(table, QStringLiteral("保存文件"), path, QStringLiteral("(*.table.json)"));
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
