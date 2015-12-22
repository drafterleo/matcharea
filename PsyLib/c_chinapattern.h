#ifndef C_CHINAPATTERN_H
#define C_CHINAPATTERN_H

#include "c_abstractpattern.h"
#include <QChar>
#include <QFont>

class CChinaPattern : public CAbstractPattern
{
public:
    CChinaPattern();

    inline const QChar & chinaChar() const {return m_chinaChar;}

    bool isEqual(CAbstractPattern * other);
    void drawTo(QPainter *painter, const QRect & drawRect, const QColor & color);
    void genRandom();
    void updateSize(int w, int h);
    CAbstractPattern * clone();

private:
    QChar m_chinaChar;
    QFont m_chinaFont;
    int m_width;
    int m_height;
};

#endif // C_CHINAPATTERN_H
