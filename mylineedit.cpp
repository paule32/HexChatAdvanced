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
#include "mylineedit.h"

#include <QKeyEvent>
#include <QWheelEvent>

#include <QCompleter>
#include <QAbstractItemView>
#include <QScrollBar>

#include <QDebug>

MyLineEdit::MyLineEdit(QWidget * parent)
    : QLineEdit(parent)
    , current_line(0)
    , completer(0)
    , completion_minchars(1)
    , completion_max(0)
{
    completer = new QCompleter(this);
}

MyLineEdit::~MyLineEdit()
{
    delete completer;
}
void MyLineEdit::keyPressEvent(QKeyEvent * event)
{
    int key  = event->key();

    if (key == Qt::Key_Down  ) { history_down(); emit cursorDownPressed (); } else
    if (key == Qt::Key_Up    ) { history_up  (); emit cursorUpPressed   (); } else
    if (key == Qt::Key_Left  ) {                 emit cursorLeftPressed (); } else
    if (key == Qt::Key_Right ) {                 emit cursorRightPressed(); } else
    if (key == Qt::Key_Escape) { setText("");    emit escapePressed     (); } else

    if (completer
    &&  completer->popup()
    &&  completer->popup()->isVisible()) {
        switch (key) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            execute();
            completer->popup()->hide();
            emit returnPressed(exec_text);
            exec_text.clear();
            break;

            case Qt::Key_F4:
            case Qt::Key_Select:
            completer->popup()->hide();
            break;
        }
    }   else {
        switch (key) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            execute();
            emit returnPressed(exec_text);
            exec_text.clear();
            break;
        }
    }

    QLineEdit::keyPressEvent(event);

    if (completer)
    {
        QString current = current_word();
        completer->setCompletionPrefix(current);

        if (current.size() < completion_minchars
        || (completer->completionCount() == 0)
        || (completion_max > 0 && completer->completionCount() > completion_max)) {
            completer->popup()->hide();
        }   else {
            // Get the selection status
            int sel = selectionStart();
            int sellength = selectedText().size();

            // Get the current cursor position
            int c = cursorPosition();

            // Get the start of the current word
            setCursorPosition(word_start());

            // Get the cursor rectangle at the beginning of the current word
            QRect rect = cursorRect();

            // Restore cursor position (clears the selection)
            setCursorPosition(c);

            // If we had a selection
            if ( sel != -1 ) {
                // If the selection started at the cursor,
                // it needs to start at the far end and go back
                // (otherwise it moves the cursor at the end)
                if (sel == c)
                setSelection(sel+sellength, -sellength); else
                setSelection(sel, sellength);
            }
            // Set the rectangle to the appropriate width
            rect.setWidth(
                 completer->popup()->sizeHintForColumn(0)
               + completer->popup()->verticalScrollBar()->sizeHint().width()
            );

            // Display the completer under the rectangle
            completer->complete(rect);
        }
    }
}

void MyLineEdit::history_down()
{
    if (history().size() >= 0) {
        next_line();
        return;
    }
}

void MyLineEdit::history_up()
{
    if (history().size() >= 0) {
        previous_line();
        return;
    }
}


void MyLineEdit::wheelEvent(QWheelEvent *ev)
{
    if ( ev->delta() > 0 )
        previous_line();
    else
        next_line();
}

void MyLineEdit::previous_line()
{
    if ( lines.empty() )
        return;

    if ( !text().isEmpty() &&
         ( current_line >= lines.size() || text() != lines[current_line] ) )
        unfinished = text();

    if ( current_line > 0 )
        current_line--;

    setText(lines[current_line]);
}


void MyLineEdit::next_line()
{
    if ( lines.empty() )
        return;

    current_line++;

    if ( current_line >= lines.size() )
    {
        setText(unfinished);
        unfinished = "";
        current_line = lines.size();
    }
    else
        setText(lines[current_line]);
}

void MyLineEdit::setExecText(QString text) { exec_text = text; }
void MyLineEdit::execute()
{
    // save string line ...
    setExecText(text());

    if ( lines.empty() || lines.back() != text() )
        lines << text();
    current_line = lines.size();
    clear();
    emit lineExecuted(lines.back());
}

void MyLineEdit::setHistory(const QStringList& history)
{
    lines = history;
    current_line = lines.size();
}

int MyLineEdit::word_start() const
{
    // lastIndexOf returns the index of the last space or -1 if there are no spaces
    // so that + 1 returns the index of the character starting the word or 0
    int after_space = text().leftRef(cursorPosition()).lastIndexOf(' ') + 1;
    if ( text().rightRef(text().size()-after_space).startsWith(completion_prefix) )
        after_space += completion_prefix.size();
    return after_space;
}

QString MyLineEdit::current_word() const
{
    int completion_index = word_start();
    return text().mid(completion_index, cursorPosition() - completion_index);
}

void MyLineEdit::autocomplete(const QString& completion)
{
    int completion_index = word_start();
    setText(text().replace(
        completion_index, cursorPosition() - completion_index,
        completion
    ));
    setCursorPosition(completion_index+completion.size());
}

void MyLineEdit::setWordCompleter(QCompleter* comp)
{
    if ( completer )
    {
        disconnect(completer, 0, this, 0);
        completer->setWidget(0);
    }

    completer = comp;

    if ( comp )
    {
        /// \todo should set these only when on focus
        connect(completer, SIGNAL(activated(QString)),
                this,      SLOT(autocomplete(QString)));
        connect(completer, SIGNAL(highlighted(QString)),
                this,      SLOT(autocomplete(QString)));
        completer->setWidget(this);
    }
}

void MyLineEdit::setWordCompleterPrefix(const QString& prefix)
{
    completion_prefix = prefix;
}

void MyLineEdit::setWordCompleterMinChars(int min_chars)
{
    completion_minchars = min_chars;
}

void MyLineEdit::setWordCompleterMaxSuggestions(int max)
{
    completion_max = max;
}
