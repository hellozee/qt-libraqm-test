#include "paintwidget.h"
#include <QPainter>

PaintWidget::PaintWidget(QWidget *parent):
    QWidget(parent), m_propertiesSet(false)
{ }

void PaintWidget::paintEvent(QPaintEvent *e)
{
    if(!m_propertiesSet){
        QWidget::paintEvent(e);
        return;
    }

    QPainter painter(this);
    Q_ASSERT(m_font.isValid());
    painter.drawGlyphRun(QPointF(m_margin, m_font.ascent() + m_margin), m_glyphRun);
    resize(m_displacement/m_scale + m_margin*2, m_font.ascent() + m_font.descent() + m_margin*2);
}

void PaintWidget::setProperties(PropertyHolder p)
{
    m_glyphRun = p.glyph;
    m_margin = p.margin;
    m_scale = p.scale;
    m_font = p.font;
    m_displacement = p.displacement;
    m_propertiesSet = true;
}
