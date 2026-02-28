#include "qnumericpad.h"
#include "keyemitter.h"
#include "qlineedit4axisvalue.h"
#include <QLineEdit>
#include <QToolButton>
#include <QTimer>
#include <QMouseEvent>
#include <QFocusEvent>

KeyEmitter keyEmitter;

QNumericPad::QNumericPad(QWidget *parent)
	: QDialog(parent), m_receiver(NULL), m_text("")
{
	ui.setupUi(this);
    this->setWindowTitle(tr("JP"));
    //  | Qt::WindowStaysOnTopHint
    this->setWindowFlags(Qt::WindowDoesNotAcceptFocus | Qt::Tool | Qt::FramelessWindowHint | Qt::BypassWindowManagerHint);  //  | Qt::WindowStaysOnTopHint
    this->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    this->setAttribute(Qt::WA_ShowWithoutActivating);
    //this->setWindowFlags(Qt::ToolTip);

    auto buttons = this->findChildren<QPushButton*>();
    for each (QPushButton* btn in buttons)
    {
        btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        //btn->setAttribute(Qt::WA_ShowWithoutActivating);
        //btn->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    setButtonDownModeWhenInTouch(this);
}


QNumericPad::~QNumericPad()
{}
QWidget* QNumericPad::getReceiver()
{
    return m_receiver;
}
void QNumericPad::setReceiver(QWidget* receiver)
{
    if (m_receiver != receiver)
    {
        // do in focus out event
        //if (m_receiver) {
        //    try
        //    {
        //        QLineEdit4Keyboard* inEdit = qobject_cast<QLineEdit4Keyboard*>(m_receiver);
        //        if (inEdit) {
        //            inEdit->setIsEditing(false);
        //            inEdit->setStyle(inEdit->style());
        //        }
        //    }
        //    catch (...)
        //    {
        //    }
        //}
        if (receiver) {
            m_receiver = receiver;
            m_text.clear();

            if (this->isVisible()) {
                this->hide();
            }
            this->show();
        }
    }
}

void QNumericPad::setButtonDownModeWhenInTouch(QWidget* parent, QAbstractButton* button)
{
    const static int keepMs = 50;
    QList<QPushButton*> buttons = parent->findChildren<QPushButton*>();
    for (QPushButton* btn: buttons)
    {
		if (button && btn != button)
			continue;
        connect(btn, &QPushButton::clicked, [btn] {
            btn->setDown(true);

            // 然后，你可以使用 QTimer::singleShot 来在一段时间后释放按钮
            QTimer::singleShot(keepMs, btn, [btn] {
                btn->setDown(false);
                });
            });
    }

    QList<QToolButton*> buttons2 = parent->findChildren<QToolButton*>();
    for (QToolButton* btn : buttons2)
    {
        if (button && btn != button)
            continue;
        connect(btn, &QToolButton::clicked, [btn] {
            btn->setDown(true);

            // 然后，你可以使用 QTimer::singleShot 来在一段时间后释放按钮
            QTimer::singleShot(keepMs, btn, [btn] {
                btn->setDown(false);
                });
            });
    }
}

void QNumericPad::on_pushButtonKey0_clicked()
{
    m_text += "0";
    keyEmitter.emitKey(Qt::Key_0, m_receiver);
}

void QNumericPad::on_pushButtonKey1_clicked()
{
    m_text += "1";
    keyEmitter.emitKey(Qt::Key_1, m_receiver);
}

void QNumericPad::on_pushButtonKey2_clicked()
{
    m_text += "2";
    keyEmitter.emitKey(Qt::Key_2, m_receiver);
}

void QNumericPad::on_pushButtonKey3_clicked()
{
    m_text += "3";
    keyEmitter.emitKey(Qt::Key_3, m_receiver);
}

void QNumericPad::on_pushButtonKey4_clicked()
{
    m_text += "4";
    keyEmitter.emitKey(Qt::Key_4, m_receiver);
}

void QNumericPad::on_pushButtonKey5_clicked()
{
    m_text += "5";
    keyEmitter.emitKey(Qt::Key_5, m_receiver);
}

void QNumericPad::on_pushButtonKey6_clicked()
{
    m_text += "6";
    keyEmitter.emitKey(Qt::Key_6, m_receiver);
}

void QNumericPad::on_pushButtonKey7_clicked()
{
    m_text += "7";
    keyEmitter.emitKey(Qt::Key_7, m_receiver);
}

void QNumericPad::on_pushButtonKey8_clicked()
{
    m_text += "8";
    keyEmitter.emitKey(Qt::Key_8, m_receiver);
}

void QNumericPad::on_pushButtonKey9_clicked()
{
    m_text += "9";
    keyEmitter.emitKey(Qt::Key_9, m_receiver);
}

void QNumericPad::on_pushButtonKeyDot_clicked()
{
    m_text += ".";
    keyEmitter.emitKey(Qt::Key_Period, m_receiver);
}

void QNumericPad::on_pushButtonKeyAdd_clicked()
{
    if (m_text.startsWith("-"))
        m_text = m_text.mid(1);
    keyEmitter.emitKey(Qt::Key_Plus, m_receiver);
}
void QNumericPad::on_pushButtonKeySub_clicked()
{
    if (m_text.startsWith("-"))
        m_text = m_text.mid(1);
    else
        m_text = "-" + m_text;
    keyEmitter.emitKey(Qt::Key_Minus, m_receiver);
}
void QNumericPad::on_pushButtonKeyHalf_clicked()
{
    //double d = m_text.toDouble();
    //d = d / 2;
    //m_text = QString::number(d, 'f', 3);
    if (m_text.startsWith("-"))
        m_text = m_text.mid(1);
    else
        m_text = "-" + m_text;
    keyEmitter.emitKey(Qt::Key_F1, m_receiver);
}
void QNumericPad::on_pushButtonKeyBack_clicked()
{
    if (!m_text.isEmpty()) {
        m_text = m_text.left(m_text.length() - 1);
    }
    keyEmitter.emitKey(Qt::Key_Backspace, m_receiver);
}

void QNumericPad::on_pushButtonKeyEnter_clicked()
{
    keyEmitter.emitKey(Qt::Key_Enter, m_receiver);
    if (m_receiver != NULL) {
        m_text.clear();
    }
    this->hide();
}

void QNumericPad::on_pushButtonKeyDel_clicked()
{
    keyEmitter.emitKey(Qt::Key_Delete, m_receiver);
    m_text.clear();
}

void QNumericPad::on_pushButtonKeyCancel_clicked()
{
    keyEmitter.emitKey(Qt::Key_F2, m_receiver);
    m_text.clear();
    this->hide();
}
