#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QTimer>
#include "layoutengine.h"

class PaintWidget : public QWidget
{
    Q_OBJECT

private:
    enum Handle{
        TopLeft = 0,
        TopRight,
        BottomRight,
        BottomLeft
    };

public:
    explicit PaintWidget(QWidget *parent = nullptr);

public Q_SLOTS:
    void setFontSize(int size);
    void setTextColor(QColor col);
    void setFontFile(QString file);
    void setAlignment(int align);
    void setLineHeight(qreal height);
    void setLetterSpacing(qreal spacing);
    void setWordSpacing(qreal spacing);
    void setTextDirection(bool dir);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void drawHandles(QPainter &gc);
    void drawIbar(QPainter &gc, qreal distanceFromLeft);
    void recalliberate();

private Q_SLOTS:
    void blinkIBar();

private:
    QVector<PropertyHolder> m_props;
    qreal m_lineHeight;
    int m_align, m_blinkCounter = 0;
    QColor m_textColor;
    QPoint m_topLeft = QPoint(0,0), m_lastCursorPos;
    QRect m_boundingRect;
    bool m_moveIt = false, m_handleSelected = false, m_insertMode = false;
    QVector<QRect> m_handleRects;
    Handle m_handle;
    QTimer m_timer;
    QString m_totalInput;
    QStringList m_input;
    LayoutEngine m_layoutEngine;
};

#endif // PAINTWIDGET_H
