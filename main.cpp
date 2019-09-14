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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <csignal>
#include <iostream>

#include <gtk/gtk.h>    // since Qt delete all at std::exit ...

#include <QObject>
#include <QThread>

#include "mainwindow.h"

#include <QSettings>
#include <QLocale>
#include <QTranslator>

#include <QMessageBox>
#include <QApplication>

#include "controller.h"
#include "mysplash.h"

class Controller * app_controller = nullptr;
class QSettings  * app_settings   = nullptr;
int                app_result     = EXIT_SUCCESS;

// -------------------------------------------
// display nice exit/welcome back message ...
// -------------------------------------------
void exitFunction(void)
{
    if (app_settings  ) delete app_settings  ;
    if (app_controller) delete app_controller;
}

// --------------------------------------------------
// try, to handle all signals that can occur in the
// underlaying OS like Linux ...
// --------------------------------------------------
void signalHandler(int signum)
{
    QString _error;
    switch (signum) {
    case SIGABRT: { _error = QString("Abnormal termination of the progeam\nsuch as a call to abort."); } break;
    case SIGBUS:  { _error = QString("Indicates an access to an invalid address.");} break;
    case SIGPIPE: { _error = QString("Broken pipe (POSIX)");} break;
    case SIGSYS:  { _error = QString("Bad system call");} break;
    case SIGFPE:  { _error = QString("An erroneous arithmetic operation,\nsuch as a divide by zero or an operation resulting in overflow.");} break;
    case SIGILL:  { _error = QString("Detection of an illegal instruction.");} break;
    case SIGINT:  { _error = QString("Reciept of an interactive attention signal.");} break;
    case SIGXCPU: { _error = QString("CPU limit exceeded.");} break;
    case SIGXFSZ: { _error = QString("File size limit exceeded.");} break;
    case SIGSEGV: { _error = printf("An invalid access to storage.\n");std::exit(1);} break;
    case SIGTERM: { _error = QString("A termination request sent to the program");} break;
    default: {
        _error = QString("undefined/unknown SIGNAL recieved.");
    } break;
    }

    if (app_controller->getAppType() == ControllerType::APP_GUI) {
        QMessageBox::warning(nullptr,
        QObject::tr("Warning"),_error);
    }   else {
        std::cerr << "Error: " << _error.toStdString() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    try {
        // ---------------------------------
        // IMPORTANT: set plugin path's ...
        // ---------------------------------
        #ifdef Q_OS_LINUX
        setenv("LD_LIBRARY_PATH",".",1);
        setenv("QT_QPA_PLATFORM_PLUGIN_PATH","./plugins",1);
        #endif

        QApplication app(argc, argv);
        QTranslator translator;

        // splash screen
        MySplash * splash = new MySplash;
        splash->show();

        app_controller = new Controller;
        app_controller->setAppType(ControllerType::APP_GUI);

        std::atexit(exitFunction);

        // --------------------------------------
        // install signal handler's ...
        // --------------------------------------
        std::signal(SIGABRT, signalHandler); // 1
        std::signal(SIGFPE , signalHandler); // 2
        std::signal(SIGILL , signalHandler); // 3
        std::signal(SIGINT , signalHandler); // 4
        std::signal(SIGSEGV, signalHandler); // 5
        std::signal(SIGTERM, signalHandler); // 6

        //if (!app_settings)
        translator.load(":/data/hexchat_de.qm");
        app.installTranslator(&translator);

        if (getuid() != 0) {
            std::cerr << "errrrr\n";
            QMessageBox::information(nullptr,
            QObject::tr("Warning"),
            QObject::tr("Running IRC as root is stupid !"));
            //return EXIT_FAILURE;
        }

        MainWindow w;
        w.show();

        // if all loaded, we remove the splash screen
        delete splash;

        app_result = app.exec();
        app_result = EXIT_SUCCESS; std::exit(app_result);
    }
    catch (...) {
        char buffer[] = "A Error occur during runing.";

        if (app_controller->getAppType() == ControllerType::APP_GUI) {
            QMessageBox::warning(nullptr,
            QObject::tr("Warning"),
            QObject::tr(buffer));
        }   else {
            std::cerr << buffer << std::endl;
        }

        app_result = EXIT_FAILURE;

        std::exit(app_result);
    }   std::exit(app_result);
}
