#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include "layoutengine.h"

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr);
    void updateWidget(QVector<PropertyHolder> props, qreal lineHeight);
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<PropertyHolder> m_props;
    qreal m_lineHeight;
};

#endif // PAINTWIDGET_H
