#include "paintwidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

PaintWidget::PaintWidget(QWidget *parent):
    QWidget(parent)
{
    m_handleRects.resize(4);
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
    QPainter gc(this);
    gc.fillRect(event->rect(), Qt::white);

    if(m_props.isEmpty()){
        QWidget::paintEvent(event);
        return;
    }

    gc.setPen(m_textColor);

    auto ascent = m_props[0].font.ascent();
    auto descent = m_props[0].font.descent();
    qreal displacement = ascent, maxWidth = 0;

    for(int i=0;i<m_props.count(); i++){
        auto glyphWidth = m_props[i].glyph.boundingRect().width();
        if(maxWidth < glyphWidth)
            maxWidth = glyphWidth;
    }

    for(int i=0; i<m_props.count(); i++){
        qreal alignment = 1;
        auto canvasWidth = maxWidth;
        auto glyphWidth = m_props[i].glyph.boundingRect().width();

        switch(m_align){
        case 0 : break;
        case 1 :{
            alignment = (canvasWidth - glyphWidth)/2;
            break;
        }
        case 2 :
            alignment = canvasWidth - glyphWidth - 1;
            break;
        default: break;
        }

        gc.drawGlyphRun(QPointF(alignment, displacement + 1) + m_topLeft, m_props[i].glyph);
        displacement += ascent + descent + m_lineHeight;
    }

    displacement = displacement - (ascent + m_lineHeight) + 1;

    m_boundingRect = QRect(QPoint(m_topLeft - QPoint(-1, 0)), QSize(maxWidth+2, displacement));

    drawHandles(gc);
}

void PaintWidget::updateWidget(QVector<PropertyHolder> props, qreal lineHeight, int align, QColor col)
{
    m_props = props;
    m_lineHeight = lineHeight;
    m_align = align;
    m_textColor = col;
    this->update();
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
        return;
    if(m_boundingRect.contains(event->pos())){
        m_lastCursorPos = event->pos() - m_topLeft;
        m_moveIt = true;
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_moveIt){
        m_topLeft = event->pos() - m_lastCursorPos;
        update();
    }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    m_moveIt = false;
}

void PaintWidget::drawHandles(QPainter &gc)
{
    gc.drawRect(m_boundingRect);

    QRect handleBox(QPoint(), QSize(5,5));

    handleBox.moveCenter(m_topLeft);
    gc.fillRect(handleBox, Qt::black);

    m_handleRects[0] = handleBox;

    handleBox.moveCenter(m_boundingRect.topRight());
    gc.fillRect(handleBox, Qt::black);

    m_handleRects[1] = handleBox;

    handleBox.moveCenter(m_boundingRect.bottomRight());
    gc.fillRect(handleBox, Qt::black);

    m_handleRects[2] = handleBox;

    handleBox.moveCenter(m_boundingRect.bottomLeft());
    gc.fillRect(handleBox, Qt::black);

    m_handleRects[3] = handleBox;
}
