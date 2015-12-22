#ifndef O_ABSTRACTSYMBOL_H
#define O_ABSTRACTSYMBOL_H

#include <QObject>
#include <QColor>
#include <QRect>
#include <QPainter>

class CAbstractSymbol : public QObject
{
    Q_OBJECT
public:
    explicit CAbstractSymbol(QObject *parent = 0);

    inline QColor patternColor() const {return m_patternColor;}
    inline QColor backgroundColor() const {return m_backgroundColor;}
    inline QColor frameColor() const {return m_frameColor;}
    inline void setPatternColor(const QColor & color) {m_patternColor = color;}
    inline void setFrameColor(const QColor & color) {m_frameColor = color;}
    inline void setBackgroundColor(const QColor & color) {m_backgroundColor = color;}

    inline QRect geometry() const {return m_geometry;}
    inline virtual void setGeometry (int x, int y, int w, int h) {m_geometry = QRect(x, y, w, h);}
    inline QPoint pos() const {return m_geometry.topLeft();}
    inline QPoint center() const {return m_geometry.center();}
    inline bool contains(const QPoint & point) const {return m_geometry.contains(point);}

    virtual void drawTo(QPainter *painter, const QPoint & drawPos = QPoint()) = 0;

private:
    QColor m_patternColor;
    QColor m_backgroundColor;
    QColor m_frameColor;
    QRect m_geometry;
};

#endif // O_ABSTRACTSYMBOL_H
