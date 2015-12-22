#include "cw_runepatternedit.h"
#include <QPainter>

CRunePatternEdit::CRunePatternEdit(QWidget *parent)
    : CAbstractEdit(parent)
{
    setPattern(CRunePattern(3, 3));
}


void CRunePatternEdit::setPattern(const CRunePattern & dp)
{
    pattern = dp;
    updatePoints();
}

void CRunePatternEdit::addLink(const QPoint & cell_a, const QPoint & cell_b)
{
    pattern.addLink(cell_a, cell_b);
    updatePoints();
}

void CRunePatternEdit::addRandomLink()
{
    pattern.addRandomLink();
    updatePoints();
}

void CRunePatternEdit::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    updatePoints();
}

void CRunePatternEdit::updatePoints()
{
    points.clear();

    if (pattern.isEmpty())
        return;

    int cols = pattern.colCount();
    int rows = pattern.rowCount();
    points.fill(QPoint(0, 0), cols * rows);
    int w = width() / cols;
    int h = height() / rows;
    for (int c = 0; c < cols; ++c) {
        for (int r = 0; r < rows; ++r) {
             points[c + r * cols] = QRect(c * w, r * h, w, h).center();
        }
    }
    this->update();
}

void CRunePatternEdit::drawLinks(QPainter *painter)
{
    painter->setPen(QPen(Qt::cyan, 5));
    foreach (const CDotLink & link, pattern.getLinks()) {
        painter->drawLine(points.at(link.aDot), points.at(link.bDot));
    }
}

void CRunePatternEdit::genSolidLink(int bridges, int crosses)
{
    pattern.genSolidLink(bridges, crosses);
    this->update();
}


void CRunePatternEdit::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRect(this->rect());

    drawLinks(&painter);

    painter.setPen(QPen(Qt::cyan, 2));
    painter.setBrush(Qt::black);
    for (int i = 0; i < points.count(); ++ i) {
        painter.drawEllipse(points.at(i), 5, 5);
    }


    painter.end();
}
