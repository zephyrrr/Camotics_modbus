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
        explicit RegWindow(QWidget *parent, NCMachine* machine, ModbusAdapter* adapter, ModbusCommSettings* settings);
        ~RegWindow();
        
        void init();
        static QString parseMsg(QString val);
        static bool MessageBoxYesNo(QString title);
    private:
        Ui::RegWindow *ui;
        void addProperyEditor(QtPropertyEditor::QtPropertyTreeEditor& propertyEditor, BasePropertyObject* propertyObject);
        void addProperyEditor(QHBoxLayout* horizontalLayout, BasePropertyObject* propertyObject);

        ModbusCommSettings* m_modbusCommSettings;
        ModbusAdapter *m_modbus;
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
            QStringLiteral("XL"),
            QStringLiteral("XL"),
            QStringLiteral("YL"),
            QStringLiteral("YL"),
            QStringLiteral("ZL"),
            QStringLiteral("ZL"),
            NULL,
            NULL,
            QStringLiteral("TOIL"),
            QStringLiteral("POIL"),
            QStringLiteral("FIRE"),
            QStringLiteral("IN0"),
            QStringLiteral("IN1"),
            QStringLiteral("IN2"),
            QStringLiteral("IN3"),
            NULL,
            QStringLiteral("XZ"),
            QStringLiteral("YZ"),
            QStringLiteral("ZZ"),
            QStringLiteral("UZ"),
            QStringLiteral("VZ"),
            NULL,
            NULL,
            QStringLiteral("DRV"),
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
            QStringLiteral("X轴正方向限位"),
            QStringLiteral("X轴负方向限位"),
            QStringLiteral("Y轴正方向限位"),
            QStringLiteral("Y轴负方向限位"),
            QStringLiteral("Z轴正方向限位"),
            QStringLiteral("Z轴负方向限位"),
            NULL,
            NULL,
            QStringLiteral("油温信号"),
            QStringLiteral("油位信号"),
            QStringLiteral("火焰信号"),
            QStringLiteral("外部信号0"),
            QStringLiteral("外部信号1"),
            QStringLiteral("外部信号2"),
            QStringLiteral("外部信号3"),
            NULL,
            QStringLiteral("X轴零点信号"),
            QStringLiteral("Y轴零点信号"),
            QStringLiteral("Z轴零点信号"),
            QStringLiteral("U轴零点信号"),
            QStringLiteral("V轴零点信号"),
            NULL,
            NULL,
            QStringLiteral("伺服报警信号"),
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
            QStringLiteral("B"),
            QStringLiteral("SET"),
            QStringLiteral("P"),
            QStringLiteral("F1"),

            QStringLiteral("X-"),
            QStringLiteral("X+"),
            QStringLiteral("SP+"),
            QStringLiteral("ST"),

            QStringLiteral("Y-"),
            QStringLiteral("Y+"),
            QStringLiteral("SP-"),
            QStringLiteral("PUMP1"),

            QStringLiteral("Z-"),
            QStringLiteral("Z+"),
            QStringLiteral("STEP"),
            QStringLiteral("PUMP2"),

            QStringLiteral("A-"),
            QStringLiteral("A+"),
            QStringLiteral("CENT"),
            QStringLiteral("START"),

            QStringLiteral("B-"),
            QStringLiteral("B+"),
            QStringLiteral("ENT"),
            QStringLiteral("STOP"),
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
