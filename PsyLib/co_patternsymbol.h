#ifndef CW_DOTPATTERNSYMBOL_H
#define CW_DOTPATTERNSYMBOL_H

#include <QWidget>
#include <QDebug>
#include "co_abstractsymbol.h"
#include "c_runepattern.h"

class CPatternSymbol : public CAbstractSymbol
{
public:
    explicit CPatternSymbol(QObject *parent = 0);

    void setPattern(CAbstractPattern * newPattern);
    inline CAbstractPattern * getPattern() const {return m_pattern;}
    inline qreal patternScale() {return m_patternScale;}
    void setPatternScale(qreal scale);

    virtual void setGeometry(int x, int y, int w, int h);
    virtual void drawTo(QPainter *painter, const QPoint & drawPos = QPoint());

signals:
    void clicked();

protected:

private:
    CAbstractPattern *m_pattern;
    bool m_mousePressed;

    qreal m_patternScale;
};

#endif // CW_DOTPATTERNSYMBOL_H
