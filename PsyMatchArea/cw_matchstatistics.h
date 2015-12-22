#ifndef CW_MATCHSTATISTICS_H
#define CW_MATCHSTATISTICS_H

#include <QWidget>
#include "c_matchglobal.h"

class CMatchStatistics : public QWidget
{
    Q_OBJECT
public:
    explicit CMatchStatistics(QWidget *parent = 0);

    void setStatistics(const QVector <SStatItem> & otherStats);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    QVector <SStatItem> stats;
    int maxDist;
    int maxTime;

};

#endif // CW_MATCHSTATISTICS_H
