#pragma once

#include <QFrame>
#include <QPushButton>
#include <QBoxLayout>
#include <QStackedLayout>
#include <QButtonGroup>
class MenuWith2Frames  : public QFrame
{
	Q_OBJECT

public:
	MenuWith2Frames(QFrame* frame4SubMenu, QWidget* parent = nullptr);
	~MenuWith2Frames();

	void addItem(QString menuText);
	QBoxLayout* getSubMenuLayout(int idx);
	void openItem(int idx);
private:
	QFrame* m_frame4SubMenu;

	QStackedLayout* stackedLayout4SubMenu;
	QList<QBoxLayout*> m_subMenuLayouts;
	QButtonGroup* m_buttonGroup4Menu;
};
