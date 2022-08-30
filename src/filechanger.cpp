#include "filechanger.h"
#include <QFile>
#include <fstream>
#include <QIODevice>
#include <QVariant>
#include <algorithm>

FileChanger::FileChanger(QObject *parent) : QObject(parent)
  , currentDir_(QDir())
  , nextExtension_("")
  , flagDeleteInputFile_(false)
  , flagRepeatingFile_(FileChanger::Modify)
  , checkerInput_(checkerOfInputSettings())
  , inputFiles_(QStringList())
{
}



bool FileChanger::setDirectoryPath(const QString& path){
    bool setupStatus = true;
    if(checkerInput_.isValidPath(path)){
        currentDir_.setPath(path);
    }else{
        setupStatus = false;
        emit inputError("Invalid input path.");
    }
    return setupStatus;
}



bool FileChanger::setInputMask(const QString& mask){
    bool setupStatus = true;
    if(checkerInput_.isValidMask(mask)){
         currentDir_.setNameFilters(QStringList(mask));
    }else{
        setupStatus = false;
        emit inputError("Invalid input mask.");
    }
    return setupStatus;
}



bool FileChanger::setOutputMask(const QString& mask){
    bool setupStatus = true;
    if(checkerInput_.isValidMask(mask)){
        nextExtension_ = mask.right(mask.size() - 1);
    }else{
        setupStatus = false;
        emit inputError("Invalid output mask.");
    }
    return setupStatus;
}



void FileChanger::setFileRewriteable(bool flag){
    flagRepeatingFile_ = (flag) ? FileChanger::Rewrite : FileChanger::Modify;
}



void FileChanger::setFlagDeleteInputFile(bool flag){
    flagDeleteInputFile_ = flag;
}



bool FileChanger::updateInputFilenames(){
    inputFiles_ = currentDir_.entryList(QDir::Files);
    return true;
}



bool FileChanger::isFileNotOpened(QFile& file) const{
    return (file.QIODevice::openMode() == QIODevice::NotOpen) ? true : false;
}



bool FileChanger::isInputOutputFilenamesEqual(const QString& inFileName) const{
    if(inFileName == makeFilenameToOutputFile(inFileName)){
        return true;
    }
    return false;
}


bool FileChanger::isThereInputFilesToModify() const{
    if(inputFiles_.isEmpty()){
        return false;
    }
    return true;
}



void FileChanger::modifyFiles(){
    if(!isThereInputFilesToModify()){
        emit inputError("There is no input files in directory.");
        return;
    }
    for(QString& filename : inputFiles_){
        modifyFile(filename);
    }
}



bool FileChanger::modifyFile(const QString& fileName){
    QFile inFile(currentDir_.path()+"/"+fileName, this);
    if(!inFile.exists()){
        emit inputError("Input file missing.");
        return false;
    }
    QFile outFile("tmp.tmp");
    if(isFileNotOpened(inFile)){
        modifyFileContent(inFile, outFile);
        QString outputName = makeFilenameToOutputFile(fileName);
        if(flagDeleteInputFile_ ||isInputOutputFilenamesEqual(inFile.fileName())){
            deleteInputFile(inFile);
        }
        if(flagRepeatingFile_ == FileChanger::Rewrite){
            QFile(outputName).remove();
        }
        outFile.rename(outputName);
        return true;
    }
    emit inputError("Error ocured duaring modifying files.");
    return false;
}



void FileChanger::modifyFileContent(QFile& inFile, QFile& outFile){
    inFile.open(QIODevice::ReadOnly);
    outFile.open(QIODevice::WriteOnly);
    QTextStream inStream(&inFile);
    QTextStream outStream(&outFile);
    QChar c;
    do{
        inStream >> c ;
        c = modifyChar(c);
        outStream << c;
    }while(!inStream.atEnd());
    inFile.close();
    outFile.close();
}



QChar FileChanger::modifyChar(QChar ch){
    int modifyer = int(0x11);
    int unicodeCh = ch.unicode();
    unicodeCh ^= modifyer;
    return QChar(unicodeCh);
}



QFile FileChanger::createOutputFile(const QString& prevFileName){
    return QFile(makeFilenameToOutputFile(prevFileName), this);
}



QString FileChanger::makeFilenameToOutputFile(const QString& prevFileName) const{
        QString fileName = "";
        QFileInfo fileInfo(prevFileName);

        fileName.append(currentDir_.path());
        fileName.append("/");
        fileName.append(fileInfo.baseName());

        if(flagRepeatingFile_ == FileChanger::Modify){
            fileName.append("_");
            fileName.append(QDate().currentDate().toString("dd-MM-yy"));
            fileName.append("_");
            fileName.append(QTime().currentTime().toString("hh-mm-ss"));
        }
        else if(flagRepeatingFile_ == FileChanger::Rewrite){
        }

        fileName.append(nextExtension_);
        return fileName;
}



void FileChanger::deleteInputFile(QFile& inputFile){
    QFileInfo fileInfo(inputFile);
    QList<QString>::const_iterator it = std::find(inputFiles_.begin(), inputFiles_.end(),fileInfo.fileName());
    inputFiles_.erase(it);
    inputFile.remove();
}
