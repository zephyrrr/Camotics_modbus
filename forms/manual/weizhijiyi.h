#ifndef WEIZIJIYIFORM_H
#define WEIZIJIYIFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTabBar>
#include "BaseChildWindow.h"
#include "../component/tool4zeroandhalfaxis.h"


namespace Ui {
class WeiZhiJiYiForm;
}

class WeiZhiJiYiForm : public BaseChildWindowWithTools
{
    Q_OBJECT

public:
    explicit WeiZhiJiYiForm(QWidget *parent = nullptr);
    ~WeiZhiJiYiForm();

private:
    Ui::WeiZhiJiYiForm*ui;

    QPushButton* inButtons[7];
    QPushButton* inButtons2[7];
    QLineEdit* inEdits[7][5];
    QTabBar* tabBar;

    int m_currentRow = -1;
    int m_currentColumn = -1;

public:
    void LoadData(QString filePath = NULL) override;
    void SaveData(QString filePath = NULL) override;
    void RunGCode() override;

private slots:
    void ReloadData();
};

#endif // WEIZIJIYIFORM_H
