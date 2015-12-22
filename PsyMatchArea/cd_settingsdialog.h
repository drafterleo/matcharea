#ifndef CD_SETTINGSDIALOG_H
#define CD_SETTINGSDIALOG_H

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QPointer>
#include <QTime>
#include "cd_dialogpages.h"
#include "c_abstractpattern.h"

class CSettingsDialog: public QWidget
{
    Q_OBJECT
public:
    explicit CSettingsDialog(QWidget *parent = 0);

    inline int gridSize() const {return configPage->gridSpin->value();}
    inline PATTERN_TYPE patternType() const {return static_cast<PATTERN_TYPE>(configPage->patternTypeCombo->currentIndex());}
    inline bool staticMode() const {return configPage->staticModeRBtn->isChecked();}
    inline bool chaosMode() const {return configPage->chaosModeRBtn->isChecked();}
    inline bool dynamicMode() const {return configPage->dynamicModeRBtn->isChecked();}
    inline bool useTip() const {return configPage->tipActivateCheck->isChecked();}
    inline int tipTime() const {return configPage->tipTimeSpin->value();}
    inline int tipIntencity() const {return configPage->tipIntencitySpin->value();}
    inline bool swapSymbols() const {return configPage->swapCheck->isChecked();}
    inline bool thrillSymbols() const {return configPage->thrillCheck->isChecked();}
    inline bool squared() const {return configPage->squaredCheck->isChecked();}
    inline bool fullScreen() const {return configPage->fullScreenCheck->isChecked();}
    inline bool colorInerferenceActivated() const {return configPage->ciActivateCheck->isChecked();}
    inline int colorInerferenceOpaque() const {return configPage->ciOpaqueSpin->value();}
    inline int colorInerferenceInterval() const {return configPage->ciIntervalSpin->value();}
    inline bool coverInerferenceActivated() const {return configPage->coverActivateCheck->isChecked();}
    inline int coverInerferenceDuration() const {return configPage->coverDurationSpin->value();}
    inline bool hideCenterPattern() const {return configPage->hideCenterPatternCheck->isChecked();}

    inline void selectConfigPage() {contentsWidget->setCurrentRow(0);}
    inline void selectStatisticsPage() {contentsWidget->setCurrentRow(1);}

    inline void setGridSize(int size) {configPage->gridSpin->setValue(size);}
    inline void setPatternType(PATTERN_TYPE pt) {configPage->patternTypeCombo->setCurrentIndex(pt);}
    inline void setStaticMode(bool on) {configPage->staticModeRBtn->setChecked(on);}
    inline void setChaosMode(bool on) {configPage->chaosModeRBtn->setChecked(on);}
    inline void setDynamicMode(bool on) {configPage->dynamicModeRBtn->setChecked(on);}
    inline void setUseTip(bool on) {configPage->tipActivateCheck->setChecked(on);}
    inline void setTipTime(int time) {configPage->tipTimeSpin->setValue(time);}
    inline void setTipIntencity(int val) {configPage->tipIntencitySpin->setValue(val);}
    inline void setSwapSymbols(bool on) {configPage->swapCheck->setChecked(on);}
    inline void setThrillSymbols(bool on) {configPage->thrillCheck->setChecked(on);}
    inline void setFullScreen(bool on) {configPage->fullScreenCheck->setChecked(on);}
    inline void setSquared(bool on) {configPage->squaredCheck->setChecked(on);}
    inline void setColorInterferenceActivated(bool on) {configPage->ciActivateCheck->setChecked(on);}
    inline void setColorInterferenceOpaque(int val) {configPage->ciOpaqueSpin->setValue(val);}
    inline void setColorInterferenceInterval(int val) {configPage->ciIntervalSpin->setValue(val);}
    inline void setCoverInterferenceActivated(bool on) {configPage->coverActivateCheck->setChecked(on);}
    inline void setCoverInterferenceDuration(int val) {configPage->coverDurationSpin->setValue(val);}
    inline void setHideCenterPattern(bool on) {configPage->hideCenterPatternCheck->setChecked(on);}

    inline void setRightHits(int hits) {statisticsPage->rightHits->setText(QString("Right hits: %1").arg(hits));}
    inline void setWrongHits(int hits) {statisticsPage->wrongHits->setText(QString("Wrong hits: %1").arg(hits));}
    inline void setElapsedTime(const QTime & time) {statisticsPage->elapsedTime->setText(QString("Elapsed time: %1").arg(time.toString("hh:mm:ss")));}
    inline void setStatistics(const QVector <SStatItem> & stats) {statisticsPage->statistics->setStatistics(stats);}

public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

signals:
    void startPressed();
    void exitPressed();
    void fullScreenToggled(bool);
    void squaredToggled(bool);

protected:
    void createIcons();

    inline virtual QSize sizeHint() {return QSize(450, 500);}
    virtual void paintEvent(QPaintEvent *event);

private:
    QListWidget *contentsWidget;
    QStackedWidget *pagesWidget;
    CConfigPage *configPage;
    CStatisticsPage *statisticsPage;
};

#endif // CD_SETTINGSDIALOG_H
