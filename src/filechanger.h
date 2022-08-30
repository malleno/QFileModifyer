#pragma once

#include <QObject>
#include <QDir>
#include <QDate>
#include <checkerofinputsettings.h>


class FileChanger : public QObject
{
    Q_OBJECT
public:
    enum RepeatingFile {Rewrite, Modify};
    explicit FileChanger(QObject *parent = nullptr);
    bool setDirectoryPath(const QString&);
    bool setInputMask(const QString&);
    bool setOutputMask(const QString&);
    void setFileRewriteable(bool);
    void setFlagDeleteInputFile(bool);
    bool updateInputFilenames();

    void modifyFiles();

signals:
    void inputError(QString);


private:
    bool isFileNotOpened(QFile&) const;
    bool isInputOutputFilenamesEqual(const QString&) const;
    bool isThereInputFilesToModify() const;

    bool modifyFile(const QString&);
    void modifyFileContent(QFile&, QFile&);
    QChar modifyChar(QChar);

    QFile createOutputFile(const QString&);
    QString makeFilenameToOutputFile(const QString&) const;
    void deleteInputFile(QFile&);

private:
    QDir currentDir_ = QDir();
    QString nextExtension_ = "";
    bool flagDeleteInputFile_ = false;
    RepeatingFile flagRepeatingFile_ = FileChanger::Modify;
    checkerOfInputSettings checkerInput_;
    QStringList inputFiles_;
};

