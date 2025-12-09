#include "qlineedit4optionwidget.h"
#include <QApplication>
#include <QDesktopWidget>

QOptionWidget::QOptionWidget(QWidget* parent)
	: QDialog(parent)
{
}
QOptionWidget::~QOptionWidget()
{}


QLineEdit4OptionWidget::QLineEdit4OptionWidget(QWidget* parent, QOptionWidget* optionWidget)
	: QLineEdit(parent), m_optionWidget(optionWidget)
{
}

QLineEdit4OptionWidget::~QLineEdit4OptionWidget()
{}

void QLineEdit4OptionWidget::focusInEvent(QFocusEvent *e)
{
	QLineEdit::focusInEvent(e); // call superclass handler first

	// show m_optionWidget below this line edit
	//auto screenGeometry = QApplication::desktop()->availableGeometry();
	//auto screenGeo = screenGeometry.bottomRight();
	//auto screenGeo = this->parentWidget()->geometry().topRight();
	//auto msgGeo = m_optionWidget->frameGeometry();
	//msgGeo.moveBottomRight(screenGeo);
	//m_optionWidget->move(msgGeo.topLeft());
	if (m_optionWidget->isVisible())
		return;

	auto r  = m_optionWidget->exec();
	if (r == QDialog::Accepted) {
		this->setText(m_optionWidget->getOption());
	}
}

void QLineEdit4OptionWidget::focusOutEvent(QFocusEvent *e)
{
	QLineEdit::focusOutEvent(e); // call superclass handler first

	//this->setText(m_optionWidget->getOption());
	//m_optionWidget->hide();
}