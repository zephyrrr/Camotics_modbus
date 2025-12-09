#include "zhuzhongxingform.h"
#include "ui_zhuzhongxingform.h"
#include <QLabel>
#include <QButtonGroup>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollArea>
#include <QThread>
#include "../modbus/NCMachine.h"
#include "utils/gcodeutils.h"

ZhuZhongXingForm::ZhuZhongXingForm(QWidget *parent) :
    BaseChildWindowWithTools(parent, TOOL_ZERO_AXIS | TOOL_SWITCH_COOR | TOOL_COOR_ITER),
    ui(new Ui::ZhuZhongXingForm)
{
    ui->setupUi(this);

    ZhuZhongXingForm::SetupTable4Result(ui->tableView);

    ZhuZhongXingForm::setButton4LineEdit(ui->btnX);
    ZhuZhongXingForm::setButton4LineEdit(ui->btnY);
    ZhuZhongXingForm::setButton4LineEdit(ui->btnZ);

    //ZhuZhongXingForm::AddTable4Result(ui->tableView, "1.1,2.1,3");
    //ZhuZhongXingForm::AddTable4Result(ui->tableView, "2.1,1.1,3");
    //ZhuZhongXingForm::AddTable4Result(ui->tableView, "3.1,0.1,1");
    //ZhuZhongXingForm::AddTable4Result(ui->tableView, "4.1,0.1,1");

    QRegExp rx("[123]");
    QRegExpValidator* validator = new QRegExpValidator(rx, this);
    ui->inCnt->setValidator(validator);
}

ZhuZhongXingForm::~ZhuZhongXingForm()
{
    delete ui;
}

void ZhuZhongXingForm::on_btnX_clicked()
{
    ZhuZhongXingForm::toggleLineEdit(!ui->btnX->isChecked(), ui->inX);
}
void ZhuZhongXingForm::on_btnY_clicked()
{
    ZhuZhongXingForm::toggleLineEdit(!ui->btnY->isChecked(), ui->inY);
}
void ZhuZhongXingForm::on_btnZ_clicked()
{
    ZhuZhongXingForm::toggleLineEdit(!ui->btnZ->isChecked(), ui->inZ);
}
void ZhuZhongXingForm::toggleLineEdit(bool readonly, QLineEdit* inEdit)
{
    inEdit->setReadOnly(readonly);
    inEdit->setStyle(inEdit->style());
    inEdit->clear();
    if (!inEdit->isReadOnly())
    {
        //inEdit->setFocus(Qt::MouseFocusReason);
    }
    else
    {
        QLineEdit4Keyboard* inEdit2 = qobject_cast<QLineEdit4Keyboard*>(inEdit);
        if (inEdit2) {
            inEdit2->setIsEditing(false);
        }
    }
}

void ZhuZhongXingForm::toggleButtonEdit(bool checkable, QPushButton* btnP, QPushButton* btnN)
{
    btnP->setEnabled(checkable);
    btnN->setEnabled(checkable);

    btnP->setCheckable(checkable);
    btnN->setCheckable(checkable);

    if (false && checkable) {
        if (!btnP->isChecked() && !btnN->isChecked())
        {
            btnP->setChecked(true);
        }
    }
    else {
        btnN->setChecked(false);
        btnP->setChecked(false);

        btnP->setAutoExclusive(false);
        btnN->setAutoExclusive(false);

        connect(btnP, &QPushButton::clicked, [btnP, btnN]() {
            if (btnP->isChecked()) {
                btnN->setChecked(false);
            }
        });
        connect(btnN, &QPushButton::clicked, [btnP, btnN]() {
            if (btnN->isChecked()) {
                btnP->setChecked(false);
            }
            });
    }
}

void ZhuZhongXingForm::setButton4LineEdit(QPushButton* btnX)
{
    btnX->click();
    btnX->setEnabled(false);
    btnX->setCheckable(false);
}

void ZhuZhongXingForm::setButton4ButtonEdit(QPushButton* btnX)
{
	btnX->click();
	btnX->setEnabled(false);
	btnX->setCheckable(false);
}

void ZhuZhongXingForm::RunGCode()
{
	QString gcode = GetGCode();

    bool inSetToZero = ui->inSetToZero->isChecked();
    double inKuaiJingX = ui->inX->text().toDouble();
    double inKuaiJingY = ui->inY->text().toDouble();

    ZhuZhongXingForm::ClearTable4Result(ui->tableView);
    //if (this->ncMachine->GetRealtimeJsonMachine() != NULL) {
    //    this->ncMachine->GetRealtimeJsonMachine()->clearLastMessage();
    //}
	m_ncMachine->GetRealtimeJsonMachine()->setFunctionDoWhenCallUI([this](std::string s) {
		QMetaObject::invokeMethod(this, [this, s]() {
            ZhuZhongXingForm::AddTable4Result(ui->tableView, QString::fromStdString(s));
			}, Qt::QueuedConnection);
		});

    m_ncMachine->RunGCode(gcode, [this, inSetToZero, inKuaiJingX, inKuaiJingY] {
        QMetaObject::invokeMethod(this, [this, inSetToZero, inKuaiJingX, inKuaiJingY]() {
            try {
                QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
                QStandardItem* x = model->item(model->rowCount() - 1, 1);
                QStandardItem* y = model->item(model->rowCount() - 1, 2);
                if (x != NULL && y != NULL) {
                    QString x1 = inKuaiJingX != 0 ? QString("X%1").arg(x->text().toDouble(), 0, 'f', 3) : "";
                    QString y1 = inKuaiJingY != 0 ? QString("Y%1").arg(y->text().toDouble(), 0, 'f', 3) : "";
                    QString gcodeHere = QString("G90 G00 %1%2\n").arg(x1).arg(y1);
                    if (inSetToZero) {
                        QString x2 = inKuaiJingX != 0 ? QString("X0") : "";
                        QString y2 = inKuaiJingY != 0 ? QString("Y0") : "";
                        gcodeHere += QString("G92 %1%2\n").arg(x2).arg(y2);
                    }
                    m_ncMachine->RunGCode(gcodeHere);
                }
            }
            catch (...) {

            }
            }, Qt::QueuedConnection);
    });
}

QString ZhuZhongXingForm::GetGCode()
{
    double inKuaiJingX = ui->inX->text().toDouble();
    double inKuaiJingY = ui->inY->text().toDouble();
    double inKuaiJingZ = ui->inZ->text().toDouble();
    //double inCheDingZhiZhiJing = 0;
    double inJieChuHouFanZhuang = ui->inBack->text().toDouble();
    bool inSetToZero = ui->inSetToZero->isChecked();
    bool inTouchSenseZ = ui->inSetToZTouch->isChecked();
    int inCnt = ui->inCnt->text().toInt();
    if (inCnt <= 0 || inCnt > 3) {
        inCnt = 1;
    }

    QString gcode2 = GCodeUtils::Zhuzhongxing(inKuaiJingX, inKuaiJingY, inKuaiJingZ, inJieChuHouFanZhuang, inSetToZero, inTouchSenseZ);
    //ui->textBrowser->setText(gcode2);

    QString gcode = gcode2;
    gcode += "\n";

    for (int i = 0; i < inCnt; ++i) {
        //if (i == 0)
        //    gcode2 = "G00 X2Y4Z6\n(debug, do_call_ui 2,4,6)\n";
        //else if (i == 1)
        //    gcode2 = "G00 X3Y5Z7\n(debug, do_call_ui 3,5,7)\n";
        //else
        //    gcode2 = "G00 X4Y6Z8\n(debug, do_call_ui 4,6,8)\n";
        gcode += "o901 call\n";
    }
    gcode += "do_beep 100\n";
    return gcode;
}


void ZhuZhongXingForm::ClearTable4Result(QTableView* tableView)
{
	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(tableView->model());
	for (int i = 0; i < model->rowCount(); ++i) {
		for (int j = 0; j < model->columnCount(); ++j) {
			QStandardItem* item = model->item(i, j);
			if (item != NULL) {
				item->setText("");
			}
		}
	}
}
QStandardItemModel* ZhuZhongXingForm::SetupTable4Result(QTableView* tableView, QStringList headerNames, int rowCount)
{
    if (headerNames.count() == 0) {
        headerNames = QStringList() << "#1" << "#2" << "d1" << "d2";
    }
    headerNames.insert(0, "N");

    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStandardItemModel* model = new QStandardItemModel(rowCount + 1, headerNames.count(), tableView);

    for (int i = 0; i < headerNames.count(); ++i) {
        model->setHeaderData(i, Qt::Horizontal, headerNames[i]);
    }
    for (int i = 0; i < rowCount; ++i) {
        model->setHeaderData(i, Qt::Vertical, tr("D%C").arg(i+1));
    }
    model->setHeaderData(rowCount, Qt::Vertical, tr("JG"));

    tableView->setModel(model);
    tableView->setColumnHidden(0, true);

    AdjustTableViewHeight(tableView);

    return model;
}

void ZhuZhongXingForm::AdjustTableViewHeight(QTableView* tableView)
{
    int rows = tableView->model()->rowCount();
    int height = 0;
    for (int i = 0; i < rows; ++i) {
        height += tableView->rowHeight(i);
    }
    int headerHeight = tableView->horizontalHeader()->height();
    //tableView->setMinimumHeight(height + headerHeight);
    tableView->setFixedHeight(height + headerHeight);
}

void ZhuZhongXingForm::AddTable4Result(QTableView* tableView, QString s)
{
    if (s.isEmpty())
        return;
    QStringList ss = s.split(',');
    if (ss.count() != tableView->model()->columnCount() - 1)
        return;

    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(tableView->model());
    int setRowIdx = -1;
    for (int i = 0; i < model->rowCount() - 1; ++i)
    {
        QStandardItem* xItem = model->item(i, 0);
        if (xItem == NULL || xItem->text().isEmpty())
        {
            setRowIdx = i;
            break;
		}
    }
    if (setRowIdx == -1)
        return;
    int nextSetRowIdx = setRowIdx + 1;
    if (nextSetRowIdx >= model->rowCount() - 1)
    {
		nextSetRowIdx = 0;
	}
    {
        QStandardItem* xItem = model->item(nextSetRowIdx, 0);
        if (xItem != NULL && !xItem->text().isEmpty()) {
            model->item(nextSetRowIdx, 0)->setText("");
        }
    }

    //QList<QStandardItem*> items;
    for (int j = 0; j < model->columnCount(); ++j) {
        QStandardItem* xItem;
        if (j == 0) {
            xItem = new QStandardItem("set");
        }
        else {
            xItem = new QStandardItem(QString::number(ss[j - 1].toDouble(), 'f', 3));
        }
        
        xItem->setTextAlignment(Qt::AlignCenter);
        model->setItem(setRowIdx, j , xItem);

        //items << xItem;
    }
	
    for (int j = 1; j < model->columnCount(); ++j)
    {
        double sum = 0;
        int cnt = 0;
        for (int i = 0; i < model->rowCount() - 1; ++i) {
            if (model->item(i, j) != NULL && !model->item(i, j)->text().isEmpty()) {
                sum += model->item(i, j)->text().toDouble();
                ++cnt;
            }
        }
        if (cnt > 0) {
			QStandardItem* xItem = new QStandardItem(QString::number(sum / cnt, 'f', 3));
			xItem->setTextAlignment(Qt::AlignCenter);
			model->setItem(model->rowCount() - 1, j, xItem);
		}
	}

    //model->appendRow(items);
    //model->setItem(0, 0, xItem);
    //model->setItem(0, 1, yItem);
    //model->setItem(0, 2, zItem);

    //AdjustTableViewHeight(tableView);
}


