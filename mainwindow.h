#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "layoutengine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_doneBtn_clicked();
    void on_fontBtn_clicked();

private:
    Ui::MainWindow *ui;
    QString m_fontLocation;
    LayoutEngine m_layoutEngine;
};

#endif // MAINWINDOW_H
