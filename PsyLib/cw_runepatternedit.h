#ifndef CW_RUNEPATTERN_H
#define CW_RUNEPATTERN_H

#include "c_runepattern.h"
#include "cw_abstractedit.h"

class CRunePatternEdit : public CAbstractEdit
{
    Q_OBJECT
public:
    explicit CRunePatternEdit(QWidget *parent = 0);

   void setPattern(const CRunePattern & dp);
   inline const CRunePattern & getPattern() {return pattern;}

   void addLink(const QPoint & cell_a, const QPoint & cell_b);
   void addRandomLink();
   void genSolidLink(int bridges, int crosses);

protected:
   void updatePoints();
   void drawLinks(QPainter *painter);

   virtual void paintEvent(QPaintEvent *event);
   virtual void resizeEvent(QResizeEvent *event);

private:
    CRunePattern pattern;
    QVector <QPoint> points;
};

#endif // CW_RUNEPATTERN_H
