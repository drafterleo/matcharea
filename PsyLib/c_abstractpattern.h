#ifndef CO_ABSTRACTPATTERN_H
#define CO_ABSTRACTPATTERN_H

#include <QObject>
#include <QRect>
#include <QPainter>

enum PATTERN_TYPE {
    ptDotPattern,
    ptRectanglesPattern,
    ptPolygonPattern,
    ptBelarusPattern,
    ptChinaPattern,
    ptStrokePattern
};

class CAbstractPattern
{
public:
    explicit CAbstractPattern();

    inline PATTERN_TYPE patternType() {return m_patternType;}
    inline void setPatternType(PATTERN_TYPE pt) {m_patternType = pt;}

    virtual bool isEqual(CAbstractPattern * other) {return false;}
    virtual bool isSimilar(CAbstractPattern * other) {return isEqual(other);}
    virtual void drawTo(QPainter *painter, const QRect & drawRect, const QColor & color) = 0;
    virtual void genRandom() = 0;
    virtual void updateSize(int w, int h) = 0;
    virtual CAbstractPattern *clone() = 0;

private:
    PATTERN_TYPE m_patternType;

};

#endif // CO_ABSTRACTPATTERN_H
