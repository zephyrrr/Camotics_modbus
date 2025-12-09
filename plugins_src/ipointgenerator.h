#pragma once

#include <QtPlugin>
#include "iplugin.h"

class IPointGenerator : public IDoPlugin
{
public:
	~IPointGenerator() = default;

	virtual QVector<double> generatePoints() = 0;
};

Q_DECLARE_INTERFACE(IPointGenerator, "com.edm.IPointGenerator")