#pragma once

#include <QWidget>
#include "ui_boollineedit.h"
#include "qlineedit4axisvalue.h"

class BoolLineEdit : public QWidget, public ILineEdit
{
	Q_OBJECT

public:
	BoolLineEdit(QWidget* parent = nullptr);
	~BoolLineEdit();

	void setReadOnly(bool readonly) override { QWidget::setEnabled(!readonly); }

	void setIsEditing(bool b) override { Q_UNUSED(b); };

	void setText(const QString& text) override;
	QString text() const override { return ui.btnOn->isChecked() ? "1" : "0"; }

	QStyle* style() const override { return QWidget::style(); }
	void setStyle(QStyle* style) override { QWidget::setStyle(style); }

private:
	Ui::BoolLineEditClass ui;
};
