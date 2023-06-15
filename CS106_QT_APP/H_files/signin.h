#ifndef SIGNIN_H
#define SIGNIN_H

#include <QDialog>

#include "./Utils/filemanagement.h"

namespace Ui {
class Signin;
}

class Signin : public QDialog
{
    Q_OBJECT

public:
    explicit Signin(QWidget *parent = nullptr);
    ~Signin();

    bool staffLoggedOn;
    bool userFound;

    QString getUsername();
    QString getPassword();
    int getId();

private slots:

    void on_pushButton_StaffLogin_clicked();

    void on_pushButton_memberLogin_clicked();

private:
    Ui::Signin *ui;

    fileManagement files;

    QString currentUserPass;
    QString currentUserName;
    int currentuserid;

signals:

    void userChecked();
};

#endif // SIGNIN_H
