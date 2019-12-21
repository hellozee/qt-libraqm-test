#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include "layoutengine.h"

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e) override;
    void setProperties(PropertyHolder p);

private:
    QGlyphRun m_glyphRun;
    qreal m_margin, m_scale, m_displacement;
    QRawFont m_font;
    bool m_propertiesSet;
    int count = 0;
};

#endif // PAINTWIDGET_H
