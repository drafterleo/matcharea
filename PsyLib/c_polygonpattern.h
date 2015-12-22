#ifndef C_POLYGONPATTERN_H
#define C_POLYGONPATTERN_H

#include "c_abstractpattern.h"

class CPolygonPattern : public CAbstractPattern
{
public:
    CPolygonPattern();

    inline int punktojKvanto() {return punktoj.count();}
    void difiniPunktojKvanto(int kvanto);
    QPointF punkto(int indico);

    /* virtuals */
    bool isEqual(CAbstractPattern * other);
    bool isSimilar(CAbstractPattern * other);
    void drawTo(QPainter *painter, const QRect & drawRect, const QColor & color);
    void genRandom();
    void updateSize(int w, int h);
    CAbstractPattern *clone();
protected:
    bool chuLinioSekcasIntimaZono(QPointF lp1, QPointF lp2, QPointF cz);
private:
    QVector <QPointF> punktoj;
    qreal kalibro;
    qreal interIntimeco;
    qreal traIntimeco;

    int m_width;
    int m_height;

};

#endif // C_POLYGONPATTERN_H
