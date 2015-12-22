#include "c_runepattern.h"
#include <QtDebug>
#include <QLineF>

CRunePattern::CRunePattern()
    : CAbstractPattern()
{
    initMemberValues();
    setRate(3, 3);
}

CRunePattern::CRunePattern(int c, int r)
    : CAbstractPattern()
{
    initMemberValues();
    setRate(c, r);
}


void CRunePattern::initMemberValues(void)
{
    setPatternType(ptDotPattern);
    m_width = 0;
    m_height = 0;
    m_patternCaliber = 2.0;
}

CRunePattern::~CRunePattern()
{
    //qDebug() << "~CDotPattern()";
    //links.clear();
}

void CRunePattern::setRate(int c, int r)
{
    m_links.clear();
    if (c > 0 && r > 0) {
        m_cols = c;
        m_rows = r;
    } else {
        m_cols = 0;
        m_rows = 0;
    }
}

bool CRunePattern::isCellValid(const QPoint & cell) const
{
    if (cell.x() >= 0 && cell.y() >= 0 && cell.x() < m_cols && cell.y() < m_rows) {
        return true;
    }
    return false;
}

int CRunePattern::indexOfCell(const QPoint & cell) const
{
    if (isCellValid(cell)) {
        return cell.x() + cell.y() * m_cols;
    }
    qWarning("CDotPattern::indexOfCell: cell[%d, %d] out of pattern", cell.x(), cell.y());
    return -1;
}

QPoint CRunePattern::pointOfDot(int dotIdx) const
{
    if (dotIdx >= 0 && dotIdx < m_cols * m_rows) {
        return QPoint(dotIdx % m_cols, dotIdx / m_cols);
    }
    return QPoint(-1, -1); //invalid cell
}

int CRunePattern::colByDot(int dotIdx) const
{
    if (dotIdx >= 0 && dotIdx < m_cols * m_rows) {
        return dotIdx % m_cols;
    }
    return -1;
}

int CRunePattern::rowByDot(int dotIdx) const
{
    if (dotIdx >= 0 && dotIdx < m_cols * m_rows) {
        return dotIdx / m_cols;
    }
    return -1;
}

bool CRunePattern::hasLink(const CDotLink & link) const
{
    for(int i = 0; i < m_links.count(); ++i){
        CDotLink iLink = m_links.at(i);
        if ((iLink.aDot == link.aDot && iLink.bDot == link.bDot) ||
            (iLink.aDot == link.bDot && iLink.bDot == link.aDot))
        {
            return true;
        }
    }
    return false;
}

bool CRunePattern::isEqual(CAbstractPattern * other)
{
    CRunePattern *pattern = dynamic_cast <CRunePattern *> (other);
    if (pattern) {
        for(int i = 0; i < m_links.count(); ++i){
            if (!pattern->hasLink(m_links.at(i)))
                return false;
        }
        return true;
    }
    return false;
}

bool CRunePattern::isLinksCrossed(const CDotLink & aLink, const CDotLink & bLink) const
{
    QLineF aLine = QLineF(pointOfDot(aLink.aDot), pointOfDot(aLink.bDot));
    QLineF bLine = QLineF(pointOfDot(bLink.aDot), pointOfDot(bLink.bDot));

    return (aLine.intersect(bLine, NULL) == QLineF::BoundedIntersection);
}

bool CRunePattern::hasCrossLink(const CDotLink & link) const
{
    for(int i = 0; i < m_links.count(); ++i){
        CDotLink iLink = m_links.at(i);
        if (iLink.aDot != link.aDot && iLink.bDot != link.bDot &&
            iLink.aDot != link.bDot && iLink.bDot != link.aDot &&
            isLinksCrossed(link, iLink))
        {
            return true;
        }
    }
    return false;
}

bool CRunePattern::addLink(const QPoint & cell_a, const QPoint & cell_b)
{
    int dot_a = indexOfCell(cell_a);
    int dot_b = indexOfCell(cell_b);

    if (dot_a >= 0 && dot_b >= 0 && dot_a != dot_b) {
        CDotLink link = CDotLink(dot_a, dot_b);
        if (!m_links.contains(link)) {
            m_links.append(link);
            return true;
        }
    }
    return false;
}

void CRunePattern::addRandomLink()
{
    addLink(QPoint(qrand()%m_cols, qrand()%m_rows), QPoint(qrand()%m_cols, qrand()%m_rows));
}

void CRunePattern::genSolidLink(int linkCount, int crossesCount)
{
    if (this->isEmpty())
        return;

    int broadCharge = 5;

    while (broadCharge) {
        m_links.clear();

        QVector <int> dots;
        int currDot = qrand()%(m_cols*m_rows);

        for (int i = 0; i < linkCount; ++i)
        {
            dots.append(currDot);

            int crossCharge = 5;
            while (crossCharge) {
                int nextDotCharge = 5;
                int nextDot = qrand()%(m_cols*m_rows);
                while (nextDotCharge && dots.contains(nextDot)) {
                    nextDot = qrand()%(m_cols*m_rows);
                    nextDotCharge --;
                }

                if (nextDotCharge) {
                    CDotLink newLink = CDotLink(currDot, nextDot);
                    if (this->hasCrossLink(newLink)) {
                        if (crossesCount) {
                            crossesCount --;
                        } else {
                            crossCharge --;
                            continue; // find another link that is'nt crosses with others
                        }
                    }
                    m_links.append(newLink);
                    currDot = nextDot;
                }
                break;
            }
        }

        int maxRow = -1;
        int minRow = m_rows;
        int maxCol = -1;
        int minCol = m_rows;
        for (int d = 0; d < dots.count(); ++d) {
            if (rowByDot(dots.at(d)) < minRow)
                minRow = rowByDot(dots.at(d));
            if (rowByDot(dots.at(d)) > maxRow)
                maxRow = rowByDot(dots.at(d));
            if (colByDot(dots.at(d)) < minCol)
                minCol = colByDot(dots.at(d));
            if (colByDot(dots.at(d)) > maxCol)
                maxCol = colByDot(dots.at(d));
        }

        if (minCol == 0 && minRow == 0 && maxRow == m_rows - 1 && maxCol == m_cols - 1)
            break;

        broadCharge --;
        continue;
    }
}


void CRunePattern::updateSize(int w, int h)
{
    m_width = w;
    m_height = h;

    m_points.fill(QPoint(0, 0), m_cols * m_rows);
    int iw = w / m_cols;
    int ih = h / m_rows;
    for (int c = 0; c < m_cols; ++c) {
        for (int r = 0; r < m_rows; ++r) {
            m_points[c + r * m_cols] = QRect(c * iw, r * ih, iw, ih).center();
        }
    }

    qreal d = h;
    if (w > h)
        d = w;

    m_patternCaliber = qBound(4.0, d/20, 7.0);
    if (w < 60 || h < 60)
        m_patternCaliber = 3.0;
}

void CRunePattern::genRandom()
{
    this->genSolidLink(5, 1);
}

void CRunePattern::drawTo(QPainter *painter, const QRect & drawRect, const QColor & color)
{


    QRect paintRect = QRect(drawRect.left() + (drawRect.width() - m_width)/2,
                            drawRect.top() + (drawRect.height() - m_height)/2,
                            m_width, m_height);

//    QTransform savedTransform = painter->transform();
//    int tx = paintRect.center().x();
//    int ty = paintRect.center().y();
//    paintRect.translate(-tx, -ty);
//    painter->translate(tx, ty);
//    painter->rotate(45);



    /* draw links */
    painter->setPen(QPen(color, m_patternCaliber));
    for (int i = 0; i < m_links.count(); ++i){
        const CDotLink & link = m_links[i];
        painter->drawLine(paintRect.topLeft() + m_points[link.aDot], paintRect.topLeft() + m_points[link.bDot]);
    }

    /* draw points */
    painter->setPen(QPen(color, 2));
    painter->setBrush(Qt::transparent);
    for (int i = 0; i < m_points.count(); ++ i) {
        int radius = qRound(m_patternCaliber/2);
        painter->drawEllipse(paintRect.topLeft() + m_points[i], radius, radius);
    }

//    painter->setTransform(savedTransform);

}

CAbstractPattern *CRunePattern::clone()
{
    CRunePattern *result = new CRunePattern;
    *result = *this;
    return result;
}

