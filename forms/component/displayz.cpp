#include "displayz.h"

#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

#define CHART_MAX_POINT 20
#define CHART_REFRESH_INTERVAL 20

DisplayZ::DisplayZ(QWidget *parent)
	: QChartView(parent)
{
    this->setRenderHint(QPainter::Antialiasing);

    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFixedSize(400, 250);
    this->setMaximumSize(400, 250);
    this->setFrameStyle(QFrame::NoFrame);
    this->setFrameShape(QFrame::Shape::NoFrame);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setViewportMargins(0, 0, 0, 0);

    chart = new QChart();
    chart->setBackgroundBrush(QBrush(Qt::transparent));
    chart->legend()->hide();
    chart->createDefaultAxes();
    chart->setTitle(tr("ZZ SD"));

    this->setChart(chart);

    series = new QLineSeries(chart);
    chart->addSeries(series);

    QDateTimeAxis* axisX = new QDateTimeAxis(chart);
    axisX->setFormat("mm:ss");
    axisX->setTitleText(tr("SJ"));
    //QFont font = axisX->labelsFont();
    //font.setPointSize(4);
    //axisX->setLabelsFont(font);
    axisX->setLabelsAngle(0);
    //axisX->setTickCount(50);

    //axisX->setMin(QDateTime::fromMSecsSinceEpoch(std::get<0>(m_data.first())));
    //axisX->setMax(QDateTime::fromMSecsSinceEpoch(std::get<0>(m_endData)));
    axisX->setMin(QDateTime::fromMSecsSinceEpoch(0));
    axisX->setMax(QDateTime::fromMSecsSinceEpoch(1000 * 3600));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // 创建一个 QValueAxis 对象，并设置其范围
    QValueAxis* axisY = new QValueAxis(this);
    axisX->setTitleText(tr("ShenDu"));
    axisY->setMin(0);
    axisY->setMax(10);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    setIsRunning(false);

	chart->setMargins(QMargins(5, 5, 5, 5));

    //axisX->setVisible(false); // Hide the X-axis
    //axisY->setVisible(false); 
    axisX->setGridLineVisible(true);
    axisY->setGridLineVisible(true);
	axisX->setLabelsVisible(true);
	axisY->setLabelsVisible(true);
    axisX->setTitleVisible(true);
    axisY->setTitleVisible(true);
    QPen axisPen(qRgb(255, 255, 255));
    axisX->setLinePen(axisPen);
    axisY->setLinePen(axisPen);

    QBrush axisBrush(Qt::white);
    axisX->setLabelsBrush(axisBrush);
    axisY->setLabelsBrush(axisBrush);

    chart->setTitleBrush(QBrush(Qt::white));
	axisX->setTitleBrush(QBrush(Qt::white));
}

DisplayZ::~DisplayZ()
{
    delete series;
    delete chart;
}

void DisplayZ::draw()
{
    if (m_isRunning) {
        m_drawCount++;
        if (m_drawCount < CHART_REFRESH_INTERVAL)
			return;
        m_drawCount = 0;
        series->clear();

        //series = chart->series()[0];
        if (m_data.count() > CHART_MAX_POINT) {
            int step = m_data.count() / CHART_MAX_POINT;

            auto firstTime = std::get<0>(m_data.first());
            //double firstY = std::get<1>(m_data.first());

            double miny = 0;
            double maxy = std::get<1>(m_data.last());
            //if (miny < maxy) {
            //    miny = floor(miny);
            //    maxy = ceil(maxy);
            //}
            //else {
            //    double tmp = miny;
            //    miny = maxy;
            //    maxy = tmp;
            //    miny = floor(miny);
            //    maxy = ceil(maxy);
            //}

            int n = 0;
            
            for (const auto& item : m_data) {
                if (n++ % step != 0)
					continue;
                qint64 timestamp = std::get<0>(item) - firstTime;
                double value = std::get<1>(item);
                series->append(timestamp, value);
                if (value > maxy)
                    maxy = value;
                if (value < miny)
                    miny = value;
            }
            double stepy = ((maxy - miny) / 5);
            if (stepy < 0.2)
                stepy = 0.2;
            miny = floor(miny / stepy) * stepy;
            maxy = ceil(maxy / stepy) * stepy;

            qint64 maxx = std::get<0>(m_data.last()) - std::get<0>(m_firstData);
            double stepx = 1000 * 20;
            maxx = ceil(maxx / stepx) * stepx;

            QDateTimeAxis* axisX = (QDateTimeAxis*)chart->axes(Qt::Horizontal).at(0);
            //axisX->setMin(QDateTime::fromMSecsSinceEpoch(std::get<0>(m_data.first())));
            //axisX->setMax(QDateTime::fromMSecsSinceEpoch(std::get<0>(m_endData)));
            axisX->setMin(QDateTime::fromMSecsSinceEpoch(0));
            axisX->setMax(QDateTime::fromMSecsSinceEpoch(maxx));

            QValueAxis* axisY = (QValueAxis*)chart->axes(Qt::Vertical).at(0);
            axisY->setMin(miny);
            axisY->setMax(maxy);
        }
    }
    else
    {
    }
}

void DisplayZ::setIsRunning(bool isRunning)
{
    m_isRunning = isRunning; 
    if (!m_isRunning) {
        m_data.clear();
        m_endData = std::make_tuple(0, 0);
        m_firstData = std::make_tuple(0, 0);
    }
}

void DisplayZ::addData(QDateTime dateTime, double value)
{
    if (dateTime.toMSecsSinceEpoch() == 0) {
        return;
    }
    if (std::isnan(value) || std::isinf(value))
        return;

    value = value / 1000.0;
    if (std::get<0>(m_firstData) == 0) {
		m_firstData = std::make_tuple(dateTime.toMSecsSinceEpoch(), value);
        return;
	}
    double firstV = std::get<1>(m_firstData);
    value = abs(value - firstV);
    qint64 t = dateTime.toMSecsSinceEpoch();
    if (m_data.count() > 0) {
		qint64 lastT = std::get<0>(m_data.last());
        double lastV = std::get<1>(m_data.last());
		if (t <= lastT)
			return;
        if (value <= lastV)
            value = lastV;
	}
    m_data.append(std::make_tuple(t, value));
}

void DisplayZ::setEndData(QDateTime dateTime, double value)
{
    value = -value / 1000.0;
    double lastV = std::get<1>(m_endData);
    if (lastV != value)
    {
        m_data.clear();
        m_endData = std::make_tuple(dateTime.toMSecsSinceEpoch(), value);
        return;
    }

    qint64 t = dateTime.toMSecsSinceEpoch();
    qint64 lastT = std::get<0>(m_endData);
    if (t <= lastT)
        return;
    if (t - lastT >= 1000 * 60 * 60 * 1)  // 1 hour
        return;
    m_endData = std::make_tuple(dateTime.toMSecsSinceEpoch(), value);
}
