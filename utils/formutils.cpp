#include "formutils.h"
#include <QMessageBox>
#include <QPushButton>
#include <QFile>
#include <QLineEdit>
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include "widgets/qlineedit4axisvalue.h"
#include "widgets/qnumericpad.h"
#include "BaseChildWindow.h"

FormUtils::FormUtils()
{}

FormUtils::~FormUtils()
{}

bool FormUtils::MessageBoxYesNo(QString title)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(QObject::tr("Confirm"));
    //msgBox.setWindowFlags(Qt::FramelessWindowHint);
    msgBox.setText(title);
    //QPushButton* yesButton = msgBox.addButton(tr("S"), QMessageBox::YesRole);
    //QPushButton* noButton = msgBox.addButton(tr("F"), QMessageBox::NoRole);
    //msgBox.setDefaultButton(yesButton);

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    msgBox.button(QMessageBox::Yes)->setText(QObject::tr("Yes"));
    msgBox.button(QMessageBox::No)->setText(QObject::tr("No"));
    int ret = msgBox.exec();

    return ret == QMessageBox::Yes;
}

void FormUtils::MessageBoxInfo(QString title)
{
    QMessageBox::information(nullptr, QObject::tr("Message"), title);
}

QString FormUtils::ShowTextboxForm(QString title)
{
    QDialog dialog; // Use QDialog for a modal window
    dialog.setWindowTitle(title);
	dialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    QLineEdit4Keyboard passwordLineEdit(&dialog);
    passwordLineEdit.setEchoMode(QLineEdit::Password);

    QDialogButtonBox buttonBox = QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    QPushButton* okButton = buttonBox.button(QDialogButtonBox::StandardButton::Ok);
    QPushButton* cancelButton = buttonBox.button(QDialogButtonBox::StandardButton::Cancel);
    okButton->setText(QObject::tr("OK"));
    cancelButton->setText(QObject::tr("Cancel"));

    //SoftKeyboardWidget widget(&dialog);
    QVBoxLayout layout(&dialog);

    //layout.addWidget(&numericPad);
    layout.addWidget(&passwordLineEdit);
    layout.addWidget(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    QNumericPad::setButtonDownModeWhenInTouch(&dialog);
    //SoftKeyboardWidget::addVirtualKeyboardAll(&dialog);


    //QObject::connect(okButton, &QPushButton::clicked, [&]() {
    //    QString password = passwordLineEdit.text();
    //    if (password.isEmpty()) {
    //        QMessageBox::warning(&dialog, "Warning", "Password cannot be empty.");
    //    }
    //    else {
    //        // Process the password here (e.g., validate it)
    //        // ...
    //        QMessageBox::information(&dialog, "Success", "Password entered successfully.");
    //        dialog.accept(); // Close the dialog
    //    }
    //    });

	if (dialog.exec() == QDialog::Accepted) {
		return passwordLineEdit.text();
	}
    
	return QString();
}

QString FormUtils::ShowPasswordInput()
{
    QNumericPad numericPad;
    numericPad.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    numericPad.setWindowTitle(QObject::tr("Please enter password"));
    numericPad.exec();
    return numericPad.getText();
}

bool FormUtils::CopyFileWithOverwrite(const QString& sourceFilePath, const QString& destinationFilePath, bool overwrite)
{
    QFile sourceFile(sourceFilePath);
    QFile destinationFile(destinationFilePath);

    // Check if the source file exists
    if (!sourceFile.exists()) {
        return false;
    }

    // Check if the destination file exists and remove it if needed
    if (destinationFile.exists()) {
        if (!overwrite) {
            return false;
        }
        if (!destinationFile.remove()) {
            return false;
        }
    }

    // Copy the file
    if (!sourceFile.copy(destinationFilePath)) {
        return false;
    }

    return true;
}
