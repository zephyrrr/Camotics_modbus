#include "qtquickwindow.h"
#include <QVBoxLayout>
#include <QQuickItem>
#include <QQmlError>
#include <QQmlContext>
#include <QtWidgets>

QtQuickWindow::QtQuickWindow(QWidget *parent)
	: BaseChildWindow(parent)
{
	m_quickWidget = new QQuickWidget();
	QSurfaceFormat format;
	m_quickWidget->setFormat(format);

	//QUrl source("qrc:quickwidget/rotatingsquare.qml");
	QUrl source = QUrl::fromLocalFile("./qml/rotatingsquare.qml");
	bool b = source.isValid();
	if (!b) {
		return;
	}
	//m_quickWidget->resize(300, 300);
	m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
	m_quickWidget->setSource(source);

	connect(m_quickWidget, &QQuickWidget::statusChanged,
		this, &QtQuickWindow::quickWidgetStatusChanged);
	connect(m_quickWidget, &QQuickWidget::sceneGraphError,
		this, &QtQuickWindow::sceneGraphError);

	QVBoxLayout* layout = new QVBoxLayout(parent);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_quickWidget);
	//layout->addWidget(new QPushButton("sfdfsdf"));

	this->setLayout(layout);

	m_quickWidget->show();
}
void QtQuickWindow::quickWidgetStatusChanged(QQuickWidget::Status status)
{
	if (status == QQuickWidget::Error) {
		QStringList errors;
		const auto widgetErrors = m_quickWidget->errors();
		for (const QQmlError& error : widgetErrors)
			errors.append(error.toString());
		//statusBar()->showMessage(errors.join(QStringLiteral(", ")));
	}
}

void QtQuickWindow::sceneGraphError(QQuickWindow::SceneGraphError, const QString& message)
{
	//statusBar()->showMessage(message);
}
QtQuickWindow::~QtQuickWindow()
{}

void QtQuickWindow::RunGCode()
{
	//QMetaObject::invokeMethod(m_quickWidget->rootObject(), "performLayerBasedGrab",
	//	Q_ARG(QVariant, QString()));

	QObject* rootObject = m_quickWidget->rootObject();

	QVariant result;
	
	// Call the QML function and store the return value in the QVariant
	QMetaObject::invokeMethod(rootObject, "getValues", Qt::DirectConnection, Q_RETURN_ARG(QVariant, result));

	auto s = result.toString();
}
