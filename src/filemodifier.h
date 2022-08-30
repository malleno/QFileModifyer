#pragma once

#include <QWidget>
#include "filechanger.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class FileModifier; }
QT_END_NAMESPACE

class FileModifier : public QWidget
{
    Q_OBJECT

public:
    enum RepeatingFile {Rewrite, Modify};
    FileModifier(QWidget *parent = nullptr);
    ~FileModifier();

public slots:
    void slotBrowse();
    void slotRun();
    void slotStop();
    void slotInputError(QString);
    void slotModifyFile();
    void slotSetupTimer(int);

private:
    void setupConnections();
    void setupRegexForMask();
    bool setupFileChanger();
    void setupTimer();

    void disableSettingsInput();
    void enableSettingsInput();

    void modifyFilesInPath();

private:
    QTimer* timer_;
    FileChanger* fileChanger_;
    Ui::FileModifier *ui;

};
