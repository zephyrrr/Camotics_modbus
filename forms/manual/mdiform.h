#ifndef MDIFORM_H
#define MDIFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

// to fix winsock compile error

#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#else
#endif

#include "camotics/qt/NCEdit.h"
#include "BaseChildWindow.h"

// Custom event filter class
class KeyPressFilter : public QObject {
	Q_OBJECT

public:
	explicit KeyPressFilter(QObject* parent = nullptr) : QObject(parent) {}

protected:
	bool eventFilter(QObject* obj, QEvent* event) override {
		if (event->type() == QEvent::KeyPress) {
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
				// Emit a signal when Enter is pressed
				emit enterKeyPressed();
				// Return true to stop the event from being processed further by the widget
				return false;
			}
		}
		// Pass the event along to the widget for default handling
		return QObject::eventFilter(obj, event);
	}

signals:
	void enterKeyPressed();
};

namespace Ui {
class MdiForm;
}

class MdiForm : public BaseChildWindow
{
    Q_OBJECT

public:
    explicit MdiForm(QWidget *parent = nullptr);
    
    ~MdiForm();

private:
    Ui::MdiForm *ui;
    CAMotics::NCEdit* editor;

public:
    static CAMotics::NCEdit* createNCEdit(QLayout* layout);
public:
    void RunGCode() override;
    void HideKeyboard() override;
    void ShowKeyboard(QWidget* receiver = NULL, bool autoHide = true, int keyboardType = 0) override;

    bool serialize();
    bool deserialize();

private slots:
    void on_btnKeyboard_clicked();
    void handleEnterKeyPress();
};

#endif // MDIFORM_H
