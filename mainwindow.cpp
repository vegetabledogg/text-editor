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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenuBar *menubar = menuBar();
    QMenu *file = menubar->addMenu("&File");
    QMenu *help = menubar->addMenu("&Help");

    QAction *openAction = new QAction("&Open", this);
    file->addAction(openAction);
    QAction *saveAction = new QAction("&Save", this);
    file->addAction(saveAction);

    QToolBar *toolBar = addToolBar("Toolbar");
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);

    textedit = new QTextEdit(this);
    setCentralWidget(textedit);

    connect(openAction, &QAction::triggered, this, &MainWindow::openfile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::savefile);
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
