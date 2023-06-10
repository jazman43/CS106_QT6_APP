#include "./H_files/Utils/loginwindow.h"
#include "./UI_files/ui_loginwindow.h"
#include "./H_files/bookdata.h"
#include "H_files/userdata.h"


#include <QMessageBox>


loginWindow::loginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::loginWindow)
{
    ui->setupUi(this);


    stackedWidget = findChild<QStackedWidget*>("stackedWidget");

    stackedWidget->setCurrentIndex(0);



    connect(signinWindow, &SignIn::userChecked, this, &loginWindow::loginCheck);

    QJsonObject adminUser = files.selectObjectByID(files.filePathMemberData,1);

    if(adminUser.empty())
    {
        defaultAdminUser();
    }


    connect(this, &loginWindow::homeWindowHidden, this, &loginWindow::onHomeWindowHidden);
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

    QJsonObject jsonUserData = files.readFromJson(files.filePathMemberData);
    QJsonArray jsonUserDataArray = jsonUserData.contains("data") ? jsonUserData["data"].toArray() : QJsonArray();

    int rowCount = jsonUserDataArray.size();



    if(signinWindow->staffLoggedOn)
    {
        // admin is logged in
        stackedWidget->setCurrentIndex(2);

        QString loggedInUsername;

        for (int i = 0; i < rowCount; i++)
        {
            QJsonObject object = jsonUserDataArray[i].toObject();

            QString userName = object["userName"].toString();
            QString password = object["password"].toString();

            if (userName == "Admin" && password == "Admin")
            {
                QMessageBox::warning(this, "Change Password", "Please change the default password for the admin user.");
            }


            if (userName == signinWindow->getUsername() && password == signinWindow->getPassword())
            {
                loggedInUsername = userName;
                break;
            }
        }


        currentUser = loggedInUsername;


    }else
    {
        stackedWidget->setCurrentIndex(1);
    }



}

void loginWindow::defaultAdminUser()
{
    QJsonObject jsonNewUserObj;

    jsonNewUserObj["userName"] = "Admin";
    jsonNewUserObj["password"] = "Admin";
    jsonNewUserObj["isAdmin"] = true;
    jsonNewUserObj["address"] = "";
    jsonNewUserObj["id"] = 1;
    jsonNewUserObj["phoneNumber"] = "";
    //defult velues
    jsonNewUserObj["currentBooks"] = "no books";//will be changed with a custom arrary of books
    jsonNewUserObj["over-dueBooks"] = "no over-due";


    if(files.writeToJson(files.filePathMemberData ,jsonNewUserObj, 1))
    {
    }

}

void loginWindow::on_pushButton_logoutStaff_clicked()
{

    if (!currentUser.isEmpty())
    {
        stackedWidget->setCurrentIndex(0);
        QMessageBox::information(this, "Log Out", "User " + currentUser + " has Logged out");
    }


}


void loginWindow::on_pushButton_BookMenu_clicked()
{
    this->hide();
    bookData *bookdata = new bookData();
    bookdata->show();
}


void loginWindow::on_pushButton_UserMenus_clicked()
{
    this->hide();
    userData *userdata = new userData();
    userdata->show();
}

void loginWindow::onHomeWindowHidden()
{

    show();
    loginCheck();

}
