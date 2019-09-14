// -----------------------------------------------------------------------------
// Copyright (c) 2019 Jens Kallup - non-profit => kallup.jens@web.de
// all rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
// THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// -----------------------------------------------------------------------------
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QString>
#include <QWidget>
#include <QTreeView>
#include <QModelIndex>
#include <QMainWindow>
#include <QCloseEvent>
#include <QStringList>
#include <QTcpSocket>
#include <QListWidget>
#include <QListWidgetItem>

class QListWidgetItem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(class QWidget *parent = nullptr);
    virtual ~MainWindow();

    virtual void closeEvent(QCloseEvent * event);
public:
    Q_SLOT void about();
    Q_SLOT void onReadyRead();

private:
    Q_SLOT void on_actionConnect_triggered();
    Q_SLOT void on_userListWidget_mouseRightClicked(QListWidgetItem * item);

    Q_SLOT void on_lineEdit_textChanged(const QString &arg1);
    Q_SLOT void on_lineEdit_returnPressed(QString text);
    Q_SLOT void on_lineEdit_cursorUpPressed();
    Q_SLOT void on_lineEdit_cursorDownPressed();

private:
    Ui::MainWindow *ui;
    QTcpSocket _socket;
};

#endif // MAINWINDOW_H
