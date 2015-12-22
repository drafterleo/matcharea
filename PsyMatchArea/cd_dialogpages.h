#ifndef CD_DIALOGPAGES_H
#define CD_DIALOGPAGES_H

#include <QWidget>
#include <QCheckBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include "cw_matchstatistics.h"

class CDialogPage : public QWidget
{
    Q_OBJECT
public:
    explicit CDialogPage(QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent *event);
};

class CConfigPage : public CDialogPage
{
    Q_OBJECT
public:
    explicit CConfigPage(QWidget *parent = 0);

    QSpinBox  *gridSpin;
    QRadioButton *staticModeRBtn;
    QRadioButton *chaosModeRBtn;
    QRadioButton *dynamicModeRBtn;
    QComboBox *patternTypeCombo;
    QCheckBox *tipActivateCheck;
    QSpinBox  *tipTimeSpin;
    QSpinBox  *tipIntencitySpin;
    QCheckBox *swapCheck;
    QCheckBox *thrillCheck;
    QCheckBox *squaredCheck;
    QCheckBox *fullScreenCheck;
    QCheckBox *ciActivateCheck;
    QSpinBox  *ciOpaqueSpin;
    QSpinBox  *ciIntervalSpin;
    QCheckBox *coverActivateCheck;
    QSpinBox  *coverDurationSpin;
    QCheckBox *hideCenterPatternCheck;

private slots:
    void toggleTipActivate(bool);
    void toggleColorInterference(bool);
    void toggleCoverInterference(bool);
};

class CStatisticsPage : public CDialogPage
{
    Q_OBJECT
public:
    explicit CStatisticsPage(QWidget *parent = 0);

    QLabel *rightHits;
    QLabel *wrongHits;
    QLabel *elapsedTime;

    CMatchStatistics *statistics;
};

#endif // CD_DIALOGPAGES_H
