#pragma once

#include "BaseChildWindow.h"
#include <QQuickWidget>

class QtQuickWindow  : public BaseChildWindow
{
	Q_OBJECT

public:
	QtQuickWindow(QWidget *parent);
	void quickWidgetStatusChanged(QQuickWidget::Status status);
	void sceneGraphError(QQuickWindow::SceneGraphError, const QString& message);
	~QtQuickWindow();

	void RunGCode() override;

private:
	QQuickWidget* m_quickWidget;
};
