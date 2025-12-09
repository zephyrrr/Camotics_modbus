#include "filedialog.h"
#include <QBoxLayout>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QDir>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStorageInfo>
#include <QDateTime>
#include "BaseChildWindow.h"
#include "widgets/qnumericpad.h"
#include <windows.h>

class MyFilterProxyModelToExcludeUnderscoreFilename : public QSortFilterProxyModel {
public:
    MyFilterProxyModelToExcludeUnderscoreFilename(QObject* parent = nullptr) : QSortFilterProxyModel(parent)
    {}

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override
    {
        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

        QString fileName = sourceModel()->data(index, Qt::DisplayRole).toString();

        // Your filtering logic here
        return !fileName.contains("_s_"); // Example: Filter out files with underscores
    }

    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override
    {
        QFileSystemModel* model = dynamic_cast<QFileSystemModel*>(sourceModel());
        QFileInfo leftFileInfo(model->fileInfo(left));
        QFileInfo rightFileInfo(model->fileInfo(right));

        return leftFileInfo.created().toSecsSinceEpoch() > rightFileInfo.created().toSecsSinceEpoch();
    }
};

class MyFilterProxyModelToExcludeSpecialNames : public QSortFilterProxyModel {
public:
    MyFilterProxyModelToExcludeSpecialNames(QObject* parent = nullptr, const QStringList& excludeNames = QStringList()) : QSortFilterProxyModel(parent)
    {
        m_excludeNames = excludeNames;
    }
private:
    QStringList m_excludeNames;
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override
    {
        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

        QString fileName = sourceModel()->data(index, Qt::DisplayRole).toString();

        // Your filtering logic here
        return !m_excludeNames.contains(fileName);
    }

    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override
    {
        QFileSystemModel* model = dynamic_cast<QFileSystemModel*>(sourceModel());
        QFileInfo leftFileInfo(model->fileInfo(left));
        QFileInfo rightFileInfo(model->fileInfo(right));

        return leftFileInfo.created().toSecsSinceEpoch() > rightFileInfo.created().toSecsSinceEpoch();
    }
};

class MyFileSystemModel : public QFileSystemModel {
public:
    MyFileSystemModel(QObject* parent = nullptr) : QFileSystemModel(parent) {}

protected:
    QVariant data(const QModelIndex& index, int role) const override {
        if (index.isValid() && role == Qt::DisplayRole) {
            QFileInfo fileInfo(filePath(index));
            return fileInfo.baseName();
        }
        return QFileSystemModel::data(index, role);
    }
};

NFileDialog::NFileDialog(QWidget *parent, const QString& caption, const QString& directory, const QString& filter, const bool canInputNewName)
	: QDialog(parent), lineEdit(NULL), m_filePath(QString())
{
	m_keyboardWidget = new SoftKeyboardWidget(this, false);

	this->setWindowFlag(Qt::WindowContextHelpButtonHint, false);   
    setWindowTitle(caption);
    this->setFixedSize(QSize(300, 400));

	m_filters = filter.split(";;", Qt::SkipEmptyParts);
    QRegularExpression regex("\\((.*)\\)");
    for each (QString var in m_filters)
    {
        QRegularExpressionMatch match = regex.match(var);
        if (match.hasMatch()) {
            QString subStr = match.captured(1);
			m_filterExtentions.append(subStr);
        }
    }

    // Create UI components
    listView = new QListView;
    
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(listView);
    
    setLayout(layout);

    if (canInputNewName) {
        lineEdit = new QLineEdit;
        connect(lineEdit, &QLineEdit::textEdited, [this, directory] {
            if (m_filterExtentions.size() == 0) {
                m_filePath = directory + QDir::separator() + lineEdit->text();
            }
            else {
                m_filePath = directory + QDir::separator() + lineEdit->text() + m_filterExtentions[0].replace("*", "");
            }
            });
        layout->addWidget(lineEdit);

        m_keyboardWidget->addVirtualKeyboard(lineEdit);
    }

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Orientation::Horizontal, this);
    connect(buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &QDialog::accept);
    connect(buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &QDialog::reject);

    if (canInputNewName) {
        QPushButton* deleteButton = buttonBox->addButton(QStringLiteral("删除"), QDialogButtonBox::ApplyRole);
        connect(deleteButton, &QPushButton::clicked, [this] {
   //         auto s = this->findFirstRemovableDisk();
			//bool r = this->ejectUsbDrive(s[0].toLatin1());
   //         if (!r) {
   //             this->ejectUsbDrive(s[0].toLatin1(), true);
   //         }
   //         return;

            QModelIndex index = listView->currentIndex();
            if (index.isValid()) {
                QSortFilterProxyModel* model = dynamic_cast<QSortFilterProxyModel*>(listView->model());
                QString filePath = index.data(QFileSystemModel::FilePathRole).toString();
                QFile file(filePath);
                if (file.remove()) {
                    model->removeRow(index.row());

                    // Now, delete related files
                    QFileInfo fileInfo(filePath);
                    QString baseName = fileInfo.baseName(); // Get the basename (e.g., "image" from "image.jpg")
					baseName += "_s_";
                    QString directoryPath = fileInfo.dir().absolutePath(); // Get the directory of the file

                    QDir directory(directoryPath);

                    // Get a list of all files in the directory
                    // QDir::Files: Only list files, not directories
                    // QDir::NoDotAndDotDot: Exclude "." and ".." entries
                    QStringList filesInDir = directory.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

                    for (const QString& fileName : filesInDir) {
                        if (fileName.startsWith(baseName)) {
                            // Construct the full path of the related file
                            QString relatedFilePath = directory.absoluteFilePath(fileName);

                            QFile relatedFile(relatedFilePath);
                            if (relatedFile.remove()) {
                            }
                        }
                    }
                }
            }
            });
    }

    layout->addWidget(buttonBox);
    QNumericPad::setButtonDownModeWhenInTouch(buttonBox);

    // Create file model
    QFileSystemModel* model = new MyFileSystemModel();
    model->setRootPath(directory);
    //QFileSystemModel* model = new QFileSystemModel;
    //model->setRootPath(rootPath);

    QSortFilterProxyModel* proxyModel = NULL;

	if (filter == "dir") {
		model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        proxyModel = new MyFilterProxyModelToExcludeSpecialNames(this, QStringList() << "nc");
        proxyModel->setSourceModel(model);
	}
    else {
        proxyModel = new MyFilterProxyModelToExcludeUnderscoreFilename(this);
        proxyModel->setSourceModel(model);
        proxyModel->sort(0, Qt::AscendingOrder);

        model->setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        model->setNameFilters(m_filterExtentions);
    }
	
    model->setNameFilterDisables(false);

    listView->setModel(proxyModel);
    listView->setRootIndex(proxyModel->mapFromSource(model->index(directory)));
    

    // Connect signals and slots
    connect(listView, &QListView::clicked, this, &NFileDialog::fileSelected);
}

NFileDialog::~NFileDialog()
{}

void NFileDialog::fileSelected(const QModelIndex& index) {
	m_filePath = index.data(QFileSystemModel::FilePathRole).toString();
    if (lineEdit != NULL) {
        QFileInfo fileInfo(m_filePath);
        QString baseName = fileInfo.baseName();
        lineEdit->setText(baseName);
    }
}


QString NFileDialog::getOpenFileName(QWidget* parent,
    const QString& caption,
    const QString& dir,
    const QString& filter)
{
    NFileDialog dialog(parent, caption, dir, filter, false);
	if (dialog.exec() == QDialog::Accepted) {
		return dialog.getSelectedFilePath();
	}
    return "";
}

QString NFileDialog::getSaveFileName(QWidget* parent,
    const QString& caption,
    const QString& dir,
    const QString& filter)
{
    NFileDialog dialog(parent, caption, dir, filter, true);
    if (dialog.exec() == QDialog::Accepted) {
        return dialog.getSelectedFilePath();
    }
    return "";
}

bool isRemovable(const QStorageInfo& storage) {
    DWORD driveType = GetDriveTypeW(storage.rootPath().toStdWString().c_str());
    if (driveType == DRIVE_REMOVABLE)
        return true;
    return false;
}

QString NFileDialog::findFirstRemovableDisk()
{
    QList<QStorageInfo> storageList = QStorageInfo::mountedVolumes();
    foreach(const QStorageInfo & storage, storageList) {
        if (storage.isValid() && storage.isReady() && isRemovable(storage)) {
            //qDebug() << "第一个可移动磁盘名称：" << storage.name();
            //qDebug() << "第一个可移动磁盘路径：" << storage.rootPath();
            //qDebug() << "第一个可移动磁盘总容量：" << storage.bytesTotal() / 1024 / 1024 << "MB";
            //qDebug() << "第一个可移动磁盘可用容量：" << storage.bytesAvailable() / 1024 / 1024 << "MB";
            return storage.rootPath(); // 找到第一个可移动磁盘后返回
        }
    }
    foreach(const QStorageInfo& storage, storageList) {
        if (storage.isValid() && storage.isReady()) {
            QString s = storage.rootPath();
            if (s[0] >= 'F')
                return s;
        }
    }

    return "";
}

bool NFileDialog::ejectUsbDrive(char driveLetter, bool force)
{
    // Format the path for CreateFile. Example: \\.\E:
    wchar_t volumePath[] = L"\\\\.\\E:";
    volumePath[4] = static_cast<wchar_t>(driveLetter);

    // 1. Get a handle to the volume
    HANDLE hVolume = CreateFileW(
        volumePath,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );

    if (hVolume == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD dwBytesReturned = 0;
    BOOL bResult = FALSE;

    // 2. Lock the volume to prevent other applications from using it
    bResult = DeviceIoControl(
        hVolume,
        FSCTL_LOCK_VOLUME,
        NULL, 0,
        NULL, 0,
        &dwBytesReturned,
        NULL
    );

    if (!bResult && !force) {
        DWORD errorCode = GetLastError();

        // Now you can handle specific errors
        if (errorCode == ERROR_ACCESS_DENIED) {
            
        }
        else if (errorCode == ERROR_BUSY) {
        }
        else {
            // You can look up other error codes here:
            // https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes
        }
        CloseHandle(hVolume);
        return false;
    }

    // 3. Dismount the volume
    bResult = DeviceIoControl(
        hVolume,
        FSCTL_DISMOUNT_VOLUME,
        NULL, 0,
        NULL, 0,
        &dwBytesReturned,
        NULL
    );

    if (!bResult) {
        CloseHandle(hVolume);
        return false;
    }

    // 4. Send the eject command
    bResult = DeviceIoControl(
        hVolume,
        IOCTL_STORAGE_EJECT_MEDIA,
        NULL, 0,
        NULL, 0,
        &dwBytesReturned,
        NULL
    );

    if (!bResult) {
        CloseHandle(hVolume);
        return false;
    }

    // On success, close the handle
    CloseHandle(hVolume);
    return true;
}

QString NFileDialog::getExistingDirectory(QWidget* parent, const QString& caption, const QString& dir, QFileDialog::Options options)
{
    NFileDialog dialog(parent, caption, dir, "dir", true);
    if (dialog.exec() == QDialog::Accepted) {
        return dialog.getSelectedFilePath();
    }
    return "";
}
