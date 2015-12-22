#include "c_belaruspattern.h"
#include <QtDebug>

CBelarusPattern::CBelarusPattern()
    : CAbstractPattern()
{
    initValues();
    m_bricks.setSize(5);
}


CBelarusPattern::CBelarusPattern(int matrixSize)
    : CAbstractPattern()
{
    initValues();
    m_bricks.setSize(matrixSize);
}


void CBelarusPattern::initValues()
{
    setPatternType(ptBelarusPattern);
    m_width = 0;
    m_height = 0;
}


bool CBelarusPattern::hasSameBricks(CBricks &bricks)
{
    if (m_bricks.isEqual(bricks)) {
        return true;
    }
    return false;
}

bool CBelarusPattern::isEqual(CAbstractPattern * other)
{
    CBelarusPattern *pattern = dynamic_cast <CBelarusPattern *> (other);
    if (pattern) {
        if (pattern->hasSameBricks(m_bricks))
            return true;
    }
    return false;
}


void CBelarusPattern::drawTo(QPainter *painter, const QRect & drawRect, const QColor & color)
{
    if (m_bricks.size() <= 0)
        return;

    QRect paintRect = QRect(drawRect.left() + (drawRect.width() - m_width)/2,
                            drawRect.top() + (drawRect.height() - m_height)/2,
                            m_width, m_height).adjusted(5, 5, -5, -5);

    QTransform savedTransform = painter->transform();
    int tx = paintRect.center().x();
    int ty = paintRect.center().y();
    paintRect.translate(-tx, -ty);
    painter->translate(tx, ty);
    painter->rotate(45);
    qreal sqrt2 = 1.41421356237;
    painter->scale(1/sqrt2, 1/sqrt2);

    painter->setPen(QPen(color, 2));
    //painter->drawRect(paintRect);

    int step = paintRect.width()/m_bricks.size();
    QColor brickColor = Qt::red;
    brickColor.setAlpha(color.alpha());
    painter->setBrush(color);
    painter->setPen(color);
    for (int x = 0; x < m_bricks.size(); ++x) {
        for (int y = 0; y < m_bricks.size(); ++y) {
            if (m_bricks.brick(x, y) > 0 ) {
                QRect box = QRect(paintRect.left() + x*step, paintRect.top() + y*step, step, step);
                painter->drawRect(box);
            }
        }
    }

    painter->setTransform(savedTransform);

}

bool CBelarusPattern::checkEtchMerging(int x1, int x2, int y1, int y2)
{
    bool ok = false;

    // check merging
    if (x1 == x2) {
        for (int i = y1; i <= y2; ++i) {
            if (m_bricks.brick(x1, i) == 0) {
                ok = true;
                break;
            }
        }
    } else {
        for (int i = x1; i <= x2; ++i) {
            if (m_bricks.brick(i, y1) == 0) {
                ok = true;
                break;
            }
        }
    }

    return ok;

//    if (x1 == x2) {
//        int m = 0;
//        for (int i = y1; i <= y2; ++i) {
//            if (brick(x1, i) > 0) m++;
//            else m = 0;
//            if (m > 2)
//                return false;
//        }
//    } else {
//        int m = 0;
//        for (int i = x1; i <= x2; ++i) {
//            if (brick(i, y1) > 0) m++;
//            else m = 0;
//            if (m > 2)
//                return false;
//        }
//    }
//    return true;
}

bool CBelarusPattern::checkEtchClustering(int x1, int x2, int y1, int y2)
{
    CBricks bricks = m_bricks;
    if (x1 == x2) {
        for (int i = y1; i <= y2; ++i)
            bricks.setBrick(x1, i, 1);
    } else {
        for (int i = x1; i <= x2 + 1; ++i)
            bricks.setBrick(i, y1, 1);
    }

    return !bricks.hasCluster(2, 2);

//    if (x1 == x2) {
//        int side0 = x1;
//        int side1 = x1 - 1;
//        int side2 = x2 + 1;
//        int f0 = 0;
//        int f1 = 0;
//        int f2 = 0;
//        for (int i = y1; i <= y2; ++i) {
//            if (m_bricks.brick(side0, i) > 0) f0++;
//            else f0 = 0;
//            if (m_bricks.brick(side1, i) > 0) f1++;
//            else f1 = 0;
//            if (m_bricks.brick(side2, i) > 0) f2++;
//            else f2 = 0;

//            if (f0 > 1 || f1 > 1 || f2 > 1)
//                return false;
//        }
//    } else {
//        int side0 = y1;
//        int side1 = y1 - 1;
//        int side2 = y2 + 1;
//        int f0 = 0;
//        int f1 = 0;
//        int f2 = 0;
//        for (int i = x1; i <= x2; ++i) {
//            if (m_bricks.brick(i, side0) > 0) f0++;
//            else f0 = 0;
//            if (m_bricks.brick(i, side1) > 0) f1++;
//            else f1 = 0;
//            if (m_bricks.brick(i, side2) > 0) f2++;
//            else f2 = 0;

//            if (f0 > 1 || f1 > 1 || f2 > 1)
//                return false;
//        }
//    }
//    return true;
}

bool CBelarusPattern::checkEtchIntersections(int x1, int x2, int y1, int y2, int intsCount)
{
    if (intsCount == 0)
        return true;

    int ints = 0;

    if (x1 == x2) {
        for (int i = y1; i < y2 + 1; ++i) {
            if (m_bricks.brick(x1, i) > 0)
                ints ++;
        }
    } else {
        for (int i = x1; i < x2 + 1; ++i) {
            if (m_bricks.brick(i, y1) > 0)
                ints ++;
        }
    }

    if (ints >= intsCount)
        return true;
    else
        return false;
}


bool CBelarusPattern::genEtchLine(int attempts, int intersects)
{
    while (attempts > 0) {
        attempts --;

        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;
        while (qAbs(x1 - x2) < 3) {
            x1 = qrand() % m_bricks.size();
            x2 = qrand() % m_bricks.size();

        }
        while (qAbs(y1 - y2) < 3) {
            y1 = qrand() % m_bricks.size();
            y2 = qrand() % m_bricks.size();

        }

        if (qrand() % 2) {
            x1 = x2;
        } else {
            y1 = y2;
        }

        int tmp;
        if (x1 > x2) {
            tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        if (y1 > y2) {
            tmp = y1;
            y1 = y2;
            y2 = tmp;
        }


        if (!checkEtchMerging(x1, x2, y1, y2))
            continue;
        if (!checkEtchClustering(x1, x2, y1, y2))
            continue;

        //fill bricks
        if (x1 == x2) {
            for (int i = y1; i <= y2; ++i)
                m_bricks.setBrick(x1, i, 1);
        } else {
            for (int i = x1; i <= x2 + 1; ++i)
                m_bricks.setBrick(i, y1, 1);
        }
        return true;
    }
    return false;
}

bool CBelarusPattern::genEtchDot(int attempts, bool strong)
{
    while (attempts > 0) {
        attempts --;

        int x = 0;
        int y = 0;
        x = qrand() % m_bricks.size();
        y = qrand() % m_bricks.size();

        if (m_bricks.brick(x, y) > 0     ||
            m_bricks.brick(x - 1, y) > 0 ||
            m_bricks.brick(x + 1, y) > 0 ||
            m_bricks.brick(x, y - 1) > 0 ||
            m_bricks.brick(x, y + 1) > 0)
        {
            continue;
        }

        if (strong) {
            if (m_bricks.brick(x + 1, y + 1) > 0 ||
                m_bricks.brick(x - 1, y - 1) > 0 ||
                m_bricks.brick(x + 1, y - 1) > 0 ||
                m_bricks.brick(x - 1, y + 1) > 0)
            {
                continue;
            }
        }

        m_bricks.setBrick(x, y, 1);
        return true;
    }

    return false;
}

void CBelarusPattern::genEtchRandom()
{
    if (m_bricks.size() <=0)
        return;

    m_bricks.clear();
    int lineRndCount = qrand() % 4 + 3;
    for (int i = 0; i < lineRndCount; ++i) {
        int intersections = 0;
        if (i > 1) {
            intersections = 1;
        }
        genEtchLine(10000, intersections);
    }

    int dotRndCount = qrand() % 8 - lineRndCount;
    if (lineRndCount < 4)
        dotRndCount ++;
    if (dotRndCount > 3)
        dotRndCount = 3;
    for (int i = 0; i < dotRndCount; ++i) {
        genEtchDot(1000);
    }
}

void CBelarusPattern::genGrowPoints(int count)
{
    if (m_growPoints.count() + count > m_bricks.size() - 1)
        return;

    while (count > 0) {
        int x = qrand() % m_bricks.size();
        int y = qrand() % m_bricks.size();
        bool ok = true;
        for (int i = 0; i < m_growPoints.count(); ++ i) {
            if (m_growPoints.at(i).x() == x || m_growPoints.at(i).y() == y) {
                ok = false;
                break;
            }
        }

        if (ok) {
            count --;
            m_growPoints.append(QPoint(x, y));
            //qDebug() << x << y;
        }
    }
}


bool CBelarusPattern::checkGrowMerging(QLineF line)
{
/*    for (int i = 0; i < m_growLines.count(); ++i) {
        QLineF iLine = m_growLines.at(i);
        int x1 = qMin(qMax(line.p1().x(), iLine.p1().x()), qMax(line.p2().x(), iLine.p2().x()));
        int x2 = qMax(qMin(iLine.p1().x(), line.p1().x()), qMin(iLine.p2().x(), line.p2().x()));
        if (qAbs(x1 - x2) > 1)
            return false;
    }
    return true*/;

    int x1 = qMin(line.p1().x(), line.p2().x());
    int x2 = qMax(line.p1().x(), line.p2().x());
    int y1 = qMin(line.p1().y(), line.p2().y());
    int y2 = qMax(line.p1().y(), line.p2().y());

    return checkEtchMerging(x1, x2, y1, y2);
}


void CBelarusPattern::genGrowLine(int attempts)
{
    while (attempts > 0) {
        attempts --;
        QLineF line;
        QPoint base = m_growPoints.at(qrand() % m_growPoints.count());
        int growDirection = qrand() % 2;
        int lineLength = qrand() % 3 + 3;
        if (growDirection == 0) { // Horizontnal
            int shiftX = 0;
            int x1 = 0;
            if (base.x() + lineLength > m_bricks.size()) {
                shiftX = base.x() + lineLength - m_bricks.size();
            }
            x1 = base.x() - shiftX;
            line.setLine(x1, base.y(), x1 + (lineLength - 1), base.y());
        } else {                  // Vertical
            int shiftY = 0;
            int y1 = 0;
            if (base.y() + lineLength > m_bricks.size()) {
                shiftY = base.y() + lineLength - m_bricks.size();
            }
            y1 = base.y() - shiftY;
            line.setLine(base.x(), y1, base.x(), y1 + (lineLength - 1));
        }

//        if (!checkGrowMerging(line))
//            continue;

        CBricks bricks = m_bricks;
        bricks.fillLine(line);
        if (!bricks.hasCluster(2, 2)) {
            m_bricks.fillLine(line);
            m_growLines.append(line);
            //qDebug() << line.p1().x() << line.p1().y() << line.p2().x() << line.p2().y();
            //m_bricks.show();
            return;
        }
    }
}


void CBelarusPattern::genGrowLines(int count)
{
    for (int i = 0; i < count; ++i) {
        genGrowLine(1000);
    }
}


void CBelarusPattern::genGrowRandom()
{
    m_growPoints.clear();
    m_growLines.clear();

    genGrowPoints(qrand() % 2 + 2);
    genGrowLines(qrand() % 2 + 2);

    while (m_bricks.sum() < m_bricks.length() / 2) {
        genGrowPoints(1);
        genGrowLines(2);
    }

    for (int i = 0; i < qrand() % 3; ++i) {
        genEtchDot(1000, true);
    }
}

void CBelarusPattern::genRandom()
{
    m_bricks.clear();
    //genEtchRandom();
    genGrowRandom();
}


void CBelarusPattern::updateSize(int w, int h)
{
    if (w > h) {
        m_width = h;
        m_height = h;
    } else {
        m_width = w;
        m_height = w;
    }
}

CAbstractPattern *CBelarusPattern::clone()
{
    CBelarusPattern *result = new CBelarusPattern;
    *result = *this;
    return result;
}

/* ------------ CBricks ---------------- */

CBricks::CBricks()
{
    setSize(5);
}

CBricks::CBricks(int size)
{
    setSize(size);
}

int CBricks::brick(int idx)
{
    if (idx >= 0 && idx < m_bricks.count()) {
        return m_bricks.at(idx);
    }
    return -1;
}

int CBricks::brick(int col, int row)
{
    return brick(col + m_size * row);
}

void CBricks::setBrick(int idx, int value)
{
    if (idx >= 0 && idx < m_bricks.count()) {
        m_bricks[idx] = value;
    }
}

void CBricks::setBrick(int col, int row, int value)
{
    setBrick(col + m_size * row, value);
}

void CBricks::setSize(int matrixSize)
{
    m_bricks.clear();
    if (matrixSize > 0) {
        m_size = matrixSize;
        m_bricks.fill(0, matrixSize * matrixSize);
    } else {
        m_size = 0;
    }
}

void CBricks::clear()
{
    m_bricks.fill(0);
}


bool CBricks::isEqual(CBricks & other)
{
    if (other.size() == m_size) {
        for (int i = 0; i < m_bricks.count(); ++i){
            if (m_bricks.at(i) != other.brick(i))
                return false;
        }
    } else {
        return false;
    }

    return true;
}

bool CBricks::hasClulsterAt(int x, int y, int w, int h)
{
    for (int cx = x; cx < x + w; ++cx) {
        for (int cy = y; cy < y + h; ++cy){
            if (brick(cx, cy) < 1) {
                return false;
            }
        }
    }
    return true;
}

bool CBricks::hasCluster(int w, int h)
{
    for (int x = 0; x <= m_size - w; ++x) {
        for (int y = 0; y <= m_size - h; ++y) {
            if (hasClulsterAt(x, y, w, h))
                return true;
        }
    }
    return false;
}

void CBricks::fillLine(QLineF line)
{
    if (qAbs(line.p1().x() - line.p2().x()) > qAbs(line.p1().y() - line.p2().y())) {
        int a = qRound(qMin(line.p1().x(), line.p2().x()));
        int b = qRound(qMax(line.p1().x(), line.p2().x()));
        int y = qRound(line.p1().y());
        for (int i = a; i <= b; ++i) {
            setBrick(i, y, 1);
        }
    } else {
        int a = qRound(qMin(line.p1().y(), line.p2().y()));
        int b = qRound(qMax(line.p1().y(), line.p2().y()));
        int x = qRound(line.p1().x());
        for (int i = a; i <= b; ++i) {
            setBrick(x, i, 1);
        }
    }
}

void CBricks::show()
{
    for (int row = 0; row < m_size; ++row) {
        QString rowStr;
        for (int col = 0; col < m_size; ++col) {
            rowStr.append(QString::number(brick(col, row)));
        }
        qDebug() << rowStr;
    }
}

int CBricks::sum()
{
    int result = 0;
    for (int i = 0; i < m_bricks.count(); ++i) {
        result = result + m_bricks.at(i);
    }
    return result;
}


