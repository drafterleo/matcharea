#include <QApplication>
#include <QThread>
#include <QTime>
#include <QFontDatabase>
#include <QMessageBox>

#include "cf_mainform.h"


int main(int argc, char *argv[])
{
    QApplication::setGraphicsSystem("raster");

    QApplication app(argc, argv);

    qApp->thread()->setPriority(QThread::HighPriority);

    QFontDatabase fontDatabase;
    if (!fontDatabase.families().contains("FangSong")) {
        int loaded = QFontDatabase::addApplicationFont(qApp->applicationDirPath() + "/fonts/simfang.ttf");
        if (loaded < 0)
            QMessageBox::warning(0, "PsyMatchArea", "Can't load FangSong font!");
    }

    QApplication::setStyle("plastique");

    QTime currTime = QTime::currentTime();
    qsrand(currTime.msec() + currTime.second() + currTime.minute() + currTime.hour());

    CMainForm *mainForm = new CMainForm;

    if (mainForm->isSettingsFullScreen())
        mainForm->showFullScreen();
    else
        mainForm->show();


    return app.exec();
}

