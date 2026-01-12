#ifndef ZUOBIAOXISHEZIFORM_H
#define ZUOBIAOXISHEZIFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTabBar>
#include "BaseChildWindow.h"
#include "../component/tool4zeroandhalfaxis.h"


namespace Ui {
class ZuoBiaoXiSheZiForm;
}

class ZuoBiaoXiSheZiForm : public BaseChildWindowWithTools
{
    Q_OBJECT

public:
    explicit ZuoBiaoXiSheZiForm(QWidget *parent = nullptr);
    ~ZuoBiaoXiSheZiForm();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;

private:
    using BaseType = BaseChildWindowWithTools;

    Ui::ZuoBiaoXiSheZiForm *ui;

    QButtonGroup* buttonGroup;
    QPushButton* inButtons[7];
    QLineEdit* inEdits[7][4];
    QTabBar* tabBar;
    int m_currentRow = -1;
    bool m_needReload = true;

public:
    void LoadData(QString filePath = NULL) override;
    void SaveData(QString filePath = NULL) override;
    void UpdateState() override;

    QString GetGCode(bool forRun = true) override;
    void RunGCode() override;
    void ClearInputWidgets(QWidget* parent) override;

private slots:
    void ReloadData();

    void ReloadDataAfterTime();
};

#endif // ZUOBIAOXISHEZIFORM_H
