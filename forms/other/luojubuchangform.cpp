#include "luojubuchangform.h"
#include "ui_luojubuchangform.h"
#include "widgets/spreadsheet.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QIntValidator>
#include <QMenu>
#include "modbus/NCMachineProperties.h"
#include "modbus/NCCommand.h"
#include "forms_v1/ModbusFile.h"
#include "../modbus/NCMachine.h"
#include "widgets/filedialog.h"
#include "utils/formutils.h"

LuoJuBuChangForm::LuoJuBuChangForm(QWidget *parent) :
    BaseChildWindow(parent),
    ui(new Ui::LuoJuBuChangForm)
{
    ui->setupUi(this);

    table1 = new SpreadSheet(6, 6, new QIntValidator(-32768, 32767), this);
    ui->verticalLayoutTable->addWidget(table1, 1);
    table1->setVerticalHeaderLabels(QStringList() << tr("CKDBCH") << tr("FFXBCH") << tr("ZFXBCH") << tr("BCDJG") << tr("JXBC") << tr("JXBCSD"));
    table1->setHorizontalHeaderLabels(QStringList() << QString("X") << QString("Y") << QString("Z") << QString("U") << QString("B") << "");
    table1->horizontalHeader()->setObjectName("header-small");
    table1->verticalHeader()->setObjectName("header-small");
    table1->setObjectName("luojubuchang1");

    table2 = new SpreadSheet(50, 20, new QIntValidator(-32768, 32767), this);
    ui->verticalLayoutTable->addWidget(table2, 2);
    table2->horizontalHeader()->setObjectName("header-small");
    table2->verticalHeader()->setObjectName("header-small");
    table2->setObjectName("luojubuchang2");

    table2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int c = 0; c < table2->columnCount(); ++c) {
        table2->setHorizontalHeaderItem(c, new QTableWidgetItem(QString::number(c)));
        //table2->setColumnWidth(c, 15);
    }
    for (int c = 0; c < table2->rowCount(); ++c) {
        table2->setVerticalHeaderItem(c, new QTableWidgetItem(QString::number(c * table2->columnCount())));
    }

    table2->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(table2, &QTableView::customContextMenuRequested,
        this, &LuoJuBuChangForm::showContextMenu);

    menu4Table2 = new QMenu(this);
    QAction* action1 = menu4Table2->addAction(tr("QKDQX"));
    connect(action1, &QAction::triggered, this, &LuoJuBuChangForm::onActionClearRowTriggered);

    QAction* action2 = menu4Table2->addAction(tr("QKQB"));
    connect(action2, &QAction::triggered, this, &LuoJuBuChangForm::onActionClearAllTriggered);

    QAction* action3 = menu4Table2->addAction(tr("DQ"));
    connect(action3, &QAction::triggered, this, &LuoJuBuChangForm::onActionReadTriggered);
}
void LuoJuBuChangForm::onActionClearRowTriggered() {
    int i = index4Table2.row();
    if (i < 0 || i >= table2->rowCount())
        return;
    for (int j = 0; j < table2->columnCount(); ++j)
    {
        QModelIndex index = table2->model()->index(i, j);
        table2->model()->setData(index, QVariant());
    }
    table2->viewport()->update();
}

void LuoJuBuChangForm::onActionClearAllTriggered() {
    for (int i = 0; i < table2->rowCount(); ++i) {
        for (int j = 0; j < table2->columnCount(); ++j)
        {
            QModelIndex index = table2->model()->index(i, j);
            table2->model()->setData(index, QVariant());
        }
    }
    table2->viewport()->update();
}
void LuoJuBuChangForm::onActionReadTriggered() {
    int priority = 0;
    m_ncMachine->enterSetPriority(priority);
    m_ncMachine->GoApi();

    for (int i = 0; i < table2->rowCount(); ++i) {
        auto func = [i, this](int ret, ModbusTask* task, ModbusAdapter* adapter) {
            if (ret == -1)
                return;
            auto data = adapter->GetReadedData16();
            for (int j = 0; j < table2->columnCount(); ++j)
            {
                QModelIndex index = table2->model()->index(i, j);
                table2->model()->setData(index, (int16_t)data[j]);
            }
            table2->viewport()->update();
            table2->serialize(GetDataFilePath(table2, SystemSettings::UserFlag));
            };
        ModbusTask* task = m_ncMachine->getModbus()->getTaskReadFile(i * table2->columnCount(), table2->columnCount());
		task->setPostFunction(func, "Read LuoJuBuChang");
        m_ncMachine->getModbus()->addTask(task, priority);
    }
    m_ncMachine->GoJog();
    m_ncMachine->exitSetPriority();

}

void LuoJuBuChangForm::showContextMenu(const QPoint& pos) {
    QPoint globalPos = table2->mapToGlobal(pos);
    index4Table2 = table2->indexAt(pos);

    int x = globalPos.x() + menu4Table2->sizeHint().width() / 2;
    int y = globalPos.y() + menu4Table2->sizeHint().height() / 2;

    QPoint topLeftPoint(x, y);

    menu4Table2->exec(topLeftPoint);
}

LuoJuBuChangForm::~LuoJuBuChangForm()
{
    delete ui;
    delete table1;
    delete table2;
    delete menu4Table2;
}

void LuoJuBuChangForm::SaveData(QString filePath)
{
    if (filePath != NULL) {
        throw std::invalid_argument("filePath must be NULL");
    }
    table1->serialize(GetDataFilePath(table1, SystemSettings::UserFlag));
    table2->serialize(GetDataFilePath(table2, SystemSettings::UserFlag));
}

void LuoJuBuChangForm::LoadData(QString filePath)
{
    if (filePath != NULL) {
        throw std::invalid_argument("filePath must be NULL");
    }

    table1->deserialize(GetDataFilePath(table1, SystemSettings::UserFlag));
    table2->deserialize(GetDataFilePath(table2, SystemSettings::UserFlag));

    for (int i = 0; i < LJBC_AXIS_LEN; ++i) {
        if (table1->item(3, i) != NULL && table1->item(3, i)->text().isEmpty())
            table1->item(3, i)->setText(QString::number(PropertyObjects::getInstance()->propertyObjectZLJBC[i]->bcjg()));
        if (table1->item(0, i) != NULL && table1->item(0, i)->text().isEmpty())
            table1->item(0, i)->setText(QString::number(PropertyObjects::getInstance()->propertyObjectZLJBC[i]->bcckd()));
        if (table1->item(2, i) != NULL && table1->item(2, i)->text().isEmpty())
            table1->item(2, i)->setText(QString::number(PropertyObjects::getInstance()->propertyObjectZLJBC[i]->zfxbczdjl()));
        if (table1->item(1, i) != NULL && table1->item(1, i)->text().isEmpty())
            table1->item(1, i)->setText(QString::number(PropertyObjects::getInstance()->propertyObjectZLJBC[i]->ffxbczdjl()));
        if (table1->item(4, i) != NULL && table1->item(4, i)->text().isEmpty())
            table1->item(4, i)->setText(QString::number(PropertyObjects::getInstance()->propertyObjectZLJBC[i]->fxjxbc()));
        if (table1->item(5, i) != NULL && table1->item(5, i)->text().isEmpty())
            table1->item(5, i)->setText(QString::number(PropertyObjects::getInstance()->propertyObjectZLJBC[i]->dwsjzdbc()));
    }
    //onActionReadTriggered();
    //if (table2->item(0, 0) != NULL && table2->item(0, 0)->text().isEmpty()) {
    //    onActionReadTriggered();
    //}
}

void LuoJuBuChangForm::RunGCode()
{
    //if (table1->item(0, 0) == NULL)
    //    return;

    SaveData();

    //ModbusFile* dialog = new ModbusFile(this, ncMachine->getModbus());
    //dialog->exec();
    //delete dialog;

    m_ncMachine->enterSetPriority(0);
    m_ncMachine->GoApi();

    for (int i = 0; i < LJBC_AXIS_LEN; ++i) {
        if (table1->item(3, i) != NULL && !table1->item(3, i)->text().isEmpty())
            PropertyObjects::getInstance()->propertyObjectZLJBC[i]->setbcjg(table1->item(3, i)->text().toInt());
        if (table1->item(0, i) != NULL && !table1->item(0, i)->text().isEmpty())
            PropertyObjects::getInstance()->propertyObjectZLJBC[i]->setbcckd(table1->item(0, i)->text().toInt());
        if (table1->item(2, i) != NULL && !table1->item(2, i)->text().isEmpty())
            PropertyObjects::getInstance()->propertyObjectZLJBC[i]->setzfxbczdjl(table1->item(2, i)->text().toInt());
        if (table1->item(1, i) != NULL && !table1->item(1, i)->text().isEmpty())
            PropertyObjects::getInstance()->propertyObjectZLJBC[i]->setffxbczdjl(table1->item(1, i)->text().toInt());
        if (table1->item(4, i) != NULL && !table1->item(4, i)->text().isEmpty())
            PropertyObjects::getInstance()->propertyObjectZLJBC[i]->setfxjxbc(table1->item(4, i)->text().toInt());
        if (table1->item(5, i) != NULL && !table1->item(5, i)->text().isEmpty())
            PropertyObjects::getInstance()->propertyObjectZLJBC[i]->setdwsjzdbc(table1->item(5, i)->text().toInt());

        PropertyObjects::getInstance()->propertyObjectZLJBC[i]->setbcsx(false);
    }
    // 先螺距补偿使能=false
    ZLJBCPropertyObject::ExecuteCmds2(m_ncMachine, PropertyObjects::getInstance()->propertyObjectZLJBC);
    ZLJBCPropertyObject::ExecuteCmds1(m_ncMachine, PropertyObjects::getInstance()->propertyObjectZLJBC);

    int sub_addr = 0;
    int nb = table2->columnCount();
    //uint16_t* src = new uint16_t[nb];
    std::vector<uint16_t> vs2;
    for (int i = 0; i < table2->rowCount(); ++i) {
        vs2.clear();
        for (int j = 0; j < table2->columnCount(); ++j)
        {
            int x = 0;
            if (table2->item(i, j) != NULL && !table2->item(i, j)->text().isEmpty())
                x = table2->item(i, j)->text().toInt();
            vs2.push_back((uint16_t)x);
        }
        std::string cmd = NCCommand::GetWriteString(sub_addr + i * nb, nb, vs2, "write_file");

        m_ncMachine->executeCmds(cmd);

        //int ret = modbus_write_file_record(ncMachine->getModbus()->GetRawInterface(), addr, sub_addr + i * nb, 1, src);
        //if (ret < 0) {
        //    QMessageBox::critical(this, tr("CW"), tr("FSCW:") + EUtils::libmodbus_strerror(errno));
        //    break;
        //}
    }
    //delete[] src;

    m_ncMachine->GoJog();
    m_ncMachine->exitSetPriority();
}

void LuoJuBuChangForm::on_btnExport_clicked()
{
    QString path = NFileDialog::findFirstRemovableDisk(); // "data"
    if (path.isEmpty()) {
        FormUtils::MessageBoxInfo(tr("ZBDUP"));
        return;
    }
    QString fileName = NFileDialog::getSaveFileName
    (this, tr("Save File"), path,
        (
            tr("Json Files") + QString("(*.json);;")
        )
    );
    if (!fileName.isEmpty())
        table1->serialize(fileName);

    fileName = NFileDialog::getSaveFileName
    (this, tr("Save File"), path,
        (
            tr("Json Files") + QString("(*.json);;")
            )
    );
    if (!fileName.isEmpty())
        table2->serialize(fileName);
}
void LuoJuBuChangForm::on_btnImport_clicked()
{
    QString path = NFileDialog::findFirstRemovableDisk(); // "data"
    if (path.isEmpty()) {
        FormUtils::MessageBoxInfo(tr("ZBDUP"));
        return;
    }
    QString fileName = NFileDialog::getOpenFileName
    (this, tr("Open File"), path,
        (
            tr("Json Files") + QString("(*.json);;")
            )
    );
    table1->deserialize(fileName);

    fileName = NFileDialog::getOpenFileName
    (this, tr("Open File"), path,
        (
            tr("Json Files") + QString("(*.json);;")
            )
    );
    table2->deserialize(fileName);
}




