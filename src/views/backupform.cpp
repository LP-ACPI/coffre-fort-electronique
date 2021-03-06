#include "backupform.h"
#include "../controllers/UsersBackupController.h"
#include "../controllers/TargetController.h"
#include "../controllers/BackupController.h"
#include <QFileDialog>
#include <QFileIconProvider>
#include <QMessageBox>
#include <QStyle>
#include <QDateTime>
#include <QDebug>

BackupForm::BackupForm(QWidget *parent) :
    QDialog(parent),_backupKey("null")
{   
    init();

    connect(this,SIGNAL(postCreateBackupData(std::map<std::string,std::string>)),
            this,SLOT(onNewBackupAdded(std::map<std::string,std::string>)));

    connect(this,SIGNAL(addNewBackup(std::map<std::string,std::string>)),
            parent,SLOT(onBackupAdd(std::map<std::string,std::string>)));

    setWindowTitle("Nouvelle Sauvegarde");
}

BackupForm::BackupForm(std::string backupKey, QWidget *parent) :
    QDialog(parent),_backupKey(backupKey)
{
    init();

     backup_info = UsersBackupController::getInstance().getUsersBackupInfo(backupKey);

    targetChoiceButton->setText(QString::fromStdString(backup_info["target_tag"]));
    noteInput->setPlainText(QString::fromStdString(backup_info["note"]));
    setSourceText(QString::fromStdString(backup_info["source_path"]));
    nameInput->setText(QString::fromStdString(backup_info["name"]));
    frequencyButton->setText(QString::fromStdString(backup_info["frequency"]));

    connect(this,SIGNAL(postUpdateBackupData(std::map<std::string,std::string>)),
            this,SLOT(onBackupUpdated(std::map<std::string,std::string>)));

    setWindowTitle("Modification de "+nameInput->text());

}

BackupForm::~BackupForm(){
    if(progressDialog)
        delete progressDialog;
    if(targetChoice)
        delete targetChoice;
    if(frequencyForm)
        delete frequencyForm;
}

void BackupForm::init(){
    setupUi(this);
    setModal(true);

    sourceIcon->setPixmap(QFileIconProvider().icon(QFileIconProvider::Folder).pixmap(25,25));
    targetIcon->setPixmap(style()->standardIcon(QStyle::SP_DialogSaveButton).pixmap(25,25));
}

void BackupForm::setSourceText(QString sourcePath){
    sourceChoiceButton->setText(sourcePath);
    sourceChoiceButton->setToolTip(sourcePath);
}


void BackupForm::on_backupButtonBox_accepted(){


    backup_info["name"]                = nameInput->text().toStdString();
    backup_info["source_path"]  = sourceChoiceButton->text().toStdString();
    backup_info["target_tag"]      = targetChoiceButton->text().toStdString();
    backup_info["note"]                  = noteInput->toPlainText().toStdString();

    //TODO - lastSave : DateTime
    QDateTime date_time;
    backup_info["last_save"]    = date_time.currentDateTime().toString().toStdString();
    backup_info["frequency"]  = frequencyButton->text().toStdString();


    if(_backupKey == "null"){
        if(!isCreateFormValid())
            return;

        hide();
        emit postCreateBackupData(backup_info);
    } else {
        if(!isUpdateFormValid())
            return;

        hide();
        backup_info["key"] = _backupKey;
        emit postUpdateBackupData(backup_info);
    }
}

void BackupForm::on_backupButtonBox_rejected(){
    close();
}

void BackupForm::onNewBackupAdded(std::map<std::string,std::string> backupInfo){
    if(BackupController::getInstance().servicesAreBusy()){
        QMessageBox::warning(this, "Attention!",
                             "les services sont actuellement occupés à effectuer une sauvegarde. "
                             "\nMerci de reéssayer plus tard");
        return;
    }

    progressDialog = new ProgressDialog(this);
    progressDialog->setFtp(backup_info["target_type"]  == "FTP");
    progressDialog->setUpload(true);
    progressDialog->init();
    progressDialog->show();

    std::map<std::string,std::string> bc_info_wth_key
            = UsersBackupController::getInstance().addNewUsersBackup(backupInfo);
    emit addNewBackup(bc_info_wth_key);
}


void BackupForm::onBackupUpdated(std::map<std::string,std::string> backupInfo){
    if(BackupController::getInstance().servicesAreBusy()){
        QMessageBox::warning(this, "Attention!",
                             "les services sont actuellement occupés à effectuer une sauvegarde. "
                             "\nMerci de reéssayer plus tard");
        return;
    }
    progressDialog = new ProgressDialog(this);
    progressDialog->setFtp(BackupController::getInstance().isBackupFtp(backupInfo["key"]));
    progressDialog->setUpload(true);
    progressDialog->init();
    progressDialog->show();
    UsersBackupController::getInstance().updateUsersBackup(backupInfo);

}

bool BackupForm::isCreateFormValid(){

    if(UsersBackupController::getInstance().backupAlreadyExists(nameInput->text().toStdString())){
        QMessageBox::warning(this, "Attention!",
          "Vous avez déjà une sauvegarde <b>"+nameInput->text()+"</b>");
      return false;
    }

    if(!isFormValid())
        return false;
    return true;
}

bool BackupForm::isUpdateFormValid(){

    if(UsersBackupController::getInstance().anotherBackupAlreadyExists(_backupKey,nameInput->text().toStdString())){
        QMessageBox::warning(this, "Attention!",
                             "Vous avez déjà une sauvegarde <b>"+nameInput->text()+"</b>");
        return false;
    }

    if(!isFormValid())
        return false;
    return true;
}

bool BackupForm::isFormValid(){
    bool empty_entries = sourceChoiceButton->text().isEmpty()
            || nameInput->text().isEmpty()
            || targetChoiceButton->text().isEmpty()
            || frequencyButton->text().isEmpty();

    if(empty_entries){
        QMessageBox::warning(this, "Attention!",
          "Merci de remplir toutes les informations");
      return false;
    }
    return true;
}

void BackupForm::on_sourceChoiceButton_clicked(){
    QString sourceDir = QFileDialog::getExistingDirectory();
    sourceChoiceButton->setText(sourceDir);
    sourceChoiceButton->setToolTip(sourceDir);
}

void BackupForm::on_targetChoiceButton_clicked(){
    targetChoice     = new TargetChoiceDialog(this);
    targetChoice->show();
    connect(targetChoice,SIGNAL(targetSelected(QString)),this,SLOT(onTargetSelected(QString)));
}

void BackupForm::onTargetSelected(QString targetTag){
    targetChoiceButton->setText(targetTag);
    targetChoiceButton->setToolTip(targetTag);
    backup_info["target_type"] = TargetController::getInstance().getFavoriteTargetsType( targetTag.toStdString() );
}

void BackupForm::on_frequencyButton_clicked(){
    frequencyForm = new FrequencyForm(this);
    frequencyForm->show();
    connect(frequencyForm,SIGNAL(frequencySelected(QString)),this,SLOT(onFrequencySelected(QString)));
}

void BackupForm::onFrequencySelected(QString frequencyTag){
    frequencyButton->setText(frequencyTag);
    frequencyButton->setToolTip(frequencyTag);
}
