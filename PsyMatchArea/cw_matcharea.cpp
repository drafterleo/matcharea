#include "cw_matcharea.h"
#include "c_runepattern.h"
#include "c_chinapattern.h"
#include "c_polygonpattern.h"
#include "c_strokepattern.h"
#include "c_rectanglespattern.h"
#include "c_belaruspattern.h"

#include <qmath.h>
#include <QPainter>
#include <QKeyEvent>
#include <QtDebug>

CMatchArea::CMatchArea(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(600, 600);
    patternType = ptDotPattern;
    centralSymbol = 0;
    centralPattern = 0;
    mouseHittedSymbolIdx = -1;
    chaosMode = false;
    dynamicMode = false;
    chaosScale = 1;
    squared = false;
    swapSymbols = true;
    matchStarted = false;
    backgroundColor = QColor(235, 235, 235);
    symbolBackgroundColor = Qt::transparent;
    symbolBorderColor = Qt::gray;
    symbolPatternColor = Qt::gray;
    centralBorderColor = Qt::darkGray;
    centralPatternColor = Qt::black;
    rightHits = 0;
    wrongHits = 0;
    useTip = true;
    tipActivated = false;
    tipDarkFactor = 200;
    tipDarkVecor = +1;
    tipDarkIntencity = 50;
    colorInterference = false;
    colorInterferenceOpaque = 20;
    colorInterferenceInterval = 100;
    coverInerference = false;
    coverRadius = 0;
    coverRounds = 0;
    coverDuration = 100;
    thrillInterference = false;
    thrillInterval = 100;
    hideCenterInterference = false;
    hideCenterTime = 10;
    hideCenterTickCount = 0;

    setAttribute(Qt::WA_OpaquePaintEvent);

    tipTime = 5000;
    tipTimer = new QTimer(this);
    connect(tipTimer, SIGNAL(timeout()), SLOT(tipTimerTick()));

    tipDarkTimer = new QTimer(this);
    connect(tipDarkTimer, SIGNAL(timeout()), SLOT(tipDarkTimerTick()));

    backgroundTimer = new QTimer(this);
    connect(backgroundTimer, SIGNAL(timeout()), SLOT(backgroundTimerTick()));

    dynamicTimer = new QTimer(this);
    dynamicTimer->setInterval(10);
    connect(dynamicTimer, SIGNAL(timeout()), SLOT(dynamicTimerTick()));

    recolorizeTimer = new QTimer(this);
    connect(recolorizeTimer, SIGNAL(timeout()), SLOT(recolorizeTimerTick()));

    coverTimeLine = new QTimeLine(1000, this);
    connect(coverTimeLine, SIGNAL(frameChanged(int)), SLOT(coverFrameChanged(int)));
    connect(coverTimeLine, SIGNAL(finished()), SLOT(coverFinished()));

    recolorizeTimer = new QTimer(this);
    connect(recolorizeTimer, SIGNAL(timeout()), SLOT(recolorizeTimerTick()));

    thrillTimer = new QTimer(this);
    connect(thrillTimer, SIGNAL(timeout()), SLOT(thrillTimerTick()));

    hideCenterTimer = new QTimer(this);
    hideCenterTimer->setInterval(10);
    connect(hideCenterTimer, SIGNAL(timeout()), SLOT(hideCenterTick()));

    setGrid(4);
}


void CMatchArea::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    allocateSymbols();
}

void CMatchArea::startMatching()
{
    matchList = QList<CAbstractPattern *>::fromVector(patterns);
    matchStarted = true;
    rightHits = 0;
    wrongHits = 0;
    tipActivated = false;
    statistics.clear();
    if (dynamicMode)
        dynamicTimer->start();
    if (colorInterference)
        recolorizeTimer->start(colorInterferenceInterval);
    if (coverInerference)
        startCoverAnimation();
    if (thrillInterference)
        thrillTimer->start(thrillInterval);
    if (hideCenterInterference) {
        hideCenterTickCount = 0;
        hideCenterTimer->start();
    }
    matchTime.start();
    updateMatch();
}

void CMatchArea::setGrid(int size)
{
    if (size >= 4) {
        gridSize = size;
        makeSymbolsAndPatterns();
        distributePatterns();
        if (chaosMode)
            redisposeChaos();
        if (dynamicMode)
            disposeDynamic();
        allocateSymbols();
    } else {
        qWarning("CMatchArea::setGridSize - too small size: %d", gridSize);
    }
}

void CMatchArea::setPatternType(PATTERN_TYPE pt)
{
    if (patternType != pt) {
        patternType = pt;
        setGrid(gridSize);
    }
}

void CMatchArea::setChaosMode(bool on)
{
    if (chaosMode != on) {
        chaosMode = on;
        dynamicMode = false;
        setGrid(gridSize);
    }
}

void CMatchArea::setDynamicMode(bool on)
{
    if (dynamicMode != on) {
        dynamicMode = on;
        chaosMode = false;
        setGrid(gridSize);
    }
}

void CMatchArea::setSquared(bool on)
{
    squared = on;
    if (symbols.count() > 0)
        allocateSymbols();
}

void CMatchArea::setColorInterference(bool on)
{
    if (colorInterference != on) {
        colorInterference = on;
        if (colorInterference) {
            recolorizeSymbols();
        } else {
            refreshSymbols();
        }
    }
}

void CMatchArea::clearSymbols()
{
    for (int i = 0; i < symbols.count(); ++i) {
        delete symbols[i];
    }
    symbols.clear();

    delete centralSymbol;
    centralSymbol = 0;
}

void CMatchArea::clearPatterns()
{
    for (int i = 0; i < patterns.count(); ++i) {
        delete patterns[i];
    }
    patterns.clear();

    delete centralPattern;
    centralPattern = 0;
}

CAbstractPattern *CMatchArea::makePattern(PATTERN_TYPE pt)
{
    switch (pt)
    {
    case ptDotPattern:
        return new CRunePattern(3, 3);
    case ptChinaPattern:
        return new CChinaPattern();
    case ptPolygonPattern:
        return new CPolygonPattern();
    case ptStrokePattern:
        return new CStrokePattern;
    case ptRectanglesPattern:
        return new CRectanglesPattern();
    case ptBelarusPattern:
        return new CBelarusPattern(5);
    default:
        return new CRunePattern(3, 3);
    }
}

void CMatchArea::makeSymbolsAndPatterns()
{
    clearSymbols();
    clearPatterns();

    int symbolCount;

    if (chaosMode) {
        int reducedGrid = gridSize - 1;
        if (reducedGrid%2 > 0)
            symbolCount = reducedGrid * reducedGrid - 1;
        else
            symbolCount = reducedGrid * reducedGrid - 4;
    } else {
        if (gridSize%2 > 0)
            symbolCount = gridSize * gridSize - 9;
        else
            symbolCount = gridSize * gridSize - 4;
    }

    for (int i = 0; i < symbolCount; ++i) {
        //CDotPattern *pattern = new CDotPattern(3, 3);
        //CChinaPattern *pattern = new CChinaPattern;
        CAbstractPattern *pattern = makePattern(patternType);
        bool haveRepeatedPattern = true;
        while (haveRepeatedPattern) {
            pattern->genRandom();
            haveRepeatedPattern = false;
            for (int j = 0; j < patterns.count(); j++) {
                if (pattern->isSimilar(patterns[j])) {
                    haveRepeatedPattern = true;
                    break;
                }
            }
        }
        CPatternSymbol *symbol = new CPatternSymbol(this);
        patterns.append(pattern);
        symbols.append(symbol);
    }

    centralSymbol = new CPatternSymbol(this);
    centralSymbol->setPatternColor(centralPatternColor);
    centralSymbol->setFrameColor(centralBorderColor);

    if (chaosMode)
        redisposeChaos();
    if (dynamicMode)
        disposeDynamic();
}

void CMatchArea::distributePatterns()
{
    if (patterns.count() > 0 && patterns.count() == symbols.count() && centralSymbol) {
        QList<CAbstractPattern *> list = QList<CAbstractPattern *>::fromVector(patterns);
        centralSymbol->setPattern(list.at(qrand()%list.count()));
        for (int i = 0; i < symbols.count(); ++i) {
            if (list.count() > 0) {
                symbols[i]->setPattern(list.takeAt(qrand()%list.count()));
            }
        }
    }
}

void CMatchArea::refreshSymbols()
{
    if (patterns.count() > 0 && patterns.count() == symbols.count()) {
        QList<CAbstractPattern *> list;
        if (swapSymbols) {
            list = QList<CAbstractPattern *>::fromVector(patterns);
        }
        for (int i = 0; i < symbols.count(); ++i) {
            if (swapSymbols) {
                symbols[i]->setPattern(list.takeAt(qrand()%list.count()));
            }
            symbols[i]->setPatternColor(symbolPatternColor);
            symbols[i]->setFrameColor(symbolBorderColor);
            if (!colorInterference)
                symbols[i]->setBackgroundColor(symbolBackgroundColor);
        }
        if (chaosMode) {
            redisposeChaos();
            allocateChaos();
        }
    }
    this->update();
}

void CMatchArea::moveDynamic()
{
    dynamicRects.append(dynamicCenterRect);
    dynamicVectors.append(QPointF(0, 0));

    for (int i = 0; i < dynamicRects.count(); ++i) {
        QPointF v = dynamicVectors.at(i);
        QRectF r = dynamicRects.at(i).translated(v);

        for (int j = 0; j < dynamicRects.count(); ++j) {
            if (j == i) continue;
            QRectF jr = dynamicRects.at(j);
            if (r.intersects(jr)) {
                if (r.bottom() > jr.top() || r.top() < jr.bottom())
                    v.setY(-1 * v.y());
                if (r.right() > jr.left() || r.left() < jr.right())
                    v.setX(-1 * v.x());
                r = dynamicRects.at(i);
                //break;
            }
        }

        if (r.left() < 0 || r.right() > gridSize) {
            v.setX(-1 * v.x());
            r = dynamicRects.at(i);
        }
        if (r.top() < 0 || r.bottom() > gridSize) {
            v.setY(-1 * v.y());
            r = dynamicRects.at(i);
        }

        dynamicRects[i] = r;
        dynamicVectors[i] = v;
    }


    // forigi la centra rektangulo
    dynamicRects.remove(dynamicRects.count() - 1);
    dynamicVectors.remove(dynamicVectors.count() - 1);

    allocateDynamic();
}

void CMatchArea::recolorizeSymbols()
{
    for (int i = 0; i < symbols.count(); ++i) {
        int r;
        int g;
        int b;
        forever {
            r = qrand()%200 + 50;
            g = qrand()%200 + 50;
            b = qrand()%200 + 50;
            int dc = 20;
            int r1 = r - dc;
            int r2 = r + dc;
            int g1 = g - dc;
            int g2 = g + dc;
            int b1 = b - dc;
            int b2 = b + dc;
            if ((b > r1 && b < r2) || (g > r1 && g < r2) ||
                (b > g1 && b < g2) || (r > g1 && r < g2) ||
                (r > b1 && r < b2) || (g > b1 && g < b2))
                continue;
            break;
        }
        symbols[i]->setBackgroundColor(QColor(r, g, b, colorInterferenceOpaque));
    }
    this->update();
}

void CMatchArea::allocateSymbols()
{
    tipDarkTimer->stop();
    recalcCoverRounds();

    if (chaosMode)
        allocateChaos();
    else
    if (dynamicMode)
        allocateDynamic();
    else
        allocateTable();
}

void CMatchArea::allocateDynamic()
{
    if (gridSize <= 0 || symbols.count() == 0 || !centralSymbol)
        return;

    int w = width();
    int h = height();

    int sx = (w%gridSize)/2;
    int sy = (h%gridSize)/2;

    if (squared) {
        if (width() > height()) {
            w = height();
            sx = (width() - w)/2;
        } else
        if (height() > width()) {
            h = width();
            sy = (height() - h)/2;
        }
    }

    int kx = w/gridSize;
    int ky = h/gridSize;

    for (int i = 0; i < symbols.count(); ++i) {
        if (i < dynamicRects.count()) {
            QRectF r = dynamicRects.at(i);
            symbols[i]->setGeometry(sx + qRound(r.x()*kx),
                                    sy + qRound(r.y()*ky),
                                    qRound(r.width()*kx),
                                    qRound(r.height()*ky));
            symbols[i]->setPatternColor(symbolPatternColor);
            symbols[i]->setFrameColor(symbolBorderColor);
        }
    }

    QRectF r = dynamicCenterRect;
    centralSymbol->setGeometry(sx + qRound(r.x()*kx),
                               sy + qRound(r.y()*ky),
                               qRound(r.width()*kx),
                               qRound(r.height()*ky));
    //centralSymbol->setPatternCaliber(7.0);
//    centralSymbol->setPatternColor(centralPatternColor);
//    centralSymbol->setFrameColor(centralBorderColor);

    this->update();
}

int rndSign()
{
    if (qrand()%2 == 0)
        return 1;
    else
        return  -1;
}

void CMatchArea::disposeDynamic()
{
    int cx = gridSize/2 - 1;
    int cy = gridSize/2 - 1;
    int cw = 2;
    int ch = 2;
    if (gridSize % 2 > 0) {
        cw = 3;
        ch = 3;
    }

    qreal scale = 0.7;

    qreal wx = 1.0;
    qreal hy = 1.0;
    qreal sx = (wx - wx*scale)/2;
    qreal sy = (hy - hy*scale)/2;

    dynamicCenterRect = QRectF((static_cast<qreal>(gridSize) - wx*scale)/2,
                              (static_cast<qreal>(gridSize) - hy*scale)/2, wx*scale, hy*scale);


    dynamicRects.clear();
    dynamicVectors.clear();
    int currSymbol = 0;
    for (int x = 0; x < gridSize; ++x) {
        for (int y = 0; y < gridSize; ++y) {
            if (x >= cx && x < cx + cw && y >= cy && y < cy + ch)
                continue;
            if (currSymbol >= symbols.count()) {
                break;
            } else {

                dynamicRects.append(QRectF(x * wx + sx, y * hy + sy, wx*scale, hy*scale));
                dynamicVectors.append(QPointF(rndSign()*0.007, rndSign()*0.007));
                currSymbol++;
            }
        }
    }
}

void CMatchArea::allocateChaos()
{
    if (gridSize <= 0 || symbols.count() == 0 || !centralSymbol)
        return;

    int w = width();
    int h = height();

    int sx = (w%gridSize)/2;
    int sy = (h%gridSize)/2;

    if (squared) {
        if (width() > height()) {
            w = height();
            sx = (width() - w)/2;
        } else
        if (height() > width()) {
            h = width();
            sy = (height() - h)/2;
        }
    }

    int kx = w/gridSize;
    int ky = h/gridSize;

    for (int i = 0; i < symbols.count(); ++i) {
        if (i < chaosRects.count()) {
            QRectF r = chaosRects.at(i);
            symbols[i]->setGeometry(sx + qRound(r.x()*kx),
                                    sy + qRound(r.y()*ky),
                                    qRound(r.width()*kx),
                                    qRound(r.height()*ky));
            symbols[i]->setPatternColor(symbolPatternColor);
            symbols[i]->setFrameColor(symbolBorderColor);
        }
    }

    QRectF r = chaosCenterRect;
    centralSymbol->setGeometry(sx + qRound(r.x()*kx),
                               sy + qRound(r.y()*ky),
                               qRound(r.width()*kx),
                               qRound(r.height()*ky));
    //centralSymbol->setPatternCaliber(7.0);
//    centralSymbol->setPatternColor(centralPatternColor);
//    centralSymbol->setFrameColor(centralBorderColor);

    this->update();
}

void CMatchArea::redisposeChaos()
{
    if (gridSize <= 0 || symbols.count() == 0 || !centralSymbol)
        return;

    int reducedGrid = gridSize - 1;
    qreal kf = static_cast<qreal>(gridSize)/reducedGrid;
    int cx;
    int cy;
    int cxd;
    int cyd;
    if (reducedGrid%2 > 0) {
        cx = reducedGrid/2;
        cy = reducedGrid/2;
        cxd = cx;
        cyd = cy;
    } else {
        cx = reducedGrid/2 - 1;
        cy = reducedGrid/2 - 1;
        cxd = cx + 1;
        cyd = cy + 1;
    }

    chaosRects.clear();
    chaosCenterRect = QRectF((static_cast<qreal>(gridSize) - 1)/2,
                             (static_cast<qreal>(gridSize) - 1)/2, 1, 1);

    int symbolIdx = 0;
    for (int x = 0; x < reducedGrid; ++x)
        for (int y = 0; y < reducedGrid; ++y) {
            if (x >= cx && x <= cxd && y >= cy && y <= cyd)
                continue;
            if (symbolIdx < symbols.count()) {
                qreal symSize = static_cast<qreal>(qrand()%40 + 60) / 100;
                qreal perX = static_cast<qreal>(qrand()%100 + 1)/100;
                qreal perY = static_cast<qreal>(qrand()%100 + 1)/100;
                qreal dx = (1 - symSize)*perX + (kf - 1)/2;
                qreal dy = (1 - symSize)*perY + (kf - 1)/2;
                QRectF r = QRectF(kf*(x + dx), kf*(y + dy), symSize, symSize);
                chaosRects.append(r);
                symbolIdx ++;
            }
        }
}

void CMatchArea::allocateTable()
{
    if (gridSize <= 0 || symbols.count() == 0 || !centralSymbol)
        return;

    int w = width();
    int h = height();

    int sx = (w%gridSize)/2;
    int sy = (h%gridSize)/2;

    if (squared) {
        if (width() > height()) {
            w = height();
            sx = (width() - w)/2;
        } else
        if (height() > width()) {
            h = width();
            sy = (height() - h)/2;
        }
    }

    int wx = w/gridSize;
    int hy = h/gridSize;

    int cx = gridSize/2 - 1;
    int cy = gridSize/2 - 1;
    int cw = 2;
    int ch = 2;
    int csx = wx/2;
    int csy = hy/2;
    if (gridSize % 2 > 0) {
        cw = 3;
        ch = 3;
        csx = wx;
        csy = hy;
    }

    int currSymbol = 0;
    for (int x = 0; x < gridSize; ++x) {
        for (int y = 0; y < gridSize; ++y) {
            if (x >= cx && x < cx + cw && y >= cy && y < cy + ch)
                continue;
            if (currSymbol >= symbols.count()) {
                break;
            } else {
                symbols[currSymbol]->setGeometry(x * wx + sx, y * hy + sy, wx, hy);
                symbols[currSymbol]->setPatternColor(symbolPatternColor);
                symbols[currSymbol]->setFrameColor(symbolBorderColor);
                currSymbol++;
            }
        }
    }

    centralSymbol->setGeometry(cx * wx + sx + csx, cy * hy + sy + csy, wx, hy);
    //centralSymbol->setPatternCaliber(7.0);
//    centralSymbol->setPatternColor(centralPatternColor);
//    centralSymbol->setFrameColor(centralBorderColor);

    this->update();
}

void CMatchArea::tipTimerTick()
{
    //distributePatterns();
    if (centralSymbol && matchStarted && tipSymbol) {
        //tipSymbol->setBorderColor(QColor(0xFF7474));
        //tipSymbol->setBackgroundColor(QColor(0xFF, 0x74, 0x74, 5));
        //QColor color = tipSymbol->getPatternColor();
        //tipSymbol->setPatternColor(color.darker(300));
        tipActivated = true;
        tipDarkFactor = 100;
        tipDarkVecor = +1;
        tipDarkTimer->start(50);
        this->update();
    }
    tipTimer->stop();
}

void CMatchArea::tipDarkTimerTick()
{
    if (tipActivated && tipSymbol) {
        if (tipDarkFactor > 100 + tipDarkIntencity)
            tipDarkVecor = -1;
        if (tipDarkFactor < 100)
            tipDarkVecor = +1;
        tipDarkFactor += tipDarkVecor*2;
        tipSymbol->setPatternColor(symbolPatternColor.darker(tipDarkFactor));
        this->update();
    } else {
        if (tipActivated)
            tipSymbol = findSymbolByPattern(centralSymbol->getPattern());
        tipDarkTimer->stop();
    }
}

void CMatchArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    /* draw background */
    painter.setPen(Qt::NoPen);
    painter.setBrush(backgroundColor);
    painter.drawRect(this->rect());

    /* draw symbols */
    for (int i = 0; i < symbols.count(); ++i) {
        symbols[i]->drawTo(&painter, symbols[i]->pos());
    }
    if (centralSymbol) {
        centralSymbol->drawTo(&painter, centralSymbol->pos());
    }

    /* draw cover */
    if (coverInerference) {
        for (int i = 0; i < coverRounds; ++i) {
            painter.setPen(QPen(QColor(100, 100, 100, 100), 20));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(this->rect().center(), coverRadius + i*100, coverRadius + i*100);
        }
    }

    painter.end();
}

void CMatchArea:: stopMatching()
{
    matchStarted = false;
    //centralSymbol->setPattern(CDotPattern(3, 3));
    tipTimer->stop();
    tipDarkTimer->stop();
    coverTimeLine->stop();
    thrillTimer->stop();
    dynamicTimer->stop();
    QTime elapsedTime(0, 0, 0, 0);
    matchTime = elapsedTime.addMSecs(matchTime.elapsed());
    emit matchOver();
}

void CMatchArea::updateMatch()
{
    if (!matchStarted)
        return;

    if(matchList.count() == 0) {
        stopMatching();
        return;
    }

    if (matchList.count() > 0) {
        centralSymbol->setPattern(0);
        delete centralPattern;
        centralPattern = matchList.takeAt(qrand()%matchList.count())->clone();
        centralSymbol->setPattern(centralPattern);
        centralSymbol->setPatternScale(1.0);
        tipSymbol = 0;
        tipActivated = false;
        tipDarkTimer->stop();
        tipTimer->stop();
        if (hideCenterInterference) {
            hideCenterTickCount = 0;
            centralSymbol->setPatternColor(centralPatternColor);
        }

        refreshSymbols();

        tipSymbol = findSymbolByPattern(centralSymbol->getPattern());
        hitTime.start();
        if (useTip)
            tipTimer->start(tipTime);
    }
}

CPatternSymbol * CMatchArea::findSymbolByPattern(CAbstractPattern * pattern)
{
    if (pattern) {
        for (int i = 0; i < symbols.count(); ++i){
            if (symbols[i]->getPattern()->isEqual(pattern)) {
                return symbols[i];
            }
        }
    }
    return 0;
}

void CMatchArea::symbolClicked(CPatternSymbol *symbol)
{
    if (matchStarted && symbol && centralPattern) {
        SStatItem stat;
        if (symbol->getPattern()->isEqual(centralPattern)) {
            rightHits ++;
            stat.right = true;
            //qDebug() << "right";
        } else {
            wrongHits ++;
            startBackgroundTimer(QColor(0xFF, 0x74, 0x74));
            stat.right = false;
            //qDebug() << "wrong";
        }

        if (tipActivated)
            stat.tip = true;

        stat.msecs = hitTime.elapsed();

        qreal dx = symbol->center().x() - centralSymbol->center().x();
        qreal dy = symbol->center().y() - centralSymbol->center().y();
        stat.dist = qRound(qSqrt(dx*dx + dy*dy));

        statistics.append(stat);

        updateMatch();
    }
}

void CMatchArea::mousePressEvent(QMouseEvent *event)
{
    mouseHittedSymbolIdx = -1;
    if (matchStarted && event->button() == Qt::LeftButton) {
        for (int i = 0; i < symbols.count(); ++i) {
            if (symbols.at(i)->contains(event->pos())) {
                mouseHittedSymbolIdx = i;
                break;
            }
        }
        if (centralSymbol->contains(event->pos()) && hideCenterInterference) {
            centralSymbol->setPatternColor(centralPatternColor);
            hideCenterTickCount = 0;
            this->update();
        }
    }
}

void CMatchArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (matchStarted && mouseHittedSymbolIdx >=0 && event->button() == Qt::LeftButton) {
        int releasedIdx = -1;
        for (int i = 0; i < symbols.count(); ++i) {
            if (symbols.at(i)->contains(event->pos())) {
                releasedIdx = i;
                break;
            }
        }
        if (mouseHittedSymbolIdx == releasedIdx) {
            symbolClicked(symbols.at(mouseHittedSymbolIdx));
        }
    }
}

void CMatchArea::startBackgroundTimer(const QColor & color)
{
    backgroundColor = color;
    this->update();
    backgroundTimer->start(100);
}

void CMatchArea::backgroundTimerTick()
{
    backgroundColor = QColor(235, 235, 235);
    this->update();
    backgroundTimer->stop();
}

void CMatchArea::dynamicTimerTick()
{
    moveDynamic();
}


void CMatchArea::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && matchStarted) {
        stopMatching();
    }
}

void CMatchArea::recolorizeTimerTick()
{
    if (matchStarted && colorInterference) {
        recolorizeSymbols();
    } else {
        recolorizeTimer->stop();
    }
}

void CMatchArea::recalcCoverRounds()
{
    int maxCoverRadius;
    if (width() > height())
        maxCoverRadius = width()/2;
    else
        maxCoverRadius = height()/2;
    coverRounds = maxCoverRadius/100 + 1;
}

void CMatchArea::startCoverAnimation()
{
    if (matchStarted && coverInerference) {
        recalcCoverRounds();
        coverTimeLine->setFrameRange(100, 0);
        coverTimeLine->setDuration(coverDuration);
        coverTimeLine->setLoopCount(0);
        coverTimeLine->start();
    } else {
        coverTimeLine->stop();
    }
}

void CMatchArea::coverFrameChanged(int frame)
{
    coverRadius = frame;
    this->update();
}

void CMatchArea::coverFinished()
{
}

void CMatchArea::thrillTimerTick()
{
    if (matchStarted && thrillInterference) {
        thrillSymbols();
    } else {
        thrillTimer->stop();
    }
}

void CMatchArea::thrillSymbols()
{
    for (int i = 0; i < symbols.count(); ++i) {
        qreal scale = 0.7 + static_cast<qreal>(qrand()%30)/100;
        symbols[i]->setPatternScale(scale);
    }
    //centralSymbol->setPatternScale(1.0);
    this->update();
}

void CMatchArea::hideCenterTick()
{
    if (matchStarted && hideCenterInterference) {
        hideCenterTickCount++;
        if (hideCenterTickCount*hideCenterTimer->interval() >= hideCenterTime) {
            QColor color = centralSymbol->patternColor();
            if (color.alpha() > 0) {
                int newAlpha = color.alpha() - 2;
                if (newAlpha < 0) newAlpha = 0;
                color.setAlpha(newAlpha);
                centralSymbol->setPatternColor(color);
                this->update();
            }
        }
    } else {
        hideCenterTimer->stop();
    }
}

