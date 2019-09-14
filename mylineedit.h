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
#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QObject>
#include <QString>
#include <QWidget>
#include <QLineEdit>
#include <QKeyEvent>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MyLineEdit(QWidget * parent = nullptr);
            ~MyLineEdit();

    void history_up();
    void history_down();
    void history_save();

    Q_SIGNAL void cursorDownPressed();
    Q_SIGNAL void cursorUpPressed();
    Q_SIGNAL void cursorLeftPressed();
    Q_SIGNAL void cursorRightPressed();

    Q_SIGNAL void returnPressed(QString text);
    Q_SIGNAL void escapePressed();

    Q_SIGNAL void lineExecuted(QString);

    int lineCount() const { return lines.size(); }
    QStringList history() const { return lines; }

    void setExecText(QString text);
    void setHistory(const QStringList& history);
    void setWordCompleter(QCompleter* completer);
    void setWordCompleterPrefix(const QString& prefix);
    void setWordCompleterMinChars(int min_chars);
    void setWordCompleterMaxSuggestions(int max);

    Q_SLOT   void autocomplete(const QString& completion);
    Q_SLOT   void execute();

protected:
    virtual void keyPressEvent(QKeyEvent   * event) Q_DECL_OVERRIDE;
    virtual void wheelEvent   (QWheelEvent * event) Q_DECL_OVERRIDE;

    void previous_line();
    void next_line();

    QString current_word() const;

private:
    int word_start() const;

    int         current_line;
    QStringList lines;
    QString     unfinished;

    QCompleter* completer;
    QString     completion_prefix;
    int         completion_minchars;
    int         completion_max;

    QString     exec_text;
};

#endif // MYLINEEDIT_H
