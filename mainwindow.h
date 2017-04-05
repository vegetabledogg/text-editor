#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCharFormat>

class QTextEdit;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QTextEdit *textedit;
    bool boldChecked;
    bool italicChecked;
    bool underlineChecked;

    void openfile();
    void savefile();
    void transToPDF();

    void transToBold();
    void transToItalic();
    void transToUnderline();

    void setTextColor();
    void setEditorColor();
    void setTextBackgroundColor();

    void setFont();

    void setAlignLeft();
    void setAlignRight();
    void setAlignCenter();
    void setAlignJustify();
};

#endif // MAINWINDOW_H
