#include <qmath.h>
#include <QtDebug>
#include "c_polygonpattern.h"

CPolygonPattern::CPolygonPattern()
{
    kalibro = 2.0;
    interIntimeco = 0.3;
    traIntimeco = 0.1;

    m_width = 10;
    m_height = 10;

    difiniPunktojKvanto(5);
}

void CPolygonPattern::difiniPunktojKvanto(int kvanto)
{
    if (kvanto > 0) {
        punktoj.clear();
        punktoj.fill(QPointF(), kvanto);
    }
}

QPointF CPolygonPattern::punkto(int indico)
{
    if (indico >= 0 && indico < punktoj.count()) {
        return punktoj.at(indico);
    }
    return QPointF();
}

bool CPolygonPattern::isEqual(CAbstractPattern * other)
{
    CPolygonPattern *pattern = dynamic_cast <CPolygonPattern *> (other);
    if (pattern) {
        if (punktojKvanto() != pattern->punktojKvanto())
            return false;
        for (int i = 0; i < punktojKvanto(); ++i) {
            if (punktoj.at(i) != pattern->punkto(i))
                return false;
        }
        return true;
    }
    return false;
}

bool CPolygonPattern::isSimilar(CAbstractPattern * other)
{
    CPolygonPattern *pattern = dynamic_cast <CPolygonPattern *> (other);
    if (pattern) {
        int koincidoj = 0;
        if (punktojKvanto() != pattern->punktojKvanto())
            return false;
        for (int i = 0; i < punktojKvanto(); ++i) {
            for (int j = 0; j < punktojKvanto(); ++j) {
                qreal dx = punktoj[i].x() - pattern->punkto(j).x();
                qreal dy = punktoj[i].y() - pattern->punkto(j).y();
                qreal dist = qSqrt(dx*dx + dy*dy);
                if (dist < traIntimeco) {
                    koincidoj ++;
                    break;
                }
            }
        }
        if (koincidoj >= punktojKvanto())
            return true;
    }
    return false;
}


void CPolygonPattern::drawTo(QPainter *painter, const QRect & drawRect, const QColor & color)
{
//    int wMarg = qRound(0.08*drawRect.width());
//    int hMarg = qRound(0.08*drawRect.height());

    int wMarg = qRound(0.08*m_width);
    int hMarg = qRound(0.08*m_height);

    QRect bildRekt = drawRect.adjusted(wMarg, hMarg, -wMarg, -hMarg);
    bildRekt.moveTo(drawRect.left() + (drawRect.width() - bildRekt.width())/2,
                    drawRect.top() + (drawRect.height() - bildRekt.height())/2);

    painter->setPen(QPen(color, kalibro));
    painter->setBrush(color);
    qreal xSkalo = bildRekt.width();
    qreal ySkalo = bildRekt.height();
    int antauha = punktojKvanto() - 1;
    for (int i = 0; i < punktojKvanto(); ++i) {
        QPointF antauhaPunkto = QPointF(punktoj[antauha].x()*xSkalo, punktoj[antauha].y()*ySkalo) + bildRekt.topLeft();
        QPointF aktualaPunkto = QPointF(punktoj[i].x()*xSkalo, punktoj[i].y()*ySkalo) + bildRekt.topLeft();
        painter->drawLine(antauhaPunkto, aktualaPunkto);
        painter->drawEllipse(aktualaPunkto, kalibro + 1, kalibro + 1);
        if (antauha >= punktojKvanto() - 1)
            antauha = 0;
        else
            antauha++;
    }

}

bool CPolygonPattern::chuLinioSekcasIntimaZono(QPointF lp1, QPointF lp2, QPointF cz)
{
    // ekvacio de rekto, trapasante du punktojn
    qreal a = lp2.y() - lp1.y();
    qreal b = lp1.x() - lp2.x();
    qreal c =  -a * lp1.x() - b * lp1.y();

    // apuda punkto
    qreal k = (a*cz.x() + b*cz.y() + c) / (a*a + b*b);
    QPointF p = QPointF(cz.x() - a * k, cz.y() - b * k);

    // distanco
    qreal dx = p.x() - cz.x();
    qreal dy = p.y() - cz.y();
    qreal d = qSqrt(dx*dx + dy*dy);

    if (traIntimeco >= d)
        return true;

    return false;
}


void CPolygonPattern::genRandom()
{
    QPointF intimGrandeco = QPointF(interIntimeco, interIntimeco);

    bool bone = false;
    int ekstraPenoj = 0;
    while (!bone && ekstraPenoj < 20) {
        for (int i = 0; i < punktojKvanto(); ++i) {
            QPointF kandidato;
            int penoj = 0;
            bone = false;
            while (!bone && penoj < 50) {
                qreal x = (qrand()%100)/100.0;
                qreal y = (qrand()%100)/100.0;
                kandidato = QPointF(x, y);
                bone = true;
                for (int j = 0; j <= i; ++j) {
                    QRectF intimaZono = QRectF(punktoj[j] - intimGrandeco, punktoj[j] + intimGrandeco);
                    if (intimaZono.contains(kandidato)) {
                        bone = false;
                        break;
                    }
                    if (j > 0 && chuLinioSekcasIntimaZono(punktoj[j-1], punktoj[j], kandidato)) {
                        bone = false;
                        break;
                    }
                }
                penoj ++;
            }
            if (!bone)
                break;
            punktoj[i] = kandidato;
        }
        ekstraPenoj++;
    }
}

void CPolygonPattern::updateSize(int w, int h)
{

    m_width = w;
    m_height = h;

    qreal d = h;
    if (w > h)
        d = w;

    kalibro = qBound(3.0, d/40.0, 7.0);
    if (d < 50)
        kalibro = 2.0;

}

CAbstractPattern *CPolygonPattern::clone()
{
    CPolygonPattern *result = new CPolygonPattern;
    *result = *this;
    return result;
}
