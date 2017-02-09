#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include "ui_firstauthdialog.h"
#include "../controllers/UserController.h"
#include <QDialog>

class AuthDialog :
        public QDialog,
        private Ui::AuthDialog
{
    Q_OBJECT
    static UserController userController;
    void proceedToMainWindow(std::string);
    bool validation(bool,bool);
public:
    explicit AuthDialog(QWidget *parent = 0);
    ~AuthDialog();

protected slots:
    void on_authButtonBox_accepted();
    void on_authButtonBox_rejected();

    void on_subscriptButtonBox_accepted();
    void on_subscriptButtonBox_rejected();
};

#endif // AUTHDIALOG_H