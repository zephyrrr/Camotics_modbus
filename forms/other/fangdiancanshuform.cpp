#include "fangdiancanshuform.h"
#include "ui_fangdiancanshuform.h"
#include "widgets/spreadsheet.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QIntValidator>
#include <QMenu>
#include <QFileDialog>
#include <QTextStream>
#include "widgets/spreadsheetdelegate.h"
#include "modbus/NCMachineProperties.h"
#include "modbus/NCCommand.h"
#include "modbus/NCMachineParametersC.h"
#include "forms_v1/ModbusFile.h"
#include "widgets/qlineedit4optionwidget.h"
#include "forms/edit/cp/widget_cp_ln.h"
#include "widgets/filedialog.h"
#include "utils/nfile.h"
#include "utils/formutils.h"

FangDianCanShuForm::FangDianCanShuForm(QWidget *parent) :
    BaseChildWindow(parent),
    ui(new Ui::FangDianCanShuForm)
{
    ui->setupUi(this);

    //m_pNames.push_back("C.No");
    for (const auto& s : NCMachineParametersC::instance().names) {
        m_pNames.push_back(QString::fromStdString(s));
    }

    table1 = new SpreadSheet(1000, m_pNames.count(), nullptr, this);
    ui->verticalLayoutTable->addWidget(table1, 2);
    table1->horizontalHeader()->setObjectName("header-small");
    table1->verticalHeader()->setObjectName("header-small");
    table1->setObjectName("fangdiancanshu_global");

    table1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int c = 0; c < table1->columnCount(); ++c) {
        table1->setHorizontalHeaderItem(c, new QTableWidgetItem(m_pNames[c]));
        //table2->setColumnWidth(c, 15);
    }
    for (int row = 0; row < table1->rowCount(); ++row) {
        table1->setVerticalHeaderItem(row, new QTableWidgetItem(QString("C%01").arg(row, 3, 10, QLatin1Char('0'))));

        //for (int col = 0; col < table1->columnCount(); ++col) {
        //    if (m_pNames[col] == "LN") {
        //        //table1->setCellWidget(row, col, new QLineEdit4OptionWidget(table1, new WidgetCPLN(table1)));
        //    }
        //}
    }

    auto namesRegex = NCMachineParametersC::instance().namesRegex;
    
    for (int c = 0; c < table1->columnCount(); ++c) {
        SpreadSheetDelegate* d = new SpreadSheetDelegate(table1);
        QRegExp regExp(QString::fromStdString(namesRegex[c]));
        QRegExpValidator* validator = new QRegExpValidator(regExp, this);
        d->setValidator(validator);
        d->setFunction4CreateEdit([c](int row, int col, QWidget* parent) {
            int len = NCMachineParametersC::namesLen[c];
            return new QLineEdit4Fdcs(len, parent);
            //return (ILineEdit*)NULL;
        });

        //if (m_pNames[c] == "LN") {
        //    //d->setEditorMetaObject(&QLineEdit4OptionWidget::staticMetaObject);
        //    d->setEditorDialog(new WidgetCPLN(table1));
        //}
        table1->setItemDelegateForColumn(c, d);
    }

    connect(table1, &QTableWidget::cellChanged, [this](int row, int col) {
		//int len = NCMachineParametersC::namesLen[col];
  //      QString newText = table1->item(row, col)->text().rightJustified(len, '0');
  //      QRegExp regExp(QString::fromStdString(NCMachineParametersC::instance().namesRegex[col]));
  //      QRegExpValidator* validator = new QRegExpValidator(regExp, this);
  //      int pos = -1;
  //      if (validator->validate(newText, pos) != QValidator::Invalid) {
  //          table1->item(row, col)->setText(newText);
  //      }

        if (this->m_pNames[col] == "HP" || this->m_pNames[col] == "PP") {
            int col1 = 12; // HP
            int col2 = 13; // PP
            if (table1->item(row, col1)->text().startsWith('3'))
            {
                if (table1->item(row, col2) != NULL) {
                    QString v = table1->item(row, col2)->text();
                    if (v.length() == 2) {
                        v[0] = '0';
                    }
                    table1->item(row, col2)->setText(v);
                }
            }
        }
    });
}


FangDianCanShuForm::~FangDianCanShuForm()
{
    delete ui;
    delete table1;
}

void FangDianCanShuForm::SaveData(QString filePath)
{
    if (filePath == NULL) {
        filePath = GetDataFilePath(table1, SystemSettings::UserFlag);
    }
    table1->serialize(filePath);
    NCMachineParametersC::instance().deserialize();
}

void FangDianCanShuForm::LoadData(QString filePath)
{
    if (filePath == NULL) {
        filePath = GetDataFilePath(table1, SystemSettings::UserFlag);
    }
    table1->deserialize(filePath);
}

void FangDianCanShuForm::on_btnExport_clicked()
{
    QString path = NFileDialog::findFirstRemovableDisk(); // "data"
    if (path.isEmpty()) {
        FormUtils::MessageBoxInfo(tr("ZBDUP"));
        return;
    }
    QString fileName = NFileDialog::getSaveFileName
    (this, tr("Save File"), path,
        (
            tr("txt Files") + QString("(*.txt);;") +
            tr("csv Files") + QString("(*.csv)")
        )
    );
    if (fileName.isEmpty())
        return;
    if (fileName.endsWith(".csv")) {
        table1->exportToCsv(table1, fileName);
    }
    else {
        NFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);

            QHeaderView* header = table1->horizontalHeader();
            QAbstractItemModel* model = header->model();

            stream << "\t\t";
            for (int col = 0; col < model->columnCount(); ++col) {
                QString headerName = model->headerData(col, Qt::Horizontal).toString();
                stream << headerName << "\t";

                QModelIndex index = model->index(0, col);
                QString s = model->data(index).toString();
				if (s.length() >= 4) {
					stream << "\t";
				}
            }
            stream << "\n";

            model = table1->model();
            for (int row = 0; row < model->rowCount(); ++row) {
                stream << QString("C%1 = ").arg(row, 3, 10, QLatin1Char('0')) << "\t";
                for (int col = 0; col < model->columnCount(); ++col) {
                    QModelIndex index = model->index(row, col);
                    QString s = model->data(index).toString();
                    if (s.isEmpty())
                        stream << "\t";
                    else
                        stream << s << "\t";
                }
                stream << "\n";
            }
            file.close();
        }
    }
}

void FangDianCanShuForm::on_btnImport_clicked()
{
    QString path = NFileDialog::findFirstRemovableDisk(); // "data"
    if (path.isEmpty()) {
        FormUtils::MessageBoxInfo(tr("ZBDUP"));
        return;
    }
    QString fileName = NFileDialog::getOpenFileName
    (this, tr("Open File"), path,
        (
            tr("txt Files") + QString("(*.txt);;") +
            tr("csv Files") + QString("(*.csv)")
        )
    );

    if (fileName.isEmpty())
        return;

    if (fileName.endsWith(".csv")) {
        table1->importFromCsv(table1, fileName);
    }
    else {
        auto model = table1->model();

        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            QHash<int, int> headerMap;
            QRegularExpression re("\\s+");
            while (!stream.atEnd()) {
                QString line = stream.readLine().trimmed();
                if (line.startsWith("ON")) {
                    QStringList headers = line.split(re, Qt::SkipEmptyParts);

                    QHeaderView* header = table1->horizontalHeader();
                    QAbstractItemModel* model2 = header->model();

                    for (int j = 0; j < headers.count(); ++j) {
                        for (int i = 0; i < model2->columnCount(); ++i) {
                            QString headerName = model2->headerData(i, Qt::Horizontal).toString();
                            if (headerName == headers[j]) {
                                headerMap[j] = i;
                                break;
                            }
                        }
                    }
                }
                else {
                    QStringList values = line.split(re, Qt::SkipEmptyParts);
                    int cIndex = values[0].mid(1).toInt();
                    values.removeAt(1);
                    values.removeAt(0);
                    //assert(headerMap.count() + 1 == values.count());
                    int row = cIndex;
                    for (int col = 0; col < values.count(); ++col) {
                        int col2 = col;

                        if (!headerMap.contains(col))
                            continue;
                        col2 = headerMap[col];

                        QModelIndex index = model->index(row, col2);
                        model->setData(index, values[col]);
                    }
                }
            }
            file.close();
        }
    }
}

void FangDianCanShuForm::RunGCode()
{
    SaveData();
}




