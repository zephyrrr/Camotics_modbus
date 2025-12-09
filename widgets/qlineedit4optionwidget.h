#ifndef QLINEEDIT4OPTIONWIDGET_H
#define QLINEEDIT4OPTIONWIDGET_H

#include <QLineEdit>
#include <QDialog>

class QOptionWidget : public QDialog
{
	Q_OBJECT

public:
	QOptionWidget(QWidget* parent);
	~QOptionWidget();

	virtual QString getOption() const = 0;
};

class QLineEdit4OptionWidget :
    public QLineEdit
{
	Q_OBJECT

public:
	QLineEdit4OptionWidget(QWidget* parent, QOptionWidget* optionWidget);
	~QLineEdit4OptionWidget();

protected:
	void focusInEvent(QFocusEvent* e) override;
	void focusOutEvent(QFocusEvent* e) override;

private:
	QOptionWidget* m_optionWidget;
};

#endif

