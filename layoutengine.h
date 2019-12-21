#ifndef LAYOUTENGINE_H
#define LAYOUTENGINE_H

#include <raqm.h>
#include <QString>
#include <QGlyphRun>
#include <QRawFont>

struct PropertyHolder{
    QGlyphRun glyph;
    qreal margin, scale, displacement;
    QRawFont font;
};


class LayoutEngine
{
public:
    LayoutEngine() = default;
    ~LayoutEngine();

    inline void setFont(QString font) { m_font = font; }
    inline void setText(QString text) { m_text = text; }
    inline void setFontSize(int size) { m_fontSize = size; }
    PropertyHolder calculate();

private:
    QString m_font, m_text;
    raqm_direction_t m_direction = RAQM_DIRECTION_LTR;
    const char *m_language = "hi";
    FT_Face m_face;
    FT_Library m_library;
    raqm_t *m_rq;
    int m_fontSize;
};

#endif // LAYOUTENGINE_H
