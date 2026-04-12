#include "nfile.h"
#include <QFileInfo>
#include <QDir>
#include <windows.h>
#include <cbang/log/Logger.h>

void NFile::close()
{
    // 1. 如果文件未打开，直接返回
    if (!isOpen()) {
        return;
    }

    QFileInfo fi(this->fileName());
    LOG_INFO(8, "NCMachine: NFile Saving file " << fi.absoluteFilePath().toStdString());

    // ==============================================
    // 核心：在 commit() 之前，强制刷入物理磁盘
    // ==============================================
    HANDLE hFile = reinterpret_cast<HANDLE>(handle());
    if (hFile != INVALID_HANDLE_VALUE) {
        // 1. 刷新 Qt 缓冲区
        flush();

        // 2. 强制 Windows 把数据写入磁盘盘片（断电不丢）
        FlushFileBuffers(hFile);
    }

    // ==============================================
    // QSaveFile 原子提交（自动关闭文件）
    // ==============================================
    bool ok = commit();

    // 4. 【关键】fsync 目录，确保持久化重命名操作
    if (ok) {
        QString volumePath = fi.absolutePath();
        HANDLE hDir = CreateFileW(
            (LPCWSTR)QDir::toNativeSeparators(volumePath).utf16(),
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS,  // 打开目录必须用这个标志
            NULL
        );
        if (hDir != INVALID_HANDLE_VALUE) {
            FlushFileBuffers(hDir);
            CloseHandle(hDir);
        }
    }

    if (ok) {
        LOG_INFO(8, "NCMachine: NFile Save file done!");
    }
    else {
        LOG_ERROR("NCMachine: NFile commit failed! " << errorString().toStdString());
    }
}

// 清理指定目录下的 QSaveFile 残留临时文件
void NFile::cleanStaleTempFiles(const QString& dirPath)
{
    QDir dir(dirPath);
    // 只读取文件，不匹配任何后缀（避免误删）
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    for (const QFileInfo& fi : list) {
        QString name = fi.fileName();
        int lastDot = name.lastIndexOf('.');

        // 规则 1：必须有 . 且不是隐藏文件
        if (lastDot <= 0)
            continue;

        // 规则 2：后缀长度必须 = 6
        QString suffix = name.mid(lastDot + 1);
        if (suffix.length() != 6)
            continue;

        // 规则 3：后缀必须是【字母+数字】（QSaveFile 标准格式）
        bool isQtTempFile = true;
        for (int i = 0; i < 6; ++i) {
            if (!suffix[i].isLetterOrNumber()) {
                isQtTempFile = false;
                break;
            }
        }
        if (!isQtTempFile)
            continue;

        // 【新增】规则 4：必须有对应的原文件（可选，减少误删）
        QString baseName = name.left(lastDot);
        if (!QFile::exists(QDir(fi.absolutePath()).filePath(baseName)))
            continue;

        // 【新增】规则 5：尝试以独占方式打开，失败则跳过（文件正在使用）
        QFile file(fi.absoluteFilePath());
        if (!file.open(QIODevice::ReadWrite)) {
            continue;  // 文件被占用，跳过
        }
        file.close();

        // 删除
        QFile::remove(fi.absoluteFilePath());
    }
}
