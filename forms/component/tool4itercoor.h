#pragma once

#include <thread>
#include <atomic>
#include <QWidget>
#include "ui_tool4itercoor.h"
#include "tool4base.h"
#include "BaseChildWindow.h"

class NCMachine;

class Tool4IterCoor : public QToolWidget
{
	Q_OBJECT

public:
	Tool4IterCoor(BaseChildWindow* childWindow, QWidget *parent = nullptr);
	~Tool4IterCoor();

	void UpdateState() override;

private:
	Ui::Tool4IterCoorClass ui;

	BaseChildWindow* m_childWindow;
	std::thread m_thread;
	std::atomic<bool> thread_finished;
private slots:
	void on_btnRunIterCoor_clicked();
};
