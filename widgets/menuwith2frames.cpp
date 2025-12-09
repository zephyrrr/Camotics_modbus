#include "menuwith2frames.h"
#include "qnumericpad.h"

MenuWith2Frames::MenuWith2Frames(QFrame* frame4SubMenu, QWidget*parent)
	: m_frame4SubMenu(frame4SubMenu), QFrame(parent)
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setMargin(0);
	layout->setSpacing(0);
	this->setLayout(layout);

	stackedLayout4SubMenu = new QStackedLayout(frame4SubMenu);
	m_buttonGroup4Menu = new QButtonGroup(this);
}

MenuWith2Frames::~MenuWith2Frames()
{}

void MenuWith2Frames::addItem(QString menuText)
{
	QPushButton* btn = new QPushButton(menuText, this);
	btn->setObjectName(QString("%1_menuButton_%2").arg(this->objectName()).arg(menuText));
	m_buttonGroup4Menu->addButton(btn);
	btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	btn->setCheckable(true);

	this->layout()->addWidget(btn);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setMargin(0);
	layout->setSpacing(0);
	QWidget* w = new QWidget(m_frame4SubMenu);
	w->setLayout(layout);
	stackedLayout4SubMenu->addWidget(w);
	m_subMenuLayouts.append(layout);

	connect(btn, &QAbstractButton::clicked, [this, btn, w]() {
		btn->setChecked(true);
		stackedLayout4SubMenu->setCurrentWidget(w);
		auto x = w->findChildren<QAbstractButton*>();
		if (x.count() > 0) {
			x[0]->click();
		}
		});

	QNumericPad::setButtonDownModeWhenInTouch(this, btn);
}

QBoxLayout* MenuWith2Frames::getSubMenuLayout(int idx)
{
	return m_subMenuLayouts[idx];
	//return static_cast<QBoxLayout*>(stackedLayout4SubMenu->itemAt(idx)->layout());
}
