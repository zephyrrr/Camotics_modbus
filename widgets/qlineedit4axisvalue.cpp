#include "qlineedit4axisvalue.h"
#include <QDoubleValidator>
#include <QFocusEvent>
#include <QTimer>
#include <QWidgetAction>
#include <QSize>
#include <QToolButton>
#include <QStyle>
#include <QPainter>
#include <QStyleOption>
#include "BaseChildWindow.h"

MyDoubleValidator::MyDoubleValidator(double bottom, double top, int decimals, QObject* parent) :
	QDoubleValidator(bottom, top, decimals, parent)
{
}

QValidator::State MyDoubleValidator::validate(QString& s, int& i) const
{
	Q_UNUSED(i);

	if (s.isEmpty() || s == "-") {
		return QValidator::Intermediate;
	}

	QChar decimalPoint = locale().decimalPoint();

	if (s.indexOf(decimalPoint) != -1) {
		int charsAfterPoint = s.length() - s.indexOf(decimalPoint) - 1;

		if (charsAfterPoint > decimals()) {
			return QValidator::Invalid;
		}
	}

	bool ok;
	double d = locale().toDouble(s, &ok);

	if (ok && d >= bottom() && d <= top()) {
		return QValidator::Acceptable;
	}
	else {
		return QValidator::Invalid;
	}
}

QLineEdit4Int::QLineEdit4Int(QWidget* parent)
	: QLineEdit4Keyboard(parent)
{
	this->setValidator(new QIntValidator(1, 100000, this));
}

QLineEdit4Int::~QLineEdit4Int()
{
}

QLineEdit4AxisValue::QLineEdit4AxisValue(QWidget* parent)
	: QLineEdit4Keyboard(parent)
{
	this->setValidator(new MyDoubleValidator(-100000, 100000, 3, this));
	//this->setInputMask("#####.###");
	this->setInputMethodHints(Qt::ImhDigitsOnly);
}

QLineEdit4AxisValue::~QLineEdit4AxisValue()
{}


void QLineEdit4AxisValue::keyPressEvent(QKeyEvent* event)
{
	QLineEdit4Keyboard::keyPressEvent(event);
	if (event->key() == Qt::Key_F1) {
		QString s = this->text();
		
		if (!s.isEmpty()) {
			double d = s.toDouble();
			//d = d / 2;
			d = -d;
			QString newText = QString::number(d, 'f', 3);

			const QValidator* validator = this->validator();
			if (validator) {
				int pos = 0;
				if (validator->validate(newText, pos) != QValidator::Invalid) {
					this->setText(newText);
				}
			}
			else {
				this->setText(newText);
			}
		}
	}
}

void QLineEdit4AxisValue::setIsEditing(bool b)
{
	QLineEdit4Keyboard::setIsEditing(b);
	if (!b) {
		if (m_allowNull && this->text().isEmpty()) {

		}
		else {
			QString s = this->text();
			if (!s.isEmpty()) {
				double d = s.toDouble();
				this->setText(QString::number(d, 'f', 3));
			}
			else {
				if (!m_allowNull) {
					setText(m_originalText);
				}
			}
		}
	}
}

NLineEdit::NLineEdit(QWidget* parent)
	: QLineEdit(parent)
{
}

NLineEdit::~NLineEdit()
{
}

void NLineEdit::setReadOnly(bool readonly)
{
	QLineEdit::setReadOnly(readonly);
}

QString NLineEdit::text() const
{
	return QLineEdit::text();
}

void NLineEdit::setText(const QString& text)
{
	QLineEdit::setText(text);
}

QStyle* NLineEdit::style() const
{
	return QLineEdit::style();
}

void NLineEdit::setStyle(QStyle* style)
{
	QLineEdit::setStyle(style);
}

void NLineEdit::setIsEditing(bool b)
{
	if (m_isEditing == b)
		return;
	m_isEditing = b;
	this->setStyle(this->style());
	if (b) {
		m_originalText = text();
	}
	else {

	}
}

QLineEdit4Keyboard::QLineEdit4Keyboard(QWidget* parent)
	: NLineEdit(parent)
{
	setFocusPolicy(Qt::StrongFocus);
}

QLineEdit4Keyboard::~QLineEdit4Keyboard()
{
}

void QLineEdit4Keyboard::focusInEvent(QFocusEvent* event)
{
	QLineEdit::focusInEvent(event);

	if (event->reason() == Qt::ActiveWindowFocusReason)
		return;
	if (this->isReadOnly())
		return;

	emit(focusChanged(true));

	if (event->reason() == Qt::MouseFocusReason || event->reason() == Qt::OtherFocusReason) {
		if (event->reason() == Qt::MouseFocusReason) {
			QTimer::singleShot(0, this, SLOT(selectAll()));
		}
		SoftKeyboardWidget* w = SoftKeyboardWidget::GetParentOfThis<SoftKeyboardWidget>(this);
		if (w) {
			w->ShowKeyboard(this);
		}
	}
	
	if (!m_isEditing) {
		this->setIsEditing(true);
	}
}

void QLineEdit4Keyboard::focusOutEvent(QFocusEvent* event)
{
	QLineEdit::focusOutEvent(event);

	if (event->reason() == Qt::ActiveWindowFocusReason)
		return;
	if (this->isReadOnly())
		return;

	emit(focusChanged(false));

	if (event->reason() == Qt::MouseFocusReason) {
		SoftKeyboardWidget* w = SoftKeyboardWidget::GetParentOfThis<SoftKeyboardWidget>(this);
		if (w) {
			w->HideKeyboard();
		}
	}
	if (m_isEditing) {
		this->setIsEditing(false);

		if (!m_allowNull && text().isEmpty()) {
			setText(m_originalText);
		}
	}
}



void QLineEdit4Keyboard::keyPressEvent(QKeyEvent* event)
{
	QLineEdit::keyPressEvent(event);
	if (event->key() == Qt::Key_Delete) {
		this->clear();
		event->accept();
	}
	else if (event->key() == Qt::Key_F2) {
		this->setText(m_originalText);
		this->clearFocus();
		event->accept();
	}
	else if (event->key() == Qt::Key_Enter) {
		this->clearFocus();
		event->accept();

		//BaseChildWindow* widget = BaseChildWindow::GetChildWindow(this);
		//QMouseEvent pressEvent(QEvent::MouseButtonPress, QPoint(1, 1), Qt::LeftButton, Qt::MouseButtons(Qt::LeftButton), Qt::NoModifier);
		//QApplication::sendEvent(widget, &pressEvent);
	}
}

QInputWithHelp::QInputWithHelp(QWidget* parent, QWidget* input, QString label)
	:QWidget(parent)
{
	Q_UNUSED(input);
	Q_UNUSED(label);
}

QInputWithHelp::~QInputWithHelp()
{
}
QLineEdit4Fdcs::QLineEdit4Fdcs(QWidget* parent)
	: QLineEdit4Keyboard(parent)
{
}

QLineEdit4Fdcs::QLineEdit4Fdcs(int len, QWidget* parent)
	: QLineEdit4Keyboard(parent), m_len(len)
{
	assert(len >= -1);
	this->setAllowNull(false);
}

void QLineEdit4Fdcs::setText(const QString& text)
{
	if (m_len > 0) {
		if (text.length() > m_len) {
			QLineEdit4Keyboard::setText(text.right(m_len));
		}
		else {
			QLineEdit4Keyboard::setText(text.rightJustified(m_len, '0'));
		}
	}
	else {
		QLineEdit4Keyboard::setText(text);
	}
}

void QLineEdit4Fdcs::keyPressEvent(QKeyEvent* event)
{
	//QLineEdit4Keyboard::keyPressEvent(event);
	if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9){
		QString newText;
		if (m_firstTime) {
			if (m_len > 0)
			{
				newText = event->text().rightJustified(m_len, '0');
			}
			else {
				newText = event->text();
			}
			m_firstTime = false;
		}
		else {
			newText = this->text() + event->text();
			if (m_len > 0) {
				if (newText.length() > m_len) {
					newText = newText.right(m_len);
				}
				else {
					newText = newText.rightJustified(m_len, '0');
				}
			}
		}

		const QValidator* validator = this->validator();
		if (validator) {
			if (m_len > 0) {
				bool accepted = false;
				for (int i = 0; i < m_len; ++i) {
					int pos = this->cursorPosition();
					if (validator->validate(newText, pos) != QValidator::Invalid) {
						this->setText(newText);
						accepted = true;
					}
					else {
						newText = newText.right(newText.length() - 1).rightJustified(m_len, '0');
					}
				}
				if (!accepted) {
					int pos = this->cursorPosition();
					QString newTextNoZero = newText.remove(QRegularExpression("^0+"));
					if (validator->validate(newTextNoZero, pos) != QValidator::Invalid) {
						this->setText(newTextNoZero);
						accepted = true;
					}
				}
			}
			else {
				int pos = this->cursorPosition();
				if (validator->validate(newText, pos) != QValidator::Invalid) {
					this->setText(newText);
				}
			}
		}
		else {
			this->setText(newText);
		}
	}
	else {
		QLineEdit4Keyboard::keyPressEvent(event);
	}
}

void QLineEdit4Fdcs::setIsEditing(bool b)
{
	QLineEdit4Keyboard::setIsEditing(b);
	if (b) {
		m_firstTime = true;
	}
}

QLineEdit4Coor::QLineEdit4Coor(QWidget* parent)
	: QLineEdit4Keyboard(parent)
{
	QRegExp regExp("^[0-9][5][4-9]|[5][4-9]$");
	QRegExpValidator* validator = new QRegExpValidator(regExp, this);
	this->setValidator(validator);
	this->setAllowNull(true);
}

void QLineEdit4Coor::setIsEditing(bool b)
{
	QLineEdit4Keyboard::setIsEditing(b);
	if (!b) {
		QString newText = this->text();
		if (!newText.isEmpty()) {
			const QValidator* validator = this->validator();
			int pos = newText.length();
			if (validator->validate(newText, pos) != QValidator::Acceptable) {
				this->setText("");
			}
			else {
				if (newText.length() < 3) {
					newText = newText.rightJustified(3, '0');
					this->setText(newText);
				}
			}
		}
	}
}

QLineEditWithButton::QLineEditWithButton(QWidget* parent)
	: NLineEdit(parent)
{
	btn = new QToolButton(this);
	btn->setCursor(Qt::ArrowCursor);

	//int frameWidth = this->style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
	//QSize buttonSize = btn->sizeHint();
	//this->setStyleSheet(QString("QLineEdit{padding - right: %1 dpx;}").arg(buttonSize.width() + frameWidth + 1));
	//this->setMinimumSize(qMax(this->minimumSizeHint().width(), buttonSize.width() + frameWidth * 2 + 2),
	//	qMax(this->minimumSizeHint().height(), buttonSize.height() + frameWidth * 2 + 2));

	btn->setText("");
	QWidgetAction* action = new QWidgetAction(this);
	action->setDefaultWidget(btn);
	this->addAction(action, QLineEdit::TrailingPosition);

	connect(btn, &QToolButton::clicked, [this]() {
		m_isYes = !m_isYes;
		this->setText(m_isYes ? "Y" : "N");
		});
	btn->setStyleSheet("padding: 0px; margin: 0px; background-color: green;");
	btn->setFixedWidth(this->width() / 2);
	btn->setFixedHeight(this->height());

	connect(this, &QLineEdit::textChanged, [this](const QString& s) {
		if (s == "Y")
			m_isYes = true;
		else
			m_isYes = false;
		});
}

QLineEditWithButton::~QLineEditWithButton()
{
}

const QStringList QLineEditLikeButton::Values = { QStringLiteral("ÊÇ"), QStringLiteral("·ñ") };

QLineEditLikeButton::QLineEditLikeButton(QWidget* parent)
	: NLineEdit(parent)
{
	this->setMaxLength(1);
	QRegExp rx(QString("^[%01%02]$").arg(Values[0]).arg(Values[1]));
	QRegExpValidator* validator = new QRegExpValidator(rx, this);
	this->setValidator(validator);

	//setAttribute(Qt::WA_Hover, true);
	//setStyleSheet("background-color: rgb(220, 220, 220);");
	//setStyleSheet("color: black;");
	////setFont(QFont("Arial", 12, QFont::Bold));
	//setStyleSheet("border: 2px solid rgb(220, 220, 220);");

	//connect(this, &QLineEdit::mousePressEvent, [this](const QMouseEvent* e) {
	//	if (this->text() == QStringLiteral("ÊÇ"))
	//		this->setText(QStringLiteral("·ñ"));
	//	else
	//		this->setText(QStringLiteral("ÊÇ"));
	//	});
	//setReadOnly(true);
	setAlignment(Qt::AlignCenter);
	setFocusPolicy(Qt::NoFocus);
	//setStyleSheet("QLineEdit { border: 2px solid gray; border-radius: 5px; padding: 0 8px; background: lightgray; selection-background-color: darkgray; }");
}

void QLineEditLikeButton::ToggleValue()
{
	if (this->text() == Values[0])
		this->setText(Values[1]);
	else
		this->setText(Values[0]);
	this->setIsEditing(false);
}

void QLineEditLikeButton::setText(const QString& text)
{
	QString newText = text;
	if (text.toUpper() == "N" || text.toUpper() == "FALSE" || text == Values[1] || text.isEmpty())
		newText = Values[1];
	//if (text.toUpper() == "Y" || text.toUpper() == "TRUE")
	else
		newText = Values[0];
	NLineEdit::setText(newText);
}

//QString QLineEditLikeButton::text() const
//{
//	QString text = NLineEdit::text();
//	if (text == m_values[0])
//		return "Y";
//	else if (text == m_values[1])
//		return "N";
//	else
//		return "";
//}


void QLineEditLikeButton::mousePressEvent(QMouseEvent* event)
{
	if (this->isReadOnly())
		return;
	this->ToggleValue();
	NLineEdit::mousePressEvent(event); // Call the base class implementation
}

// disable it for default behavior of selectAll
void QLineEditLikeButton::mouseDoubleClickEvent(QMouseEvent*)
{
	if (this->isReadOnly())
		return;
	this->ToggleValue();
}

NPushButton4Edit::NPushButton4Edit(QWidget* parent)
	:QPushButton(parent)
{
}

NPushButton4Edit::~NPushButton4Edit()
{
}

void NPushButton4Edit::setReadOnly(bool readonly)
{
	//QPushButton::setEnabled(!readonly);
}

QString NPushButton4Edit::text() const
{
	return QPushButton::text();
}

void NPushButton4Edit::setText(const QString& text)
{
	if (text.isEmpty()) {
		QPushButton::setText("...");
	}
	else {
		QPushButton::setText(text);
	}
}

QStyle* NPushButton4Edit::style() const
{
	return QPushButton::style();
}

void NPushButton4Edit::setStyle(QStyle* style)
{
	QPushButton::setStyle(style);
}

void NPushButton4Edit::setIsEditing(bool b)
{
}

NComboBox::NComboBox(QWidget* parent)
	: QComboBox(parent)
{
}

NComboBox::~NComboBox()
{
}


void NComboBox::setReadOnly(bool readonly)
{
	QComboBox::setEnabled(!readonly);
}

QString NComboBox::text() const
{
	return QComboBox::currentText();
}

void NComboBox::setText(const QString& text)
{
	QComboBox::setCurrentText(text);
}

QStyle* NComboBox::style() const
{
	return QComboBox::style();
}

void NComboBox::setStyle(QStyle* style)
{
	QComboBox::setStyle(style);
}

void NComboBox::setIsEditing(bool b)
{
}

QLineEditLikeCombo::QLineEditLikeCombo(QHash<QString, QString> combo, QString defaultValue, QWidget* parent)
	: QLineEdit4Keyboard(parent), m_combo(combo), m_defaultValue(defaultValue)
{
}

void QLineEditLikeCombo::setIsEditing(bool b)
{
	QLineEdit4Keyboard::setIsEditing(b);
	if (!b) {
		QString newText = this->text();
		if (m_combo.contains(newText)) {
			this->setText(m_combo[newText]);
		} 
		else if (m_combo.values().contains(newText)) {
		}
		else {
			this->setText(m_defaultValue);
		}
	}

}


