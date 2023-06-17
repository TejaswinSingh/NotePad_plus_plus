#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QPrintDialog>
#include <QPrinter>
#include <QFontDialog>
#include <QFont>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QColorDialog>
#include <QColor>
#include <QImage>
#include <QRegularExpression>
#include <QTextImageFormat>
#include <QPushButton>



#include <QTextBlock>
#include <QTextLayout>
#include <QPainter>
#include <QTextObjectInterface>
#include <QAbstractTextDocumentLayout>
#include <QTextDocumentFragment>
#include <QImageReader>
#include <QTextEdit>
#include <QMimeData>
#include <QGroupBox>
#include <QTabWidget>
#include "lineEdit.h"
#include <QDrag>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionRedo_triggered();

    void on_actionUndo_triggered();

    void on_actionNotePad_Pro_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionPrint_triggered();

    void on_actionExit_triggered();

    void on_actionFont_triggered();

    void on_actionFont_Color_triggered();

    void on_actionFont_Background_triggered();

    void on_actionSubscript_toggled(bool arg1);

    void on_actionSuperscript_toggled(bool arg1);

    void on_actionAlign_Text_Left_triggered();

    void on_actionAlign_Text_Right_triggered();

    void on_actionCenter_triggered();

    void on_actionJustify_triggered();

    void on_actionClear_All_Formating_triggered();

    void on_actionClear_All_Text_triggered();

    void redoDeleteAllText();

    void on_actionImage_triggered();


    void on_dial_valueChanged(int value);

    void updateWordCount();


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void clearHighlighting();

    void on_pushButton_3_clicked();

    //void on_radioButton_toggled(bool checked);
    void handleRadioButtonToggled(bool checked);
    //void onTextChanged();
    //void keyPressEvent(QKeyEvent* event);

    void on_actionSelect_All_triggered();

    void on_actionBOLD_toggled(bool arg1);

    void on_actionItalics_toggled(bool arg1);

    void on_actionUnderline_toggled(bool arg1);

private:
    Ui::MainWindow *ui;
    QString file_path_;
    QString deletedText;
    int undo_flag = 0;
    int prevVal = 50;
    QList<QTextCursor> highlightedPositions;
    //TextEdit* textEdit;
    LineNumberTextEdit *textEdit;
    int previousLineCount;
};


#endif // MAINWINDOW_H
