#ifndef FANGDIANCANSHUFORM_H
#define FANGDIANCANSHUFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "widgets/spreadsheet.h"

#include "BaseChildWindow.h"


namespace Ui {
class FangDianCanShuForm;
}

class FangDianCanShuForm : public BaseChildWindow
{
    Q_OBJECT

public:
    explicit FangDianCanShuForm(QWidget *parent = nullptr);
    
    ~FangDianCanShuForm();

private:
    Ui::FangDianCanShuForm*ui;
    SpreadSheet* table1;

    QStringList m_pNames;

public:
    void LoadData(QString filePath = NULL) override;
    void SaveData(QString filePath = NULL) override;

    void RunGCode() override;

private slots:
    void on_btnExport_clicked();
    void on_btnImport_clicked();
};

#endif
