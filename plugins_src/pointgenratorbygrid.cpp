#include "pointgenratorbygrid.h"
#include "ui_pointgeneratorbygrid.h"
#include "widgets/buttonedittablewidget.h"

PointGenratorByGrid::PointGenratorByGrid()
	:PointGenratorByGrid(NULL)
{
}

PointGenratorByGrid::PointGenratorByGrid(QWidget* parent)
	: QDialog(parent), ui(new Ui::PointGenratorByGrid)
{
	ui->setupUi(this);
	connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
}

PointGenratorByGrid::~PointGenratorByGrid()
{
}

QVector<double> PointGenratorByGrid::generatePoints()
{
	QVector<double> rs;
	if(this->exec() == QDialog::Accepted)
	{
		try
		{
			double x1 = ui->txtStartX->text().toDouble();
			double x2 = ui->txtEndX->text().toDouble();
			double y1 = ui->txtStartY->text().toDouble();
			double y2 = ui->txtEndY->text().toDouble();
			double dx = ui->txtStepX->text().toDouble();
			double dy = ui->txtStepY->text().toDouble();
			if (dx < 0.001 || dy < 0.001)
			{
				throw std::exception("步长太小");
			}
			for (double x = x1; x <= x2; x += dx)
			{
				for (double y = y1; y <= y2; y += dy)
				{
					rs.push_back(x);
					rs.push_back(y);
				}
			}
		}
		catch (...)
		{
			rs.clear();
		}
	}
	return rs;
}

QString PointGenratorByGrid::getName()
{
	return tr("GZ1");
}

QWidget* PointGenratorByGrid::getWidget()
{
	return this;
}

QVariant PointGenratorByGrid::doIt()
{
	QVector<double> rs = generatePoints();
	return QVariant::fromValue(rs);
}
