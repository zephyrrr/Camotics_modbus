#include "weizhijiyi.h"
#include "ui_weizhijiyi.h"
#include <QLabel>
#include <QButtonGroup>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "widgets/qlineedit4axisvalue.h"
#include "../../modbus/NCMachine.h"
#include "widgets/filedialog.h"
#include "utils/nfile.h"

#define POS_SET_COUNT_PERPAGE 6

WeiZhiJiYiForm::WeiZhiJiYiForm(QWidget *parent) :
	BaseChildWindowWithTools(parent, TOOL_LOAD_SAVE | TOOL_SWITCH_COOR | TOOL_COOR_SET),
    ui(new Ui::WeiZhiJiYiForm), inButtons{ NULL }, inButtons2{ NULL }, m_currentRow(-1), m_currentColumn(-1)
{
    ui->setupUi(this);
    
    auto layout = ui->mainTableLayout;

    tabBar = new QTabBar(this);
    for (int i = 0; i < 10; ++i) {
        tabBar->addTab(QString("%01").arg(i + 1));
    }
    //layout->setContentsMargins(10, 0, 10, 10);
    layout->insertWidget(0, tabBar);
    connect(tabBar, &QTabBar::currentChanged, [this](int index) {
		int tabIndex = tabBar->currentIndex();
        this->LoadData();
        for (int j = 0; j < POS_SET_COUNT_PERPAGE; ++j) {
            this->inButtons[j]->setText(tr("JY") + QString("%1").arg(tabIndex == 0 ? "" : QString::number(tabIndex)).arg(j));
            this->inButtons2[j]->setText(tr("YD") + QString("%1").arg(tabIndex == 0 ? "" : QString::number(tabIndex)).arg(j));
        }
        });

    QTableWidget* table = new QTableWidget(POS_SET_COUNT_PERPAGE + 2, 7, this);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->verticalHeader()->hide();
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->hide();
    //table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(table);
    table->setMinimumHeight(400);

    // Add header labels 
    QString headers[] = { "", tr("ZBX"), "X", "Y", "Z", "U", ""};
    for (int i = 0; i < POS_SET_COUNT_PERPAGE + 1; ++i) {
        QLabel* label = new QLabel(headers[i], table);
        label->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
        label->setAlignment(Qt::AlignCenter);
        table->setCellWidget(0, i, label);
    }
    
    QButtonGroup* buttonGroup = new QButtonGroup(this);
    for (int row = 1; row <= POS_SET_COUNT_PERPAGE + 1; ++row) {

        if (row != POS_SET_COUNT_PERPAGE + 1) {
            QPushButton* button = new QPushButton(tr("JY") + QString("%1").arg(row), table);
            inButtons[row - 1] = button;
            button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
            button->setStyle(button->style());

            button->setCheckable(true);
            buttonGroup->addButton(button);

            connect(button, &QPushButton::clicked, [row, this]() {
                m_currentRow = row;
                m_currentColumn = 0;
                });
            table->setCellWidget(row, 0, button);

            QPushButton* button2 = new QPushButton(tr("YD") + QString("%1").arg(row), table);
            inButtons2[row - 1] = button2;
            button2->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
            button2->setStyle(button2->style());

            button2->setCheckable(true);
            buttonGroup->addButton(button2);

            connect(button2, &QPushButton::clicked, [row, this]() {
                m_currentRow = row;
                m_currentColumn = 1;
                });
            table->setCellWidget(row, 6, button2);
        }

        for (int col = 0; col < POS_AXIS_LEN + 1; ++col) {
            QLineEdit* inEdit = new QLineEdit4AxisValue(table);
            inEdit->setAlignment(Qt::AlignCenter);
            inEdit->setStyle(inEdit->style());
            inEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            inEdit->setReadOnly(true);
            //layout->addWidget(inEdit, row, col);
            table->setCellWidget(row, col + 1, inEdit);
            inEdits[row - 1][col] = inEdit;
        }
    }

	if (LOC_AXIS_LEN >= 4) {
	}
	else {
        ui->btnChangeU->group()->buttons()[0]->setEnabled(false);
        ui->btnChangeU->group()->buttons()[1]->setEnabled(false);
	}
    
    table->resizeColumnsToContents();
    table->setColumnWidth(0, 90);
    table->setColumnWidth(6, 90);
    table->setColumnWidth(1, 110);
    for (int i = 2; i < 6; ++i) {
        table->setColumnWidth(i, 130);
    }
}

WeiZhiJiYiForm::~WeiZhiJiYiForm()
{
    delete ui;
}

void WeiZhiJiYiForm::LoadData(QString filePath)
{
  //  if (filePath != NULL) {
		//throw std::invalid_argument("filePath must be NULL");
  //  }
    auto controller = m_ncMachine->GetController();

    if (filePath == NULL) {
		int tabIndex = tabBar->currentIndex();
        for (int row = 1; row <= POS_SET_COUNT_PERPAGE + 1; ++row)
        {
            if (row != POS_SET_COUNT_PERPAGE + 1)
            {
                for (int i = 0; i < POS_AXIS_LEN + 1; ++i)
                {
                    double v = controller->get(GCodeTool::GetPosMomoryName(tabIndex * POS_SET_COUNT_PERPAGE + row - 1, i));
                    QString s;
                    if (i == 0) {
                        int csIndex = (int)v;
                        s = QString("%1%2").arg(csIndex / 6).arg(GCodeTool::csNames[csIndex % 6]);
                    }
                    else {
                        s = QString::number(v, 'f', 3);
                    }
                    inEdits[row - 1][i]->setText(s);
                }
            }
            else
            {
                auto pos = controller->getAbsolutePosition();
                for (int i = 0; i < POS_AXIS_LEN + 1; ++i)
                {
                    if (i == 0) {
                        inEdits[row - 1][i]->setText(tr("JX"));
                    }
                    else {
                        double v = pos.get(GCodeTool::axis[i - 1]);
                        inEdits[row - 1][i]->setText(QString::number(v, 'f', 3));
                    }
                }
            }
        }
    }
    else {
        QFile file(filePath + ".pos");
        if (!file.open(QIODevice::ReadOnly)) {
            return;
        }
        QString jsonString = file.readAll();
        file.close();
        QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());

        QJsonArray posList = doc.array();
        // pos[0] is header
        for (int row = 1; row <= posList.count() - 1; ++row)
        {
            QJsonArray jsonArrayRow = posList[row].toArray();
            for (int i = 0; i < POS_AXIS_LEN + 1; ++i)
            {
				QString s = jsonArrayRow[i].toString();
                //inEdits[row - 1][i]->setText(s);
                if (i == 0) {
					int cs = s.toInt() / 100 * POS_SET_COUNT_PERPAGE + (s.toInt() % 100 - 54);
                    controller->set(GCodeTool::GetPosMomoryName(row - 1, i), cs);
                }
                else {
                    controller->set(GCodeTool::GetPosMomoryName(row - 1, i), s.toDouble());
                }
            }
        }
        LoadData();
    }
    BaseChildWindow::LoadData(filePath);
}

void WeiZhiJiYiForm::SaveData(QString filePath)
{
    if (filePath == NULL) {
        m_ncMachine->serialize();
    }
    else {
        auto controller = m_ncMachine->GetController();

        QJsonArray jsonArray;
        
        for (int row = 0; row <= POS_MEMORY_LEN; ++row)
        {
            QJsonArray jsonArrayRow;
            if (row == 0) {
                jsonArrayRow.append(QJsonValue(tr("ZBX")));
				jsonArrayRow.append(QJsonValue(QString("X")));
				jsonArrayRow.append(QJsonValue(QString("Y")));
				jsonArrayRow.append(QJsonValue(QString("Z")));
				jsonArrayRow.append(QJsonValue(QString("U")));
            }
            else {
                for (int i = 0; i < POS_AXIS_LEN + 1; ++i)
                {
                    double v = controller->get(GCodeTool::GetPosMomoryName(row - 1, i));
                    QString s;
                    if (i == 0) {
                        int csIndex = (int)v;
                        s = QString("%1%2").arg(csIndex / 6).arg(GCodeTool::csNames[csIndex % 6]);
                    }
                    else {
                        s = QString::number(v, 'f', 3);
                    }
                    jsonArrayRow.append(QJsonValue(s));
                }
            }
			jsonArray.append(jsonArrayRow);
        }

        QJsonDocument doc(jsonArray);
        QString jsonString = doc.toJson(QJsonDocument::Indented);
        NFile file(filePath + ".pos");
        if (file.open(QIODevice::WriteOnly)) {
            file.write(jsonString.toUtf8());
            file.close();
        }
    }

	BaseChildWindow::SaveData(filePath);
}

void WeiZhiJiYiForm::RunGCode()
{
    if (m_currentRow < 0)
        return;

    if (m_currentColumn == 1) {
        QString gcode = "G90\n";
        auto controller = m_ncMachine->GetController();

        for (int i = 0; i < POS_AXIS_LEN + 1; ++i)
        {
            double v;
            if (i == 0 && ui->btnChangeCs->isChecked())
            {
                gcode += QString("G%1\n").arg(inEdits[m_currentRow - 1][i]->text());
            }
            else {
                if (i == 1)
                {
                    gcode += "G00 ";
                }
                if (i == 1 || i == 2 || (i == 3 && ui->btnChangeZ->isChecked()) || (i == 4 && ui->btnChangeU->isChecked()))
                {
                    v = inEdits[m_currentRow - 1][i]->text().toDouble();
                    gcode += QString("%1%2").arg(GCodeTool::axis[i - 1]).arg(v, 0, 'f', 3);
                }
            }
        }
        m_ncMachine->RunGCode(gcode, [this]() {
            QMetaObject::invokeMethod(this, "ReloadData", Qt::QueuedConnection);
        });
    }
    else if (m_currentColumn == 0) {
		int tabIndex = tabBar->currentIndex();
        int row = m_currentRow;
        auto controller = this->m_ncMachine->GetController();
        for (int i = 0; i < POS_AXIS_LEN + 1; ++i)
        {
            double v;
            if (i == 0)
            {
                v = controller->get(CURRENT_COORD_SYSTEM) - 1;
                controller->set(GCodeTool::GetPosMomoryName(tabIndex * POS_SET_COUNT_PERPAGE + row - 1, i), v);
            }
            else {
                v = controller->getAxisPosition(GCodeTool::axis[i - 1]);
                controller->set(GCodeTool::GetPosMomoryName(tabIndex * POS_SET_COUNT_PERPAGE + row - 1, i), v);
            }

            QString s;
            if (i == 0) {
                int csIndex = (int)v;
                s = QString("%1%2").arg(csIndex / 6).arg(GCodeTool::csNames[csIndex % 6]);
            }
            else {
                s = QString::number(v, 'f', 3);
            }
            inEdits[row - 1][i]->setText(s);
        }

        m_ncMachine->serialize();
    }
}

void WeiZhiJiYiForm::ReloadData()
{
    LoadData();
}