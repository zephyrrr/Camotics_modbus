#pragma once

#include <QWidget>
class QToolWidget : public QWidget {
public:
	QToolWidget(QWidget* parent):
		QWidget(parent) {}
	virtual ~QToolWidget() {}

	virtual int GetHeightCount() { return 1; }	

	virtual void UpdateState() {};
};
