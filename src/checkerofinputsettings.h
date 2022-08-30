#pragma once

#include <QObject>

class checkerOfInputSettings : public QObject
{
    Q_OBJECT
public:
    checkerOfInputSettings();

    bool isValidMask(const QString&) const;
    bool isValidPath(const QString&) const;
};

