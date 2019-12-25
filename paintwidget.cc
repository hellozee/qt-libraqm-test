#include "paintwidget.h"
#include <QPainter>
#include <QPaintEvent>

PaintWidget::PaintWidget(QWidget *parent):
    QWidget(parent)
{ }

void PaintWidget::paintEvent(QPaintEvent *event)
{
    if(m_props.isEmpty()){
        QWidget::paintEvent(event);
        return;
    }

    QPainter gc(this);
    auto ascent = m_props[0].font.ascent();
    auto descent = m_props[0].font.descent();
    qreal displacement = ascent;
    for(int i=0; i<m_props.count(); i++){
        gc.drawGlyphRun(QPointF(0.5, displacement + 0.5), m_props[i].glyph);
        displacement += ascent + descent + m_lineHeight;
    }
}

void PaintWidget::updateWidget(QVector<PropertyHolder> props, qreal lineHeight)
{
    m_props = props;
    m_lineHeight = lineHeight;
    this->update();
}
