#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QTextEdit>
#include <QPainter>
#include <QScrollBar>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>

class LineNumberTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit LineNumberTextEdit(QWidget *parent = nullptr);
    //void setZoomFactor(float zoomFactor);
    void setLineNumbersEnabled(bool enabled);
    int counter = 1;


protected:
    //void keyPressEvent(QKeyEvent *event) override;
    int countLines();
    void keyPressEvent(QKeyEvent *event)
    {
        if (m_lineNumbersEnabled && (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter))
        {
            // Enter key was pressed
            // Perform your desired action here
            if (this->toPlainText() == "")
            {
                counter = 1;
            }
            QString line = QString("%1").arg(counter++, 5, 10, QChar(' '));         //QString::number(countLines());
            append(line + ". ");
            event->accept();
            //emit enterKeyPressed();  // Emit the custom signal
            return;
        }

        QTextEdit::keyPressEvent(event);
    }

private:
        bool m_lineNumbersEnabled;

};





#endif // LINEEDIT_H
