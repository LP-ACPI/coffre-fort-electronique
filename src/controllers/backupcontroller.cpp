#include "BackupController.h"
#include "UserController.h"
#include "TargetController.h"
#include "../services/Crypt.h"
#include "../services/ConfigManager.h"
#include "../services/CompressCrypt.h"
#include "../services/Ftp.h"
#include <QDir>

bool BackupController::servicesAreBusy(){

    if(Ftp::getInstance().isCurrentlyBusy() || CompressCrypt::getInstance().isCurrentlyBusy()){
        return true;
    }
    return false;
}

void BackupController::subscribeObserverToBackup(Observer *obs,std::string bcKey){
    UserController::getInstance().getCurrentUser()->getBackupByKey(bcKey)->attach(obs);
}

void BackupController::unsubscribeObserverFromBackup(Observer *obs,std::string bcKey){
    UserController::getInstance().getCurrentUser()->getBackupByKey(bcKey)->detach(obs);
}

void BackupController::restoreBackupData(std::string backupKey){
    UserController::getInstance().getCurrentUser()->getBackupByKey(backupKey)->recoverData();
}

bool BackupController::isBackupFtp(std::string backupKey){
    return UserController::getInstance().getCurrentUser()->getBackupByKey(backupKey)->getTarget()->isFtp();
}

bool BackupController::hasBackupLoadedData(std::string backupKey){
    return UserController::getInstance().getCurrentUser()->getBackupByKey(backupKey)->hasLoadedData();
}

Backup BackupController::getBackupFromInfoMap(std::map<std::string,std::string> backupInfo){

    Backup backup_from_info(backupInfo["key"] .c_str());
    backup_from_info.setName(backupInfo["name"]);
    backup_from_info.setSource(backupInfo["source_path"]);

    AbsTarget *target = UserController::getInstance().
            getCurrentUser()->getFavoriteTargetByTag(backupInfo["target_tag"]);

    backup_from_info.setTarget(target);
    backup_from_info.setFrequency(backupInfo["frequency"]);
    backup_from_info.setLastSave(backupInfo["last_save"]);
    backup_from_info.setNote(backupInfo["note"]);

    return backup_from_info;

}

std::map<std::string,std::string> BackupController::getInfoMapFromBackup(Backup* backupToInfo){

    std::map<std::string,std::string> backup_info;
    backup_info["key"]                   = backupToInfo->getKey();
    backup_info["name"]               = backupToInfo->getName();
    backup_info["source_path"] = backupToInfo->getSource();
    backup_info["target_tag"]        = backupToInfo->getTarget()->getTag();
    backup_info["last_save"]       = backupToInfo->getLastSave();
    backup_info["note"]                = backupToInfo->getNote();
    backup_info["frequency"]     = backupToInfo->getFrequency().toString();
    backup_info["data_loaded"] = backupToInfo->hasLoadedData() ? "oui" : "non";

    return backup_info;
}


json BackupController::getJsonifiedDataTree(std::string la_cle){
    User* user = UserController::getInstance().getCurrentUser();
    Backup *backupByKey = user->getBackupByKey(la_cle);
    if(backupByKey->hasLoadedData())
        return backupByKey->getData()->to_json();
    else
        return NULL;
}
