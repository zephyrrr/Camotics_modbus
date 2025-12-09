#pragma once

#include <QDialog>
#include "ipointgenerator.h"

namespace Ui {
	class PointGenratorByGrid;
}

class PointGenratorByGrid  : public QDialog, IPointGenerator
{
	Q_OBJECT
		Q_PLUGIN_METADATA(IID "com.edm.PointGenratorByGrid")
		Q_INTERFACES(IPointGenerator)

public:
	PointGenratorByGrid();
	PointGenratorByGrid(QWidget* parent);
	~PointGenratorByGrid();

public:
	QVector<double> generatePoints() override;
	QString getName() override;
	QWidget* getWidget() override;
	QVariant doIt() override;

private:
	Ui::PointGenratorByGrid* ui;

	
};
