#ifndef NCRUNFORM_H
#define NCRUNFORM_H

#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#else
#endif

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

#include "camotics/qt/NCEdit.h"
#include "BaseChildWindow.h"

namespace Ui {
class NcRunForm;
}

class NcRunForm : public BaseChildWindow
{
    Q_OBJECT

public:
    explicit NcRunForm(QWidget *parent = nullptr);
    ~NcRunForm();

private:
    Ui::NcRunForm*ui;
    CAMotics::NCEdit* editor;
    
public:
    void LoadData(QString filePath = NULL) override;
    void SaveData(QString filePath = NULL) override;

    void RunGCode() override;

private:
    void setHValues();
    QString srcFilePath;
};

#endif
