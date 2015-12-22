#ifndef CRECTANGLESPATTERN_H
#define CRECTANGLESPATTERN_H

#include "c_abstractpattern.h"

class CRectanglesPattern : public CAbstractPattern
{
public:
    CRectanglesPattern();

    inline int ortangulojKvanto() {return ortanguloj.count();}
    void difiniOrtangulojKvanto(int kvanto);
    QRectF ortangulo(int indico);

    /* virtuals */
    bool isEqual(CAbstractPattern * other);
    bool isSimilar(CAbstractPattern * other);
    void drawTo(QPainter *painter, const QRect & drawRect, const QColor & color);
    void genRandom();
    void updateSize(int w, int h);
    CAbstractPattern *clone();
protected:
    bool chuOrtangulojSimila(const QRectF & r1, const QRectF & r2);

private:
    QVector <QRectF> ortanguloj;
    qreal kalibro;

    int m_width;
    int m_height;
};

#endif // CRECTANGLESPATTERN_H
