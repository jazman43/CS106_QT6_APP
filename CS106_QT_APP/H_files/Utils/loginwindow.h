#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

#include <QStackedWidget>
#include "filemanagement.h"
#include "signin.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class loginWindow;
}
QT_END_NAMESPACE

class loginWindow : public QMainWindow
{
    Q_OBJECT

public:
    loginWindow(QWidget *parent = nullptr);
    ~loginWindow();

    void login();
    void loginCheck();

    void onHomeWindowHidden();

private slots:


    void on_pushButton_login_clicked();

    void on_pushButton_logoutStaff_clicked();

    void on_pushButton_BookMenu_clicked();

    void on_pushButton_UserMenus_clicked();

private:
    Ui::loginWindow *ui;

    QString currentUser;

    QStackedWidget* stackedWidget;

    fileManagement files;

    SignIn* signinWindow = new SignIn();

    void defaultAdminUser();


signals:
    void homeWindowHidden();

};

#endif // LOGINWINDOW_H
