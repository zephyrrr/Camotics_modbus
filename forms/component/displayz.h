#pragma once

#include <QObject>
#include <QDateTime>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
//#include <QtCharts/QDateTimeAxis>
//#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

class DisplayZ  : public QChartView
{
	Q_OBJECT

public:
	DisplayZ(QWidget *parent);
	~DisplayZ();

	void draw();

	void setIsRunning(bool isRunning);
	void addData(QDateTime dateTime, double value);
	void setEndData(QDateTime dateTime, double value);
private:
	bool m_isRunning = false;
	QList<std::tuple<qint64, double>> m_data;
	std::tuple<qint64, double> m_endData;
	std::tuple<qint64, double> m_firstData;

	int m_drawCount = 0;
	QChart* chart;
	QLineSeries* series;
	//QDateTimeAxis* axisX;
	//QValueAxis* axisY;

signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent* event) override {
        emit clicked();
        QWidget::mousePressEvent(event);
    }
};
