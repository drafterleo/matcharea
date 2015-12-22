#include "cd_settingsdialog.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>

CSettingsDialog::CSettingsDialog(QWidget *parent)
    : QWidget(parent)
{
    resize(450, 500);

    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(90, 90));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(100);
    //contentsWidget->setMinimumHeight(400);
    //contentsWidget->setSpacing(12);

    contentsWidget->setStyleSheet("QListWidget{border: 2px solid gray;}"
                                  "QListView::item:selected{margin: -2px; color: white; border: 1px solid gray;}"
                                  "QListView::item:selected:active{color: white; border: 1px solid gray;}"
                                  "QListView::item:selected{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #808080, stop: 1 #444444);}");

    pagesWidget = new QStackedWidget;
    configPage = new CConfigPage;
    statisticsPage = new CStatisticsPage;
    pagesWidget->addWidget(configPage);
    pagesWidget->addWidget(statisticsPage);

    connect(configPage->fullScreenCheck, SIGNAL(toggled(bool)), SIGNAL(fullScreenToggled(bool)));
    connect(configPage->squaredCheck, SIGNAL(toggled(bool)), SIGNAL(squaredToggled(bool)));

    QPushButton *exitButton = new QPushButton(tr("Exit"));
    connect(exitButton, SIGNAL(clicked()), SIGNAL(exitPressed()));

    QPushButton *startButton = new QPushButton(tr("Start"));
    connect(startButton, SIGNAL(clicked()), SIGNAL(startPressed()));

    createIcons();
    contentsWidget->setCurrentRow(0);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(exitButton);
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(startButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    //mainLayout->addStretch(1);
    //mainLayout->addSpacing(8);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);
}

void CSettingsDialog::createIcons()
{
    QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
    configButton->setIcon(QIcon(":/images/config.png"));
    configButton->setText(tr("Options"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *statisticsButton = new QListWidgetItem(contentsWidget);
    statisticsButton->setIcon(QIcon(":/images/statistics.png"));
    statisticsButton->setText(tr("Statistics"));
    statisticsButton->setTextAlignment(Qt::AlignHCenter);
    statisticsButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void CSettingsDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

void CSettingsDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    /* draw background */
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0x202020));
    painter.setPen(QPen(QBrush(QColor(Qt::gray)), 2));
    painter.drawRoundRect(this->rect().adjusted(1, 1, -2, -2), 5, 5);

    painter.end();
}
