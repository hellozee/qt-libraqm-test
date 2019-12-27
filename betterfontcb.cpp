#include "betterfontcb.h"
#include <QDebug>
#include <fontconfig/fontconfig.h>

BetterFontCB::BetterFontCB(QWidget *parent):
    QComboBox(parent)
{
   setCurrentIndex(0);

   Q_ASSERT(FcInit());
   auto config = FcConfigGetCurrent();
   FcConfigSetRescanInterval(config, 0);
   auto objectSet = FcObjectSetBuild(FC_FAMILY,FC_FILE, nullptr);

   auto fontSet = FcFontList(config, FcPatternCreate(), objectSet);

   for(int i=0; i < fontSet->nfont; i++){
       auto font = fontSet->fonts[i];
       auto s = FcNameUnparse(font);
       QString d(reinterpret_cast<char*>(s));
       auto temp = d.split(':');
       m_fontNames.push_back(temp.first().split(',').first());
       m_fontFiles.push_back(temp.last().mid(5));
       addItem(m_fontNames.last());
   }

   setEditable(true);
}
