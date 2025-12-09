#pragma once
#include <QString>
#include <QWidget>	
#include <QVariant>

class INamePlugin
{
public:
	~INamePlugin() = default;

	virtual QString getName() = 0;
};

class IWidgetPlugin : public INamePlugin
{
public:
	~IWidgetPlugin() = default;

	virtual QWidget* getWidget() = 0;
};

class IDoPlugin : public IWidgetPlugin
{
public:
	~IDoPlugin() = default;

	virtual QVariant doIt() = 0;
};

Q_DECLARE_INTERFACE(IDoPlugin, "com.edm.IDoPlugin")