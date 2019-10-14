#include "Console.h"
#include <QKeyEvent>
#include <QTextLine>
#include <QTextCursor>

Console::Console(QWidget *parent) : QTextEdit(parent)
{
}

void Console::clean()
{
    this->clear();
}

void Console::write(QString msg)
{
    this->append(msg);
    QTextCursor cursor = this->textCursor();
    cursor.movePosition(QTextCursor::NextRow);
    this->setTextCursor(cursor);
    ensureCursorVisible();
}

void Console::keyPressEvent(QKeyEvent *event)
{
    QTextCursor cursor = this->textCursor();
    cursor.movePosition(QTextCursor::End);
    this->setTextCursor(cursor);
    ensureCursorVisible();

    if (event->key() == Qt::Key_Backspace){       
        cursor.deletePreviousChar();
        return;
    }
    if (event->key() == Qt::Key_Delete){
        cursor.deleteChar();
        return;
    }
    if (this->textCursor().hasSelection())
        return;
    if (event->key() == Qt::Key_Return) {
        cursor.select(QTextCursor::LineUnderCursor);
        QString lastLine = cursor.selectedText();
        newLineWritten(lastLine);
    }
    QTextEdit::keyPressEvent(event);
}
