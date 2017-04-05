#include "mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QtoolBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QPainter>
#include <QPdfWriter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenuBar *mainMenu = menuBar();

    QMenu *fileSubMenu = mainMenu->addMenu("&File");

    QAction *openAction = new QAction("&Open", this);
    fileSubMenu->addAction(openAction);
    QAction *saveAction = new QAction("&Save", this);
    fileSubMenu->addAction(saveAction);
    QAction *pdfAction = new QAction("&Export As PDF", this);
    fileSubMenu->addAction(pdfAction);

    QMenu *editSubMenu = new QMenu(this);
    QAction *editAction = new QAction("&Edit", this);
    editAction->setMenu(editSubMenu);
    mainMenu->addAction(editAction);

    QAction *copyAction = new QAction("&Copy", this);
    editSubMenu->addAction(copyAction);
    QAction *pasteAction = new QAction("&Paste", this);
    editSubMenu->addAction(pasteAction);
    QAction *cutAction = new QAction("&Cut", this);
    editSubMenu->addAction(cutAction);
    QAction *redoAction = new QAction("&Redo", this);
    editSubMenu->addAction(redoAction);
    QAction *undoAction = new QAction("&Undo", this);
    editSubMenu->addAction(undoAction);
    QAction *clearAction = new QAction("&Clear", this);
    editSubMenu->addAction(clearAction);

    QToolBar *toolBar = addToolBar("Toolbar");
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addAction(pdfAction);

    textedit = new QTextEdit(this);
    setCentralWidget(textedit);

    connect(openAction, &QAction::triggered, this, &MainWindow::openfile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::savefile);
    connect(pdfAction, &QAction::triggered, this, &MainWindow::transToPDF);

    connect(copyAction, SIGNAL(triggered()), textedit, SLOT(copy()));
    connect(pasteAction, SIGNAL(triggered()), textedit, SLOT(paste()));
    connect(cutAction, SIGNAL(triggered()), textedit, SLOT(cut()));
    connect(redoAction, SIGNAL(triggered()), textedit, SLOT(redo()));
    connect(undoAction, SIGNAL(triggered()), textedit, SLOT(undo()));
    connect(clearAction, SIGNAL(triggered()), textedit, SLOT(clear()));
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
