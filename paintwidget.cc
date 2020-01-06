#include "paintwidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <cmath>

PaintWidget::PaintWidget(QWidget *parent):
    QWidget(parent)
{
    m_handleRects.resize(4);
    m_timer.start(500);
    connect(&m_timer, &QTimer::timeout, this, &PaintWidget::blinkIBar);
    setFocusPolicy(Qt::ClickFocus);
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
    QPainter gc(this);
    gc.fillRect(event->rect(), Qt::white);

    if(m_props.isEmpty()){
        drawIbar(gc, 0);
        return;
    }

    gc.setPen(m_textColor);

    auto ascent = m_props[0].font.ascent();
    auto descent = m_props[0].font.descent();
    qreal displacement = ascent, maxWidth = 0, lastWidth = 0;

    for(int i=0;i<m_props.count(); i++){
        auto glyphWidth = m_props[i].glyph.boundingRect().width();
        if(maxWidth < glyphWidth)
            maxWidth = glyphWidth;
        lastWidth = glyphWidth;
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
    drawIbar(gc, lastWidth);
}

void PaintWidget::drawIbar(QPainter &gc, qreal distanceFromLeft)
{
    if(m_insertMode && m_blinkCounter % 2){
        QPoint pos = m_lastCursorPos;

        if(m_boundingRect.size() != QSize(0, 0)){
            pos = m_boundingRect.bottomLeft() + QPoint(distanceFromLeft + 3, 0);
        }

        int size = 20;
        if(m_props.size() != 0){
            size = m_props[0].font.pixelSize();
        }

        gc.fillRect(QRect(pos - QPoint(0, size), QSize(1, size)), Qt::black);
        m_blinkCounter = 1;
    }
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

    for(int i=0; i<4; i++) {
        if(m_handleRects[i].contains(event->pos())){
            m_handleSelected = true;
            m_handle = static_cast<PaintWidget::Handle>(i);
            return;
        }
    }

    if(m_boundingRect.contains(event->pos())){
        m_lastCursorPos = event->pos() - m_topLeft;
        m_moveIt = true;
        return;
    }

    m_insertMode = true;
    m_lastCursorPos = event->pos();
    m_topLeft = m_lastCursorPos;
    update();
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_moveIt){
        m_topLeft = event->pos() - m_lastCursorPos;
        update();
    }

    if(m_handleSelected){
        auto oldSize = m_boundingRect.size();
        switch (m_handle) {
            case TopLeft:{
                m_boundingRect.setTopLeft(event->pos());
                m_topLeft = event->pos();
                break;
            }
            case TopRight:{
                m_boundingRect.setTopRight(event->pos());
                m_topLeft.setY(event->pos().y());
                break;
            }
            case BottomLeft:{
                m_boundingRect.setBottomLeft(event->pos());
                m_topLeft.setY(event->pos().x());
                break;
            }
            case BottomRight:{
                m_boundingRect.setBottomRight(event->pos());
                break;
            }
        }

        update();
    }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    m_moveIt = false;
    m_handleSelected = false;
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

void PaintWidget::blinkIBar()
{
    m_blinkCounter++;
    this->update();
}

void PaintWidget::keyPressEvent(QKeyEvent *event)
{
    if(!m_insertMode){
        QWidget::keyPressEvent(event);
        return;
    }

    QString text = event->text();

    if(!text[0].isSpace()){
        text = text.trimmed();
    }

    if(text == "\r")
        text = "\n";

    if(text == "\b"){
        m_input.resize(m_input.length() - 1);
    }else{
        m_input += text;
    }

    emit textEntered(m_input);
    this->update();
}
