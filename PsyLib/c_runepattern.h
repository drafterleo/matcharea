#ifndef C_RUNEPATTERN_H
#define C_RUNEPATTERN_H

#include <QVector>
#include <QPoint>
#include <QObject>
#include "c_abstractpattern.h"

class CDotLink;

class CRunePattern: public CAbstractPattern
{
public:
    CRunePattern();
    CRunePattern(int c, int r);
    ~CRunePattern();

    void setRate(int c, int r);

    bool addLink(const QPoint & cell_a, const QPoint & cell_b);
    void addRandomLink();
    void genSolidLink(int linkCount, int crosses = 3);

    inline const QList <CDotLink> & getLinks() const {return m_links;}

    inline int colCount() const {return m_cols;}
    inline int rowCount() const {return m_rows;}

    inline bool isEmpty(void) const {return m_cols == 0 || m_rows == 0;}

    /* virtuals */
    bool isEqual(CAbstractPattern * other);
    void drawTo(QPainter *painter, const QRect & drawRect, const QColor & color);
    void genRandom();
    void updateSize(int w, int h);
    CAbstractPattern *clone();

protected:
    int    indexOfCell(const QPoint & cell) const;
    QPoint pointOfDot(int dotIdx) const;
    int    colByDot(int dotIdx) const;
    int    rowByDot(int dotIdx) const;

    bool isCellValid(const QPoint & cell) const;
    bool hasLink (const CDotLink & link) const;
    bool isLinksCrossed(const CDotLink & aLink, const CDotLink & bLink) const;
    bool hasCrossLink(const CDotLink & link) const;


private:
    void initMemberValues(void);

private:
    int m_cols;
    int m_rows;

    int m_width;
    int m_height;

    qreal m_patternCaliber;

    QList <CDotLink> m_links;
    QVector <QPoint> m_points;
};

class CDotLink
{
public:
    CDotLink(int a, int b)
    {
        aDot = a;
        bDot = b;
    }

    CDotLink()
    {
        aDot = -1;
        bDot = -1;
    }

    inline bool isValid(void) {return aDot >= 0 && bDot >= 0;}

    inline bool operator== (const CDotLink & other) const
    {
        return (aDot == other.aDot && bDot == other.bDot) ||
               (aDot == other.bDot && bDot == other.aDot);
    }

public:
    int aDot;
    int bDot;
};

#endif // C_RUNEPATTERN_H
