#ifndef CW_MATCHAREA_H
#define CW_MATCHAREA_H

#include <QWidget>
#include <QVector>
#include <QTime>
#include <QTimeLine>
#include <QTimer>
#include <QPointer>
#include "c_matchglobal.h"
#include "co_patternsymbol.h"

class CMatchArea : public QWidget
{
    Q_OBJECT
public:
    explicit CMatchArea(QWidget *parent = 0);
    void setGrid(int size);
    void setPatternType(PATTERN_TYPE pt);
    void setChaosMode(bool on);
    void setDynamicMode(bool on);
    void setSquared(bool on = true);
    inline void setSwapSymbols(bool on) {swapSymbols = on;}
    inline void setThrillInterference(bool on) {thrillInterference = on;}
    inline void setHideCenterInterference(bool on) {hideCenterInterference = on;}
    inline void setUseTip(bool on) {useTip = on;}
    inline void setTipTime(int msecs) {tipTime = msecs;}
    inline void setTipIntencity(int val) {tipDarkIntencity = val;}
    void setColorInterference(bool on);
    void setColorInterferenceOpaque(int opaque) {colorInterferenceOpaque = opaque;}
    void setColorInterferenceInterval(int interval) {colorInterferenceInterval = interval;}
    inline void setCoverInterference(bool on) {coverInerference = on;}
    inline void setCoverInterferenceDuration(int val) {coverDuration = val;}

    inline int getRightHits() const {return rightHits;}
    inline int getWrongHits() const {return wrongHits;}
    inline const QTime & getElapsedTime() const {return matchTime;}
    inline const QVector <SStatItem> & getStatistics() const {return statistics;}

public slots:
    void startMatching();
    void stopMatching();

signals:
    void matchOver();

private slots:
    void tipTimerTick();
    void tipDarkTimerTick();
    void startBackgroundTimer(const QColor & color);
    void backgroundTimerTick();
    void dynamicTimerTick();
    void recolorizeTimerTick();
    void thrillTimerTick();
    void hideCenterTick();
    void coverFrameChanged(int);
    void coverFinished();
    void startCoverAnimation();

protected:
    void clearSymbols();
    void clearPatterns();
    CAbstractPattern *makePattern(PATTERN_TYPE pt);
    void makeSymbolsAndPatterns();
    void distributePatterns();
    void allocateSymbols();
    void allocateTable();
    void allocateChaos();
    void redisposeChaos();
    void allocateDynamic();
    void disposeDynamic();
    void moveDynamic();
    void refreshSymbols();
    void recolorizeSymbols();
    void thrillSymbols();
    void updateMatch();
    void symbolClicked(CPatternSymbol *symbol);
    CPatternSymbol *findSymbolByPattern(CAbstractPattern * pattern);
    void recalcCoverRounds();

    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    QPointer <CPatternSymbol> centralSymbol;
    CAbstractPattern *centralPattern;
    QVector <QPointer <CPatternSymbol> > symbols;
    QVector <CAbstractPattern *> patterns;
    QList <CAbstractPattern *> matchList;

    QVector <QRectF> chaosRects;
    QRectF chaosCenterRect;

    QVector <QRectF> dynamicRects;
    QRectF dynamicCenterRect;

    QVector <QPointF> dynamicVectors;

    PATTERN_TYPE patternType;

    int  gridSize;
    bool squared;
    bool chaosMode;
    bool dynamicMode;
    int  chaosScale;
    bool swapSymbols;
    bool matchStarted;

    bool useTip;
    QTimer *tipTimer;
    int tipTime;
    bool tipActivated;
    QTimer *tipDarkTimer;
    int tipDarkFactor;
    int tipDarkVecor;
    int tipDarkIntencity;
    QPointer <CPatternSymbol> tipSymbol;

    QTimer *recolorizeTimer;
    bool colorInterference;
    int  colorInterferenceOpaque;
    int  colorInterferenceInterval;

    QTimeLine *coverTimeLine;
    bool coverInerference;
    int  coverRadius;
    int  coverRounds;
    int  coverDuration;

    QTimer *thrillTimer;
    bool thrillInterference;
    int thrillInterval;

    QTimer *hideCenterTimer;
    bool hideCenterInterference;
    int  hideCenterTime;
    int  hideCenterTickCount;

    QTimer *backgroundTimer;

    QTimer *dynamicTimer;

    QColor backgroundColor;
    QColor symbolBorderColor;
    QColor symbolBackgroundColor;
    QColor symbolPatternColor;
    QColor centralBorderColor;
    QColor centralPatternColor;

    int rightHits;
    int wrongHits;
    QTime matchTime;
    QTime hitTime;

    int mouseHittedSymbolIdx;

    QVector <SStatItem> statistics;
};

#endif // CW_MATCHAREA_H
