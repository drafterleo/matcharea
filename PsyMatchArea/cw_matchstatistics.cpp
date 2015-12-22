#include "cw_matchstatistics.h"
#include <QPainter>
#include <QtDebug>

CMatchStatistics::CMatchStatistics(QWidget *parent)
    : QWidget(parent)
{
    maxDist = 0;
    maxTime = 0;
}

void CMatchStatistics::setStatistics(const QVector <SStatItem> & otherStats)
{
    stats = otherStats;
    maxDist = 0;
    maxTime = 0;
    for (int i = 0; i < stats.count(); ++i) {
        if (stats[i].dist > maxDist)
            maxDist = stats[i].dist;
        if (stats[i].msecs > maxTime)
            maxTime = stats[i].msecs;
    }
    this->update();
}

void CMatchStatistics::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0x585858));
    painter.drawRect(this->rect());

    int slx = 5;
    int srx = width() - 5;
    int cy = height() - 5;

    painter.setPen(QPen(Qt::lightGray, 2));
    painter.drawLine(slx, cy, srx, cy);

    if (stats.count() == 0 || maxTime == 0 || maxDist == 0)
        return;

    qreal scaleTime = static_cast<qreal>(cy - 8)/maxTime;
    //qreal scaleDist = static_cast<qreal>(cy - 5)/maxDist;

    int dx = 2;
    int marks = 10;
    int dm = cy/marks;
    qreal dt = maxTime/(static_cast<qreal>(marks)*1000);

    QPen markPen(QColor(200, 200, 200, 150), 1, Qt::DotLine);
    for (int i = 0; i < marks; ++i){
        int dmy = cy - dm*i;
        painter.setPen(markPen);
        painter.drawLine(slx, dmy, srx, dmy);
        painter.setPen(QColor(Qt::lightGray));
        painter.drawText(QRect(srx - 60, dmy - 20, 60, 20),
                         Qt::AlignRight | Qt::AlignBottom,
                         QString::number(i*dt, 'f', 2) + "s");
    }

    //qreal lightFactor = 100.0/maxDist;
    for (int i = 0; i < stats.count(); ++i) {
        QColor threadColor;
        if (!stats[i].right) {
            threadColor = QColor(0xFF6347);
        } else {
            if (stats[i].tip)
                threadColor = QColor(0xFFD700);
            else
                threadColor = QColor(0x32CD32);
        }
        int x = slx + i*dx + dx/2;
        //threadColor = threadColor.darker(100 + qRound(stats[i].dist * lightFactor));
        //threadColor.setAlpha(50 + qRound(stats[i].dist * lightFactor));
        painter.setPen(QPen(threadColor, 2));
        painter.drawLine(x, cy - 2, x, cy - qRound(stats[i].msecs * scaleTime));

    }

    painter.end();
}
