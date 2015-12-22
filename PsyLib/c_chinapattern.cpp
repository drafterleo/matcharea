#include "c_chinapattern.h"
#include <QPainter>
#include <QtDebug>

CChinaPattern::CChinaPattern()
{
    setPatternType(ptChinaPattern);
    m_chinaFont.setFamily("FangSong");
    m_width = 0;
    m_height = 0;
}

bool CChinaPattern::isEqual(CAbstractPattern * other)
{
    CChinaPattern *pattern = dynamic_cast <CChinaPattern *> (other);
    if (pattern && pattern->chinaChar() == m_chinaChar) {
        return true;
    }
    return false;
}

void CChinaPattern::drawTo(QPainter *painter, const QRect & drawRect, const QColor & color)
{
    int w = drawRect.width();
    int h = drawRect.height();

    int d = h;
    if (w > h) {
       d = w;
    }


//    QFontMetrics fm(m_chinaFont);
//    int fsz = d - 10;
//    m_chinaFont.setPointSize(fsz);
//    while (fm.width(m_chinaChar) > w - 16) {
//        fsz--;
//        m_chinaFont.setPointSize(fsz);
//        fm = QFontMetrics(m_chinaFont);
//    }

    painter->setFont(m_chinaFont);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setPen(color);
    painter->drawText(drawRect, Qt::AlignCenter, m_chinaChar);
}

void CChinaPattern::genRandom()
{
    m_chinaChar = QChar(qrand()%10000 + 0x56D6);
    updateSize(m_width, m_height);
}

void CChinaPattern::updateSize(int w, int h)
{
    m_width = w;
    m_height = h;
    int d = h;
    if (h > w) {
       d = w;
    }

    int margin = qRound(static_cast<qreal>(d) * 0.1);

    if ( d < 20 || w < margin || h < margin || m_chinaChar.isNull())
        return;

    int symArea = d - margin;
    int fsz = symArea;
    while (true) {
        m_chinaFont.setPixelSize(fsz);
        QFontMetrics fm(m_chinaFont);

        if (w <= h && fm.maxWidth() <= symArea)
            break;
        if (h < w && fm.height() <= symArea)
            break;

        fsz--;
    }
}

CAbstractPattern *CChinaPattern::clone()
{
    CChinaPattern *result = new CChinaPattern;
    *result = *this;
    return result;
}
