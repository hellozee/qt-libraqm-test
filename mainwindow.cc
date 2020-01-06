#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QDebug>
#include "paintwidget.h"
#include "betterfontcb.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_layoutEngine.setFontSize(ui->fontSize->value());
    m_lineHeight = ui->lineHeight->value();
    m_layoutEngine.setLetterSpacing(ui->letterSpacing->value());
    m_layoutEngine.setWordSpacing(ui->wordSpacing->value());

    m_alignment = 0;
    ui->leftAlign->setChecked(true);

    auto fontWidget = qobject_cast<BetterFontCB*>(ui->fontCB);
    m_layoutEngine.setFontFace(fontWidget->getFile(fontWidget->currentIndex()));

    m_layoutEngine.setDirection(ui->textDirection->isChecked());

    m_textColor = Qt::black;
    ui->colorBtn->setStyleSheet("QPushButton{background:"+ m_textColor.name() +"}");

    auto paintWidget = qobject_cast<PaintWidget*>(ui->canvas);
    connect(paintWidget, &PaintWidget::textIsScaled, this, &MainWindow::on_textScaled);

    connect(paintWidget, &PaintWidget::textEntered, this, &MainWindow::on_textEntered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_textEntered(QString text)
{
    m_input = text.split("\n");
    recalliberate();
}

void MainWindow::on_fontSize_valueChanged(int arg1)
{
    m_layoutEngine.setFontSize(arg1);
    recalliberate();
}

void MainWindow::recalliberate()
{
    QVector<PropertyHolder>props;

    foreach(QString str, m_input) {
        m_layoutEngine.setText(str);
        auto p = m_layoutEngine.calculate();
        props.push_back(p);
    }

    if(props.isEmpty())
        return;

    auto widget = qobject_cast<PaintWidget*>(ui->canvas);
    widget->updateWidget(props, m_lineHeight, m_alignment, m_textColor);
}

void MainWindow::on_lineHeight_valueChanged(qreal arg1)
{
    m_lineHeight = arg1;
    recalliberate();
}

void MainWindow::on_wordSpacing_valueChanged(qreal arg1)
{
    m_layoutEngine.setWordSpacing(arg1);
    recalliberate();
}

void MainWindow::on_letterSpacing_valueChanged(qreal arg1)
{
    m_layoutEngine.setLetterSpacing(arg1);
    recalliberate();
}

void MainWindow::on_fontCB_activated(int index)
{
    auto widget = qobject_cast<BetterFontCB*>(ui->fontCB);
    auto fontFile = widget->getFile(index);
    m_layoutEngine.setFontFace(fontFile);
    recalliberate();
}

void MainWindow::on_textDirection_toggled(bool checked)
{
    m_layoutEngine.setDirection(checked);
    recalliberate();
}

void MainWindow::on_leftAlign_toggled(bool checked)
{
    if(checked){
        m_alignment = 0;
        recalliberate();
    }
}

void MainWindow::on_centerAlign_toggled(bool checked)
{
    if(checked){
        m_alignment = 1;
        recalliberate();
    }
}

void MainWindow::on_rightAlign_toggled(bool checked)
{
    if(checked){
        m_alignment = 2;
        recalliberate();
    }
}

void MainWindow::on_colorBtn_clicked()
{
    auto clr = new QColorDialog(this);
    clr->exec();
    m_textColor = clr->selectedColor();
    ui->colorBtn->setStyleSheet("QPushButton{background:"+ m_textColor.name() +"}");
    recalliberate();
}

void MainWindow::on_textScaled(qreal ratio)
{
    ui->fontSize->setValue(ratio * ui->fontSize->value());
}
