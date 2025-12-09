#pragma once

#include <QGraphicsView>
#include <QGraphicsEllipseItem>

class DisplayLoran : public QGraphicsView
{
	Q_OBJECT

public:
	DisplayLoran(QWidget *parent = nullptr);
	~DisplayLoran();

	void draw();

	void setIsRunning(bool isRunning) { m_isRunning = isRunning; }
	void setP(int step, int center_x, int center_y) { if (step != 0) m_step = step; m_center = QPoint(center_x, center_y); }
	void setCurrentPos(int x, int y) { m_currentPos = QPoint(x, y); }
private:
	bool m_isRunning = false;
	int m_step = 1;
	QPoint	m_center, m_currentPos;
	QGraphicsScene* scene;
	QGraphicsEllipseItem currentPoint;
signals:
	void clicked();

protected:
	void mousePressEvent(QMouseEvent* event) override {
		emit clicked();
		QWidget::mousePressEvent(event);
	}
};
