#pragma once
#include <QString>

class FormUtils 
{
public:
	FormUtils();
	~FormUtils();

	static bool MessageBoxYesNo(QString title);
	static void MessageBoxInfo(QString title);
	static QString ShowTextboxForm(QString title);
	static QString ShowPasswordInput();
	static bool CopyFileWithOverwrite(const QString& sourceFilePath, const QString& destinationFilePath, bool overwrite = true);
};
