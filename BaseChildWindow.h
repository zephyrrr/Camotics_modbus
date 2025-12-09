#ifndef BASECHILDWINDOW_H
#define BASECHILDWINDOW_H

#include <QWidget>
#include <QDir>
#include "widgets/qnumericpad.h"
#include "widgets/WidgetKeyboard.h"
#include "modbus/NCMachineProperties.h"
#include "forms/component/tool4zeroandhalfaxis.h"

class BaseMainWindow;
class NCMachine;

class SoftKeyboardWidget : public QWidget
{
    Q_OBJECT

public:
    SoftKeyboardWidget(QWidget* parent = nullptr, bool useNumericKeyboard = true);
    ~SoftKeyboardWidget();

    bool eventFilter(QObject* obj, QEvent* event) override;
    void addVirtualKeyboard(QWidget* widget);
    void addVirtualKeyboardAll(QWidget* parent);

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    QNumericPad* m_keyboardNumeric;
    WidgetKeyboard* m_keyboardFull;
    bool m_bAutoHideKeyboard = true;
    bool m_useNumericKeyboard = true;
public:
    virtual void ShowKeyboard(QWidget* receiver = NULL, bool autoHide = true, int keyboardType = 0);
    virtual void HideKeyboard();
    bool IsVirtualKeyboardVisible();

    template<class T>
    static T* GetParentOfThis(QWidget* widget)
    {
        QWidget* window = nullptr;
        QWidget* parent = widget->parentWidget();

        while (parent) {
            window = parent;
            T* child = qobject_cast<T*>(window);
            if (child) {
                return child;
            }
            parent = parent->parentWidget();
        }

        return nullptr;
    }
};

class BaseChildWindow : public SoftKeyboardWidget
{
    Q_OBJECT

public:
    BaseChildWindow(QWidget* parent = nullptr, bool useNumericKeyboard = true);
    ~BaseChildWindow();

protected:
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;

    //void focusInEvent(QFocusEvent* event) override;
    //void focusOutEvent(QFocusEvent* event) override;

    //void keyPressEvent(QKeyEvent* event) override;
    //void closeEvent(QCloseEvent* event) override;
    

    NCMachine* m_ncMachine;

private:
    bool m_initialized = false;
    QHash<QString, QString> m_fileContent;
	QHash<QString, QVariant> m_data;

protected:
    QList<QWidget*> m_toClearInputWidgets;

public:
    NCMachine* GetMachine() { return m_ncMachine; }
    virtual void SetMachine(NCMachine* ncMachine)  { this->m_ncMachine = ncMachine; }
    virtual void LoadData(QString filePath = NULL);
    virtual void SaveData(QString filePath = NULL);
    static void LoadData(QWidget* parent, QString filePath = NULL, QHash<QString, QString> *alreadyFileContent = NULL);
    static void SaveData(QWidget* parent, QString filePath = NULL, QHash<QString, QString> *alreadyFileContent = NULL);

    virtual void ClearInputWidgets(QWidget* parent = NULL);

    virtual QString GetGCode() { return NULL; }
    virtual void RunGCode() {};

    static BaseChildWindow* GetChildWindow(QWidget* widget);
    static BaseMainWindow* GetMainWindow();

    QString GetProjectDir();
    static QString GetDataFilePath(QObject* obj, QString projectDir = NULL);

    void SetData(QString key, QVariant value) { m_data[key] = value; }
    QVariant GetData(QString key) { return m_data.contains(key) ? m_data[key] : NULL; }

    virtual void UpdateState() {};

//private slots:
//    void hideVirtualKeyboard();
};


class BaseChildWindowWrapper : public QObject {

    Q_OBJECT

public Q_SLOTS:
    // add a constructor
    BaseChildWindow* new_BaseChildWindow(QWidget* parent) { return new BaseChildWindow(parent); }

    // add a destructor
    void delete_BaseChildWindow(BaseChildWindow* o) { delete o; }

    void setObjectName(BaseChildWindow* o, const QString& name) { o->setObjectName(name); }


    void setData(BaseChildWindow* o, QString key, QVariant value) { o->SetData(key, value); }
    QVariant getData(BaseChildWindow* o, QString key) { return o->GetData(key); }
    NCMachine* GetMachine(BaseChildWindow* o) { return o->GetMachine(); }

    void HideKeyboard(BaseChildWindow* o) { o->HideKeyboard(); }
    void ShowKeyboard(BaseChildWindow* o) { o->ShowKeyboard(); }

    void static_BaseChildWindow_LoadData(QWidget* parent) { BaseChildWindow::LoadData(parent); }
    void static_BaseChildWindow_SaveData(QWidget* parent) { BaseChildWindow::SaveData(parent); }
};

class BaseChildWindowWithTools : public BaseChildWindow
{
    Q_OBJECT

public:
    explicit BaseChildWindowWithTools(QWidget* parent = nullptr, int toolsType = TOOL_ZERO_AXIS | TOOL_HALF_AXIS);
    ~BaseChildWindowWithTools();

    void UpdateState() override;
	Tool4All* GetTool() { return m_tool4ZeroAndHalfAxis; }

protected:
    void showEvent(QShowEvent* event) override;
    
protected:
    Tool4All* m_tool4ZeroAndHalfAxis = NULL;
    int m_toolsType;
};

class BaseChildWindowWithToolsWrapper : public QObject {

    Q_OBJECT
public Q_SLOTS:
    // add a constructor
    Tool4All* GetTool(BaseChildWindowWithTools* o) { return o->GetTool(); }

};

#endif