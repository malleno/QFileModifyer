#include "filemodifier.h"
#include "ui_filemodifier.h"
#include "QFileDialog"
#include "QDebug"
#include "QRegularExpression"
#include "checkerofinputsettings.h"
#include "QMessageBox"

FileModifier::FileModifier(QWidget *parent)
    : QWidget(parent)
    , timer_(new QTimer())
    , fileChanger_(new FileChanger(this))
    , ui(new Ui::FileModifier)
{
    ui->setupUi(this);
    setupConnections();
    setupRegexForMask();
    setupTimer();
}



FileModifier::~FileModifier()
{
    delete ui;
}



void FileModifier::slotBrowse(){
    QString path = QFileDialog::getExistingDirectory(0, "Select a Directory",
                                                    ui->lEinputPath->text());
    if(!path.isEmpty()){
        ui->lEinputPath->setText(path);
    }
}



void FileModifier::slotRun(){
    disableSettingsInput();
    if(setupFileChanger()){
        timer_->start();
    }
}



void FileModifier::slotStop(){
    enableSettingsInput();
    timer_->stop();
}



void FileModifier::slotInputError(QString errorMessege){
    slotStop();
    QMessageBox messege(QMessageBox::Warning, "Error", errorMessege);
    messege.exec();
}



void FileModifier::slotModifyFile(){
    modifyFilesInPath();
}



void FileModifier::slotSetupTimer(int timeDelay){
    timer_->setInterval(1000 * timeDelay);
}



void FileModifier::setupConnections(){
    connect(ui->pbtnBrowseInputPath, SIGNAL(clicked()), SLOT(slotBrowse()));
    connect(ui->pbtnRun, SIGNAL(clicked()), SLOT(slotRun()));
    connect(ui->pbtnStop, SIGNAL(clicked()), SLOT(slotStop()));
    connect(fileChanger_, SIGNAL(inputError(QString)), this, SLOT(slotInputError(QString)));
    connect(timer_, SIGNAL(timeout()), this, SLOT(slotModifyFile()));
    connect(ui->sBTimerDelay, SIGNAL(valueChanged(int)),this, SLOT(slotSetupTimer(int)));
}



void FileModifier::setupRegexForMask(){
    QRegularExpression regexpMask("^[*]{1}[.]{1}[a-zA-Z]+$");
    QRegularExpressionValidator* maskValidator = new QRegularExpressionValidator(
                                                  regexpMask, this
                                                  );
    ui->lEMaskInput->  setValidator(maskValidator);
    ui->lEMaskOutput-> setValidator(maskValidator);
}



bool FileModifier::setupFileChanger(){
    bool setupStatus = true;
    setupStatus = fileChanger_->setDirectoryPath(ui->lEinputPath->text()) &&
    fileChanger_->setInputMask(ui->lEMaskInput->text()) &&
    fileChanger_->setOutputMask(ui->lEMaskOutput->text());
    fileChanger_->setFlagDeleteInputFile(ui->chckBDeleteInputFile->checkState());
    fileChanger_->setFileRewriteable(ui->rBtnRewrite->isChecked());
    setupStatus = setupStatus && fileChanger_->updateInputFilenames();
    return setupStatus;
}



void FileModifier::setupTimer(){
    timer_->setInterval(1000);
}



void FileModifier::disableSettingsInput(){
    ui->lEMaskInput->setReadOnly(true);
    ui->lEMaskOutput->setReadOnly(true);
    ui->lEinputPath->setReadOnly(true);
    ui->chckBDeleteInputFile->setEnabled(false);
    ui->sBTimerDelay->setReadOnly(true);
    ui->rBtnModify->setEnabled(false);
    ui->rBtnRewrite->setEnabled(false);
    ui->pbtnBrowseInputPath->setEnabled(false);
    ui->pbtnRun->setEnabled(false);
    ui->pbtnStop->setEnabled(true);
}



void FileModifier::enableSettingsInput(){
    ui->lEMaskInput->setReadOnly(false);
    ui->lEMaskOutput->setReadOnly(false);
    ui->lEinputPath->setReadOnly(false);
    ui->chckBDeleteInputFile->setEnabled(true);
    ui->sBTimerDelay->setReadOnly(false);
    ui->rBtnModify->setEnabled(true);
    ui->rBtnRewrite->setEnabled(true);
    ui->pbtnBrowseInputPath->setEnabled(true);
    ui->pbtnRun->setEnabled(true);
    ui->pbtnStop->setEnabled(false);
}



void FileModifier::modifyFilesInPath(){
    fileChanger_->modifyFiles();
}
