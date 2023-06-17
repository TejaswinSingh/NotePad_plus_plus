#include <QTextEdit>
#include <QPainter>
#include <QScrollBar>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>
#include "lineEdit.h"
#include <QApplication>

LineNumberTextEdit::LineNumberTextEdit(QWidget *parent) : QTextEdit(parent)
{
    // Set any desired text edit properties here
    connect(this, &QTextEdit::textChanged, this, &LineNumberTextEdit::countLines);
    setLineNumbersEnabled(false); // Line numbering disabled by default
}

int LineNumberTextEdit::countLines()
{
    QString text = this->toPlainText();
    int lineCount = text.count('\n');

    // Check if the last line ends with a newline character
    if (!text.endsWith('\n'))
        lineCount += 1;

    return lineCount;
}


void LineNumberTextEdit::setLineNumbersEnabled(bool enabled)
{
    m_lineNumbersEnabled = enabled;
    viewport()->update(); // Redraw the text edit to reflect the changes
}

