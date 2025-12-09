#pragma once

#include <QWidget>
#include "ui_gaotiao4jiagong.h"
#include "../widgets/dynamicform.h"
#include "BaseChildWindow.h"

class NCMachine;

class Gaotiao4Jiagong : public BaseChildWindow
{
	Q_OBJECT

public:
	Gaotiao4Jiagong(NCMachine* ncMachine, QWidget* parent = nullptr);
	~Gaotiao4Jiagong();

	void LoadData(QString filePath = NULL) override;
	void RunGCode() override;

private:
	Ui::Gaotiao4JiagongClass ui;
	DynamicForm* m_dynamicForm;
};
