#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QDebug>
#include "betterfontcb.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    canvas = qobject_cast<PaintWidget*>(ui->canvas);

    canvas->setFontSize(ui->fontSize->value());
    canvas->setLineHeight(ui->lineHeight->value());
    canvas->setLetterSpacing(ui->letterSpacing->value());
    canvas->setWordSpacing(ui->wordSpacing->value());

    canvas->setAlignment(0);
    ui->leftAlign->setChecked(true);

    auto fontWidget = qobject_cast<BetterFontCB*>(ui->fontCB);
    canvas->setFontFile(fontWidget->getFile(fontWidget->currentIndex()));
    canvas->setTextDirection(ui->textDirection->isChecked());

    QColor col = Qt::black;
    ui->colorBtn->setStyleSheet("QPushButton{background:"+ col.name() +"}");
    canvas->setTextColor(col);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fontSize_valueChanged(int arg1)
{
    canvas->setFontSize(arg1);
}

void MainWindow::on_lineHeight_valueChanged(qreal arg1)
{
    canvas->setLineHeight(arg1);
}

void MainWindow::on_wordSpacing_valueChanged(qreal arg1)
{
    canvas->setWordSpacing(arg1);
}

void MainWindow::on_letterSpacing_valueChanged(qreal arg1)
{
    canvas->setLetterSpacing(arg1);
}

void MainWindow::on_fontCB_activated(int index)
{
    auto widget = qobject_cast<BetterFontCB*>(ui->fontCB);
    auto fontFile = widget->getFile(index);
    canvas->setFontFile(fontFile);
}

void MainWindow::on_textDirection_toggled(bool checked)
{
    canvas->setTextDirection(checked);
}

void MainWindow::on_leftAlign_toggled(bool checked)
{
    if(checked){
        canvas->setAlignment(0);
    }
}

void MainWindow::on_centerAlign_toggled(bool checked)
{
    if(checked){
        canvas->setAlignment(1);
    }
}

void MainWindow::on_rightAlign_toggled(bool checked)
{
    if(checked){
        canvas->setAlignment(2);
    }
}

void MainWindow::on_colorBtn_clicked()
{
    auto clr = new QColorDialog(this);
    clr->exec();
    QColor col = clr->selectedColor();
    ui->colorBtn->setStyleSheet("QPushButton{background:"+ col.name() +"}");
    canvas->setTextColor(col);
}
