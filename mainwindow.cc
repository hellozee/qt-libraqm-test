#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include "paintwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_layoutEngine.setFontSize(ui->fontSize->value());
    m_lineHeight = ui->lineHeight->value();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fontChooser_clicked()
{
    QString fontLocation = QFileDialog::getOpenFileName(
                    this, ("Choose Font"),
                    QStandardPaths::standardLocations(QStandardPaths::FontsLocation)[1],
                    ("Fonts (*.ttf *.otf)"));

    if(!fontLocation.isEmpty())
            ui->fontChooser->setText(QFileInfo(fontLocation).fileName());

    m_layoutEngine.setFontFace(fontLocation);
    recalliberate();
}

void MainWindow::on_plainTextEdit_textChanged()
{
    auto str = ui->plainTextEdit->toPlainText();
    if(str.isEmpty())
        return;
    m_input = str.split("\n");
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

    for(int i=0; i<m_input.count(); i++) {
        m_layoutEngine.setText(m_input.at(i));
        auto p = m_layoutEngine.calculate();
        props.push_back(p);
    }

    if(props.isEmpty())
        return;

    auto widget = qobject_cast<PaintWidget*>(ui->canvas);
    widget->updateWidget(props, m_lineHeight);
}

void MainWindow::on_lineHeight_valueChanged(int arg1)
{
    m_lineHeight = arg1;
    recalliberate();
}
