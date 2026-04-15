#include "buttonedittablewidget.h"
#include <QApplication>
#include <QTimer>
#include <QHeaderView>
#include <QButtonGroup>
#include <Qdir>
#include <QMenu>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFocusEvent>
#include "widgets/qlineedit4axisvalue.h"
#include "widgets/spreadsheet.h"
#include "utils/nfile.h"

ButtonEditTableWidget::ButtonEditTableWidget(QWidget* parent)
	: QTableWidget(parent), m_enableAddNew(true), m_enableDelete(true), m_enableMultiSelection(true)
{
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    this->verticalHeader()->hide();
    //this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->horizontalHeader()->hide();
    //this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    buttonGroup = new QButtonGroup(this);

    this->setRowCount(1);

    //this->setSelectionBehavior(QAbstractItemView::SelectRows);
}

ButtonEditTableWidget::~ButtonEditTableWidget()
{}

void ButtonEditTableWidget::createForm(int rows, int columns, std::function<ILineEdit* (int, QWidget*)> function4CreateLineEdit)
{
    m_initialRowCount = rows;

    this->setColumnCount(columns + 1);
    // Add header labels 
    for (int col = 0; col <= columns; ++col) {
        QLabel* label = new QLabel(QString("%1").arg(col), this);
        label->setAlignment(Qt::AlignCenter);
        this->setCellWidget(0, col, label);
		inLabels.append(label);
    }

    m_function4CreateEdit = function4CreateLineEdit;
    for (int row = 1; row <= m_initialRowCount; ++row) {
        insertNewRow();
        //inButtons[row - 1]->setText(QString("%1").arg(row));
    }
}

//void ButtonEditTableWidget::enableAddNewRow(bool b)
//{
//    if (m_enableAddNew != b) {
//        m_enableAddNew = b;
//        if (b) {
//            this->addNewRow();
//            //int row = this->rowCount() - 1;
//            //inButtons[row - 1]->setText(QString("%1").arg(row));
//        }
//        else {
//            int row = this->rowCount() - 1;
//            this->removeRow(row);
//            inButtons.removeAt(row);
//            inEdits.removeAt(row);
//        }
//    }
//}
//
//void ButtonEditTableWidget::enableDeleteRow(bool b)
//{
//    if (m_enableDelete != b) {
//        m_enableDelete = b;
//
//        if (b) {
//            for (int row = 1; row < this->rowCount(); ++row) {
//                QPushButton* button = dynamic_cast<QPushButton*>(this->cellWidget(row, 0));
//                button->setContextMenuPolicy(Qt::CustomContextMenu);
//                connect(button, &QPushButton::customContextMenuRequested, [this, button](const QPoint& pos) {
//                    QMenu menu(this);
//                    QAction* deleteRowAction = new QAction(tr("SCX"), this);
//                    connect(deleteRowAction, &QAction::triggered, [this, button]() {
//                        int row = this->indexAt(button->pos()).row();
//                        if (row != -1) {
//                            this->removeRow(row);
//                            inButtons.removeAt(row - 1);
//                            inEdits.removeAt(row - 1);
//                        }
//                        });
//                    menu.addAction(deleteRowAction);
//                    menu.exec(button->mapToGlobal(pos));
//                    });
//            }
//        }
//        else {
//            for (int row = 0; row < this->rowCount(); ++row) {
//                QPushButton* button = dynamic_cast<QPushButton*>(this->cellWidget(row, 0));
//                button->setContextMenuPolicy(Qt::NoContextMenu);
//            }
//        }
//    }
//}
QJsonArray ButtonEditTableWidget::GetData(ButtonEditTableWidget* parent, bool asMap)
{
    QJsonArray data;
    for (int row = 0; row < parent->rowCount(); ++row) {
        bool hasData = false;

        if (!asMap) {
            QJsonArray rowData;
            
            if (row == 0) {
                for (int col = 0; col < parent->columnCount(); ++col) {
                    rowData.append(parent->inLabels[col]->text());
                }
                hasData = true;
            }
            else {
                for (int col = 0; col < parent->columnCount(); ++col) {
                    if (col == 0) {
                        //QPushButton* btn = qobject_cast<QPushButton*>(this->cellWidget(row, col));
                        QPushButton* btn = parent->inButtons[row - 1];
                        rowData.append(btn->isChecked() ? "True" : "False");
                        if (btn->isChecked()) {
                            hasData = true;
                        }
                    }
                    else {
                        //ILineEdit* edit = qobject_cast<ILineEdit*>(this->cellWidget(row, col));
                        ILineEdit* edit = parent->inEdits[row - 1][col - 1];
                        rowData.append(edit->text());
                        if (!edit->text().isEmpty()) {
                            hasData = true;
                        }
                    }
                }
            }
            if (true || hasData) {
                data.append(rowData);
            }
        }
        else {
            if (row == 0) {
                continue;
            }
            QJsonObject rowData;
            for (int col = 0; col < parent->columnCount(); ++col) {
                if (col == 0) {
                    //QPushButton* btn = qobject_cast<QPushButton*>(this->cellWidget(row, col));
                    QPushButton* btn = parent->inButtons[row - 1];
                    rowData.insert(parent->inLabels[col]->text(), (btn->isChecked() ? "True" : "False"));
                    if (btn->isChecked()) {
                        hasData = true;
                    }
                }
                else {
                    //ILineEdit* edit = qobject_cast<ILineEdit*>(this->cellWidget(row, col));
                    ILineEdit* edit = parent->inEdits[row - 1][col - 1];
                    rowData.insert(parent->inLabels[col]->text(), edit->text());
                    if (!edit->text().isEmpty()) {
                        hasData = true;
                    }
                }
            }
            if (true || hasData) {
                data.append(rowData);
            }
        }
    }

    return data;
}

bool ButtonEditTableWidget::serialize(const QString& filePathName)
{
    try {
        QJsonArray data = GetData(this);

        QJsonDocument doc(data);
        QString json = doc.toJson();

        NFile file(filePathName);
        file.open(QIODevice::WriteOnly);
        file.write(json.toUtf8());
        file.close();
    }
    catch (...) {
        return false;
    }
    return true;
}

bool ButtonEditTableWidget::deserialize(const QString& filePathName)
{
 //   for (QPushButton* btn : inButtons)
 //   {
	//	delete btn;
 //   }
 //   for (QList<ILineEdit*> edits : inEdits)
 //   {
 //       for (ILineEdit* e : edits)
 //       {
 //           delete e;
 //       }
 //   }
 //   this->inButtons.clear();
	//this->inEdits.clear();
 //   // header row
 //   this->setRowCount(1);

    try {
        if (QFile::exists(filePathName)) {
            QFile file(filePathName);
            file.open(QIODevice::ReadOnly);
            QString json = file.readAll();
            file.close();

            QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
            QJsonArray data = doc.array();

			QHash<int, int> map4Columns;
            for (int row = 0; row < data.size(); ++row) {
                if (row == 0) {
                    QJsonArray rowData = data[row].toArray();
                    for (int col = 0; col < rowData.size(); ++col) {
                        QString s = rowData[col].toString();
						for (int i = 0; i < inLabels.count(); ++i) {
							if (inLabels[i]->text() == s) {
								map4Columns[col] = i;
								break;
							}
						}
                    }
                    continue;
                }
                QJsonArray rowData = data[row].toArray();
                //if (rowData.size() != this->columnCount() && map4Columns.count() == 0)
                //    continue;

                for (int col = 0; col < rowData.size(); ++col) {
                    QString s = rowData[col].toString();
                    int newCol = col;
                    if (map4Columns.count() > 0) {
                        if (map4Columns.contains(col))
                            newCol = map4Columns[col];
                        else
                            continue;
                    }
                    if (newCol == 0) {
                        //QPushButton* btn = qobject_cast<QPushButton*>(this->cellWidget(row, col));
						QPushButton* btn = NULL;
                        if (row - 1 >= inButtons.count()) {
                            insertNewRow();
                        }
                        btn = inButtons[row - 1];
                        if ((s == "True" && !btn->isChecked()) || (s == "False" && btn->isChecked())) {
							btn->click();
						}
                    }
                    else {
                        //ILineEdit* edit = qobject_cast<ILineEdit*>(this->cellWidget(row, col));
						ILineEdit* edit = NULL;
                        if (row - 1 >= inEdits.count()) {
                            insertNewRow();
                            //inButtons[row]->setText(QString("%1").arg(row));
                            //edit = qobject_cast<ILineEdit*>(this->cellWidget(row, col));
                        }
                        if (newCol - 1 >= 0 && newCol - 1 < inEdits[row - 1].count()) {
                            edit = inEdits[row - 1][newCol - 1];
                            if (edit != NULL) {
                                edit->setText(s);
                            }
                            else {
                                throw std::runtime_error("inEdits widget is NULL");
                            }
						}
                        
                    }
                }
            }

            int n = this->rowCount();
            for (int row = n - 1; row >= std::max(m_initialRowCount + 1, data.size()); --row) {
                deleteRow(row);
            }
        }
        else {
            int n = this->rowCount();
            for (int row = n - 1; row >= std::max(m_initialRowCount + 1, 1); --row) {
                deleteRow(row);
            }
        }
        this->scrollToTop();
    }
    catch (...) {
        return false;
    }
    return true;
}


QString ButtonEditTableWidget::getValue(int row, int col) const
{
    if (row < 0 || row > inButtons.count() - 1) {
        return QString();
    }
    if (col == -1) {
        //QPushButton* edit = qobject_cast<QPushButton*>(this->cellWidget(row + 1, 0));
		QPushButton* edit = inButtons[row];
        return edit->isChecked() ? "True" : "False";
    }
    if (col >= inEdits[row].count()) {
        return QString();
	}
    //ILineEdit* edit = qobject_cast<ILineEdit*>(this->cellWidget(row + 1, col + 1));
	ILineEdit* edit = inEdits[row][col];
    return edit->text();
}

void ButtonEditTableWidget::setValue(int row, int col, const QString& value)
{
    if (row < 0 || row > inButtons.count() - 1) {
        return;
    }

    if (col == -1) {
        //QPushButton* edit = qobject_cast<QPushButton*>(this->cellWidget(row + 1, 0));
        QPushButton* edit = inButtons[row];
        if (value.toUpper() == "TRUE") {
			edit->setChecked(true);
        }
        else {
			edit->setChecked(false);
        }
        return;
    }
    if (col >= inEdits[row].count()) {
        return;
    }
    //ILineEdit* edit = qobject_cast<ILineEdit*>(this->cellWidget(row + 1, col + 1));
    ILineEdit* edit = inEdits[row][col];
    edit->setText(value);
}


int ButtonEditTableWidget::getRowOfEdit(int col, ILineEdit* edit)
{
    for (int i = 0; i < inEdits.count(); i++)
    {
        if (col - 1>= 0 && col -1 < inEdits[i].count()) {
            if (inEdits[i][col - 1] == edit) {
                return i + 1;
            }
		}
    }
    return -1;
}

ILineEdit* ButtonEditTableWidget::createEdit(int col, QWidget* parent)
{
    ILineEdit* ret = NULL;
    if (m_function4CreateEdit != NULL) {
        ret = m_function4CreateEdit(col, parent);
    }
    if (ret == NULL) {
        NLineEdit* inEdit = new QLineEdit4Keyboard(this);
        inEdit->setAlignment(Qt::AlignCenter);
        ret = inEdit;
	}
    //ret->setReadOnly(true);
    return ret;
}

void ButtonEditTableWidget::setRowSelection(int row, bool selected)
{
    if (row == -1) {
        for (int row = 1; row < this->rowCount(); ++row) {
            if (inButtons[row - 1]->isChecked() != selected) {
                inButtons[row - 1]->setChecked(selected);
            }
            for (int col = 1; col < this->columnCount(); ++col) {
                inEdits[row - 1][col - 1]->setIsEditing(selected);
            }
        }
        this->clearFocus();
        return;
    }

	if (row - 1 < 0 || row - 1 >= inEdits.count()) {
		return;
	}
    // isChecked() is auto set by ui
	//if (m_currentRow == row && inButtons[row - 1]->isChecked() == selected) {
	//	return;
	//}

    //if (m_currentRow >= 0 && m_currentRow < this->rowCount()) {
    //    for (int col = 1; col < this->columnCount(); ++col) {
    //        inEdits[m_currentRow - 1][col - 1]->setIsEditing(false);
    //    }
    //}

    inButtons[row - 1]->setChecked(selected);
	for (int col = 1; col < this->columnCount(); ++col) {
		inEdits[row - 1][col - 1]->setIsEditing(selected);
	}

    //m_currentRow = row;
	//emit currentRowChanged(row, selected);
}

void ButtonEditTableWidget::insertNewRow(int row) {
    // default to add row to last
    if (row == -1) {
        //row = this->currentRow();
        row = this->rowCount() - 1;
    }

    if (row < 0 || row > this->rowCount() - 1) {
        row = this->rowCount() - 1;
    }
    this->insertRow(row + 1);
    this->setCurrentCell(row + 1, 0);

    int lastRowIndex = 0;
    if (row > 0) {
        lastRowIndex = inButtons[row - 1]->text().toInt();
    }
    QPushButton* button = new QPushButton(QString("%1").arg(lastRowIndex + 1), this);
    inButtons.insert(row, button);
    //button->setEnabled(false);
    button->setCheckable(true);
    button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    if (!m_enableMultiSelection) {
        buttonGroup->addButton(button);
    }

    //if (m_enableDelete) {
    //    button->setContextMenuPolicy(Qt::CustomContextMenu);
    //    connect(button, &QPushButton::customContextMenuRequested, [this, button](const QPoint& pos) {
    //        QMenu menu(this);
    //        QAction* deleteRowAction = new QAction(tr("SC"), this);
    //        connect(deleteRowAction, &QAction::triggered, [this, button]() {
    //            int row = this->indexAt(button->pos()).row();
    //            if (row != -1) {
    //                this->removeRow(row);
    //                inButtons.removeAt(row - 1);
    //                inEdits.removeAt(row - 1);
    //            }
    //            });
    //        menu.addAction(deleteRowAction);

    //        QAction* copyAction = new QAction(tr("FZ"), this);
    //        connect(copyAction, &QAction::triggered, [this, button]() {
    //            int row = this->indexAt(button->pos()).row();
    //            if (row != -1) {
    //                m_copyInfos.clear();
    //                for (int i = 0; i < inEdits[row - 1].count(); ++i) {
    //                    m_copyInfos.append(inEdits[row - 1][i]->text());
    //                }
    //            }
    //            });
    //        menu.addAction(copyAction);

    //        QAction* pasteAction = new QAction(tr("ZT"), this);
    //        connect(pasteAction, &QAction::triggered, [this, button]() {
    //            int row = this->indexAt(button->pos()).row();
    //            if (row != -1) {
    //                if (m_copyInfos.count() == inEdits[row - 1].count()) {
    //                    for (int i = 0; i < inEdits[row - 1].count(); ++i) {
    //                        inEdits[row - 1][i]->setText(m_copyInfos[i]);
    //                    }
    //                }
    //            }
    //            });
    //        menu.addAction(pasteAction);

    //        QAction* insertAction = new QAction(tr("CR"), this);
    //        connect(insertAction, &QAction::triggered, [this, button]() {
    //            int row = this->indexAt(button->pos()).row();
    //            if (row != -1) {
    //                insertNewRow(row);
    //            }
    //            });
    //        menu.addAction(insertAction);

    //        menu.exec(button->mapToGlobal(pos));
    //        });
    //}

    connect(button, &QPushButton::clicked, [this, button](bool checked) {
        // 不在选中整个行，只是以button状态来控制操作
        int rowHere = this->indexAt(button->pos()).row();
		//this->setRowSelection(rowHere, checked);

        //inButtons[m_currentRow - 1]->setChecked(true);

        //for (int row = 1; row < this->rowCount(); ++row) {
        //    for (int col = 1; col < this->columnCount(); ++col) {
        //        //inEdits[rowHere - 1][col - 1]->setReadOnly(!inButtons[rowHere - 1]->isChecked());
        //        inEdits[row - 1][col - 1]->setStyle(inEdits[row - 1][col - 1]->style());
        //    }
        //}

        //inEdits[m_currentRow - 1][0]->setFocus(Qt::MouseFocusReason);
        //inEdits[m_currentRow - 1][0]->selectAll();

        //if (m_enableAddNew && this->m_currentRow == this->rowCount() - 1) {
        //    this->insertNewRow();
        //}

        emit rowButtonClicked(rowHere - 1, checked);
        });
    this->setCellWidget(row + 1, 0, button);

    inEdits.insert(row, QList<ILineEdit*>());
    for (int col = 1; col < this->columnCount(); ++col) {
        ILineEdit* inEdit = createEdit(col, this);
		//QLineEdit4Keyboard* p = dynamic_cast<QLineEdit4Keyboard*>(inEdit);
  //      if (p != NULL) {
  //          connect(p, &QLineEdit4Keyboard::focusChanged, [this, p](bool hasFocus) {
  //              int rowHere = this->indexAt(p->pos()).row();
  //              if (hasFocus && !p->isReadOnly()) {
  //                  //this->selectCurrentRow(rowHere, true);
  //                  //m_currentRow = rowHere;
  //              }
  //              });
  //      }
        inEdits[row].append(inEdit);
        
        QWidget* p2 = dynamic_cast<QWidget*>(inEdit);
        if (p2 != NULL) {
            p2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            this->setCellWidget(row + 1, col, p2);
        }
    }

    for (int i = row; i < inButtons.count(); ++i) {
        inButtons[i]->setText(QString("%1").arg(i + 1));
    }
	//m_currentRow = row + 1;
    emit rowInserted(row);
}

void ButtonEditTableWidget::deleteRow(int row)
{
    this->clearFocus();

 //   if (row == -1) {
 //       row = m_currentRow;
 //   }
 //   if (row < 0 || row > this->rowCount() - 1) {
	//	return;
	//}
    if (row == -1) {
        for (int rowI = rowCount() - 1; rowI >= 1; --rowI) {
            if (inButtons[rowI - 1]->isChecked()) {
                this->removeRow(rowI);
                inButtons.removeAt(rowI - 1);
                inEdits.removeAt(rowI - 1);
            }
        }
    }
    else {
        int rowI = row;
        this->removeRow(rowI);
        inButtons.removeAt(rowI - 1);
        inEdits.removeAt(rowI - 1);
    }

    for(int i=0; i<inButtons.count(); ++i) {
		inButtons[i]->setText(QString("%1").arg(i + 1));
	}
}

void ButtonEditTableWidget::copyRow(int row)
{
    //if (row == -1) {
    //    row = m_currentRow;
    //}
    //if (row < 0 || row > this->rowCount() - 1) {
    //    return;
    //}
    m_copyInfos.clear();
    if (row == -1) {
        for (int row = 1; row <= rowCount() - 1; ++row) {
            if (inButtons[row - 1]->isChecked()) {
                for (int i = 0; i < inEdits[row - 1].count(); ++i) {
                    m_copyInfos.append(inEdits[row - 1][i]->text());
                }
            }
        }
    }
    else {
        if (row - 1 < 0 || row - 1 >= inButtons.count()) {
            return;
        }
        //if (inButtons[row - 1]->isChecked()) {
            for (int i = 0; i < inEdits[row - 1].count(); ++i) {
                m_copyInfos.append(inEdits[row - 1][i]->text());
            }
        //}
    }
}

void ButtonEditTableWidget::pasteRow(int row)
{
    if (row == -1) {
		//row = m_currentRow;
        row = this->currentRow();
	}
    if (row - 1 < 0 || row - 1 >= inEdits.count()) {
        return;
    }
    
	int editsCount = inEdits[row - 1].count();
    if (m_copyInfos.count() % editsCount == 0) {
        int n = m_copyInfos.count() / editsCount;
        for (int j = 0; j < n; ++j) {
            if (j == 0) {
                for (int i = 0; i < editsCount; ++i) {
                    inEdits[row - 1][i]->setText(m_copyInfos[i]);
                }
            }
            else {
                insertNewRow(row + j - 1);
                for (int i = 0; i < editsCount; ++i) {
                    inEdits[row + j - 1][i]->setText(m_copyInfos[j * editsCount + i]);
                }
            }
        }
    }
}

void ButtonEditTableWidget::setReadOnlyOnRow(int rowHere, bool readonly)
{
    if (rowHere - 1 < 0 || rowHere - 1 >= inEdits.count()) {
        return;
    }
	inButtons[rowHere - 1]->setEnabled(!readonly);
    for (int col = 1; col < this->columnCount(); ++col) {
        inEdits[rowHere - 1][col - 1]->setReadOnly(readonly);
        inEdits[rowHere - 1][col - 1]->setStyle(inEdits[rowHere - 1][col - 1]->style());
    }
}
void ButtonEditTableWidget::setPropertyOnRow(int rowHere, const char* name, const QVariant& value)
{
    if (rowHere - 1 < 0 || rowHere - 1 >= inEdits.count()) {
        return;
    }
    
    for (int col = 1; col < this->columnCount(); ++col) {
		dynamic_cast<QObject*>(inEdits[rowHere - 1][col - 1])->setProperty(name, value);
        inEdits[rowHere - 1][col - 1]->setStyle(inEdits[rowHere - 1][col - 1]->style());
    }
}
void ButtonEditTableWidget::setReadOnlyOnCell(int row, int col, bool readonly)
{
    if (row - 1 < 0 || row - 1 >= inEdits.count()) {
        return;
    }
    if (col == 0) {
        inButtons[row - 1]->setEnabled(!readonly);
    }
    else {
        inEdits[row - 1][col - 1]->setReadOnly(readonly);
        inEdits[row - 1][col - 1]->setStyle(inEdits[row - 1][col - 1]->style());
    }
}

void ButtonEditTableWidget::ScrollToRow(int row)
{
    int scrollRow = row + 0;
    if (scrollRow >= this->rowCount()) {
        scrollRow = this->rowCount() - 1;
    }
    else if (scrollRow < 0) {
        scrollRow = 0;
	}
    if (this->isVisible()) {
        this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        QTimer::singleShot(0, this, [=]() {
            QModelIndex index = this->model()->index(scrollRow, 0);
            if (index.isValid()) {
                this->scrollTo(index, QAbstractItemView::PositionAtCenter);
            }
            });
    }
}

void ButtonEditTableWidget::setReadOnly(bool readonly)
{
    if (m_isReadOnly == readonly)
        return;
    m_isReadOnly = readonly;
    for (int row = 1; row < this->rowCount(); ++row) {
        setReadOnlyOnRow(row, readonly);
	}
}
