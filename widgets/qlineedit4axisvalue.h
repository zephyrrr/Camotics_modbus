#ifndef QLINEEDIT4AXISVALUE_H
#define QLINEEDIT4AXISVALUE_H

#include <QLineEdit>
#include <QDoubleValidator>
#include <QPushButton>
#include <QComboBox>

class MyDoubleValidator : public QDoubleValidator
{
public:
	MyDoubleValidator(double bottom, double top, int decimals, QObject* parent);

	QValidator::State validate(QString& s, int& i) const;
};


class ILineEdit {
public:
	ILineEdit() {}

	// Ensures to invoke actual object destructor
	virtual ~ILineEdit() {}

	virtual void setReadOnly(bool readonly) = 0; // Pure virtual function

	virtual void setIsEditing(bool b) = 0;

	virtual QString text() const = 0;
	virtual void setText(const QString& text) = 0;

	virtual QStyle* style() const = 0;
	virtual void setStyle(QStyle* style) = 0;
};

class NComboBox : public QComboBox, public ILineEdit
{
	Q_OBJECT
public:
	NComboBox(QWidget* parent);
	~NComboBox();

	void setReadOnly(bool readonly) override;
	QString text() const override;
	void setText(const QString& text) override;
	QStyle* style() const override;
	virtual void setStyle(QStyle* style) override;

	void setIsEditing(bool b) override;
};

class NLineEdit : public QLineEdit, public ILineEdit
{
	Q_OBJECT

		Q_PROPERTY(bool isEditing READ isEditing WRITE setIsEditing)
public:
	NLineEdit(QWidget* parent);
	~NLineEdit();

	void setReadOnly(bool readonly) override;
	QString text() const override;
	void setText(const QString& text) override;
	QStyle* style() const override;
	virtual void setStyle(QStyle* style) override;

	void setIsEditing(bool b) override;
	bool isEditing() const { return m_isEditing; }

protected:
	bool m_isEditing = false;
	QString m_originalText;
};

class QLineEdit4Keyboard : public NLineEdit
{
	Q_OBJECT

public:
	QLineEdit4Keyboard(QWidget* parent);
	~QLineEdit4Keyboard();

	void keyPressEvent(QKeyEvent* event) override;
	void focusInEvent(QFocusEvent* event) override;
	void focusOutEvent(QFocusEvent* event) override;

	void setAllowNull(bool allowNull) { m_allowNull = allowNull; }
Q_SIGNALS:
	void focusChanged(bool hasFocus);

protected:
	bool m_allowNull = true;	// 是否允许为空
};

class QLineEdit4Int : public QLineEdit4Keyboard
{
	Q_OBJECT

public:
	QLineEdit4Int(QWidget* parent);
	~QLineEdit4Int();

private:
};
class QLineEdit4AxisValue : public QLineEdit4Keyboard
{
	Q_OBJECT

public:
	QLineEdit4AxisValue(QWidget *parent);
	~QLineEdit4AxisValue();

	void keyPressEvent(QKeyEvent* event) override;
	void setIsEditing(bool b) override;
private:
};

class QLineEdit4Fdcs : public QLineEdit4Keyboard
{
	Q_OBJECT

public:
	QLineEdit4Fdcs(QWidget* parent);
	QLineEdit4Fdcs(int len, QWidget* parent);
	~QLineEdit4Fdcs() {};

	void setText(const QString& text) override;
	void keyPressEvent(QKeyEvent* event) override;
	void setIsEditing(bool b) override;

	void setLen(int len) { m_len = len; }
private:
	bool m_firstTime = true;
	int m_len = -1;
};

class QLineEdit4Coor : public QLineEdit4Keyboard
{
	Q_OBJECT

public:
	QLineEdit4Coor(QWidget* parent);
	~QLineEdit4Coor() {};

	void setIsEditing(bool b) override;
};

class QLineEditLikeCombo : public QLineEdit4Keyboard
{
	Q_OBJECT

public:
	QLineEditLikeCombo(QHash<QString, QString> combo, QString defaultValue = "", QWidget * parent = NULL);
	~QLineEditLikeCombo() {};

	void setIsEditing(bool b) override;
private:
	QHash<QString, QString> m_combo;
	QString m_defaultValue;
};

class QLineEditWithButton : public NLineEdit
{
	Q_OBJECT
public:
	QLineEditWithButton(QWidget* parent);
	~QLineEditWithButton();

private:
	bool m_isYes = false;
	QToolButton* btn;
};

class QInputWithHelp : public QWidget
{
	Q_OBJECT

public:
	QInputWithHelp(QWidget* parent, QWidget* input, QString label);
	~QInputWithHelp();
};

class QLineEditLikeButton : public NLineEdit
{
public:
	static bool IsYes(QString s) { return Values.indexOf(s) == 0; }

	QLineEditLikeButton(QWidget* parent = nullptr);
	void ToggleValue();
	void setText(const QString& text) override;
	//QString text() const override;
	bool IsYes() const { return IsYes(text()); }
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent*) override;

public:
	static QStringList Values;
};

class NPushButton4Edit : public QPushButton, public ILineEdit
{
	Q_OBJECT

public:
	NPushButton4Edit(QWidget* parent = nullptr);
	~NPushButton4Edit();

	void setReadOnly(bool readonly) override;
	QString text() const override;
	void setText(const QString& text) override;
	QStyle* style() const override;
	virtual void setStyle(QStyle* style) override;

	void setIsEditing(bool b) override;
};

class QLineEdit4AxisValueWrapper : public QObject {

	Q_OBJECT

public Q_SLOTS:
	// add a constructor
	QLineEdit4AxisValue* new_QLineEdit4AxisValue(QWidget* parent) { return new QLineEdit4AxisValue(parent); }

	// add a destructor
	void delete_QLineEdit4AxisValue(QLineEdit4AxisValue* o) { delete o; }

	void show(QLineEdit4AxisValue* o) { o->show(); }
	void setObjectName(QLineEdit4AxisValue* o, const QString &name) { o->setObjectName(name); }
};
class QLineEdit4IntWrapper : public QObject {

	Q_OBJECT

public Q_SLOTS:
	// add a constructor
	QLineEdit4Int* new_QLineEdit4Int(QWidget* parent) { return new QLineEdit4Int(parent); }

	// add a destructor
	void delete_QLineEdit4Int(QLineEdit4Int* o) { delete o; }

	void show(QLineEdit4Int* o) { o->show(); }
	void setObjectName(QLineEdit4Int* o, const QString& name) { o->setObjectName(name); }
};


class QLineEdit4KeyboardWrapper : public QObject {

	Q_OBJECT

public Q_SLOTS:
	// add a constructor
	QLineEdit4Keyboard* new_QLineEdit4Keyboard(QWidget* parent) { return new QLineEdit4Keyboard(parent); }

	// add a destructor
	void delete_QLineEdit4Keyboard(QLineEdit4Keyboard* o) { delete o; }

	void setObjectName(QLineEdit4Keyboard* o, const QString& name) { o->setObjectName(name); }
};
#endif