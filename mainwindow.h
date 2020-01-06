#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "paintwidget.h"

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
    void on_colorBtn_clicked();
    void on_rightAlign_toggled(bool checked);
    void on_centerAlign_toggled(bool checked);
    void on_leftAlign_toggled(bool checked);
    void on_textDirection_toggled(bool checked);
    void on_fontCB_activated(int index);
    void on_letterSpacing_valueChanged(qreal arg1);
    void on_wordSpacing_valueChanged(qreal arg1);
    void on_lineHeight_valueChanged(qreal arg1);
    void on_fontSize_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    PaintWidget *canvas;

};

#endif // MAINWINDOW_H
