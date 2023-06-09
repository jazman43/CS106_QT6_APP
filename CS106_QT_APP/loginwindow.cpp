#include "./H_files/Utils/loginwindow.h"
#include "./UI_files/ui_loginwindow.h"
#include "./H_files/Utils/mainwindow.h"
#include "./H_files/Utils/membermenu.h"

#include <QMessageBox>


loginWindow::loginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::loginWindow)
{
    ui->setupUi(this);


    stackedWidget = findChild<QStackedWidget*>("stackedWidget");

    stackedWidget->setCurrentIndex(0);

     //signinWindow;

    connect(signinWindow, &SignIn::userChecked, this, &loginWindow::loginCheck);

}

loginWindow::~loginWindow()
{
    delete ui;
}





void loginWindow::on_pushButton_login_clicked()
{
    signinWindow->show();
}

void loginWindow::loginCheck()
{

    if(signinWindow->staffLoggedOn)
    {
        //admin is logged in
        stackedWidget->setCurrentIndex(2);
    }else
    {
        stackedWidget->setCurrentIndex(1);
    }



}
