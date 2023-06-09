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

private slots:


    void on_pushButton_login_clicked();

private:
    Ui::loginWindow *ui;


    QStackedWidget* stackedWidget;

    fileManagement files;

    SignIn* signinWindow = new SignIn();



};

#endif // LOGINWINDOW_H
