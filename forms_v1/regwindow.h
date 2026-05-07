#ifndef REGWINDOW_H
#define REGWINDOW_H

#pragma warning(disable : 4100)
#pragma warning(disable : 4003)

#include <QDialog>
#include <QRadioButton>
#include <QHBoxLayout>
#include <cbang/json/JSON.h>
#include "../modbus/modbusadapter.h"
#include "../modbus/modbuscommsettings.h"
#include "../modbus/NCMachine.h"

#include "../modbus/QtPropertyEditor.h"
#include "../modbus/NCMachineProperties.h"

namespace Ui {
    class RegWindow;
}

class RegWindow : public QDialog
{
    Q_OBJECT;
    public:
        explicit RegWindow(QWidget *parent, NCMachine* machine, ModbusAdapter* adapter);
        ~RegWindow();
        
        void init();
        static QString parseMsg(QString val);
        static bool MessageBoxYesNo(QString title);
    private:
        Ui::RegWindow *ui;
        void addProperyEditor(QtPropertyEditor::QtPropertyTreeEditor& propertyEditor, BasePropertyObject* propertyObject);
        void addProperyEditor(QHBoxLayout* horizontalLayout, BasePropertyObject* propertyObject);

        ModbusAdapter * m_modbusAdapter;
        NCMachine* m_ncMachine;

        ZLJBCPropertyObject* propertyObjectZLJBC[4];

        QtPropertyEditor::QtPropertyTreeEditor m_propertyEditorTab2;
        QtPropertyEditor::QtPropertyTreeEditor m_propertyEditorTab3;
        QtPropertyEditor::QtPropertyTreeEditor m_propertyEditorTab4;
        QtPropertyEditor::QtPropertyTreeEditor m_propertyEditorTab5;
        QtPropertyEditor::QtPropertyTreeEditor m_propertyEditorTab6;
        QtPropertyEditor::QtPropertyTreeEditor m_propertyEditorTab7;
        QtPropertyEditor::QtPropertyTreeEditor m_propertyEditorTab8;
        QtPropertyEditor::QtPropertyTreeEditor m_propertyEditorTab11;
        QtPropertyEditor::QtPropertyTreeEditor m_propertyEditorTab12;
        QtPropertyEditor::QtPropertyTreeEditor m_propertyEditorTab13;
        QtPropertyEditor::QtPropertyTreeEditor m_propertyEditorTab14[4];
        QtPropertyEditor::QtPropertyTreeEditor m_propertyEditorTab16;

        std::vector<QtPropertyEditor::QtPropertyTreeEditor*> m_propertyEditors;

        static QString parseTxMsg(QString msg);
        static QString parseTxPDU(QStringList pdu, QString slave);
        static QString parseRxMsg(QString msg);
        static QString parseRxPDU(QStringList pdu, QString slave);
        static QString parseSysMsg(QString msg);

        uint16_t m_readDataBuffer[64];
        int m_readDataBufferLen;

        QRadioButton* m_machineInputRadioButtons[32];

        const QString m_machineInputFlags[32]  {
            QString("XL"),
            QString("XL"),
            QString("YL"),
            QString("YL"),
            QString("ZL"),
            QString("ZL"),
            NULL,
            NULL,
            QString("TOIL"),
            QString("POIL"),
            QString("FIRE"),
            QString("IN0"),
            QString("IN1"),
            QString("IN2"),
            QString("IN3"),
            NULL,
            QString("XZ"),
            QString("YZ"),
            QString("ZZ"),
            QString("UZ"),
            QString("VZ"),
            NULL,
            NULL,
            QString("DRV"),
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL
        };
        const QString m_machineInputNames[32] {
            tr("XZZFXXW"),
            tr("XZFFXXW"),
            tr("YZZFXXW"),
            tr("YZFFXXW"),
            tr("ZZZFXXW"),
            tr("ZZFFXXW"),
            NULL,
            NULL,
            tr("YWXH"),
            tr("YWXH"),
            tr("HYXH"),
            tr("WBXH0"),
            tr("WBXH1"),
            tr("WBXH2"),
            tr("WBXH3"),
            NULL,
            tr("XZLDXH"),
            tr("YZLDXH"),
            tr("ZZLDXH"),
            tr("UZLDXH"),
            tr("VZLDXH"),
            NULL,
            NULL,
            tr("CFBJXH"),
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL
        };

        QPushButton* m_machinePadPushButtons[24];
        QLineEdit* m_tab10LineEdits[6];
        QLineEdit* m_tab2LineEdits[POS_AXIS_LEN];

        const QString m_machinePads[24]{
            QString("B"),
            QString("SET"),
            QString("P"),
            QString("F1"),

            QString("X-"),
            QString("X+"),
            QString("SP+"),
            QString("ST"),

            QString("Y-"),
            QString("Y+"),
            QString("SP-"),
            QString("PUMP1"),

            QString("Z-"),
            QString("Z+"),
            QString("STEP"),
            QString("PUMP2"),

            QString("A-"),
            QString("A+"),
            QString("CENT"),
            QString("START"),

            QString("B-"),
            QString("B+"),
            QString("ENT"),
            QString("STOP"),
        };

    private slots:
        void on_btnStart_clicked();
        void on_btnStop_clicked();
        void on_btnReadOnce_clicked();
        void on_ckbVerbosity_stateChanged(int state);
        void on_btnWriteReg_clicked();

        void on_btnStartJog_clicked();
        void on_btnStopJog_clicked();
        void on_btnStartLoc_clicked();
        void on_btnStopLoc_clicked();
        void on_btnStartCmvTouch_clicked();
        void on_btnStopCmvTouch_clicked();
        void on_btnStartCmvToLMT_clicked();
        void on_btnStopCmvToLMT_clicked();
        void on_btnStartCmvFRMLMT_clicked();
        void on_btnStopCmvFRMLMT_clicked();
        void on_btnStartTOZSIG_clicked();
        void on_btnStopTOZSIG_clicked();
        void on_btnStartSPK_clicked();
        void on_btnStopSPK_clicked();

        void on_btnSendFangdianIO_clicked();
        void on_btnSendFuzhuIO_clicked();
        void on_btnSendKshkz_clicked();
        void on_btnSendKshkz2_clicked();
        void on_btnSendZLJBC_clicked();

        void UpdateState();
        void UpdateState2(unsigned long long key);
};

#endif // REGWINDOW_H
