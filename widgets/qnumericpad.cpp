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



void QNumericPad::onKeyPressed(Qt::Key key)
{
    if (key == Qt::Key_Plus) {
        if (m_text.startsWith("-"))
            m_text = m_text.mid(1);
    }
    else if (key == Qt::Key_Minus) {
        if (m_text.startsWith("-"))
            m_text = m_text.mid(1);
        else
            m_text = "-" + m_text;
    }
    else if (key == Qt::Key_F1) {
        //double d = m_text.toDouble();
        //d = d / 2;
        //m_text = QString::number(d, 'f', 3);
        if (m_text.startsWith("-"))
            m_text = m_text.mid(1);
        else
            m_text = "-" + m_text;
    }
    else if (key == Qt::Key_Backspace) {
        m_text = m_text.left(m_text.length() - 1);
    }
    else if (key == Qt::Key_Delete || key == Qt::Key_F2 /* cancel */)
    {
        m_text.clear();
    }
    else if (key == Qt::Key_Enter)
    {

    }
    else
    {
        m_text += QKeySequence(key).toString(QKeySequence::NativeText);
    }
    setWindowTitle(m_text);
    keyEmitter.emitKey(key, m_receiver);
}

void QNumericPad::on_pushButtonKey0_clicked()
{
    onKeyPressed(Qt::Key_0);
}

void QNumericPad::on_pushButtonKey1_clicked()
{
    onKeyPressed(Qt::Key_1);
}

void QNumericPad::on_pushButtonKey2_clicked()
{
    onKeyPressed(Qt::Key_2);
}

void QNumericPad::on_pushButtonKey3_clicked()
{
    onKeyPressed(Qt::Key_3);
}

void QNumericPad::on_pushButtonKey4_clicked()
{
    onKeyPressed(Qt::Key_4);
}

void QNumericPad::on_pushButtonKey5_clicked()
{
    onKeyPressed(Qt::Key_5);
}

void QNumericPad::on_pushButtonKey6_clicked()
{
    onKeyPressed(Qt::Key_6);
}

void QNumericPad::on_pushButtonKey7_clicked()
{
    onKeyPressed(Qt::Key_7);
}

void QNumericPad::on_pushButtonKey8_clicked()
{
    onKeyPressed(Qt::Key_8);
}

void QNumericPad::on_pushButtonKey9_clicked()
{
    onKeyPressed(Qt::Key_9);
}

void QNumericPad::on_pushButtonKeyDot_clicked()
{
    onKeyPressed(Qt::Key_Period);
}

void QNumericPad::on_pushButtonKeyAdd_clicked()
{
    onKeyPressed(Qt::Key_Plus);
}

void QNumericPad::on_pushButtonKeySub_clicked()
{
    onKeyPressed(Qt::Key_Minus);
}

void QNumericPad::on_pushButtonKeyHalf_clicked()
{
    onKeyPressed(Qt::Key_F1);
}

void QNumericPad::on_pushButtonKeyBack_clicked()
{
    onKeyPressed(Qt::Key_Backspace);
}

void QNumericPad::on_pushButtonKeyEnter_clicked()
{
    onKeyPressed(Qt::Key_Enter);
    this->hide();
}

void QNumericPad::on_pushButtonKeyDel_clicked()
{
    onKeyPressed(Qt::Key_Delete);
}

void QNumericPad::on_pushButtonKeyCancel_clicked()
{
    onKeyPressed(Qt::Key_F2);
    this->hide();
}
