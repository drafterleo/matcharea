#ifndef C_ICHINGPATTERN_H
#define C_ICHINGPATTERN_H

#include "c_abstractpattern.h"
#include <QVector>

class CStrokePattern : public CAbstractPattern
{
public:
    CStrokePattern();

    /* virtuals */
    bool isEqual(CAbstractPattern * other);
    void drawTo(QPainter *painter, const QRect & drawRect, const QColor & color);
    void genRandom();
    void updateSize(int w, int h);
    CAbstractPattern *clone();

    inline int strekojKvanto() {return strekoj.count();}
    void difiniStrekojKvanto(int kvanto);
    int streko(int indico);

protected:

private:
    QVector <int> strekoj;
    int m_width;
    int m_height;
};

#endif // C_ICHINGPATTERN_H
