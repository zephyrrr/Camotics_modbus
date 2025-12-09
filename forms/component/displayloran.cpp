#include "displayloran.h"
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>

DisplayLoran::DisplayLoran(QWidget *parent)
    : QGraphicsView(parent)
{
	//ui.setupUi(this);

    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFixedSize(250, 250);
    this->setMaximumSize(250, 250);
    this->setFrameStyle(QFrame::NoFrame);
    this->setFrameShape(QFrame::Shape::NoFrame);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setViewportMargins(0, 0, 0, 0);
    this->setAlignment(Qt::AlignCenter);

    scene = new QGraphicsScene(this);
    //view = QGraphicsView(scene)
    this->setScene(scene);

    scene->setSceneRect(-125, -125, 250, 250);
    //this->centerOn(0, 0);

    int r2 = 3;
    currentPoint.setRect(0 - r2, 0 - r2, 2 * r2, 2 * r2);
    currentPoint.setBrush(Qt::red);
    scene->addItem(&currentPoint);
}

DisplayLoran::~DisplayLoran()
{
	scene->removeItem(&currentPoint);
    delete scene;
}

void DisplayLoran::draw()
{
    //scene->clear();
    //scene->setSceneRect(-125, -125, 250, 250);

    double r = 90;

    if (false)
    {
        auto circle = new QGraphicsEllipseItem(-r, -r, 2 * r, 2 * r);
        //circle->setPen(QPen(Qt::transparent, 0));
        scene->addItem(circle);

        {
            auto line1 = new QGraphicsLineItem(-r, 0, r, 0);
            auto line2 = new QGraphicsLineItem(0, -r, 0, r);
            scene->addItem(line1);
            scene->addItem(line2);
        }
        {
            auto line1 = new QGraphicsLineItem(-r, -r, r, r);
            auto line2 = new QGraphicsLineItem(-r, r, r, -r);
            scene->addItem(line1);
            scene->addItem(line2);
        }

        {
            auto line1 = new QGraphicsLineItem(-r, -r, r, -r);
            auto line3 = new QGraphicsLineItem(-r, -r, -r, r);
            auto line2 = new QGraphicsLineItem(r, r, r, -r);
            auto line4 = new QGraphicsLineItem(r, r, -r, r);
            scene->addItem(line1);
            scene->addItem(line2);
            scene->addItem(line3);
            scene->addItem(line4);
        }
    }

    double px = 0; 
    double py = 0;
    if (m_isRunning) {
        px = (m_currentPos.x() - m_center.x()) * r / m_step;
        py = -(m_currentPos.y() - m_center.y()) * r / m_step;
    }
    if (!std::isnan(px) && !std::isnan(py)) {
        int r2 = 3;
        currentPoint.setRect(px - r2, py - r2, 2 * r2, 2 * r2);
	}
}
