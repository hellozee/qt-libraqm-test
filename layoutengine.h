#ifndef LAYOUTENGINE_H
#define LAYOUTENGINE_H

#include <raqm.h>
#include <QString>
#include <QGlyphRun>
#include <QRawFont>

struct PropertyHolder{
    QGlyphRun glyph;
    QRawFont font;

    bool operator==(const PropertyHolder p){
        return glyph == p.glyph && p.font == font;
    }
};


class LayoutEngine
{
public:
    LayoutEngine() = default;
    ~LayoutEngine();

    inline void setFontFace(QString font) { m_font = font; }
    inline void setText(QString text) { m_text = text; }
    inline void setFontSize(int size) { m_fontSize = size; }
    inline void setLetterSpacing(qreal spacing) { m_letterSpacing = spacing; }
    inline void setWordSpacing(qreal spacing) { m_wordSpacing = spacing; }

    inline void setDirection(bool vertical){
        if(vertical)
            m_direction = RAQM_DIRECTION_TTB;
        else
            m_direction = RAQM_DIRECTION_DEFAULT;
    }

    PropertyHolder calculate();

private:
    QString m_font, m_text;
    raqm_direction_t m_direction;
    const char *m_language = "en";
    raqm_t *m_rq;
    FT_Face m_face;
    FT_Library m_library;
    int m_fontSize;
    qreal m_letterSpacing, m_wordSpacing;
};

#endif // LAYOUTENGINE_H
