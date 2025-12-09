#include "nfile.h"
#include <QFileInfo>
#include <windows.h>
#include <cbang/log/Logger.h>

void NFile::close()
{
    QFileInfo fi(this->fileName());

    LOG_INFO(8, "NCMachine: NFile Saving file " << fi.absoluteFilePath().toStdString());

	QSaveFile::commit();

    HANDLE hFile = CreateFileA(fi.absoluteFilePath().toLocal8Bit().data(),
        GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING,
        NULL);

    if (hFile != INVALID_HANDLE_VALUE)
    {
        // Flush the file buffer
        bool b = FlushFileBuffers(hFile);

        // Close the file handle
        CloseHandle(hFile);
    }
    LOG_INFO(8, "NCMachine: NFile Save file done!");
}
