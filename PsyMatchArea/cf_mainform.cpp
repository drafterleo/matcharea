#include <QApplication>
#include <QSettings>
#include <QDesktopWidget>
#include <QtDebug>
#include "cf_mainform.h"
#include "cd_dialogpages.h"

CMainForm::CMainForm(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(600, 600);

    settingsDialog = new CSettingsDialog(this);
    connect(settingsDialog, SIGNAL(startPressed()), SLOT(startMatching()));
    connect(settingsDialog, SIGNAL(exitPressed()), SLOT(close()));
    connect(settingsDialog, SIGNAL(fullScreenToggled(bool)), SLOT(fullScreenChecked(bool)));
    connect(settingsDialog, SIGNAL(squaredToggled(bool)), SLOT(squaredChecked(bool)));

    readSettings();

    matchArea = new CMatchArea(this);
        matchArea->setGrid(settingsDialog->gridSize());
        matchArea->setSquared(settingsDialog->squared());
    connect(matchArea, SIGNAL(matchOver()), SLOT(stopMatching()));

    settingsDialog->raise();
}

void CMainForm::readSettings()
{
    QSettings settings(QSettings::UserScope, "DrafterSoft", "PsyMatchArea");
    settingsDialog->setGridSize(settings.value("Grid Size", 5).toInt());
    settingsDialog->setPatternType(static_cast<PATTERN_TYPE>(settings.value("Pattern Type", 0).toInt()));
    settingsDialog->setStaticMode(settings.value("Static Mode", false).toBool());
    settingsDialog->setChaosMode(settings.value("Chaos Mode", false).toBool());
    settingsDialog->setDynamicMode(settings.value("Dyanmic Mode", false).toBool());
    settingsDialog->setUseTip(settings.value("Use Tip", true).toBool());
    settingsDialog->setTipTime(settings.value("Tip Time", 20).toInt());
    settingsDialog->setTipIntencity(settings.value("Tip Intencity", 50).toInt());
    settingsDialog->setThrillSymbols(settings.value("Thrill Symbols", false).toBool());
    settingsDialog->setSwapSymbols(settings.value("Swap Symbols", false).toBool());
    settingsDialog->setFullScreen(settings.value("Full Screen", false).toBool());
    settingsDialog->setSquared(settings.value("Squared", true).toBool());
    settingsDialog->setColorInterferenceActivated(settings.value("ciActivated", false).toBool());
    settingsDialog->setColorInterferenceOpaque(settings.value("ciOpaque", 50).toInt());
    settingsDialog->setColorInterferenceInterval(settings.value("ciInterval", 400).toInt());
    settingsDialog->setCoverInterferenceActivated(settings.value("Cover Activated", false).toBool());
    settingsDialog->setCoverInterferenceDuration(settings.value("Cover Duration", 1000).toInt());
    settingsDialog->setHideCenterPattern(settings.value("Hide Center Pattern", false).toBool());

    QPoint centeredPoint = QPoint(0, 0);
    if (qApp) {
        QRect screenRect = qApp->desktop()->screenGeometry();
        centeredPoint = QPoint((screenRect.width() - this->width())/2,
                               (screenRect.height() - this->height() - 50)/2);
    }

    QPoint winPos = settings.value("MainWindow Pos", centeredPoint).toPoint();
    QSize winSize = settings.value("MainWindow Size", QSize(700, 700)).toSize();

    move(winPos);
    resize(winSize);
}

void CMainForm::writeSettings()
{
    QSettings settings(QSettings::UserScope, "DrafterSoft", "PsyMatchArea");
    settings.setValue("Grid Size", settingsDialog->gridSize());
    settings.setValue("Pattern Type", settingsDialog->patternType());
    settings.setValue("Static Mode", settingsDialog->staticMode());
    settings.setValue("Chaos Mode", settingsDialog->chaosMode());
    settings.setValue("Dyanmic Mode", settingsDialog->dynamicMode());
    settings.setValue("Swap Symbols", settingsDialog->swapSymbols());
    settings.setValue("Thrill Symbols", settingsDialog->thrillSymbols());
    settings.setValue("Use Tip", settingsDialog->useTip());
    settings.setValue("Tip Time", settingsDialog->tipTime());
    settings.setValue("Tip Intencity", settingsDialog->tipIntencity());
    settings.setValue("Full Screen", settingsDialog->fullScreen());
    settings.setValue("Squared", settingsDialog->squared());
    settings.setValue("ciActivated", settingsDialog->colorInerferenceActivated());
    settings.setValue("ciOpaque", settingsDialog->colorInerferenceOpaque());
    settings.setValue("ciInterval", settingsDialog->colorInerferenceInterval());
    settings.setValue("Cover Activated", settingsDialog->coverInerferenceActivated());
    settings.setValue("Cover Duration", settingsDialog->coverInerferenceDuration());
    settings.setValue("Hide Center Pattern", settingsDialog->hideCenterPattern());

//    qDebug() << settings.value("Dyanmic Mode", false).toBool()
//             << settings.value("Static Mode", false).toBool();

    if (!this->isFullScreen()) {
        settings.setValue("MainWindow Pos", this->pos());
        settings.setValue("MainWindow Size", this->size());
    } else {
        settings.remove("MainWindow Pos");
        settings.remove("MainWindow Size");
    }
}

void CMainForm::resizeEvent(QResizeEvent *event)
{
    matchArea->setGeometry(this->rect());
    settingsDialog->move((width() - settingsDialog->width())/2,
                         (height() - settingsDialog->height())/2);
}

void CMainForm::startMatching()
{
    settingsDialog->hide();
    matchArea->setGrid(settingsDialog->gridSize());
    matchArea->setChaosMode(settingsDialog->chaosMode());
    matchArea->setDynamicMode(settingsDialog->dynamicMode());
    matchArea->setPatternType(settingsDialog->patternType());
    matchArea->setSwapSymbols(settingsDialog->swapSymbols());
    matchArea->setThrillInterference(settingsDialog->thrillSymbols());
    matchArea->setUseTip(settingsDialog->useTip());
    matchArea->setTipTime(settingsDialog->tipTime()*1000);
    matchArea->setTipIntencity(settingsDialog->tipIntencity());
    matchArea->setColorInterferenceOpaque(settingsDialog->colorInerferenceOpaque());
    matchArea->setColorInterferenceInterval(settingsDialog->colorInerferenceInterval());
    matchArea->setColorInterference(settingsDialog->colorInerferenceActivated());
    matchArea->setCoverInterference(settingsDialog->coverInerferenceActivated());
    matchArea->setCoverInterferenceDuration(settingsDialog->coverInerferenceDuration());
    matchArea->setHideCenterInterference(settingsDialog->hideCenterPattern());

    matchArea->setFocus();
    matchArea->startMatching();
}

void CMainForm::stopMatching()
{
    settingsDialog->setRightHits(matchArea->getRightHits());
    settingsDialog->setWrongHits(matchArea->getWrongHits());
    settingsDialog->setElapsedTime(matchArea->getElapsedTime());
    settingsDialog->setStatistics(matchArea->getStatistics());
    settingsDialog->selectStatisticsPage();
    settingsDialog->show();
}

void CMainForm::fullScreenChecked(bool checked)
{
    if (!this->isVisible())
        return;

    if (checked && !this->isFullScreen()) {
        this->showFullScreen();
    } else
    if (!checked && this->isFullScreen()) {
        this->showNormal();
    }
}

void CMainForm::squaredChecked(bool checked)
{
    if (!this->isVisible())
        return;

    matchArea->setSquared(checked);
}

void CMainForm::closeEvent(QCloseEvent *event)
{
    writeSettings();
}

bool CMainForm::isSettingsFullScreen() const
{
    return settingsDialog->fullScreen();
}
