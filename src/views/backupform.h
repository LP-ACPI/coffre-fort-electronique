#ifndef FORMSAUVEGARDE_H
#define FORMSAUVEGARDE_H

#include "ui_backupformdialog.h"
#include "progressdialog.h"
#include "targetchoicedialog.h"
#include "frequencyform.h"

#include <QDialog>

class BackupForm : public QDialog, private Ui::BackupFormDialog
{
    Q_OBJECT
    std::string _backupKey;
    ProgressDialog *progressDialog;
    FrequencyForm* frequencyForm;
    TargetChoiceDialog* targetChoice;

    std::map<std::string,std::string> backup_info;

    bool isFormValid();
    bool isCreateFormValid();
    bool isUpdateFormValid();

    void init();
public:
    explicit BackupForm(QWidget *parent = 0);
    explicit BackupForm(std::string backupKey, QWidget *parent = 0);

    ~BackupForm();

    void setSourceText(QString);

signals:
    void postCreateBackupData(std::map<std::string,std::string>);
    void postUpdateBackupData(std::map<std::string,std::string>);
    void addNewBackup(std::map<std::string,std::string>);

protected slots:
    void on_backupButtonBox_accepted();
    void on_backupButtonBox_rejected();

    void on_sourceChoiceButton_clicked();
    void on_targetChoiceButton_clicked();
    void on_frequencyButton_clicked();

    void onNewBackupAdded(std::map<std::string,std::string>);
    void onBackupUpdated(std::map<std::string,std::string>);
    void onTargetSelected(QString);

private slots:
    void onFrequencySelected(QString);
};

#endif // FORMSAUVEGARDE_H
