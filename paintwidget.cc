#include "paintwidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <cmath>

PaintWidget::PaintWidget(QWidget *parent):
    QWidget(parent)
{
    m_handleRects.resize(4);
    connect(&m_timer, &QTimer::timeout, this, &PaintWidget::blinkIBar);
    setFocusPolicy(Qt::StrongFocus);
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
    QPainter gc(this);
    gc.fillRect(event->rect(), Qt::white);

    if(m_props.isEmpty()){
        drawIbar(gc, m_topLeft);
        return;
    }

    gc.setPen(m_textColor);

    auto ascent = m_props[0].font.ascent();
    auto descent = m_props[0].font.descent();
    qreal maxHeight = 0, maxWidth = 0, lastWidth = 0, displacement = 0;
    auto caretPos = m_props.last().glyph.boundingRect().bottomRight();

    if(m_layoutEngine.direction() == RAQM_DIRECTION_TTB){
        qreal width = m_props[0].font.maxCharWidth()/2;
        qreal offset = m_props[0].glyph.boundingRect().width();
        QVector<qreal> test;
        for(int i=0; i<m_props.count(); i++){
            gc.drawGlyphRun(m_topLeft + QPointF(displacement, 0), m_props[i].glyph);

            auto rect = m_props[i].glyph.boundingRect();

            if(maxWidth < rect.width())
                maxWidth = rect.width();

            if(maxHeight < rect.height())
                maxHeight = rect.height();

            lastWidth = rect.width();

            test.push_back(lastWidth);

            displacement += m_lineHeight + width*2;
        }

        caretPos += QPointF(displacement - width*3, ascent + descent) + m_topLeft;
        m_boundingRect = QRect(m_topLeft - QPoint(offset, 0), QSize(displacement-width, maxHeight + descent));

    }else{
        for(int i=0; i<m_props.count(); i++){
            gc.drawGlyphRun(m_topLeft + QPointF(0, ascent + displacement), m_props[i].glyph);
            displacement += ascent + descent + m_lineHeight;

            auto rect = m_props[i].glyph.boundingRect();

            if(maxWidth < rect.width())
                maxWidth = rect.width();

            if(maxHeight < rect.height())
                maxHeight = rect.height();

            lastWidth = rect.width();
        }
        caretPos += QPointF(0, displacement - descent) + m_topLeft;
        m_boundingRect = QRect(m_topLeft, QSize(maxWidth, displacement - m_lineHeight));
    }

    drawHandles(gc);
    drawIbar(gc, caretPos);
}

void PaintWidget::drawIbar(QPainter &gc, QPointF bottomPos)
{
    if(m_insertMode && m_blinkCounter % 2){
        int size = m_layoutEngine.fontSize();

        int scaleFactor = size/10;

        if(m_boundingRect.size() != QSize(0, 0)){
            gc.fillRect(QRect(bottomPos.toPoint() - QPoint(0, size + scaleFactor), QSize(1, size + scaleFactor * 2)), Qt::black);
        }else{
            gc.fillRect(QRect(m_topLeft, QSize(1, size + scaleFactor * 2)), Qt::black);
        }

        m_blinkCounter = 1;
    }
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
    m_timer.start(500);
    this->update();
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_moveIt){
        m_topLeft = event->pos() - m_lastCursorPos;
        this->update();
    }

    if(m_handleSelected){
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

        this->update();
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

    handleBox.moveCenter(m_boundingRect.topLeft());
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
        m_totalInput.resize(m_totalInput.length() - 1);
    }else{
        m_totalInput += text;
    }

    m_input = m_totalInput.split("\n");

    recalliberate();
}

void PaintWidget::recalliberate()
{
    QVector<PropertyHolder>props;

    foreach(QString str, m_input) {
        m_layoutEngine.setText(str);
        auto p = m_layoutEngine.calculate();
        props.push_back(p);
    }

    if(props.isEmpty())
        return;

    m_props = props;
    this->update();
}

void PaintWidget::setFontSize(int size)
{
    m_layoutEngine.setFontSize(size);
    recalliberate();
}

void PaintWidget::setTextColor(QColor col)
{
    m_textColor = col;
    recalliberate();
}

void PaintWidget::setFontFile(QString file)
{
    m_layoutEngine.setFontFace(file);
    recalliberate();
}

void PaintWidget::setAlignment(int align)
{
    m_align = align;
    recalliberate();
}

void PaintWidget::setLineHeight(qreal height)
{
    m_lineHeight = height;
    recalliberate();
}

void PaintWidget::setLetterSpacing(qreal spacing)
{
    m_layoutEngine.setLetterSpacing(spacing);
    recalliberate();
}

void PaintWidget::setWordSpacing(qreal spacing)
{
    m_layoutEngine.setWordSpacing(spacing);
    recalliberate();
}

void PaintWidget::setTextDirection(bool dir)
{
    m_layoutEngine.setDirection(dir);
    recalliberate();
}
