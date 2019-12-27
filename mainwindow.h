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

    void recalliberate();

private slots:
    void on_colorBtn_clicked();

private slots:
    void on_rightAlign_toggled(bool checked);
    void on_centerAlign_toggled(bool checked);
    void on_leftAlign_toggled(bool checked);
    void on_textDirection_toggled(bool checked);
    void on_fontCB_activated(int index);
    void on_letterSpacing_valueChanged(qreal arg1);
    void on_wordSpacing_valueChanged(qreal arg1);
    void on_lineHeight_valueChanged(qreal arg1);
    void on_fontSize_valueChanged(int arg1);
    void on_plainTextEdit_textChanged();

private:
    Ui::MainWindow *ui;
    LayoutEngine m_layoutEngine;
    QList<QString> m_input;
    qreal m_lineHeight, m_wordSpacing;
    int m_alignment = 0;
    QColor m_textColor;
};

#endif // MAINWINDOW_H
