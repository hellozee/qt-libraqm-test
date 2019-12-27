#include "paintwidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

PaintWidget::PaintWidget(QWidget *parent):
    QWidget(parent)
{ }

void PaintWidget::paintEvent(QPaintEvent *event)
{
    QPainter gc(this);
    gc.fillRect(event->rect(), Qt::white);

    if(m_props.isEmpty()){
        QWidget::paintEvent(event);
        return;
    }

    gc.setPen(Qt::black);

    auto ascent = m_props[0].font.ascent();
    auto descent = m_props[0].font.descent();
    qreal displacement = ascent;

    for(int i=0; i<m_props.count(); i++){
        qreal alignment = 1;
        auto glyphWidth = m_props[i].glyph.boundingRect().width();
        auto canvasWidth = event->rect().width();

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

        gc.drawGlyphRun(QPointF(alignment, displacement + 1), m_props[i].glyph);
        displacement += ascent + descent + m_lineHeight;
    }
}

void PaintWidget::updateWidget(QVector<PropertyHolder> props, qreal lineHeight, int align)
{
    m_props = props;
    m_lineHeight = lineHeight;
    m_align = align;
    this->update();
}
