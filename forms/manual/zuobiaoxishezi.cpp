#include "zuobiaoxishezi.h"
#include "ui_zuobiaoxishezi.h"
#include <QLabel>
#include <QButtonGroup>
#include <QTableWidget>
#include <QHeaderView>
#include <QtConcurrent>
#include <QKeyEvent>
#include "widgets/qlineedit4axisvalue.h"
#include "../modbus/NCMachine.h"

#define MAX_AXIS_LEN 4

ZuoBiaoXiSheZiForm::ZuoBiaoXiSheZiForm(QWidget *parent) :
    BaseChildWindowWithTools(parent),
    ui(new Ui::ZuoBiaoXiSheZiForm)
{
    ui->setupUi(this);
    auto layout = new QVBoxLayout(ui->widgetTable);

    tabBar = new QTabBar(this);
    for (int i = 0; i < 10; ++i) {
        tabBar->addTab(QString("%0154").arg(i));
    }

    layout->setContentsMargins(10, 0, 10, 10);
    layout->insertWidget(0, tabBar);
    connect(tabBar, &QTabBar::currentChanged, [this](int index) {
        this->LoadData();
        for (int j = 0; j < 6; ++j) {
            this->inButtons[j]->setText(QString("%1%2").arg(index).arg(this->inButtons[j]->text().right(2)));
        }
    });

    //auto layout = ui->gridLayout;
    //layout->setHorizontalSpacing(20);
    //layout->setVerticalSpacing(20);
    //layout->setContentsMargins(1, 1, 1, 1);
    
    QTableWidget* table = new QTableWidget(8, 5, this);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->verticalHeader()->hide();
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->hide();
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //layout->addWidget(table, 0, 0);
    layout->addWidget(table);

    // Add header labels 
    QString headers[] = { QStringLiteral("坐标系"), "X", "Y", "Z", "U"};
    for (int i = 0; i < POS_AXIS_LEN + 1; ++i) {
        //layout->addWidget(new QLabel(headers[i]), 0, i);
        QLabel* label = new QLabel(headers[i], table);
        label->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
        label->setAlignment(Qt::AlignCenter);
        table->setCellWidget(0, i, label);
    }

    buttonGroup = new QButtonGroup(this);
    QString coorNames[] = { "054", "055", "056", "057", "058", "059", QStringLiteral("机械")};
    // Add buttons and line edits
    for (int row = 1; row < 8; ++row) {
        QPushButton* button = new QPushButton(coorNames[row-1], table);
        inButtons[row - 1] = button;
        button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

        if (row != 7) {
            button->setCheckable(true);
            buttonGroup->addButton(button);

            connect(button, &QPushButton::clicked, [row, button, this](bool checked) {
                if (!checked) {
					return;
				}
                button->setAutoExclusive(true);
                this->m_currentRow = row;
                for (int row2 = 1; row2 < 7; ++row2) {
                    for (int col = 1; col <= POS_AXIS_LEN; ++col) {
                        auto inEdit = qobject_cast<QLineEdit4AxisValue*>(inEdits[row2 - 1][col-1]);

                        //inEdit->setReadOnly(row2 != this->m_currentRow);
                        inEdit->setIsEditing(row2 == this->m_currentRow);

                        if (row2 == this->m_currentRow) {
                            inEdit->setSelection(0, inEdit->text().length());
                        }
                        else {
                            inEdit->setSelection(0, 0);
                        }
                    }
                }
                ShowKeyboard(this);
            });
        }
        else {
            button->setEnabled(false);
        }

        //layout->addWidget(button, row, 0);
        table->setCellWidget(row, 0, button);

        for (int col = 1; col < MAX_AXIS_LEN + 1; ++col) {
            QLineEdit4AxisValue* inEdit = new QLineEdit4AxisValue(table);
			inEdit->setAllowNull(false);
            inEdit->setAlignment(Qt::AlignCenter);
            inEdit->setStyle(inEdit->style());
            inEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            // U轴
            if (col > POS_AXIS_LEN) {
                inEdit->setReadOnly(true);
            }
            table->setCellWidget(row, col, inEdit);
            inEdits[row-1][col-1] = inEdit;

            if (row != 7) {
                connect(inEdit, &QLineEdit4AxisValue::focusChanged, [this](bool hasFocus) {
                    if (hasFocus) {
                        if (m_currentRow != -1) {
                            for (int col = 1; col < POS_AXIS_LEN + 1; ++col) {
                                auto inEdit = qobject_cast<QLineEdit4AxisValue*>(inEdits[m_currentRow - 1][col - 1]);
                                inEdit->setIsEditing(false);
                                inEdit->setSelection(0, 0);
                            }
                            buttonGroup->setExclusive(false);
                            inButtons[m_currentRow - 1]->setAutoExclusive(false);
                            inButtons[m_currentRow - 1]->setChecked(false);
                            buttonGroup->setExclusive(true);
                            m_currentRow = -1;
                        }
                    }
                    else {

                    }
                    });
            }
        }
    }
}

ZuoBiaoXiSheZiForm::~ZuoBiaoXiSheZiForm()
{
	m_needReload = false;
    delete ui;
}

void ZuoBiaoXiSheZiForm::keyPressEvent(QKeyEvent* event)
{
    BaseType::keyPressEvent(event);

    if (m_currentRow != -1) {
        for (int col = 1; col < POS_AXIS_LEN + 1; ++col) {
            QLineEdit4AxisValue* inEdit = qobject_cast<QLineEdit4AxisValue*>(inEdits[m_currentRow - 1][col - 1]);
            //inEdit->setFocus();
            inEdit->keyPressEvent(event);
            if (event->key() == Qt::Key_Enter) {
                if (inEdit->isEditing()) {
                    inEdit->setIsEditing(false);
                }
            }
            //inEdit->clearFocus();

            //QCoreApplication::sendEvent(inEdit, &QFocusEvent(QEvent::FocusIn));

            //Qt::Key key = static_cast<Qt::Key>(event->key());
            //QKeyEvent pressEvent = QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier, QKeySequence(key).toString());
            //QKeyEvent releaseEvent = QKeyEvent(QEvent::KeyRelease, key, Qt::NoModifier);
            //QCoreApplication::sendEvent(inEdit, &pressEvent);
            //QCoreApplication::sendEvent(inEdit, &releaseEvent);
            //QFocusEvent focusEvent = QFocusEvent(QEvent::FocusIn, Qt::OtherFocusReason);
            //QCoreApplication::sendEvent(inEdit, &focusEvent);

            //QCoreApplication::sendEvent(inEdit, &QFocusEvent(QEvent::FocusOut));

            //QTest::keyClick(inEdit, event->key(), event->modifiers());
            // 
   //         QString oldText = inEdit->text();
   //         QString newText;
   //         if (inEdit->selectedText() == inEdit->text()) {
   //             newText = event->text();
			//}
   //         else {
   //             newText = inEdit->text() + event->text();
   //         }
   //         int i = 0;
   //         if (inEdit->validator()->validate(newText, i)) {
   //             inEdit->setText(newText);
   //         }
		}
    }
}

void ZuoBiaoXiSheZiForm::showEvent(QShowEvent* event)
{
    m_needReload = true;
    ReloadData();

    connect(m_ncMachine, &NCMachine::positionChanged, this, &ZuoBiaoXiSheZiForm::ReloadDataAfterTime);

    BaseType::showEvent(event);
}

void ZuoBiaoXiSheZiForm::hideEvent(QHideEvent* event)
{
    m_needReload = false;
    disconnect(m_ncMachine, &NCMachine::positionChanged, this, &ZuoBiaoXiSheZiForm::ReloadDataAfterTime);

    BaseType::hideEvent(event);
}


void ZuoBiaoXiSheZiForm::ReloadData() 
{ 
    if (!m_needReload)
        return;
    LoadData();
}

void ZuoBiaoXiSheZiForm::LoadData(QString filePath)
{
    if (filePath != NULL) {
        throw std::invalid_argument("filePath must be NULL");
    }

    int cs0 = tabBar->currentIndex();

    auto controller = m_ncMachine->GetController();
    int csIndex = controller->get(CURRENT_COORD_SYSTEM) - 1;
	bool isCsCurrent = csIndex >= cs0 * 6 && csIndex < cs0 * 6 + 6;
    csIndex = csIndex % 6;

    auto pos = controller->getAbsolutePosition();

    for (int j = 1; j < 8; ++j)
    {
        if (j != 7)
        {
            for (int i = 0; i < MAX_AXIS_LEN; ++i)
            {
                double v = pos.get(GCodeTool::axis[i]) - controller->getAxisCSOffset(GCodeTool::axis[i], j + cs0 * 6) - controller->getAxisToolOffset(GCodeTool::axis[i]) - controller->getAxisGlobalOffset(GCodeTool::axis[i]);
                inEdits[j - 1][i]->setText(QString::number(v, 'f', 3));

                //if (isCsCurrent && j - 1 == csIndex)
                //{
                //    inEdits[j - 1][i]->setStyleSheet("background-color: yellow");
                //}
                //else
                //{
                //    inEdits[j - 1][i]->setStyleSheet("");
                //}
            }
            if (isCsCurrent && j - 1 == csIndex)
            {
                inButtons[j - 1]->setStyleSheet("background-color: yellow");
            }
            else
            {
                inButtons[j - 1]->setStyleSheet("");
            }
        }
        else
        {
            for (int i = 0; i < MAX_AXIS_LEN; ++i)
            {
                double v = pos.get(GCodeTool::axis[i]);
                inEdits[j - 1][i]->setText(QString::number(v, 'f', 3));
            }
        }
    }
}

void ZuoBiaoXiSheZiForm::SaveData(QString filePath)
{
    m_ncMachine->serialize();
}

void ZuoBiaoXiSheZiForm::UpdateState()
{
    // 如果一直Update，会不能编辑
    //ReloadData();
}


QString ZuoBiaoXiSheZiForm::GetGCode(bool forRun)
{
    auto controller = m_ncMachine->GetController();
    //auto pos = controller->getAbsolutePosition();

    QString gcode;
    int cs0 = tabBar->currentIndex();
    for (int row = 1; row < 7; ++row) {
        int cs = cs0 * 6 + row;
        double ox = inEdits[row - 1][0]->text().toDouble();
        double oy = inEdits[row - 1][1]->text().toDouble();
        double oz = inEdits[row - 1][2]->text().toDouble();
        double ou = inEdits[row - 1][3]->text().toDouble();
        QString s;
        if (POS_AXIS_LEN == 4) {
            s = QString("G10 L20 P%1 X%2Y%3Z%4U%5\n").arg(cs).arg(ox, 0, 'f', 3).arg(oy, 0, 'f', 3).arg(oz, 0, 'f', 3).arg(ou, 0, 'f', 3);
        }
        else {
            s = QString("G10 L20 P%1 X%2Y%3Z%4\n").arg(cs).arg(ox, 0, 'f', 3).arg(oy, 0, 'f', 3).arg(oz, 0, 'f', 3);
        }
        gcode += s;
    }
    if (m_currentRow >= 1) {
        QString s2 = QString("G%1%2\n").arg(cs0).arg(GCodeTool::csNames[m_currentRow - 1]);
        gcode += s2;
    }
    return gcode;
}

void ZuoBiaoXiSheZiForm::RunGCode()
{
	QString gcode = GetGCode();
    m_ncMachine->RunGCode(gcode, [this] {
        QMetaObject::invokeMethod(this, "ReloadData", Qt::QueuedConnection);
        });

    ClearInputWidgets(this);
}

void ZuoBiaoXiSheZiForm::ClearInputWidgets(QWidget* parent)
{
    if (m_currentRow != -1) {
        for (int col = 1; col < MAX_AXIS_LEN + 1; ++col) {
            auto inEdit = qobject_cast<QLineEdit4AxisValue*>(inEdits[m_currentRow - 1][col - 1]);
            inEdit->setIsEditing(false);
            inEdit->setSelection(0, 0);
        }

        buttonGroup->setExclusive(false);
        inButtons[m_currentRow - 1]->setAutoExclusive(false);
        inButtons[m_currentRow - 1]->setChecked(false);
        buttonGroup->setExclusive(true);
        m_currentRow = -1;
    }
    else {
        for (int row = 0; row < 7; ++row) {
            for (int col = 1; col < MAX_AXIS_LEN + 1; ++col) {
				auto inEdit = inEdits[row][col - 1];
                //inEdit->setStyleSheet(inEdit->styleSheet());
                inEdit->setSelection(0, 0);
			}
		}
    }
}

void ZuoBiaoXiSheZiForm::ReloadDataAfterTime()
{
    if (!m_needReload)
        return;
    QPointer<ZuoBiaoXiSheZiForm> self(this);
  
    QtConcurrent::run([self]() {
        QThread::msleep(TASK_WAIT_MILLSECONDS_LARGE * 2);
        if (!self || !self->m_needReload)
            return;
        try {
            QMetaObject::invokeMethod(self, "ReloadData", Qt::QueuedConnection);
        }
        catch (...) {
        }
        });
}

