#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMainWindow::setWindowIcon(QIcon(":/rec/img/notes.ico"));
    QTabWidget* tabWidget = findChild<QTabWidget*>("tabWidget");  // Replace "tabWidget" with your actual object name
    QWidget* tab1 = tabWidget->widget(0);  // Assuming the first tab index is 0

    // Replace the existing textEdit widget with LineNumberTextEdit
    textEdit = new LineNumberTextEdit(tab1);
    textEdit->setObjectName("textEdit");  // Set an object name for further reference if needed
    textEdit->setGeometry(QRect(9, 9, 976, 437));  // Set the desired geometry
    //ui->setupUi(this);

    //connect(textEdit, &QTextEdit::keyPressed, this, &MainWindow::keyPressEvent);
    // Connect the radio button's toggled signal to a slot
    connect(ui->radioButton, &QRadioButton::toggled, this, &MainWindow::handleRadioButtonToggled);

    //connect(textEdit, &QTextEdit::textChanged, this, &MainWindow::keyPressEvent);
    //connect(textEdit, &QTextEdit::returnPressed, this, &MainWindow::onEnterKeyPressed);

    connect(textEdit, &QTextEdit::textChanged, this, &MainWindow::updateWordCount); //connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::updateWordCount);
    //ui->dial->setRange(50, 200);
    //this->setCentralWidget(ui->textEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionSave_triggered()
{
    QFile file(file_path_);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::information(this, "Error_11", "Can't Save File: " + file.errorString());
        return;
    }
    QTextStream output(&file);

    QString text_out = "";
    if (file_path_.endsWith(".html", Qt::CaseInsensitive))
        text_out += textEdit->toHtml(); //ui->textEdit->toHtml();
    else
        text_out += textEdit->toPlainText(); //ui->textEdit->toPlainText();

    output << text_out;
    file.flush();
    file.close();
}


void MainWindow::on_actionSave_as_triggered()
{
    QString filters = "Text File (*.txt) ;; HTML File (*.html)";
    QString file_name_ = QFileDialog::getSaveFileName(this, "Save file as", "C:/", filters);
    file_path_ = file_name_;
    QFile file(file_name_);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::information(this, "Error_13", "Can't Save File: " + file.errorString());
        return;
    }
    QString windowTitle = "NotePad++             |             ";
    windowTitle += file_name_;
    setWindowTitle(windowTitle);
    QTextStream output(&file);

    QString text_out = "";
    if (file_name_.endsWith(".html", Qt::CaseInsensitive))
        text_out += textEdit->toHtml(); //ui->textEdit->toHtml();
    else
        text_out += textEdit->toPlainText(); //ui->textEdit->toPlainText();

    output << text_out;
    file.flush();
    file.close();
}


void MainWindow::on_actionCut_triggered()
{
    textEdit->cut(); //ui->textEdit->cut();
}


void MainWindow::on_actionCopy_triggered()
{
    textEdit->copy(); //ui->textEdit->copy();
}



void MainWindow::on_actionPaste_triggered()
{
    textEdit->paste(); //ui->textEdit->paste();
}


void MainWindow::on_actionRedo_triggered()
{
    QTextCursor cursor = textEdit->textCursor(); //ui->textEdit->textCursor();
    if (!cursor.hasSelection() && cursor.position() == 0 && cursor.anchor() == 0 && (undo_flag--) < 1)
        redoDeleteAllText();
    else
        textEdit->redo(); //ui->textEdit->redo();
}


void MainWindow::on_actionUndo_triggered()
{
    undo_flag++;
    textEdit->undo(); //ui->textEdit->undo();
}


void MainWindow::on_actionNotePad_Pro_triggered()
{
    QString aboutStr = "NotePad++ ©\n";
    aboutStr += "Version 0.1\n";
    aboutStr += "Published 1st June, 2023\n\n";
    aboutStr += "NotePad++ is a lightweight and versatile text editor designed to provide a seamless editing experience. It offers a range of features to enhance productivity and facilitate efficient text manipulation.\n\n";
    aboutStr += "Key Features:\n\n";
    aboutStr += "- Syntax highlighting: Enjoy syntax highlighting for various programming languages, making your code easier to read and understand.\n";
    aboutStr += "- Multiple tabs: Work on multiple files simultaneously using the tabbed interface, allowing for better organization and quick switching between documents.\n";
    aboutStr += "- Find and replace: Easily search for specific text within your document and replace it with the desired content.\n";
    aboutStr += "- Customization options: Personalize your editing environment with customizable themes, font settings, and other preferences.\n";
    aboutStr += "- Line numbering: Keep track of line numbers in your code for better reference and debugging.\n\n";
    aboutStr += "We are committed to providing a reliable and user-friendly text editing solution that meets your needs. Your feedback is valuable to us as we continuously improve and enhance NotePad++.\n\n";
    aboutStr += "Thank you for choosing us!\n";

    QMessageBox::about(this, "About us", aboutStr);
}

void MainWindow::on_actionNew_triggered()
{
    file_path_ = "";
    setWindowTitle("NotePad++");
    deletedText = textEdit->toHtml();
    textEdit->selectAll();
    textEdit->setText("");
    QTextCharFormat format;
    textEdit->setCurrentCharFormat(format);
    ui->actionSubscript->setChecked(false);
    ui->actionSuperscript->setChecked(false);
    ui->actionBOLD->setChecked(false);
    ui->actionItalics->setChecked(false);

    // Reset the zoom level to the default size
    textEdit->document()->setDefaultFont(QFont());

    // Reset the zoom level of the line numbering text
    LineNumberTextEdit* lineNumberTextEdit = dynamic_cast<LineNumberTextEdit*>(textEdit);
    if (lineNumberTextEdit != nullptr) {
        lineNumberTextEdit->setLineNumbersEnabled(false); // Disable line numbering
        QFont lineNumberFont;
        //lineNumberTextEdit->setZoomFactor(1.0);
        lineNumberTextEdit->setFont(lineNumberFont);
        lineNumberTextEdit->counter = 1;
        // Update the toggle state in the GUI
        QRadioButton* radioButtonLineNumber = findChild<QRadioButton*>("radioButton");
        if (radioButtonLineNumber != nullptr) {
            radioButtonLineNumber->setChecked(false);
        }
    }
}





void MainWindow::on_actionOpen_triggered()
{
    QString filters = "All files (*.*) ;; Text files (*.txt) ;; HTML File (*.html)";
    QString file_name_ = QFileDialog::getOpenFileName(this, "Select a file", "C:/", filters);
    file_path_ = file_name_;
    QFile file(file_name_);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(this, "Error_12", "Couldn't Open File: " + file.errorString());
        return;
    }
    QString windowTitle = "NotePad++             |             ";
    windowTitle += file_name_;
    setWindowTitle(windowTitle);
    QTextStream input(&file);
    textEdit->selectAll();
    textEdit->setText("");
    ui->actionSubscript->setChecked(false);
    ui->actionSuperscript->setChecked(false);
    ui->actionBOLD->setChecked(false);
    ui->actionItalics->setChecked(false);

    if (file_name_.endsWith(".html", Qt::CaseInsensitive))
    {

        QString text_in = input.readAll();
        textEdit->insertHtml(text_in); //ui->textEdit->insertHtml(text_in);
        file.close();
    }
    else
    {
        QString text_in = input.readAll();
        textEdit->setText(text_in); //ui->textEdit->setText(text_in);
        file.close();
    }
}




void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer Name");
    QPrintDialog pDialog(&printer, this);
    if (pDialog.exec() == QDialog::Rejected)
    {
        QMessageBox::warning(this, "Warning_14", "Can't Print file");
        return;
    }
    textEdit->print(&printer); //ui->textEdit->print(&printer);
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    // User selected a font
    if (ok)
    {
        QTextCursor cursor(textEdit->textCursor()); //cursor(ui->textEdit->textCursor());

        // Selected text exists, so set the font for the selected text
        if (cursor.hasSelection())
        {
            QTextCharFormat format;
            format.setFont(font);
            cursor.mergeCharFormat(format);
            textEdit->mergeCurrentCharFormat(format); //ui->textEdit->mergeCurrentCharFormat(format);
        }
        else
        {
            QTextCharFormat format;
            format.setFont(font);
            format.setForeground(textEdit->currentCharFormat().foreground()); //format.setForeground(ui->textEdit->currentCharFormat().foreground()); // Preserve existing font color
            format.setBackground(textEdit->currentCharFormat().background()); //format.setBackground(ui->textEdit->currentCharFormat().background()); // Preserve existing background color
            textEdit->setCurrentCharFormat(format); //ui->textEdit->setCurrentCharFormat(format);
        }
    }
}

void MainWindow::on_actionFont_Color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this);

    QTextCursor cursor(textEdit->textCursor());  //cursor(ui->textEdit->textCursor());

    if (cursor.hasSelection())
    {
        QTextCharFormat format;
        format.setForeground(color);
        cursor.mergeCharFormat(format);
        textEdit->mergeCurrentCharFormat(format); //ui->textEdit->mergeCurrentCharFormat(format);
    }
    else
    {
        QTextCharFormat format;
        format.setForeground(color);
        format.setFont(textEdit->currentCharFormat().font()); //format.setFont(ui->textEdit->currentCharFormat().font()); // Preserve existing font style
        format.setBackground(textEdit->currentCharFormat().background()); //format.setBackground(ui->textEdit->currentCharFormat().background()); // Preserve existing background color
        textEdit->setCurrentCharFormat(format); //ui->textEdit->setCurrentCharFormat(format);
    }
}

void MainWindow::on_actionFont_Background_triggered()
{
    QColor color = QColorDialog::getColor(Qt::yellow, this);

    QTextCursor cursor(textEdit->textCursor()); //cursor(ui->textEdit->textCursor());

    if (cursor.hasSelection())
    {
        QTextCharFormat format;
        format.setBackground(color);
        cursor.mergeCharFormat(format);
        textEdit->mergeCurrentCharFormat(format); //ui->textEdit->mergeCurrentCharFormat(format);
    }
    else
    {
        QTextCharFormat format;
        format.setBackground(color);
        format.setFont(textEdit->currentCharFormat().font()); //format.setFont(ui->textEdit->currentCharFormat().font()); // Preserve existing font style
        format.setForeground(textEdit->currentCharFormat().foreground()); //format.setForeground(ui->textEdit->currentCharFormat().foreground()); // Preserve existing font color
        textEdit->setCurrentCharFormat(format); //ui->textEdit->setCurrentCharFormat(format);
    }
}

void MainWindow::on_actionSubscript_toggled(bool arg1)
{
    QTextCursor cursor(textEdit->textCursor()); //cursor(ui->textEdit->textCursor());
    QTextCharFormat format;

    if (arg1) {
        format.setVerticalAlignment(QTextCharFormat::AlignSubScript);
    } else {
        format.setVerticalAlignment(QTextCharFormat::AlignNormal);
    }

    if (cursor.hasSelection())
    {
        cursor.mergeCharFormat(format);
    }
    else
    {
        format.setFont(textEdit->currentCharFormat().font()); //format.setFont(ui->textEdit->currentCharFormat().font()); // Preserve existing font style
        format.setForeground(textEdit->currentCharFormat().foreground()); //format.setForeground(ui->textEdit->currentCharFormat().foreground()); // Preserve existing font color
        format.setBackground(textEdit->currentCharFormat().background()); //format.setBackground(ui->textEdit->currentCharFormat().background()); // Preserve existing background color
        textEdit->setCurrentCharFormat(format); //ui->textEdit->setCurrentCharFormat(format);
    }
}





void MainWindow::on_actionSuperscript_toggled(bool arg1)
{
    QTextCursor cursor(textEdit->textCursor()); //cursor(ui->textEdit->textCursor());
    QTextCharFormat format;

    if (arg1) {
        format.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
    } else {
        format.setVerticalAlignment(QTextCharFormat::AlignNormal);
    }

    if (cursor.hasSelection())
    {
        cursor.mergeCharFormat(format);
    }
    else
    {
        format.setFont(textEdit->currentCharFormat().font()); //format.setFont(ui->textEdit->currentCharFormat().font()); // Preserve existing font style
        format.setForeground(textEdit->currentCharFormat().foreground()); //format.setForeground(ui->textEdit->currentCharFormat().foreground()); // Preserve existing font color
        format.setBackground(textEdit->currentCharFormat().background()); //format.setBackground(ui->textEdit->currentCharFormat().background()); // Preserve existing background color
        textEdit->setCurrentCharFormat(format); //ui->textEdit->setCurrentCharFormat(format);
    }
}


void MainWindow::on_actionAlign_Text_Left_triggered()
{
    QTextCursor cursor(textEdit->textCursor()); //cursor(ui->textEdit->textCursor());
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignLeft);
    cursor.mergeBlockFormat(format);
}


void MainWindow::on_actionAlign_Text_Right_triggered()
{
    QTextCursor cursor(textEdit->textCursor()); //cursor(ui->textEdit->textCursor());
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignRight);
    cursor.mergeBlockFormat(format);
}


void MainWindow::on_actionCenter_triggered()
{
    QTextCursor cursor(textEdit->textCursor()); //cursor(ui->textEdit->textCursor());
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignCenter);
    cursor.mergeBlockFormat(format);
}


void MainWindow::on_actionJustify_triggered()
{
    QTextCursor cursor(textEdit->textCursor()); //cursor(ui->textEdit->textCursor());
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignJustify);
    cursor.mergeBlockFormat(format);
}



void MainWindow::on_actionClear_All_Formating_triggered()
{
    QTextCursor cursor(textEdit->textCursor()); //cursor(ui->textEdit->textCursor());
    ui->actionSubscript->setChecked(false);
    ui->actionSuperscript->setChecked(false);
    ui->actionBOLD->setChecked(false);
    ui->actionItalics->setChecked(false);

    if (cursor.hasSelection()) {
        // Remove formatting from selected text
        cursor.setCharFormat(QTextCharFormat());  // Reset character format
        cursor.mergeBlockFormat(QTextBlockFormat());  // Reset block format
    } else {
        // Remove formatting from future text
        QTextCharFormat format;
        textEdit->setCurrentCharFormat(format);
    }
}


void MainWindow::on_actionClear_All_Text_triggered()
{
    deletedText = textEdit->toHtml(); //ui->textEdit->toHtml();
    textEdit->selectAll(); //ui->textEdit->selectAll();
    textEdit->setText(""); //ui->textEdit->setText("");
    QTextCharFormat format;
    textEdit->setCurrentCharFormat(format); //ui->textEdit->setCurrentCharFormat(format);
    textEdit->counter = 1;
}

void MainWindow::redoDeleteAllText()
{
    textEdit->setHtml(deletedText); //ui->textEdit->setHtml(deletedText);
}


void MainWindow::on_dial_valueChanged(int value)
{
    // Calculate the zoom factor based on the dial value
    double zoomFactor = static_cast<double>(value) / 100.0;

    // Adjust the zoom factor based on the previous zoom level
    double previousZoomFactor = static_cast<double>(prevVal) / 100.0;
    zoomFactor /= previousZoomFactor;

    // Apply the zoom effect to the text editor
    if (value > prevVal)
        textEdit->zoomIn(zoomFactor); //ui->textEdit->zoomIn(zoomFactor);
    else if (value < prevVal)
        textEdit->zoomOut(1.0 / zoomFactor); //ui->textEdit->zoomOut(1.0 / zoomFactor);

    prevVal = value;
}

void MainWindow::updateWordCount()
{
    static QRegularExpression wordSeparator("\\W+");

    QString text = textEdit->toPlainText(); //ui->textEdit->toPlainText();

    // Split the text into words
    QStringList words = text.split(wordSeparator, Qt::SkipEmptyParts);

    // Get the word count
    int wordCount = words.count();

    // Update the QLCDNumber display
    ui->lcdNumber->display(wordCount);
}




void MainWindow::on_pushButton_clicked()
{
    // Clear any previous highlighting
    clearHighlighting();

    QString searchText = ui->lineEdit->text();
    QTextDocument* document = textEdit->document();

    QTextCursor highlightCursor(document);
    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(Qt::yellow);

    QRegularExpression regex(searchText, QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatchIterator matchIterator = regex.globalMatch(document->toPlainText());

    QTextCursor cursor(document);
    cursor.beginEditBlock();

    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        highlightCursor.setPosition(match.capturedStart());
        highlightCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, match.capturedLength());
        highlightCursor.mergeCharFormat(highlightFormat);

        // Store the highlighted position for later removal
        highlightedPositions.append(highlightCursor);
    }

    cursor.endEditBlock();
}


void MainWindow::on_pushButton_2_clicked()
{
    QTextCursor cursor(textEdit->document()); //cursor(ui->textEdit->document());
    cursor.select(QTextCursor::Document);
    cursor.setCharFormat(QTextCharFormat());  // Reset character format
    cursor.mergeBlockFormat(QTextBlockFormat());  // Reset block format
}


void MainWindow::on_actionImage_triggered()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg *.gif)");

    if (!imagePath.isEmpty()) {
        // Create a text cursor at the current cursor position
        QTextCursor cursor = textEdit->textCursor(); //ui->textEdit->textCursor();

        // Load the image using QImageReader
        QImageReader imageReader(imagePath);
        QImage image = imageReader.read();

        if (!image.isNull()) {
            // Insert a new block to place the image
            cursor.insertBlock();

            // Insert an empty QTextDocumentFragment
            QTextDocumentFragment fragment;

            // Insert an empty QTextDocument
            QTextDocument document;

            // Set the default font size to fit the image size
            QFont defaultFont = document.defaultFont();
            defaultFont.setPixelSize(image.height());
            document.setDefaultFont(defaultFont);

            // Create an HTML image tag with the image source and dimensions
            QString imageHtml = QString("<img src=\"%1\" width=\"%2\" height=\"%3\">")
                                    .arg(imagePath)
                                    .arg(image.width())
                                    .arg(image.height());

            // Set the HTML content of the QTextDocument
            document.setHtml(imageHtml);

            // Copy the content of the QTextDocument to the QTextDocumentFragment
            QTextCursor documentCursor(&document);
            documentCursor.select(QTextCursor::Document);
            fragment = documentCursor.selection();

            // Insert the QTextDocumentFragment at the cursor position
            cursor.insertFragment(fragment);
        } else {
            QMessageBox::warning(this, "Warning", "Failed to load the image.");
        }
    } else {
        QMessageBox::warning(this, "Warning", "No image selected.");
    }
}





//---------------------------------------------------

void MainWindow::clearHighlighting()
{
    QTextCursor cursor(textEdit->document());
    QTextCharFormat defaultFormat;
    defaultFormat.setBackground(Qt::white);

    // Remove the highlighting for each stored position
    foreach (const QTextCursor& highlightedPosition, highlightedPositions) {
        cursor.setPosition(highlightedPosition.selectionStart());
        cursor.setPosition(highlightedPosition.selectionEnd(), QTextCursor::KeepAnchor);
        cursor.mergeCharFormat(defaultFormat);
    }

    // Clear the stored positions
    highlightedPositions.clear();
}



void MainWindow::on_pushButton_3_clicked()
{
    // Clear the highlighting done by the "Search" button
    clearHighlighting();

    // Clear the text in the line edit
    ui->lineEdit->clear();
}


void MainWindow::handleRadioButtonToggled(bool checked)
{
    textEdit->counter = 1;
    textEdit->setLineNumbersEnabled(checked);
}


void MainWindow::on_actionSelect_All_triggered()
{
    textEdit->selectAll();
}

void MainWindow::on_actionBOLD_toggled(bool arg1)
{
    QTextCursor cursor(textEdit->textCursor());
    //QTextCharFormat format;
    if (cursor.hasSelection())
    {
        if (arg1)
        {
            QTextCharFormat format;
            format.setFontWeight(QFont::Bold);
            //format.setFont(textEdit->currentCharFormat().font());
            //format.setForeground(textEdit->currentCharFormat().foreground());
            //format.setBackground(textEdit->currentCharFormat().background());
            //textEdit->setCurrentCharFormat(format);
            cursor.mergeCharFormat(format);
            return;
        }
        else
        {
            QTextCharFormat format;
            QTextCharFormat format2 = textEdit->currentCharFormat();
            QString font_name = format2.font().styleName();
            format.setFontStyleName(font_name);
            format.setFontWeight(QFont::Normal);
            //format.setFont(textEdit->currentCharFormat().font());
            //format.setForeground(textEdit->currentCharFormat().foreground());
            //format.setBackground(textEdit->currentCharFormat().background());
            //textEdit->setCurrentCharFormat(format);
            cursor.mergeCharFormat(format);
            return;
        }
    }
    else
    {
        if (arg1)
        {
            QTextCharFormat format;
            format.setFont(textEdit->currentCharFormat().font());
            format.setFontWeight(QFont::Bold);
            format.setForeground(textEdit->currentCharFormat().foreground());
            format.setBackground(textEdit->currentCharFormat().background());
            textEdit->setCurrentCharFormat(format);
            return;
        }
        else
        {
            QTextCharFormat format;
            format.setFont(textEdit->currentCharFormat().font());
            format.setFontWeight(QFont::Normal);
            format.setForeground(textEdit->currentCharFormat().foreground());
            format.setBackground(textEdit->currentCharFormat().background());
            textEdit->setCurrentCharFormat(format);
            return;
        }
    }
}


void MainWindow::on_actionItalics_toggled(bool arg1)
{
    QTextCursor cursor(textEdit->textCursor());

    if (cursor.hasSelection())
    {
        if (arg1)
        {
            QTextCharFormat format;
            format.setFontItalic(true);
            cursor.mergeCharFormat(format);
            return;
        }
        else
        {
            QTextCharFormat format;
            QTextCharFormat format2 = textEdit->currentCharFormat();
            format.setFontStyleName(format2.font().styleName());
            format.setFontWeight(textEdit->currentCharFormat().fontWeight());
            format.setFontItalic(false);
            format.setForeground(format2.foreground());
            format.setBackground(format2.background());
            cursor.mergeCharFormat(format);
            return;
        }
    }
    else
    {
        if (arg1)
        {
            QTextCharFormat format;
            format.setFont(textEdit->currentCharFormat().font());
            format.setFontWeight(textEdit->currentCharFormat().fontWeight());
            format.setFontItalic(true);
            format.setForeground(textEdit->currentCharFormat().foreground());
            format.setBackground(textEdit->currentCharFormat().background());
            textEdit->setCurrentCharFormat(format);
            return;
        }
        else
        {
            QTextCharFormat format;
            format.setFont(textEdit->currentCharFormat().font());
            format.setFontWeight(textEdit->currentCharFormat().fontWeight());
            format.setFontItalic(false);
            format.setForeground(textEdit->currentCharFormat().foreground());
            format.setBackground(textEdit->currentCharFormat().background());
            textEdit->setCurrentCharFormat(format);
            return;
        }
    }
}


void MainWindow::on_actionUnderline_toggled(bool arg1)
{
    QTextCursor cursor(textEdit->textCursor());

    if (cursor.hasSelection())
    {
        if (arg1)
        {
            QTextCharFormat format;
            format.setFontUnderline(true);
            cursor.mergeCharFormat(format);
            return;
        }
        else
        {
            QTextCharFormat format;
            QTextCharFormat format2 = textEdit->currentCharFormat();
            format.setFontUnderline(false);
            format.setFontStyleName(format2.font().styleName());
            format.setFontWeight(textEdit->currentCharFormat().fontWeight());
            format.setFontItalic(format2.fontItalic());
            format.setForeground(format2.foreground());
            format.setBackground(format2.background());

            cursor.mergeCharFormat(format);
            return;
        }
    }
    else
    {
        if (arg1)
        {
            QTextCharFormat format;
            format.setFont(textEdit->currentCharFormat().font());
            format.setFontWeight(textEdit->currentCharFormat().fontWeight());
            format.setFontItalic(textEdit->currentCharFormat().fontItalic());
            format.setFontUnderline(true);
            format.setForeground(textEdit->currentCharFormat().foreground());
            format.setBackground(textEdit->currentCharFormat().background());
            textEdit->setCurrentCharFormat(format);
            return;
        }
        else
        {
            QTextCharFormat format;
            format.setFont(textEdit->currentCharFormat().font());
            format.setFontWeight(textEdit->currentCharFormat().fontWeight());
            format.setFontItalic(textEdit->currentCharFormat().fontItalic());
            format.setFontUnderline(false);
            format.setForeground(textEdit->currentCharFormat().foreground());
            format.setBackground(textEdit->currentCharFormat().background());
            textEdit->setCurrentCharFormat(format);
            return;
        }
    }
}

