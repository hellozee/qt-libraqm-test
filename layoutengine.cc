#include "layoutengine.h"

LayoutEngine::~LayoutEngine()
{
    raqm_destroy (m_rq);
    FT_Done_Face (m_face);
    FT_Done_FreeType (m_library);
}

PropertyHolder LayoutEngine::calculate()
{
    qreal margin = m_fontSize * 0.5;
    long fontScale = 16;

    Q_ASSERT(FT_Init_FreeType(&m_library) == 0);
    Q_ASSERT(FT_New_Face(m_library, m_font.toUtf8().data(), 0, &m_face) == 0);
    Q_ASSERT(FT_Set_Char_Size(m_face, m_fontSize, 0, 0, 0) == 0) ;

    m_rq = raqm_create();
    Q_ASSERT(m_rq);

    Q_ASSERT(raqm_set_text_utf8 (m_rq, m_text.toUtf8().data(), static_cast<size_t>(m_text.length())));
    Q_ASSERT(raqm_set_freetype_face (m_rq, m_face));
    Q_ASSERT(raqm_set_par_direction (m_rq, m_direction));
    Q_ASSERT(raqm_set_language (m_rq, m_language, 0, strlen (m_text.toUtf8().data())));
    Q_ASSERT(raqm_layout (m_rq));

    size_t count;
    raqm_glyph_t *glyphs = raqm_get_glyphs (m_rq, &count);

    Q_ASSERT(glyphs);
    Q_ASSERT(count > 0);

    //do glyphs stuff

    QVector<quint32> glyphIndexes(static_cast<int>(count));
    QVector<QPointF> glyphPositions(static_cast<int>(count));

    qreal x = 0.0, y = 0.0;
    for(int i=0; i<static_cast<int>(count); i++) {
        glyphIndexes[i] = glyphs[i].index;
        glyphPositions[i] = QPointF(x + glyphs[i].x_offset, y - glyphs[i].y_offset);
        x += glyphs[i].x_advance;
        y -= glyphs[i].y_advance;
    }


    QGlyphRun glyphRun = QGlyphRun();
    QRawFont rawFont = QRawFont(m_font, m_fontSize);
    glyphRun.setRawFont(rawFont);
    glyphRun.setGlyphIndexes(glyphIndexes);
    glyphRun.setPositions(glyphPositions);

    PropertyHolder p;
    p.glyph = glyphRun;
    p.font = rawFont;
    p.margin = margin;
    p.scale = fontScale;
    return p;
}
