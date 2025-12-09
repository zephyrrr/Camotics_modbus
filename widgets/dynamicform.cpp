#include "dynamicform.h"

#include <QtWidgets>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include "../widgets/boollineedit.h"
#include "../widgets/qlineedit4axisvalue.h"
#include "utils/nfile.h"

DynamicForm::DynamicForm(QWidget* parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

//DynamicForm::DynamicForm(int margin, int hSpacing, int vSpacing)
//    : m_hSpace(hSpacing), m_vSpace(vSpacing)
//{
//    setContentsMargins(margin, margin, margin, margin);
//}

DynamicForm::~DynamicForm()
{
    QLayoutItem* item;
    while ((item = takeAt(0)))
        delete item;
}

void DynamicForm::createForm(QStringList captions, std::function<ILineEdit*(QString, QWidget*)> createEdit)
{
    m_captions = captions;
    for each (QString caption in captions)
    {
        QWidget* widget = new QWidget(this->parentWidget());

        QLayout* layout;
        if (isHorizontalLayout) {
            layout = new QVBoxLayout();
        }
        else {
            layout = new QHBoxLayout();
        }
		m_layouts[caption] = layout;
		m_itemWidgets[caption] = widget;

        QLabel* label = new QLabel(caption, widget);
        label->setObjectName("label_" + caption);
        m_labels[caption] = label;
        layout->addWidget(label);

        ILineEdit* inEdit = NULL;
        if (createEdit) {
            inEdit = createEdit(caption, this->parentWidget());
        }
        if (!inEdit) {
            inEdit = new QLineEdit4Keyboard(widget);
        }
        m_lineEdits[caption] = inEdit;

        QWidget* p2 = dynamic_cast<QWidget*>(inEdit);
        if (p2 != NULL) {
            p2->setObjectName("edit_" + caption);
            layout->addWidget(p2);
        }
        
        widget->setLayout(layout);

        layout->setMargin(0);
        layout->setSpacing(5);
        layout->setContentsMargins(0, 0, 0, 0);
        widget->setContentsMargins(0, 0, 1, 0);

        addWidget(widget);
    }
}

QString DynamicForm::getValue(const QString& caption) const
{
    if (m_lineEdits.contains(caption)) {
        return m_lineEdits.value(caption)->text();
    }
    else {
        //if (m_lineEditsOthers.contains(caption)) {
        //    BoolLineEdit* edit = qobject_cast<BoolLineEdit*>(m_lineEditsOthers.value(caption));
        //    if (edit) {
        //        return edit->text();
        //    }
        //}
        return QString();
    }

}
void DynamicForm::setValue(const QString& caption, const QString& text)
{
    if (m_lineEdits.contains(caption)) {
        m_lineEdits.value(caption)->setText(text);
    }
    else {
   //     if (m_lineEditsOthers.contains(caption)) {
   //         BoolLineEdit* edit = qobject_cast<BoolLineEdit*>(m_lineEditsOthers.value(caption));
   //         if (edit) {
			//	edit->setText(text);
			//}
   //     }
        //assert(false);
    }
}
void DynamicForm::setRegexValidation(QString caption, QString regex)
{
    if (m_lineEdits.contains(caption)) {
        QRegExp rx(regex);
        QRegExpValidator* validator = new QRegExpValidator(rx, this);

        QLineEdit* p = dynamic_cast<QLineEdit*>(m_lineEdits.value(caption));
        if (p) {
            p->setValidator(validator);
        }
    }
}

QLabel* DynamicForm::getLabel(QString caption) const
{
    // QLabel* label = this->parentWidget()->findChild<QLabel*>("label_" + caption);
    if (m_labels.contains(caption))
        return m_labels.value(caption);
    else
        return NULL;
}
ILineEdit* DynamicForm::getLineEdit(QString caption) const
{
    if (m_lineEdits.contains(caption))
        return m_lineEdits.value(caption);
    else
        return NULL;
}

void DynamicForm::setLabelCaption(QString caption, QString text) const
{
    if (m_labels.contains(caption)) {
        m_labels.value(caption)->setText(text);
	}
}

void DynamicForm::hideItem(QString caption) const
{
    //if (m_labels.contains(caption)) {
    //    m_labels.value(caption)->hide();
    //    m_lineEdits.value(caption)->hide();
    //}
    if (m_layouts.contains(caption)) {
        auto layout = m_layouts.value(caption);
        //layout->widget()->hide();
        m_itemWidgets[caption]->hide();

        for (int i = 0; i < layout->count(); ++i) {
            QLayoutItem* item = layout->itemAt(i);
            QWidget* widget = item->widget();

            if (widget) {
                widget->hide();
            }
        }
        
    }
}

bool DynamicForm::serialize(QString filePathName)
{
    try {
        QJsonObject data;
        for each (QString caption in m_captions)
        {
            data[caption] = getValue(caption);
        }

        QJsonDocument doc(data);
        QString json = doc.toJson();
        if (json != m_fileContent) {
            m_fileContent = json;
            NFile file(filePathName);
            file.open(QIODevice::WriteOnly);
            file.write(json.toUtf8());
            file.close();
        }
    }
    catch (...) {
        return false;
    }
    return true;
}

bool DynamicForm::deserialize(QString filePathName)
{
    try {
        if (QFile::exists(filePathName)) {
            QFile file(filePathName);
            file.open(QIODevice::ReadOnly);
            QString json = file.readAll();
            file.close();
            m_fileContent = json;

            QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
            QJsonObject data = doc.object();

            for each (QString caption in m_captions)
            {
                if (data.contains(caption)) {
                    setValue(caption, data[caption].toString());
                }
            }
        }
    }
    catch (...) {
        return false;
    }
    return true;
}

void DynamicForm::clearIsEditing()
{
    for each(QString caption in m_captions)
    {
		if (m_lineEdits.contains(caption)) {
			ILineEdit* edit = m_lineEdits.value(caption);
			edit->setIsEditing(false);
        }
    }
}

void DynamicForm::setReadOnly(bool readonly)
{
    for each(QString caption in m_captions)
    {
        if (m_lineEdits.contains(caption)) {
            ILineEdit* edit = m_lineEdits.value(caption);
            edit->setReadOnly(readonly);
        }
    }
}

void DynamicForm::addItem(QLayoutItem* item)
{
    itemList.append(item);
}

int DynamicForm::horizontalSpacing() const
{
    if (m_hSpace >= 0) {
        return m_hSpace;
    }
    else {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int DynamicForm::verticalSpacing() const
{
    if (m_vSpace >= 0) {
        return m_vSpace;
    }
    else {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

int DynamicForm::count() const
{
    return itemList.size();
}

QLayoutItem* DynamicForm::itemAt(int index) const
{
    return itemList.value(index);
}

QLayoutItem* DynamicForm::takeAt(int index)
{
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    return nullptr;
}

Qt::Orientations DynamicForm::expandingDirections() const
{
    return { };
}

bool DynamicForm::hasHeightForWidth() const
{
    return true;
}

int DynamicForm::heightForWidth(int width) const
{
    int height = doLayout(QRect(0, 0, width, 0), true);
    return height;
}

void DynamicForm::setGeometry(const QRect& rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize DynamicForm::sizeHint() const
{
    return minimumSize();
}

QSize DynamicForm::minimumSize() const
{
    QSize size;
    for (QLayoutItem* item : qAsConst(itemList)) {
        const QWidget* wid = item->widget();
        if (!wid->isVisible()) {
            continue;
        }
        size = size.expandedTo(item->minimumSize());
    }

    const QMargins margins = contentsMargins();
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
    return size;
}

int DynamicForm::doLayout(const QRect& rect, bool testOnly) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();

    if (isHorizontalLayout) {
        int lineHeight = 0;

        for (QLayoutItem* item : qAsConst(itemList)) {
            const QWidget* wid = item->widget();
            if (!wid->isVisible()) {
                continue;
            }
            int spaceX = horizontalSpacing();
            if (spaceX == -1)
                spaceX = wid->style()->layoutSpacing(
                    QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
            
            int nextX = x + item->sizeHint().width() + spaceX;
            if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
                int spaceY = verticalSpacing();
                if (spaceY == -1)
                    spaceY = wid->style()->layoutSpacing(
                        QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);

                x = effectiveRect.x();
                y = y + lineHeight + spaceY;
                nextX = x + item->sizeHint().width() + spaceX;
                lineHeight = 0;
            }

            if (!testOnly)
                item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

            x = nextX;
            lineHeight = qMax(lineHeight, item->sizeHint().height());
        }
        resize();
        return y + lineHeight - rect.y() + bottom;
    }
    else {
        int lineWidth = 0;

        for (QLayoutItem* item : qAsConst(itemList)) {
            const QWidget* wid = item->widget();
            if (!wid->isVisible()) {
                continue;
            }
            int spaceY = verticalSpacing();
            if (spaceY == -1)
                spaceY = wid->style()->layoutSpacing(
                    QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);

            int nextY = y + item->sizeHint().height() + spaceY;
            if (nextY - spaceY > effectiveRect.bottom() && lineWidth > 0) {
                int spaceX = horizontalSpacing();
                if (spaceX == -1)
                    spaceX = wid->style()->layoutSpacing(
                        QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);

                y = effectiveRect.y();
                x = x + lineWidth + spaceX;
                nextY = y + item->sizeHint().height() + spaceY;
                lineWidth = 0;
            }
            if (!testOnly)
                item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

            y = nextY;
            lineWidth = qMax(lineWidth, item->sizeHint().width());
        }
        resize();
        return y - rect.y() + bottom;
    }
}

int DynamicForm::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject* parent = this->parent();
    if (!parent) {
        return -1;
    }
    else if (parent->isWidgetType()) {
        QWidget* pw = dynamic_cast<QWidget*>(parent);
        return pw->style()->pixelMetric(pm, nullptr, pw);
    }
    else {
        return dynamic_cast<QLayout*>(parent)->spacing();
    }
}

void DynamicForm::resize() const
{
    if (!isHorizontalLayout) {
        int maxLabelWidth = 0;
        for each (QString caption in m_captions)
        {
            QLabel* label = getLabel(caption);
            if (label) {
                label->adjustSize();
                maxLabelWidth = qMax(maxLabelWidth, label->sizeHint().width());
            }
        }
        for each (QString caption in m_captions)
        {
            QLabel* label = getLabel(caption);
            if (label) {
                label->setFixedWidth(maxLabelWidth);
            }
        }
    }
}
