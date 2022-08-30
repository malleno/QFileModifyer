#include "checkerofinputsettings.h"
#include "QRegularExpression"
#include "QDir"

checkerOfInputSettings::checkerOfInputSettings()
{

}



bool checkerOfInputSettings::isValidMask(const QString& inputMask) const{
    QRegularExpression regexpMask("^[*]{1}[.]{1}[a-zA-Z]+$");
    return regexpMask.match(inputMask).hasMatch();
}



bool checkerOfInputSettings::isValidPath(const QString& inputPath) const{
    if(inputPath == ""){
        return false;
    }
    return QDir(inputPath).exists();
}
