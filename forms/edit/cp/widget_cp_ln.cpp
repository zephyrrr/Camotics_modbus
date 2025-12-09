#include "widget_cp_ln.h"

WidgetCPLN::WidgetCPLN(QWidget *parent)
	: QOptionWidget(parent)
{
	ui.setupUi(this);
}

WidgetCPLN::~WidgetCPLN()
{}

QString WidgetCPLN::getOption() const
{
	return QString("123");
}

void WidgetCPLN::on_btnOk_clicked()
{
	accept();
}