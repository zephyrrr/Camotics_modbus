#pragma once

#include <QWidget>
class QToolWidget : public QWidget {
public:
	QToolWidget(QWidget* parent):
		QWidget(parent) {}
	virtual ~QToolWidget() {}

	virtual void UpdateState() {};
};
