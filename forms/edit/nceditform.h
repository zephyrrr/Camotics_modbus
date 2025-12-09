#ifndef NCEDITFORM_H
#define NCEDITFORM_H

#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#else
#endif

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

#include "camotics/qt/NCEdit.h"
#include "camotics/qt/FindDialog.h"
#include "BaseChildWindow.h"

namespace Ui {
class NcEditForm;
}

class NcEditForm : public BaseChildWindow
{
    Q_OBJECT

public:
    explicit NcEditForm(QWidget *parent = nullptr);
    ~NcEditForm();

private:
    Ui::NcEditForm*ui;

    
    CAMotics::FindDialog findAndReplaceDialog;
    QString m_defaultDirName;
public:
    void LoadData(QString filePath = NULL) override;
    void SaveData(QString filePath = NULL) override;

    void RunGCode() override;

    void HideKeyboard() override;
    void ShowKeyboard(QWidget* receiver = NULL, bool autoHide = true, int keyboardType = 2) override;

    void OpenFile(std::string filename);
protected:
    void showEvent(QShowEvent* event) override;

private:
    void setHValues();
    bool saveProject(bool saveAs);

private slots:
    void on_btnNew_clicked();
    void on_btnOpen_clicked();
    void on_btnSave_clicked();
    void on_btnSaveTo_clicked();
    void on_btnImport_clicked();
    void on_btnExport_clicked();
    void on_btnClose_clicked();

    void on_btnUndo_clicked();
    void on_btnRedo_clicked();

    void on_btnCut_clicked();
    void on_btnCopy_clicked();
    void on_btnPaste_clicked();
    void on_btnDelete_clicked();

    void on_btnSelectAll_clicked();

    void on_btnFind_clicked();
    void on_btnFindNext_clicked();

    void on_btnKeyboard_clicked();
};

#endif
