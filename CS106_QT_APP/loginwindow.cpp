#include "loginwindow.h"
#include "./UI_files/ui_loginwindow.h"
#include "mainwindow.h"
#include "membermenu.h"
#include <QMessageBox>


loginWindow::loginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::loginWindow)
{
    ui->setupUi(this);
}

loginWindow::~loginWindow()
{
    delete ui;
}

void loginWindow::on_pushButton_login_clicked()
{
    QString userName = ui->lineEdit_userName->text();
    QString password = ui->lineEdit_password->text();

    QJsonObject userJsonObject;

    userJsonObject = files.readFromJson(files.filePathMemberData);

    QJsonArray jsonUserDataArray = userJsonObject.contains("data") ? userJsonObject["data"].toArray() : QJsonArray();

    int rowCount = jsonUserDataArray.size();


    for(int i = 0; i < rowCount; i++)
    {
        QJsonObject object = jsonUserDataArray[i].toObject();


        QString userNameSaved = object["userName"].toString();
        QString passwordSaved = object["password"].toString();
        QString isAdmin = object["isAdmin"].toBool() ? "true" : "false";

        if(userName == userNameSaved && password == passwordSaved)
        {
            //can log in
            if(isAdmin == "true")
            {
                //is the admin
                this->hide();
                MainWindow* adminWindow = new MainWindow();
                adminWindow->show();
                break;
            }else
            {
                //user is not admin
                this->hide();
                memberMenu* usermenu = new memberMenu();
                usermenu->show();
                break;

            }
        }else if(userName != userNameSaved && password != passwordSaved)
        {
            QMessageBox::warning(this, "Incorrect user name or password", "try input user name and password again");
        }

    }
}

