#ifndef DYNAMICFORM_H
#define DYNAMICFORM_H

#include <QLayout>
#include <QRect>
#include <QStyle>
#include <QLineEdit>
#include <QLabel>
#include "qlineedit4axisvalue.h"
#include "utils/dataform.h"

//class IInputWidget {
//public:
//    // 纯虚函数，派生类必须实现
//    virtual QString getValue() = 0;
//
//    // 虚析构函数，确保派生类的析构函数被正确调用
//    virtual ~IInputWidget() {}
//};
//
//class InputWidget4LineEdit : public QLineEdit, public IInputWidget
//{
//    Q_OBJECT // 如果你的类使用了Qt的信号和槽机制，需要这个宏
//
//public:
//    // 使用父类QLineEdit的构造函数
//    using QLineEdit::QLineEdit;
//
//    // 实现接口中的纯虚函数
//    QString getValue() override {
//        // 具体实现
//        // 例如，改变文本框的文本
//        return this->text();
//    }
//
//    // 虚析构函数
//    virtual ~InputWidget4LineEdit() {}
//};

class DynamicForm : public QLayout, public IDataForm
{
public:
    explicit DynamicForm(QWidget* parent=NULL, int margin = 5, int hSpacing = -1, int vSpacing = -1);
    //explicit DynamicForm(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    ~DynamicForm();

    void createForm(QStringList captions, std::function<ILineEdit* (QString, QWidget*)> createEdit = nullptr);

    QString getValue(const QString& caption) const override;
    void setValue(const QString& caption, const QString& text) override;
    void setRegexValidation(QString caption, QString regex);
    QLabel* getLabel(QString caption) const;
    ILineEdit* getLineEdit(QString caption) const;
    void setLabelCaption(QString caption, QString text) const;
    void hideItem(QString caption) const;

    bool serialize(QString filePathName);
    bool deserialize(QString filePathName);

    void clearIsEditing();

	void setReadOnly(bool readonly);

    bool isHorizontalLayout = true;
    QHash<QString, QWidget*> m_itemWidgets;
    QHash<QString, QLayout*> m_layouts;
    QHash<QString, QLabel*> m_labels;
    QHash<QString, ILineEdit*> m_lineEdits;
    //QHash<QString, QWidget*> m_lineEditsOthers;
public:
    void addItem(QLayoutItem* item) override;
    int horizontalSpacing() const;
    int verticalSpacing() const;
    Qt::Orientations expandingDirections() const override;
    bool hasHeightForWidth() const override;
    int heightForWidth(int) const override;
    int count() const override;
    QLayoutItem* itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect& rect) override;
    QSize sizeHint() const override;
    QLayoutItem* takeAt(int index) override;

    void setHorizontalSpacing(int hSpace) { m_hSpace = hSpace; }
    void setVerticalSpacing(int vSpace) { m_vSpace = vSpace; }
private:
    int doLayout(const QRect& rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;

    void resize() const;

    QList<QLayoutItem*> itemList;
    int m_hSpace;
    int m_vSpace;

    QStringList m_captions;

    QString m_fileContent;
};

#endif

