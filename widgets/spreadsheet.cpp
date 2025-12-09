/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "spreadsheet.h"
#include "spreadsheetdelegate.h"
#include "spreadsheetitem.h"
#include "utils/nfile.h"
#include <QJsonDocument>

#include <QtWidgets>
#include "BaseChildWindow.h"

SpreadSheet::SpreadSheet(int rows, int cols, QValidator* validator, QWidget *parent)
    : QTableWidget(rows, cols, parent)
{
    //QTableWidget::setObjectName("SpreadSheet");

    //this->setSizeAdjustPolicy(QTableWidget::AdjustToContents);
    //for (int c = 0; c < cols; ++c) {
    //    QString character(QChar('A' + c));
    //    this->setHorizontalHeaderItem(c, new QTableWidgetItem(character));
    //}

    this->setItemPrototype(this->item(rows - 1, cols - 1));

    SpreadSheetDelegate* d = new SpreadSheetDelegate();
    d->setValidator(validator);
    this->setItemDelegate(d);

    //updateColor(nullptr);
    //setupContents();

    //connect(this, &QTableWidget::cellClicked, [this](int row, int column) {
    //    auto w = BaseChildWindow::GetChildWindow(this);
    //    if (w) {
    //        w->ShowKeyboard(this);
    //    }
    //    });
}

void SpreadSheet::enableAddNew(bool b)
{
    if (m_enableAddNew != b) {
        m_enableAddNew = b;

        if (b) {
            this->addNewRow();
            connect(this->verticalHeader(), &QHeaderView::sectionClicked, this, &SpreadSheet::onHeaderViewClicked);
        }
        else {
            this->removeRow(this->rowCount() - 1);
            disconnect(this->verticalHeader(), &QHeaderView::sectionClicked, this, &SpreadSheet::onHeaderViewClicked);
        }
    }
}

bool SpreadSheet::exportToCsv(QTableWidget* table, QString fileName)
{
    NFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);

        auto model = table->model();
        for (int row = 0; row < model->rowCount(); ++row) {
            for (int col = 0; col < model->columnCount(); ++col) {
                QModelIndex index = model->index(row, col);
                QString s = model->data(index).toString();
                if (s.isEmpty())
                    stream << ",";
                else
                    stream << s << ",";
            }
            stream << "\n";
        }
        file.close();
        return true;
    }
    return false;
}

bool SpreadSheet::importFromCsv(QTableWidget* table, QString fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        auto model = table->model();

        QTextStream stream(&file);
        int row = 0;
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList values = line.split(",");
            
            for (int col = 0; col < values.count(); ++col) {
                QModelIndex index = model->index(row, col);
                model->setData(index, values[col]);
            }
            row++;
        }
        file.close();
        return true;
    }
    return false;
}

void SpreadSheet::onHeaderViewClicked(int logicalIndex)
{
    if (logicalIndex == this->rowCount() - 1) {
        addNewRow();
    }
}

void  SpreadSheet::addNewRow() {
    int rowCount = this->rowCount();
    this->insertRow(rowCount);

    // Optionally initialize new row items
    for (int col = 0; col < this->columnCount(); col++) {
        QTableWidgetItem* item = new QTableWidgetItem();
        this->setItem(rowCount, col, item);
    }
}

bool SpreadSheet::serialize(QString filePathName)
{
    return serialize(this, filePathName);
}

bool SpreadSheet::deserialize(QString filePathName)
{
    return deserialize(this, filePathName);
}

QHash<QString, QString> SpreadSheet::m_fileContents;

bool SpreadSheet::serialize(QTableWidget* table, QString filePathName)
{
    try {
        auto model = table->model();
        QJsonArray data;
        for (int row = 0; row < model->rowCount(); ++row) {
            QJsonArray rowData;

            for (int col = 0; col < model->columnCount(); ++col) {
                QModelIndex index = model->index(row, col);
                QJsonValue value = model->data(index).toString();
                rowData.append(value);
            }
            data.append(rowData);
        }

        QJsonDocument doc(data);
        QString json = doc.toJson();
        if (!m_fileContents.contains(filePathName) || json != m_fileContents[filePathName]) {
            m_fileContents[filePathName] = json;

            NFile file(filePathName);
            file.open(QIODevice::WriteOnly);
            file.write(json.toUtf8());
            file.close();
        }
    }
    catch (...) {
        return false;
    }
    return true;
}

bool SpreadSheet::deserialize(QTableWidget* table, QString filePathName)
{
    try {
        auto model = table->model();
        
        if (QFile::exists(filePathName)) {
            QFile file(filePathName);
            file.open(QIODevice::ReadOnly);
            QString json = file.readAll();
            file.close();
            m_fileContents[filePathName] = json;

            QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
            QJsonArray data = doc.array();

            for (int row = 0; row < data.size(); ++row) {
                QJsonArray rowData = data[row].toArray();

                for (int col = 0; col < rowData.size(); ++col) {
                    QModelIndex index = model->index(row, col);
                    model->setData(index, rowData[col].toString());
                }
            }
        }
    }
    catch (...) {
        return false;
    }
    return true;
}

//void SpreadSheet::updateColor(QTableWidgetItem *item)
//{
//    QPixmap pix(16, 16);
//    QColor col;
//    if (item)
//        col = item->background().color();
//    if (!col.isValid())
//        col = palette().base().color();
//
//    QPainter pt(&pix);
//    pt.fillRect(0, 0, 16, 16, col);
//
//    QColor lighter = col.lighter();
//    pt.setPen(lighter);
//    QPoint lightFrame[] = { QPoint(0, 15), QPoint(0, 0), QPoint(15, 0) };
//    pt.drawPolyline(lightFrame, 3);
//
//    pt.setPen(col.darker());
//    QPoint darkFrame[] = { QPoint(1, 15), QPoint(15, 15), QPoint(15, 1) };
//    pt.drawPolyline(darkFrame, 3);
//
//    pt.end();
//}

//void SpreadSheet::selectColor()
//{
//    QTableWidgetItem *item = this->currentItem();
//    QColor col = item ? item->background().color() : this->palette().base().color();
//    col = QColorDialog::getColor(col, this);
//    if (!col.isValid())
//        return;
//
//    const QList<QTableWidgetItem *> selected = this->selectedItems();
//    if (selected.isEmpty())
//        return;
//
//    for (QTableWidgetItem *i : selected) {
//        if (i)
//            i->setBackground(col);
//    }
//
//    updateColor(this->currentItem());
//}

//void SpreadSheet::selectFont()
//{
//    const QList<QTableWidgetItem *> selected = this->selectedItems();
//    if (selected.isEmpty())
//        return;
//
//    bool ok = false;
//    QFont fnt = QFontDialog::getFont(&ok, font(), this);
//
//    if (!ok)
//        return;
//    for (QTableWidgetItem *i : selected) {
//        if (i)
//            i->setFont(fnt);
//    }
//}

void SpreadSheet::clear()
{
    const QList<QTableWidgetItem *> selectedItems = this->selectedItems();
    for (QTableWidgetItem *i : selectedItems)
        i->setText(QString());
}

//void SpreadSheet::setupContents()
//{
//    auto table = this;
//
//    QBrush titleBackground(Qt::lightGray);
//    QFont titleFont = this->font();
//    titleFont.setBold(true);
//
//    // column 0
//    table->setItem(0, 0, new SpreadSheetItem("Item"));
//    table->item(0, 0)->setBackground(titleBackground);
//    table->item(0, 0)->setToolTip("This column shows the purchased item/service");
//    table->item(0, 0)->setFont(titleFont);
//
//    table->setItem(1, 0, new SpreadSheetItem("AirportBus"));
//    table->setItem(2, 0, new SpreadSheetItem("Flight (Munich)"));
//    table->setItem(3, 0, new SpreadSheetItem("Lunch"));
//    table->setItem(4, 0, new SpreadSheetItem("Flight (LA)"));
//    table->setItem(5, 0, new SpreadSheetItem("Taxi"));
//    table->setItem(6, 0, new SpreadSheetItem("Dinner"));
//    table->setItem(7, 0, new SpreadSheetItem("Hotel"));
//    table->setItem(8, 0, new SpreadSheetItem("Flight (Oslo)"));
//    table->setItem(9, 0, new SpreadSheetItem("Total:"));
//
//    table->item(9, 0)->setFont(titleFont);
//    table->item(9, 0)->setBackground(titleBackground);
//
//    // column 1
//    table->setItem(0, 1, new SpreadSheetItem("Date"));
//    table->item(0, 1)->setBackground(titleBackground);
//    table->item(0, 1)->setToolTip("This column shows the purchase date, double click to change");
//    table->item(0, 1)->setFont(titleFont);
//
//    table->setItem(1, 1, new SpreadSheetItem("15/6/2006"));
//    table->setItem(2, 1, new SpreadSheetItem("15/6/2006"));
//    table->setItem(3, 1, new SpreadSheetItem("15/6/2006"));
//    table->setItem(4, 1, new SpreadSheetItem("21/5/2006"));
//    table->setItem(5, 1, new SpreadSheetItem("16/6/2006"));
//    table->setItem(6, 1, new SpreadSheetItem("16/6/2006"));
//    table->setItem(7, 1, new SpreadSheetItem("16/6/2006"));
//    table->setItem(8, 1, new SpreadSheetItem("18/6/2006"));
//
//    table->setItem(9, 1, new SpreadSheetItem());
//    table->item(9, 1)->setBackground(titleBackground);
//
//    // column 2
//    table->setItem(0, 2, new SpreadSheetItem("Price"));
//    table->item(0, 2)->setBackground(titleBackground);
//    table->item(0, 2)->setToolTip("This column shows the price of the purchase");
//    table->item(0, 2)->setFont(titleFont);
//
//    table->setItem(1, 2, new SpreadSheetItem("150"));
//    table->setItem(2, 2, new SpreadSheetItem("2350"));
//    table->setItem(3, 2, new SpreadSheetItem("-14"));
//    table->setItem(4, 2, new SpreadSheetItem("980"));
//    table->setItem(5, 2, new SpreadSheetItem("5"));
//    table->setItem(6, 2, new SpreadSheetItem("120"));
//    table->setItem(7, 2, new SpreadSheetItem("300"));
//    table->setItem(8, 2, new SpreadSheetItem("1240"));
//
//    table->setItem(9, 2, new SpreadSheetItem());
//    table->item(9, 2)->setBackground(Qt::lightGray);
//
//    // column 3
//    table->setItem(0, 3, new SpreadSheetItem("Currency"));
//    table->item(0, 3)->setBackground(titleBackground);
//    table->item(0, 3)->setToolTip("This column shows the currency");
//    table->item(0, 3)->setFont(titleFont);
//
//    table->setItem(1, 3, new SpreadSheetItem("NOK"));
//    table->setItem(2, 3, new SpreadSheetItem("NOK"));
//    table->setItem(3, 3, new SpreadSheetItem("EUR"));
//    table->setItem(4, 3, new SpreadSheetItem("EUR"));
//    table->setItem(5, 3, new SpreadSheetItem("USD"));
//    table->setItem(6, 3, new SpreadSheetItem("USD"));
//    table->setItem(7, 3, new SpreadSheetItem("USD"));
//    table->setItem(8, 3, new SpreadSheetItem("USD"));
//
//    table->setItem(9, 3, new SpreadSheetItem());
//    table->item(9, 3)->setBackground(Qt::lightGray);
//
//    // column 4
//    table->setItem(0, 4, new SpreadSheetItem("Ex. Rate"));
//    table->item(0, 4)->setBackground(titleBackground);
//    table->item(0, 4)->setToolTip("This column shows the exchange rate to NOK");
//    table->item(0, 4)->setFont(titleFont);
//
//    table->setItem(1, 4, new SpreadSheetItem("1"));
//    table->setItem(2, 4, new SpreadSheetItem("1"));
//    table->setItem(3, 4, new SpreadSheetItem("8"));
//    table->setItem(4, 4, new SpreadSheetItem("8"));
//    table->setItem(5, 4, new SpreadSheetItem("7"));
//    table->setItem(6, 4, new SpreadSheetItem("7"));
//    table->setItem(7, 4, new SpreadSheetItem("7"));
//    table->setItem(8, 4, new SpreadSheetItem("7"));
//
//    table->setItem(9, 4, new SpreadSheetItem());
//    table->item(9, 4)->setBackground(titleBackground);
//
//    // column 5
//    table->setItem(0, 5, new SpreadSheetItem("NOK"));
//    table->item(0, 5)->setBackground(titleBackground);
//    table->item(0, 5)->setToolTip("This column shows the expenses in NOK");
//    table->item(0, 5)->setFont(titleFont);
//
//    table->setItem(1, 5, new SpreadSheetItem("* C2 E2"));
//    table->setItem(2, 5, new SpreadSheetItem("* C3 E3"));
//    table->setItem(3, 5, new SpreadSheetItem("* C4 E4"));
//    table->setItem(4, 5, new SpreadSheetItem("* C5 E5"));
//    table->setItem(5, 5, new SpreadSheetItem("* C6 E6"));
//    table->setItem(6, 5, new SpreadSheetItem("* C7 E7"));
//    table->setItem(7, 5, new SpreadSheetItem("* C8 E8"));
//    table->setItem(8, 5, new SpreadSheetItem("* C9 E9"));
//
//    table->setItem(9, 5, new SpreadSheetItem("sum F2 F9"));
//    table->item(9, 5)->setBackground(titleBackground);
//}

void SpreadSheet::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QModelIndex index = indexAt(event->pos());
        if (index.isValid()) {
            edit(index);
        }
    }
    QTableWidget::mousePressEvent(event);
}

//void decode_pos(const QString &pos, int *row, int *col)
//{
//    if (pos.isEmpty()) {
//        *col = -1;
//        *row = -1;
//    } else {
//        *col = pos.at(0).toLatin1() - 'A';
//        *row = pos.right(pos.size() - 1).toInt() - 1;
//    }
//}
//
//QString encode_pos(int row, int col)
//{
//    return QString(col + 'A') + QString::number(row + 1);
//}

