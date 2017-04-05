#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QtoolBar>
#include <QTextEdit>
#include <QIcon>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QString>
#include <QPainter>
#include <QPdfWriter>
#include <QFont>
#include <QFontDialog>
#include <QColor>
#include <QColorDialog>
#include <QTextCursor>
#include <QTextCharFormat>
#include <Qt>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), boldChecked(true),
      italicChecked(true), underlineChecked(true)
{
    setWindowTitle("熊宝宝的记事本v1.0");
    QMenuBar *mainMenu = menuBar();

    QAction *openAction = new QAction("&Open", this);
    QAction *saveAction = new QAction("&Save", this);
    QAction *pdfAction = new QAction("&Export As PDF", this);

    QAction *copyAction = new QAction("&Copy", this);
    QAction *pasteAction = new QAction("&Paste", this);
    QAction *cutAction = new QAction("&Cut", this);
    QAction *redoAction = new QAction("&Redo", this);
    QAction *undoAction = new QAction("&Undo", this);
    QAction *clearAction = new QAction("&Clear", this);

    QAction *boldAction = new QAction("&B", this);
    QAction *italicAction = new QAction("&I", this);
    QAction *underlineAction = new QAction("&U", this);

    QAction *setTextColorAction = new QAction("Text Color", this);
    QAction *setEditorColorAction = new QAction("Editor Color", this);
    QAction *setTextBackgroundColorAction = new QAction("Text Background Action", this);

    QAction *setFontAction = new QAction("Font", this);

    QAction *setAlignLeftAction = new QAction("Left", this);
    QAction *setAlignRightAction = new QAction("Right", this);
    QAction *setAlignCenterAction = new QAction("Center", this);
    QAction *setAlignJustifyAction = new QAction("Justify", this);

    //构建File子菜单
    QMenu *fileSubMenu = mainMenu->addMenu("&File");

    fileSubMenu->addAction(openAction);
    fileSubMenu->addAction(saveAction);
    fileSubMenu->addAction(pdfAction);

    //构建Edit子菜单
    QMenu *editSubMenu = new QMenu(this);
    QAction *editAction = new QAction("&Edit", this);
    editAction->setMenu(editSubMenu);
    mainMenu->addAction(editAction);

    editSubMenu->addAction(copyAction);
    editSubMenu->addAction(pasteAction);
    editSubMenu->addAction(cutAction);
    editSubMenu->addAction(redoAction);
    editSubMenu->addAction(undoAction);
    editSubMenu->addAction(clearAction);

    //构建Color子菜单，并在之后添加到工具栏
    QAction *colorAction = new QAction("Color", this);
    QMenu *colorSubMenu = new QMenu(this);
    colorAction->setMenu(colorSubMenu);

    colorSubMenu->addAction(setTextColorAction);
    colorSubMenu->addAction(setEditorColorAction);
    colorSubMenu->addAction(setTextBackgroundColorAction);

    //构建Align子菜单，并在之后添加到工具栏
    QAction *alignAction = new QAction("Align", this);
    QMenu *alignSubMenu = new QMenu(this);
    alignAction->setMenu(alignSubMenu);

    alignSubMenu->addAction(setAlignLeftAction);
    alignSubMenu->addAction(setAlignRightAction);
    alignSubMenu->addAction(setAlignCenterAction);
    alignSubMenu->addAction(setAlignJustifyAction);

    //构建工具栏
    QToolBar *toolBar = addToolBar("Toolbar");
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addAction(pdfAction);
    toolBar->addAction(boldAction);
    toolBar->addAction(italicAction);
    toolBar->addAction(underlineAction);
    toolBar->addAction(colorAction);
    toolBar->addAction(setFontAction);
    toolBar->addAction(alignAction);

    textedit = new QTextEdit(this);
    setCentralWidget(textedit);

    //连接信号槽
    connect(openAction, &QAction::triggered, this, &MainWindow::openfile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::savefile);
    connect(pdfAction, &QAction::triggered, this, &MainWindow::transToPDF);

    connect(copyAction, SIGNAL(triggered()), textedit, SLOT(copy()));
    connect(pasteAction, SIGNAL(triggered()), textedit, SLOT(paste()));
    connect(cutAction, SIGNAL(triggered()), textedit, SLOT(cut()));
    connect(redoAction, SIGNAL(triggered()), textedit, SLOT(redo()));
    connect(undoAction, SIGNAL(triggered()), textedit, SLOT(undo()));
    connect(clearAction, SIGNAL(triggered()), textedit, SLOT(clear()));

    connect(boldAction, &QAction::triggered, this, &MainWindow::transToBold);
    connect(italicAction, &QAction::triggered, this, &MainWindow::transToItalic);
    connect(underlineAction, &QAction::triggered, this, &MainWindow::transToUnderline);

    connect(setTextColorAction, &QAction::triggered, this, &MainWindow::setTextColor);
    connect(setEditorColorAction, &QAction::triggered, this, &MainWindow::setEditorColor);
    connect(setTextBackgroundColorAction, &QAction::triggered, this, &MainWindow::setTextBackgroundColor);

    connect(setFontAction, &QAction::triggered, this, &MainWindow::setFont);

    connect(setAlignLeftAction, &QAction::triggered, this, &MainWindow::setAlignLeft);
    connect(setAlignRightAction, &QAction::triggered, this, &MainWindow::setAlignRight);
    connect(setAlignCenterAction, &QAction::triggered, this, &MainWindow::setAlignCenter);
    connect(setAlignJustifyAction, &QAction::triggered, this, &MainWindow::setAlignJustify);
}

MainWindow::~MainWindow()
{

}

void MainWindow::openfile(){
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Text Files(*.txt);;ini File(*.ini)"));
    if(!path.isEmpty()){
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            return;
        }
        QTextStream in(&file);
        textedit->setText(in.readAll());
        file.close();
    }
}

void MainWindow::savefile(){
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), ".", tr("Text Files(*.txt);;ini File(*.ini)"));
    if(!path.isEmpty()){
        QFile file(path);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            return;
        }
        QTextStream out(&file);
        out << textedit->toPlainText();
        file.close();
    }
}

void MainWindow::transToPDF(){
    QString path = QFileDialog::getSaveFileName(this, tr("Export As PDF"), ".", tr("PDF Files(*.pdf)"));
    if(!path.isEmpty()){
        QFile file(path);
        if(!file.open(QIODevice::WriteOnly)){
            return;
        }
        QPdfWriter *pdfwriter = new QPdfWriter(&file);
        QPainter *pdfpainter = new QPainter(pdfwriter);
        pdfwriter->setPageSize(QPagedPaintDevice::A4);
        pdfpainter->drawText(QRect(800, 700, 7800, 12300), textedit->toPlainText());
        delete pdfpainter;
        delete pdfwriter;
        file.close();
    }
}

void MainWindow::transToBold(){
    if(boldChecked){
        textedit->setFontWeight(QFont::Bold);
    }
    else{
        textedit->setFontWeight(QFont::Normal);
    }
    boldChecked = !boldChecked;
}

void MainWindow::transToItalic(){
    textedit->setFontItalic(italicChecked);
    italicChecked = !italicChecked;
}

void MainWindow::transToUnderline(){
    textedit->setFontUnderline(underlineChecked);
    underlineChecked = !underlineChecked;
}

void MainWindow::setTextColor(){
    QColor color = QColorDialog::getColor(Qt::black, this, "Select Color", QColorDialog::DontUseNativeDialog);
    if(color.isValid()){
        textedit->setTextColor(color);
    }
}

void MainWindow::setEditorColor(){
    QColor color = QColorDialog::getColor(Qt::white, this, "Select Color", QColorDialog::DontUseNativeDialog);
    if(color.isValid()){
        QString tmp("QTextEdit {background-color: ");
        tmp += color.name();
        tmp += QString(";}");
        textedit->setStyleSheet(tmp);
        textedit->setTextBackgroundColor(color);
    }
}

void MainWindow::setTextBackgroundColor(){
    QColor color = QColorDialog::getColor(Qt::white, this, "Select Color", QColorDialog::DontUseNativeDialog);
    if(color.isValid()){
        textedit->setTextBackgroundColor(color);
    }
}

void MainWindow::setFont(){
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    if(ok){
        textedit->setCurrentFont(font);
    }
}

void MainWindow::setAlignLeft(){
    textedit->setAlignment(Qt::AlignLeft);
}

void MainWindow::setAlignRight(){
    textedit->setAlignment(Qt::AlignRight);
}

void MainWindow::setAlignCenter(){
    textedit->setAlignment(Qt::AlignCenter);
}

void MainWindow::setAlignJustify(){
    textedit->setAlignment(Qt::AlignJustify);
}
