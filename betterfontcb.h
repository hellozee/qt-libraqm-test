#ifndef BETTERFONTCB_H
#define BETTERFONTCB_H

#include <QComboBox>

class BetterFontCB : public QComboBox
{
    Q_OBJECT
public:
    BetterFontCB(QWidget *parent=nullptr);
    inline QString getFile(int index) { return m_fontFiles.at(index); }

private:
    QVector<QString> m_fontNames;
    QVector<QString> m_fontFiles;
};

#endif // BETTERFONTCB_H
