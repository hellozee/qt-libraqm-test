#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include "layoutengine.h"

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr);
    void updateWidget(QVector<PropertyHolder> props, qreal lineHeight, int align, QColor col);
    void paintEvent(QPaintEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void drawHandles(QPainter &gc);

private:
    QVector<PropertyHolder> m_props;
    qreal m_lineHeight;
    int m_align;
    QColor m_textColor;
    QPoint m_topLeft = QPoint(0,0), m_lastCursorPos;
    QRect m_boundingRect;
    bool m_moveIt = false;
    QVector<QRect> m_handleRects;
};

#endif // PAINTWIDGET_H
