#ifndef CF_MAINFORM_H
#define CF_MAINFORM_H

#include <QWidget>
#include "cw_matcharea.h"
#include "cd_settingsdialog.h"

class CMainForm : public QWidget
{
    Q_OBJECT
public:
    explicit CMainForm(QWidget *parent = 0);

    bool isSettingsFullScreen() const;

public slots:
    void startMatching();
    void stopMatching();

private slots:
    void fullScreenChecked(bool);
    void squaredChecked(bool);

protected:
    void readSettings();
    void writeSettings();

    virtual void resizeEvent(QResizeEvent *event);
    virtual void closeEvent(QCloseEvent *event);

private:
    CMatchArea *matchArea;
    CSettingsDialog *settingsDialog;
};

#endif // CF_MAINFORM_H
