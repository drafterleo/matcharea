#include <QtDebug>
#include "c_rectanglespattern.h"

CRectanglesPattern::CRectanglesPattern()
{
    difiniOrtangulojKvanto(3);
    m_width = 10;
    m_height = 10;
}

void CRectanglesPattern::difiniOrtangulojKvanto(int kvanto)
{
    if (kvanto > 0) {
        ortanguloj.clear();
        ortanguloj.fill(QRectF(), kvanto);
    }
}

QRectF CRectanglesPattern::ortangulo(int indico)
{
    if (indico >= 0 && indico < ortanguloj.count()) {
        return ortanguloj.at(indico);
    }
    return QRectF();
}

bool CRectanglesPattern::isEqual(CAbstractPattern *other)
{
    CRectanglesPattern *pattern = dynamic_cast <CRectanglesPattern *> (other);
    if (pattern) {
        if (ortangulojKvanto() != pattern->ortangulojKvanto())
            return false;
        for (int i = 0; i < ortangulojKvanto(); ++i) {
            if (ortanguloj.at(i) != pattern->ortangulo(i))
                return false;
        }
        return true;
    }
    return false;
}

bool CRectanglesPattern::isSimilar(CAbstractPattern *other)
{
    CRectanglesPattern *pattern = dynamic_cast <CRectanglesPattern *> (other);
    if (pattern) {
        if (ortangulojKvanto() != pattern->ortangulojKvanto())
            return false;
        qreal spaco = 0.05;
        for (int i = 0; i < ortangulojKvanto(); ++i) {
            QRectF ekstera = ortanguloj[i].adjusted(-spaco, -spaco, spaco, spaco);
            QRectF interna = ortanguloj[i].adjusted(spaco, spaco, -spaco, -spaco);
            for (int j = 0; j < ortangulojKvanto(); ++j) {
                if (ekstera.contains(pattern->ortangulo(j)) &&
                    pattern->ortangulo(j).contains(interna))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void CRectanglesPattern::drawTo(QPainter *painter, const QRect & drawRect, const QColor & color)
{

    int wMarg = qRound(0.08 * m_width);
    int hMarg = qRound(0.08 * m_height);

    QRect bildRekt = drawRect.adjusted(wMarg, hMarg, -wMarg, -hMarg);
    bildRekt.moveTo(drawRect.left() + (drawRect.width() - bildRekt.width())/2,
                    drawRect.top() + (drawRect.height() - bildRekt.height())/2);

    painter->setPen(QPen(color, kalibro));
    painter->setBrush(Qt::NoBrush);
    int xSkalo = bildRekt.width();
    int ySkalo = bildRekt.height();
    for (int i = 0; i < ortangulojKvanto(); ++i) {
        painter->drawRect(ortanguloj[i].left()*xSkalo + bildRekt.left(),
                          ortanguloj[i].top()*ySkalo + bildRekt.top(),
                          ortanguloj[i].width()*xSkalo,
                          ortanguloj[i].height()*ySkalo);
    }
}

bool chuOrtangulojSimila(const QRectF r1, const QRectF r2)
{
    //qreal spaco = 0.05;
    return false;
}


void CRectanglesPattern::genRandom()
{
    for (int i = 0; i < ortangulojKvanto(); ++i) {
        QRectF kandidato;
        bool bona = false;
        while (!bona) {
            int w = qrand()%80 + 20;
            int h = qrand()%80 + 20;
            int x = qrand()%(100 - w);
            int y = qrand()%(100 - h);
            kandidato = QRectF(x/100.0, y/100.0, w/100.0, h/100.0);
            bona = true;
            qreal spaco = (qrand()%20 + 10)/100.0;
            for (int j = 0; j < i; ++j) {
                if (qAbs(ortanguloj[j].left() - kandidato.left()) <= spaco ||
                    qAbs(ortanguloj[j].right() - kandidato.right()) <= spaco ||
                    qAbs(ortanguloj[j].top() - kandidato.top()) <= spaco ||
                    qAbs(ortanguloj[j].bottom() - kandidato.bottom()) <= spaco)
                {
                    bona = false;
                    break;
                }
            }
        }

        ortanguloj[i] = kandidato;
    }
}

void CRectanglesPattern::updateSize(int w, int h)
{
    m_width = w;
    m_height = h;

    qreal d = h;
    if (w > h)
        d = w;

    kalibro = qBound(3.0, d/50.0, 7.0);
    if (d < 50)
        kalibro = 2.0;
}

CAbstractPattern *CRectanglesPattern::clone()
{
    CRectanglesPattern *result = new CRectanglesPattern;
    *result = *this;
    return result;
}

