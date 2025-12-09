#ifndef LUOJUBUCHANGFORM_H
#define LUOJUBUCHANGFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "widgets/spreadsheet.h"

#include "BaseChildWindow.h"


namespace Ui {
class LuoJuBuChangForm;
}

class LuoJuBuChangForm : public BaseChildWindow
{
    Q_OBJECT

public:
    explicit LuoJuBuChangForm(QWidget *parent = nullptr);
    
    void showContextMenu(const QPoint& pos);
    ~LuoJuBuChangForm();

private:
    Ui::LuoJuBuChangForm*ui;
    SpreadSheet* table1;
    SpreadSheet* table2;
    QMenu* menu4Table2;
    QModelIndex index4Table2;

public:
    void RunGCode() override;
    void LoadData(QString filePath = NULL) override;
    void SaveData(QString filePath = NULL) override;

private slots:
    void onActionClearRowTriggered();
    void onActionClearAllTriggered();
    void onActionReadTriggered();

private slots:
    void on_btnExport_clicked();
    void on_btnImport_clicked();
};

#endif
