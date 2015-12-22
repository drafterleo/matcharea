#ifndef C_BELARUSPATTERN_H
#define C_BELARUSPATTERN_H

#include <QVector>
#include <QObject>
#include "c_abstractpattern.h"

class CBricks
{
public:
    CBricks();
    CBricks(int size);

    int brick(int idx);
    int brick(int col, int row);
    void setBrick(int idx, int value);
    void setBrick(int col, int row, int value);
    inline int size(void) {return m_size;}
    void setSize(int matrixSize);
    void clear();

    bool isEqual(CBricks &other);
    bool hasClulsterAt(int x, int y, int w, int h);
    bool hasCluster(int w, int h);
    void fillLine(QLineF line);

    int sum();
    inline int length() {return m_bricks.count();}
    void show();

private:
    int m_size;
    QVector <int> m_bricks;
};

class CBelarusPattern: public CAbstractPattern
{
public:
    CBelarusPattern();
    CBelarusPattern(int matrixSize);

    bool hasSameBricks(CBricks & bricks);

    /* virtuals */
    bool isEqual(CAbstractPattern * other);
    void drawTo(QPainter *painter, const QRect & drawRect, const QColor & color);
    void genRandom();
    void updateSize(int w, int h);
    CAbstractPattern *clone();

protected:
    void initValues();
    void setSize(int matrixSize);

    bool checkEtchMerging(int x1, int x2, int y1, int y2);
    bool checkEtchClustering(int x1, int x2, int y1, int y2);
    bool checkEtchIntersections(int x1, int x2, int y1, int y2, int intsCount);
    bool genEtchLine(int attempts, int intersects = 0);
    bool genEtchDot(int attempts, bool strong = false);
    void genEtchRandom();

    bool checkGrowMerging(QLineF line);
    void genGrowPoints(int count);
    void genGrowLines(int count);
    void genGrowLine(int attempts);
    void genGrowRandom();

private:
    CBricks m_bricks;
    int m_width;
    int m_height;

    QVector <QLineF> m_growLines;
    QVector <QPoint> m_growPoints;
};


#endif // C_BELARUSPATTERN_H
