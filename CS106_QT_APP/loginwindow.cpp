#include "./H_files/Utils/loginwindow.h"
#include "./UI_files/ui_loginwindow.h"
#include "./H_files/Utils/mainwindow.h"
#include "./H_files/Utils/membermenu.h"
#include "H_files/signin.h"
#include <QMessageBox>


loginWindow::loginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::loginWindow)
{
    ui->setupUi(this);

    comboBox = findChild<QComboBox*>("comboBox_dropdown");
    stackedWidget = findChild<QStackedWidget*>("stackedWidget");

    connect(ui->pushButton_login, &QPushButton::clicked, this, &loginWindow::login);
}

loginWindow::~loginWindow()
{
    delete ui;
}

void loginWindow::login()
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
//              //is the admin
//              this->hide();
//              MainWindow* adminWindow = new MainWindow();
//              adminWindow->show();
//              break;
                stackedWidget->setCurrentIndex(0); // Show the staff page
                break;


            }else
            {
                //user is not admin
                stackedWidget->setCurrentIndex(1);
                break;

            }
        }else if(userName != userNameSaved && password != passwordSaved)
        {
            QMessageBox::warning(this, "Incorrect user name or password", "try input user name and password again");
        }

    }
}


void loginWindow::on_stackedWidget_currentChanged(int arg1)
{

}




void loginWindow::on_pushButton_login_clicked()
{
    SignIn* signinWindow = new SignIn();
    signinWindow->show();
}

