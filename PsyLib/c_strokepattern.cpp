#include "c_strokepattern.h"
#include <QtDebug>

CStrokePattern::CStrokePattern()
{
    difiniStrekojKvanto(5);
    m_width = 10;
    m_height = 10;
}

void CStrokePattern::difiniStrekojKvanto(int kvanto)
{
    if (kvanto > 0) {
        strekoj.clear();
        strekoj.fill(1, kvanto);
    }
}

int CStrokePattern::streko(int indico)
{
    if (indico >= 0 && indico < strekoj.count()) {
        return strekoj.at(indico);
    }
    return 1;
}

bool CStrokePattern::isEqual(CAbstractPattern * other)
{
    CStrokePattern *pattern = dynamic_cast <CStrokePattern *> (other);
    if (pattern) {
        if (strekojKvanto() != pattern->strekojKvanto())
            return false;
        for (int i = 0; i < strekojKvanto(); ++i) {
            if (strekoj.at(i) != pattern->streko(i))
                return false;
        }
    }
    return true;
}

void CStrokePattern::drawTo(QPainter *painter, const QRect & drawRect, const QColor & color)
{
    int wMarg = qRound(0.067*m_width);
    int hMarg = qRound(0.067*m_height);

    QRect bildRekt = drawRect.adjusted(wMarg, hMarg, -wMarg, -hMarg);
    bildRekt.moveTo(drawRect.left() + (drawRect.width() - bildRekt.width())/2,
                    drawRect.top() + (drawRect.height() - bildRekt.height())/2);

    int blokAlto = bildRekt.height()/strekoj.count();
    int strekAlto = qRound(blokAlto * (3.0/5.0));
    int altShovo = qRound(blokAlto * (3.0/10.0));

    painter->setPen(color);
    painter->setBrush(color);

    for (int i = 0; i < strekoj.count(); ++i) {
        int m = strekoj[i];
        int largho = qRound(bildRekt.width()/m);
        int blokLargho = largho;
        int blokShovo = 0;
        if (strekoj[i] > 1) {
            blokLargho = qRound(largho * 0.9);
            blokShovo = largho + qRound((largho - blokLargho)/(m - 1));
        }
        for (int j = 0; j < strekoj[i]; ++j) {
            painter->drawRect(QRect(bildRekt.left() + j * blokShovo,
                                    bildRekt.top() + i * blokAlto + altShovo,
                                    blokLargho, strekAlto));
        }
        //egaligi dekstran randon
        painter->drawRect(QRect(bildRekt.right() - blokLargho + 1,
                                bildRekt.top() + i * blokAlto + altShovo,
                                blokLargho, strekAlto));

    }
}

void CStrokePattern::genRandom()
{
    for (int i = 0; i < strekoj.count(); ++i){
        strekoj[i] = qrand()%3 + 1;
    }
}

void CStrokePattern::updateSize(int w, int h)
{
    m_width = w;
    m_height = h;
}

CAbstractPattern *CStrokePattern::clone()
{
    CStrokePattern *result = new CStrokePattern;
    *result = *this;
    return result;
}

