#include "boollineedit.h"

BoolLineEdit::BoolLineEdit(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

BoolLineEdit::~BoolLineEdit()
{}

void BoolLineEdit::setText(const QString & text)
{
	if (text.isEmpty() || text == "0") {
		ui.btnOff->setChecked(true);
	}
	else {
		ui.btnOn->setChecked(true);
	}
}
