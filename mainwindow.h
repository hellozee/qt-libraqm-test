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
    void on_letterSpacing_valueChanged(qreal arg1);
    void on_wordSpacing_valueChanged(qreal arg1);
    void on_lineHeight_valueChanged(qreal arg1);
    void on_fontSize_valueChanged(int arg1);
    void on_plainTextEdit_textChanged();
    void on_fontChooser_clicked();

private:
    Ui::MainWindow *ui;
    LayoutEngine m_layoutEngine;
    QList<QString> m_input;
    qreal m_lineHeight, m_wordSpacing;
};

#endif // MAINWINDOW_H
