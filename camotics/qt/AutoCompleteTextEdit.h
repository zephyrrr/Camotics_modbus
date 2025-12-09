#pragma once

#include <QtGlobal>
#include <QtWidgets>
#include <QCompleter>

namespace CAMotics {

	class AutoCompleteTextEdit : public QPlainTextEdit
	{
		Q_OBJECT;

	public:
		AutoCompleteTextEdit(QWidget* parent = nullptr);
		~AutoCompleteTextEdit();

		void setCompleter(QCompleter* c);
		QCompleter* completer() const;

	protected:
		void keyPressEvent(QKeyEvent* e) override;
		void focusInEvent(QFocusEvent* e) override;

	private slots:
		void insertCompletion(const QString& completion);

	private:
		QString textUnderCursor() const;

	private:
		QCompleter* c = nullptr;
	};
}

