/*
 * Copyright 2009 EMBITEL (http://www.embitel.com)
 * 
 * This file is part of Virtual Keyboard Project.
 * 
 * Virtual Keyboard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation
 * 
 * Virtual Keyboard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Virtual Keyboard. If not, see <http://www.gnu.org/licenses/>.
 */

#include "WidgetKeyboard.h"
#include "ui_WidgetKeyboard.h"
#include <QKeyEvent>
#include <QSignalMapper>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QClipboard>
#include <QList>
#include "qnumericpad.h"
#include <cbang/log/Logger.h>

WidgetKeyboard::WidgetKeyboard(QWidget* parent) 
	: QDialog(parent), ui(new Ui::WidgetKeyboard)
{
    ui->setupUi(this);
    //resize(0,0);
    this->setWindowFlags(Qt::WindowDoesNotAcceptFocus | Qt::Tool | Qt::FramelessWindowHint | Qt::WindowTitleHint | Qt::CustomizeWindowHint ); //  
    this->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    this->setAttribute(Qt::WA_ShowWithoutActivating);

    ui->btnEmpty1->setText("     ");
    m_pParent = parent;
    m_isCaps = true;
    m_isShift = false;
    isCtrl = false;
    isAlt = false;
	isIns = false;
    changeTextCaps(m_isCaps);
    ui->btnCaps->setChecked(m_isCaps);

    signalMapper = new QSignalMapper(this);
    //sliderOpacity->setRange(20,100);
    allButtons = findChildren<QToolButton *>();
    for (int i = 0; i < allButtons.count(); i++) {
        connect(allButtons.at(i), SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(allButtons.at(i), i);
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(onButtonClicked(int)));

    QNumericPad::setButtonDownModeWhenInTouch(this);
}

WidgetKeyboard::~WidgetKeyboard()
{
	delete signalMapper;
}

QWidget* WidgetKeyboard::getReceiver()
{
    return m_pParent;
}
void WidgetKeyboard::setReceiver(QWidget* receiver)
{
    if (m_pParent != receiver)
    {
        m_pParent = receiver;
        //m_text.clear();
        if (this->isVisible()) {
            this->hide();
            this->show();
        }
    }
}

void WidgetKeyboard::onButtonClicked(int btn)
{
	QString strKeyId;
    strKeyId = allButtons.at(btn)->accessibleName();
    bool isOk;
    int keyId = strKeyId.toInt(&isOk, 16);
    if (strKeyId.isEmpty() || !isOk) {
        QMessageBox::information(0,0,"Key Not Found");
        return;
    }
    // tilt to esc
    if (keyId == 0x60) {
        this->hide();
		return;
	}
    //Keys to be handled separately
	if (keyId==Qt::Key_Shift
		|| keyId==Qt::Key_Control
		|| keyId==Qt::Key_Alt
		|| keyId==Qt::Key_Print
		)
        return;

    int involvedKeys = 1;
    Qt::KeyboardModifiers Modifier = Qt::NoModifier;
    if (isCtrl) {
        Modifier = Modifier | Qt::ControlModifier;
        involvedKeys++;
    }
    if (m_isShift) {
        Modifier = Modifier | Qt::ShiftModifier;
        involvedKeys++;
    }
    if (isAlt) {
        Modifier = Modifier | Qt::AltModifier;
        involvedKeys++;
    }    
    
    bool isTextKey = false;

	if(keyId==Qt::Key_Insert && !m_isShift)
		return;
	

	QString ch = allButtons.at(btn)->text().trimmed();
    if (ch=="&&")
        ch="&";
	else if (keyId==Qt::Key_Space)
        ch = " ";
    else if (checkNotTextKey(keyId))
		ch = QString();
	else
		isTextKey = true;
	    

    if (m_pParent != NULL && m_pParent->focusWidget() != NULL) {
        if (isIns && isTextKey) {
            QKeyEvent keyEventIns(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier);
            QCoreApplication::sendEvent(m_pParent->focusWidget(), &keyEventIns);
        }

        QKeyEvent keyEvent(QEvent::KeyPress, keyId, Modifier, ch, false, involvedKeys);
        QCoreApplication::sendEvent(m_pParent->focusWidget(), &keyEvent);

        QString objectInfo = QString("Object '%1' (Class: %2)")
            .arg(m_pParent->objectName())
            .arg(m_pParent->metaObject()->className());
        QString s = QString("UI: Key Press %1 on %2")
            .arg(QKeySequence(keyId).toString())
            .arg(objectInfo);
        LOG_INFO(8, s.toUtf8().constData());
    }
    
    ui->btnShiftLeft->setChecked(false);
    //ui->btnShiftRight->setChecked(false);
    ui->btnCtrlLeft->setChecked(false);
    ui->btnAltLeft->setChecked(false);
}

void WidgetKeyboard::on_btnCaps_toggled(bool checked)
{
    changeTextCaps(checked);
    m_isCaps = checked;
}

void WidgetKeyboard::on_btnShiftLeft_toggled(bool checked)
{
    m_isShift = checked;
    if (m_isCaps) {
        changeTextShift(checked);
    }
    else {
        changeTextShift(checked);
        changeTextCaps(checked);
    }
}

//void WidgetKeyboard::on_btnShiftRight_toggled(bool checked)
//{
//    on_btnShiftLeft_toggled(checked);
//}

void WidgetKeyboard::on_btnCtrlLeft_toggled(bool checked)
{
    isCtrl = checked;
}

void WidgetKeyboard::on_btnAltLeft_toggled(bool checked)
{
    isAlt = checked;
}

//void WidgetKeyboard::on_btnIns_clicked()
//{
//	isIns = !isIns;
//}

//void WidgetKeyboard::on_btnPrint_clicked()
//{
//	QPixmap screen = QPixmap::grabWindow(QApplication::desktop()->winId());
//	QClipboard *cb = QApplication::clipboard();
//	cb->setPixmap(screen, QClipboard::Clipboard);
//}

//void WidgetKeyboard::on_sliderOpacity_valueChanged(int val)
//{
//    opacity = 120.0 - val;
//    opacity /= 100.0;    
//    setWindowOpacity(opacity);    
//}

void WidgetKeyboard::changeTextShift(bool isShift)
{    
    changeTextCaps(!isShift);
    if (isShift) {
        //ui->btnTilt->setText(QChar('~'));
        ui->btn1->setText(QChar('!'));
        ui->btn2->setText(QChar('@'));
        ui->btn3->setText(QChar('#'));
        ui->btn4->setText(QChar('$'));
        ui->btn5->setText(QChar('%'));
        ui->btn6->setText(QChar('^'));
        ui->btn7->setText("&&");
        ui->btn8->setText(QChar('*'));
        ui->btn9->setText(QChar('('));
        ui->btn0->setText(QChar(')'));
        ui->btnHiphen->setText(QChar('_'));
        ui->btnAssign->setText(QChar('+'));
        
        ui->btnStartSquare->setText(QChar('{'));
        ui->btnCloseSquare->setText(QChar('}'));
        ui->btnFwdSlash->setText(QChar('|'));
        
        ui->btnSemiColon->setText(QChar(':'));
        ui->btnSp->setText(QChar('"'));
            
        ui->btnComma->setText(QChar('<'));
        ui->btnPeriod->setText(QChar('>'));
        ui->btnBcwdSlash->setText(QChar('?'));
	}
    else {
        //ui->btnTilt->setText(QChar('`'));
        ui->btn1->setText(QChar('1'));
        ui->btn2->setText(QChar('2'));
        ui->btn3->setText(QChar('3'));
        ui->btn4->setText(QChar('4'));
        ui->btn5->setText(QChar('5'));
        ui->btn6->setText(QChar('6'));
        ui->btn7->setText(QChar('7'));
        ui->btn8->setText(QChar('8'));
        ui->btn9->setText(QChar('9'));
        ui->btn0->setText(QChar('0'));
        ui->btnHiphen->setText(QChar('-'));
        ui->btnAssign->setText(QChar('='));
            
        ui->btnStartSquare->setText(QChar('['));
        ui->btnCloseSquare->setText(QChar(']'));
        ui->btnFwdSlash->setText(QChar('\\'));
        
        ui->btnSemiColon->setText(QChar(';'));
        ui->btnSp->setText(QChar('\''));
        
        ui->btnComma->setText(QChar(','));
        ui->btnPeriod->setText(QChar('.'));
        ui->btnBcwdSlash->setText(QChar('/'));
    }
}

void WidgetKeyboard::changeTextCaps(bool isCaps)
{    
    if (isCaps) {
        ui->btnQ->setText(QChar('Q'));
        ui->btnW->setText(QChar('W'));
        ui->btnE->setText(QChar('E'));
        ui->btnR->setText(QChar('R'));
        ui->btnT->setText(QChar('T'));
        ui->btnY->setText(QChar('Y'));
        ui->btnU->setText(QChar('U'));
        ui->btnI->setText(QChar('I'));
        ui->btnO->setText(QChar('O'));
        ui->btnP->setText(QChar('P'));

        ui->btnA->setText(QChar('A'));
        ui->btnS->setText(QChar('S'));
        ui->btnD->setText(QChar('D'));
        ui->btnF->setText(QChar('F'));
        ui->btnG->setText(QChar('G'));
        ui->btnH->setText(QChar('H'));
        ui->btnJ->setText(QChar('J'));
        ui->btnK->setText(QChar('K'));
        ui->btnL->setText(QChar('L'));
                
        ui->btnZ->setText(QChar('Z'));
        ui->btnX->setText(QChar('X'));
        ui->btnC->setText(QChar('C'));
        ui->btnV->setText(QChar('V'));
        ui->btnB->setText(QChar('B'));
        ui->btnN->setText(QChar('N'));
        ui->btnM->setText(QChar('M'));
    }
	else {
        ui->btnQ->setText(QChar('q'));
        ui->btnW->setText(QChar('w'));
        ui->btnE->setText(QChar('e'));
        ui->btnR->setText(QChar('r'));
        ui->btnT->setText(QChar('t'));
        ui->btnY->setText(QChar('y'));
        ui->btnU->setText(QChar('u'));
        ui->btnI->setText(QChar('i'));
        ui->btnO->setText(QChar('o'));
        ui->btnP->setText(QChar('p'));

        ui->btnA->setText(QChar('a'));
        ui->btnS->setText(QChar('s'));
        ui->btnD->setText(QChar('d'));
        ui->btnF->setText(QChar('f'));
        ui->btnG->setText(QChar('g'));
        ui->btnH->setText(QChar('h'));
        ui->btnJ->setText(QChar('j'));
        ui->btnK->setText(QChar('k'));
        ui->btnL->setText(QChar('l'));
        
        ui->btnZ->setText(QChar('z'));
        ui->btnX->setText(QChar('x'));
        ui->btnC->setText(QChar('c'));
        ui->btnV->setText(QChar('v'));
        ui->btnB->setText(QChar('b'));
        ui->btnN->setText(QChar('n'));
        ui->btnM->setText(QChar('m'));
    }
}

bool WidgetKeyboard::checkNotTextKey(int keyId)
{
	if (keyId==Qt::Key_Shift
            || keyId==Qt::Key_Control
            || keyId==Qt::Key_Tab
            || keyId==Qt::Key_Escape
            || keyId==Qt::Key_Return
            || keyId==Qt::Key_Insert
            || keyId==Qt::Key_NumLock
            || keyId==Qt::Key_F1
            || keyId==Qt::Key_F2
            || keyId==Qt::Key_F3
            || keyId==Qt::Key_F4
            || keyId==Qt::Key_F5
            || keyId==Qt::Key_F6
            || keyId==Qt::Key_F7
            || keyId==Qt::Key_F8
            || keyId==Qt::Key_F9
            || keyId==Qt::Key_F10
            || keyId==Qt::Key_F11
            || keyId==Qt::Key_F12
            || keyId==Qt::Key_Print
            || keyId==Qt::Key_Pause
            || keyId==Qt::Key_ScrollLock
            || keyId==Qt::Key_Enter
            || keyId==Qt::Key_Home
            || keyId==Qt::Key_End
            || keyId==Qt::Key_CapsLock
            || keyId==Qt::Key_Insert
            || keyId==Qt::Key_Delete
            || keyId==Qt::Key_PageUp
            || keyId==Qt::Key_PageDown
            || keyId==Qt::Key_Down
            || keyId==Qt::Key_Up
            || keyId==Qt::Key_Left
            || keyId==Qt::Key_Right
			|| keyId==Qt::Key_Alt) {
				return true;
	}
	else
		return false;
}

//void WidgetKeyboard::setOpacity(int opacity)
//{
//    //sliderOpacity->setValue(opacity);
//}
//
//int WidgetKeyboard::getOpacity()
//{
//    return sliderOpacity->value();
//}

//void WidgetKeyboard::setSoundDisabled()
//{
//	soundFilePath = "";
//}

/* 
Microsoft Windows : The underlying multimedia system is used; only WAVE format sound files are supported.
X11	: The Network Audio System is used if available, otherwise all operations work silently. NAS supports WAVE and AU files.
Mac OS X : NSSound is used. All formats that NSSound supports, including QuickTime formats, are supported by Qt for Mac OS X.
Qt for Embedded Linux : A built-in mixing sound server is used, accessing /dev/dsp directly. Only the WAVE format is supported.
*/

//void WidgetKeyboard::setSoundEnabled(QString soundPath)
//{
//	soundFilePath = soundPath;
//}

//void WidgetKeyboard::on_checkBoxShowNumPad_stateChanged(int state)
//{
//	QList<QWidget *> children = frameNumPad->findChildren<QWidget *>();
//	if(state==Qt::Checked)
//	{
//		frameNumPad->show();
//		/*for each(QWidget *child in children)
//			child->show();*/
//	}
//	else
//	{
//		frameNumPad->hide();
//		/*for each(QWidget *child in children)
//			child->hide();*/
//	}
//	update();
//}
