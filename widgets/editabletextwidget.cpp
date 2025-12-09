#include "editabletextwidget.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>
#include <QPalette>

EditableTextWidget::EditableTextWidget(const QString& text, QWidget* parent)
    : QWidget(parent)
{
    m_nowEditName = "";

    label = new QLabel(this);
    QPalette editPalette = label->palette();
    editPalette.setColor(QPalette::Text, Qt::blue);
    editPalette.setColor(QPalette::Link, Qt::blue);
    lineEdit = new QLineEdit("", this);
    lineEdit->setPalette(editPalette);
    lineEdit->hide();
    connect(lineEdit, &QLineEdit::editingFinished, this, &EditableTextWidget::handleEditingFinished);

    setupUI(text);

    connect(label, &QLabel::linkActivated, this, &EditableTextWidget::handleLinkActivated);
}
void EditableTextWidget::handleEditingFinished() {
    if (m_nowEditName.isEmpty()) {
        return;
    }
    QString newText = label->text();
    QString newValue = lineEdit->text();
    newText.replace(QString("<a href='%1:%2'>%2</a>").arg(m_nowEditName).arg(m_nowEditValue), QString("<a href='%1:%2'>%2</a>").arg(m_nowEditName).arg(newValue));
    label->setText(newText);
    //lineEdit->deleteLater();
    lineEdit->hide();
    m_paramValues[m_nowEditName] = newValue;
    m_nowEditName = "";
}

EditableTextWidget::~EditableTextWidget()
{}

void EditableTextWidget::setupUI(const QString& text)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    QString remainingText = text;

    while (!remainingText.isEmpty()) {
        int delimiterPos = remainingText.indexOf('%');
        if (delimiterPos == -1) {
            label->setText(label->text() + remainingText);
            layout->addWidget(label);
            break;
        }

        QString part = remainingText.left(delimiterPos);
        remainingText.remove(0, delimiterPos + 1);

        delimiterPos = remainingText.indexOf('%');
        QString editablePart = delimiterPos == -1 ? remainingText : remainingText.left(delimiterPos);
        remainingText.remove(0, delimiterPos == -1 ? remainingText.length() : delimiterPos + 1);

        int colonPos = editablePart.indexOf(':');
        if (colonPos == -1) {
            label->setText(label->text() + part + QString("<a href='%1'>%2</a>").arg(editablePart).arg(editablePart));
        }
        else {
            QString editablePartName = editablePart.mid(0, editablePart.indexOf(':'));
            QString editablePartText = editablePart.mid(editablePart.indexOf(':') + 1);
            assert(!m_paramValues.contains(editablePartName));
            m_paramValues[editablePartName] = editablePartText;
            label->setText(label->text() + part + QString("<a href='%1'>%2</a>").arg(editablePart).arg(editablePartText));
        }
    }

    label->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    layout->addWidget(label);
    setLayout(layout);
}
QString EditableTextWidget::removeATags(QString& text) {
    text.replace(QRegExp("<a[^>]*>"), "");
    text.replace(QRegExp("</a>"), "");
    return text;
}

void EditableTextWidget::handleLinkActivated(const QString& link)
{
    m_nowEditName = link.mid(0, link.indexOf(':'));
    m_nowEditValue = link.mid(link.indexOf(':') + 1);
    lineEdit->setText(m_nowEditValue);
    //QIntValidator* validator = new QIntValidator(lineEdit);
    //lineEdit->setValidator(validator);

    QString lblText = label->text();
    QString strRegex = QString("<a href='%1:(%2)'>\\1</a>").arg(m_nowEditName).arg(m_nowEditValue);
    QRegExp editableRegex(strRegex);
    int startIndex = lblText.indexOf(editableRegex);
    if (startIndex != -1) {
        QString editableText = editableRegex.cap(0);
        QFontMetrics fontMetrics(label->font());
        QRect editableRect = fontMetrics.boundingRect(removeATags(editableText));
        QString leftText = label->text().mid(0, startIndex);
        QRect editableRect2 = fontMetrics.boundingRect(removeATags(leftText));
        int textStartX = editableRect.x() + editableRect2.width();
        int textStartY = editableRect.y();

        int labelHeight = label->height();
        int textHeight = label->fontMetrics().height();
        // Calculate the vertical offset for the text
        int verticalOffset = (labelHeight - textHeight) / 2;

        QPoint editablePosition = label->mapToParent(QPoint(textStartX, textStartY + verticalOffset - editableRect.height()));
        lineEdit->setGeometry(QRect(editablePosition, QSize(300, editableRect.size().height() * 2)));

        lineEdit->show();
        lineEdit->setFocus();
    }

}