#include "cd_dialogpages.h"
#include "c_abstractpattern.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFont>

CDialogPage::CDialogPage(QWidget *parent)
    : QWidget(parent)
{
}

void CDialogPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    /* draw background */
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0xF0F0F0));
    painter.setPen(QPen(QBrush(QColor(Qt::gray)), 2));
    painter.drawRoundRect(this->rect().adjusted(1, 1, -2, -2), 5, 5);

    painter.end();
}

/*************************************************************************************/

CConfigPage::CConfigPage(QWidget *parent)
    : CDialogPage(parent)
{
    QGroupBox *matchGroup = new QGroupBox(tr("Match Settings"));
    matchGroup->setMinimumHeight(180);
        QLabel *gridLabel = new QLabel("Grid Size");
        gridSpin = new QSpinBox;
        gridSpin->setRange(4, 11);
        gridSpin->setValue(5);
        gridSpin->setMaximumWidth(100);
        QVBoxLayout *gridLayout = new QVBoxLayout;
        gridLayout->setSpacing(2);
        gridLayout->addWidget(gridLabel, 0);
        gridLayout->addWidget(gridSpin, 0);
        gridLayout->addStretch(1);

        QLabel *ptypeLabel = new QLabel(tr("Symbol Type"));
        patternTypeCombo = new QComboBox;
        patternTypeCombo->insertItem(ptDotPattern, tr("Rune"));
        patternTypeCombo->insertItem(ptRectanglesPattern, tr("Rectangles"));
        patternTypeCombo->insertItem(ptPolygonPattern, tr("Polygon"));
        patternTypeCombo->insertItem(ptBelarusPattern, tr("Belarus"));
        patternTypeCombo->insertItem(ptChinaPattern, tr("Hieroglyph"));
        patternTypeCombo->insertItem(ptStrokePattern, tr("Strokes"));
        patternTypeCombo->setCurrentIndex(ptDotPattern);
        QVBoxLayout *ptypeLayout = new QVBoxLayout;
        ptypeLayout->setSpacing(2);
        ptypeLayout->addWidget(ptypeLabel, 0);
        ptypeLayout->addWidget(patternTypeCombo, 0);
        ptypeLayout->addStretch(1);

        staticModeRBtn = new QRadioButton(tr("Static Mode"));
        staticModeRBtn->setChecked(true);

        chaosModeRBtn = new QRadioButton(tr("Chaos Mode"));
        chaosModeRBtn->setChecked(false);

        dynamicModeRBtn = new QRadioButton(tr("Dynamic Mode"));
        dynamicModeRBtn->setChecked(false);

        QVBoxLayout *matchLayout = new QVBoxLayout;
        matchLayout->addLayout(gridLayout);
        matchLayout->addLayout(ptypeLayout);
        matchLayout->addWidget(staticModeRBtn);
        matchLayout->addWidget(chaosModeRBtn);
        matchLayout->addWidget(dynamicModeRBtn);
        //matchLayout->addStretch(1);
    matchGroup->setLayout(matchLayout);

    QGroupBox *tipGroup = new QGroupBox(tr("Tip Settings"));
        tipActivateCheck = new QCheckBox(tr("Tip Time (sec.)"));
        tipActivateCheck->setChecked(true);
        connect(tipActivateCheck, SIGNAL(toggled(bool)), SLOT(toggleTipActivate(bool)));
        tipTimeSpin = new QSpinBox;
        tipTimeSpin->setRange(2, 600);
        tipTimeSpin->setValue(20);
        tipTimeSpin->setMaximumWidth(100);
        QVBoxLayout *tipTimeLayout = new QVBoxLayout;
        tipTimeLayout->setSpacing(0);
        tipTimeLayout->addWidget(tipActivateCheck);
        tipTimeLayout->addWidget(tipTimeSpin);
        tipTimeLayout->addStretch(1);

        QLabel *tipIntencityLabel = new QLabel("Intencity");
        tipIntencitySpin = new QSpinBox;
        tipIntencitySpin->setRange(10, 100);
        tipIntencitySpin->setValue(50);
        tipIntencitySpin->setSingleStep(5);
        tipIntencitySpin->setMaximumWidth(100);
        QVBoxLayout *tipIntencityLayout = new QVBoxLayout;
        tipIntencityLayout->setSpacing(0);
        tipIntencityLayout->addWidget(tipIntencityLabel);
        tipIntencityLayout->addWidget(tipIntencitySpin);

        QVBoxLayout *tipLayout = new QVBoxLayout;
        tipLayout->addLayout(tipTimeLayout);
        tipLayout->addLayout(tipIntencityLayout);
        tipLayout->addStretch(1);
    tipGroup->setLayout(tipLayout);

    QHBoxLayout *settingsLayout = new QHBoxLayout;
    settingsLayout->addWidget(matchGroup);
    settingsLayout->addWidget(tipGroup);

    QGroupBox *screenGroup = new QGroupBox(tr("Screen"));
    screenGroup->setMaximumHeight(40);
        squaredCheck = new QCheckBox(tr("Squared"));
        squaredCheck->setChecked(true);
        fullScreenCheck = new QCheckBox(tr("Full Screen"));
        QHBoxLayout *screenLayout = new QHBoxLayout;
        screenLayout->setContentsMargins(10, 5, 10, 5);
        screenLayout->addWidget(squaredCheck);
        screenLayout->addWidget(fullScreenCheck);
    screenGroup->setLayout(screenLayout);

//    QHBoxLayout *swapthrillLayout = new QHBoxLayout;

//    QGroupBox *swapInterferenceGroup = new QGroupBox(tr("Swap Interference"));
//    swapInterferenceGroup->setMaximumHeight(40);
        swapCheck = new QCheckBox(tr("Swap Interference"));
//        QHBoxLayout *swapLayout = new QHBoxLayout;
//        swapLayout->setContentsMargins(10, 0, 10, 0);
//        swapLayout->addWidget(swapCheck);
//        swapLayout->addStretch(1);
//    swapInterferenceGroup->setLayout(swapLayout);

//    QGroupBox *thrillInterferenceGroup = new QGroupBox(tr("Thrill Interference"));
//    thrillInterferenceGroup->setMaximumHeight(40);
        thrillCheck = new QCheckBox(tr("Thrill Interference"));
//        QHBoxLayout *thrillLayout = new QHBoxLayout;
//        thrillLayout->setContentsMargins(10, 0, 10, 0);
//        thrillLayout->addWidget(thrillCheck);
//        thrillLayout->addStretch(1);
//    thrillInterferenceGroup->setLayout(thrillLayout);

//    swapthrillLayout->addWidget(swapInterferenceGroup);
//    swapthrillLayout->addWidget(thrillInterferenceGroup);

        hideCenterPatternCheck = new QCheckBox(tr("Hide Center Pattern"));


    //QGroupBox *colorInterferenceGroup = new QGroupBox(tr("Color Interference"));
        ciActivateCheck = new QCheckBox(tr("Color Interference"));
        connect(ciActivateCheck, SIGNAL(toggled(bool)), SLOT(toggleColorInterference(bool)));
        QLabel *ciOpaqueLabel = new QLabel("Opaque");
        ciOpaqueSpin = new QSpinBox;
        ciOpaqueSpin->setRange(20, 250);
        ciOpaqueSpin->setSingleStep(10);
        ciOpaqueSpin->setMaximumWidth(100);
        QVBoxLayout *ciOpaqueLayout = new QVBoxLayout;
        ciOpaqueLayout->addWidget(ciOpaqueLabel);
        ciOpaqueLayout->addWidget(ciOpaqueSpin);
        QLabel *ciIntervalLabel = new QLabel("Interval (msecs.)");
        ciIntervalSpin = new QSpinBox;
        ciIntervalSpin->setRange(200, 2000);
        ciIntervalSpin->setValue(400);
        ciIntervalSpin->setSingleStep(50);
        ciIntervalSpin->setMaximumWidth(100);
        QVBoxLayout *ciIntervalLayout = new QVBoxLayout;
        ciIntervalLayout->addWidget(ciIntervalLabel);
        ciIntervalLayout->addWidget(ciIntervalSpin);
        QHBoxLayout *colorInterferenceLayout = new QHBoxLayout;
        colorInterferenceLayout->setContentsMargins(0, 0, 10 ,0);
        colorInterferenceLayout->addWidget(ciActivateCheck);
        colorInterferenceLayout->addLayout(ciOpaqueLayout);
        colorInterferenceLayout->addLayout(ciIntervalLayout);
    //colorInterferenceGroup->setLayout(colorInterferenceLayout);

    //QGroupBox *coverInterferenceGroup = new QGroupBox(tr("Cover Interference"));
         coverActivateCheck = new QCheckBox(tr("Cover Interference"));
         connect(coverActivateCheck, SIGNAL(toggled(bool)), SLOT(toggleCoverInterference(bool)));
         QLabel *coverRefreshLabel = new QLabel("Duration (msecs.)");
         coverDurationSpin = new QSpinBox;
         coverDurationSpin->setRange(500, 5000);
         coverDurationSpin->setSingleStep(100);
         coverDurationSpin->setValue(1000);
         coverDurationSpin->setMaximumWidth(100);
         QVBoxLayout *coverRefreshLayout = new QVBoxLayout;
         coverRefreshLayout->addWidget(coverRefreshLabel);
         coverRefreshLayout->addWidget(coverDurationSpin);
         QHBoxLayout *coverInterferenceLayout = new QHBoxLayout;
         coverInterferenceLayout->setContentsMargins(0, 0, 10 ,0);
         coverInterferenceLayout->addWidget(coverActivateCheck);
         coverInterferenceLayout->addLayout(coverRefreshLayout);
    //coverInterferenceGroup->setLayout(coverInterferenceLayout);

    QFrame *line1 = new QFrame;
         line1->setFrameShape(QFrame::HLine);
         line1->setFrameShadow(QFrame::Sunken);
    QFrame *line2 = new QFrame;
         line2->setFrameShape(QFrame::HLine);
         line2->setFrameShadow(QFrame::Sunken);

    QVBoxLayout *pageLayout = new QVBoxLayout;
    pageLayout->addLayout(settingsLayout);
    pageLayout->addWidget(screenGroup);
    //pageLayout->addLayout(swapthrillLayout);
    pageLayout->addWidget(swapCheck);
    pageLayout->addWidget(thrillCheck);
    pageLayout->addWidget(hideCenterPatternCheck);
    pageLayout->addWidget(line1);
    pageLayout->addLayout(colorInterferenceLayout);
    pageLayout->addWidget(line2);
    pageLayout->addLayout(coverInterferenceLayout);
    pageLayout->addStretch(1);
    this->setLayout(pageLayout);

    toggleTipActivate(true);
    toggleColorInterference(false);
    toggleCoverInterference(false);
}

void CConfigPage::toggleTipActivate(bool check)
{
    tipTimeSpin->setEnabled(check);
    tipIntencitySpin->setEnabled(check);
}

void CConfigPage::toggleColorInterference(bool check)
{
    ciOpaqueSpin->setEnabled(check);
    ciIntervalSpin->setEnabled(check);
}

void CConfigPage::toggleCoverInterference(bool check)
{
    coverDurationSpin->setEnabled(check);
}


/*************************************************************************************/

CStatisticsPage::CStatisticsPage(QWidget *parent)
    : CDialogPage(parent)
{
    QFont font(this->font());
    font.setPointSize(font.pointSize() + 2);
    this->setFont(font);

    rightHits = new QLabel("Right hits: 0");
    wrongHits = new QLabel("Wrong hits: 0");
    elapsedTime = new QLabel("Elapsed time: 00:00:00");

    statistics = new CMatchStatistics;

    QVBoxLayout *pageLayout = new QVBoxLayout;
    pageLayout->addWidget(rightHits);
    pageLayout->addWidget(wrongHits);
    pageLayout->addWidget(elapsedTime);
    pageLayout->addWidget(statistics, 1);
    //pageLayout->addStretch(1);
    this->setLayout(pageLayout);
}
