#ifndef SIGNIN_H
#define SIGNIN_H

#include <QDialog>
#include "./Utils/filemanagement.h"

namespace Ui {
class SignIn;
}

class SignIn : public QDialog
{
    Q_OBJECT

public:
    explicit SignIn(QWidget *parent = nullptr);
    ~SignIn();


    bool staffLoggedOn;
    bool userFound;


private slots:
    void on_pushButton_memberSignin_clicked();

    void on_pushButton_staffSignin_clicked();

private:
    Ui::SignIn *ui;

    fileManagement files;


signals:

    void userChecked();
};

#endif // SIGNIN_H
