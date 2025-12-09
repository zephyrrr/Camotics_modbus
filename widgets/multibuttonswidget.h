#pragma once

#include <QWidget>
#include "ui_multibuttonswidget.h"
#include <QPushButton>

class ButtonEditTableWidget;

class MultiButtonsWidget : public QWidget
{
	Q_OBJECT

public:
	MultiButtonsWidget(QWidget *parent = nullptr);
	~MultiButtonsWidget();

	void addButton(QString caption, std::function<void(bool)> btnFunc);
	void addDefaultButtons(ButtonEditTableWidget* table);
	void removeButton(int idx);

	void setEnabled(bool enabled);
private:
	Ui::MultiButtonsWidgetClass ui;
	QList<QPushButton*> m_buttons;
};


class MultiButtonsWidgetWrapper : public QObject {

    Q_OBJECT

public Q_SLOTS:
    // add a constructor
    MultiButtonsWidget* new_MultiButtonsWidget(QWidget* parent) { return new MultiButtonsWidget(parent); }

    // add a destructor
    void delete_MultiButtonsWidget(MultiButtonsWidget* o) { delete o; }

    void setObjectName(MultiButtonsWidget* o, const QString& name) { o->setObjectName(name); }


    void addButton(MultiButtonsWidget* o, QString caption, std::function<void(bool)> btnFunc) { o->addButton(caption, btnFunc); }
	void removeButton(MultiButtonsWidget* o, int idx) { return o->removeButton(idx); }
};