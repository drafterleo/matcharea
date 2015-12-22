#include "co_patternsymbol.h"
#include <QPainter>
#include <QMouseEvent>


CPatternSymbol::CPatternSymbol(QObject *parent)
    : CAbstractSymbol(parent)
{
    m_pattern = 0;
    m_patternScale = 1;
    m_mousePressed = false;
    setGeometry(0, 0, 50, 50);
    setPatternColor(Qt::black);
    setFrameColor(Qt::gray);
    setBackgroundColor(Qt::transparent);
}

void CPatternSymbol::setPattern(CAbstractPattern * newPattern)
{
    m_pattern = newPattern;
}

void CPatternSymbol::setGeometry(int x, int y, int w, int h)
{
    CAbstractSymbol::setGeometry(x, y, w, h);
    if (m_pattern)
        m_pattern->updateSize(qRound(w * m_patternScale), qRound(h * m_patternScale));
}

void CPatternSymbol::setPatternScale(qreal scale)
{
    m_patternScale = scale;
    if (m_pattern)
        m_pattern->updateSize(qRound(geometry().width() * m_patternScale),
                              qRound(geometry().height() * m_patternScale));
}

void CPatternSymbol::drawTo(QPainter *painter, const QPoint & drawPos)
{
    QRect drawRect = QRect(drawPos, geometry().size());

    //qDebug() << drawRect << geometry << this->pos() << geometry.x() << geometry.y() << geometry.topLeft();

    /* draw border */
    painter->setPen(QPen(frameColor(), 2));
    if (backgroundColor() == Qt::transparent)
        painter->setBrush(Qt::NoBrush);
    else
        painter->setBrush(backgroundColor());
    painter->drawRoundRect(drawRect.adjusted(2, 2, -2, -2));

    if (m_pattern)
        m_pattern->drawTo(painter, drawRect, patternColor());
}
