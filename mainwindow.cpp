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
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QCloseEvent>
#include <QString>
#include <QChar>
#include <QHostAddress>

#include <QDebug>

extern int app_result;
QChar      app_server_command_char = '/';

struct server_commands {
    int id;
    QString cmd;
} server_command[] = {
    {1, "topic" },
    {2, "join"  }
};

MainWindow::MainWindow(class QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _socket(0)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText("Enter text here");

    QTreeWidgetItem * w1 = ui->channelList->topLevelItem(0);
    QTreeWidgetItem * w2 = ui->channelList->topLevelItem(1);

    w1->treeWidget()->expandAll();
    w2->treeWidget()->expandAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    QMessageBox::information(nullptr,
    QObject::tr("Information"),
    QObject::tr("Thanks for using !!!\nPlease thinking over a Donation."));

    std::exit(app_result);
}

void MainWindow::about()
{
    QMessageBox::aboutQt(this,"About");
}

void MainWindow::on_actionConnect_triggered()
{
    _socket.connectToHost(QString("irc.euirc.net"),6667);
    QObject::connect(&_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void MainWindow::onReadyRead()
{
    QByteArray  data = _socket.readAll();
    qDebug() << data;
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if (arg1.size() < 1) {
        QPixmap pix(":/data/speaking.svg");
        pix   = pix.scaled(32,32);
        ui->label->setPixmap(pix);
    }   else {
        QPixmap pix(":/data/speaking.png");
        pix   = pix.scaled(32,32);
        ui->label->setPixmap(pix);
    }
}

void MainWindow::on_lineEdit_returnPressed(QString text)
{
    QString tmp, pmt, str;

    str = text.simplified();
    tmp = text;
    pmt = text;

    if (tmp.size() > 255)
    {
        tmp.clear();
        str.clear();
        pmt.clear();

        QMessageBox::warning(nullptr,
        QObject::tr("Warning"),
        QObject::tr("chat string too long."));

        ui->lineEdit->text().clear();
        return;
    }

    QStringList str_list = tmp.split(QChar(32));
    if (str_list.size() > 0) {
        tmp = str_list[0];
        if (tmp.startsWith("/topic")) {
            if (str_list.size() > 1) {
                str.clear();
                for (int i = 1; i < str_list.size(); i++) {
                    str.append(str_list[i]);
                    str.append(QChar(32));
                }
                ui->lineEdit->setText("");
                ui->topicLineEdit->setText(str);
            }
        }
    }
}

void MainWindow::on_lineEdit_cursorUpPressed()
{
    qDebug() << "up press";
}

void MainWindow::on_lineEdit_cursorDownPressed()
{
    qDebug() << "down press";
}

void MainWindow::on_userListWidget_mouseRightClicked(QListWidgetItem * item)
{
    QMessageBox::information(nullptr,
    "information",item->text());
}
