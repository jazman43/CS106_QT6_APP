#include "./H_files/Utils/mainwindow.h"
#include "./UI_files/ui_mainwindow.h"

#include <QDebug>
#include "./H_files/userdata.h"
#include "./H_files/bookdata.h"

#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //set up ui
    ui->setupUi(this);


    //hide Account Drop Down Menus
    ui->account_dropDown_guestHome->hide();
    ui->Account_StaffHome->hide();


    //find stacked widgets
    stackedWidget = findChild<QStackedWidget*>("stackedWidget");

    //set frist Home sreen as guest
    stackedWidget->setCurrentIndex(1);



    connect(signinWindow, &Signin::userChecked, this, &MainWindow::loginCheck);

    QJsonObject adminUser = files.selectObjectByID(files.filePathMemberData,1);

    if(adminUser.empty())
    {
        defaultAdminUser();
    }

    connect(this, &MainWindow::homeWindowHidden, this, &MainWindow::onHomeWindowHidden);

}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::loginCheck()
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

void MainWindow::defaultAdminUser()
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




void MainWindow::onHomeWindowHidden()
{

    show();
    loginCheck();

}




void MainWindow::on_pushButton_guestSignIn_clicked()
{
    signinWindow->show();
}


void MainWindow::on_pushButton_StaffUsersEdit_clicked()
{
    this->hide();
    userData *userdata = new userData();
    userdata->show();
}


void MainWindow::on_pushButton_StaffBooksEdit_clicked()
{
    this->hide();
    bookData *bookdata = new bookData();
    bookdata->show();
}


void MainWindow::on_pushButton_StaffSignOut_clicked()
{
    if (!currentUser.isEmpty())
    {
        stackedWidget->setCurrentIndex(1);
        QMessageBox::information(this, "Log Out", "User " + currentUser + " has Logged out");
    }

}





void MainWindow::on_navAccount_guest_clicked(bool checked)//guess Account DropDown menu
{

    isGuestFrameVisible = !isGuestFrameVisible;


    if(isGuestFrameVisible)
    {
        ui->account_dropDown_guestHome->show();
        //isGuestFrameVisible = true;
    }else {
        ui->account_dropDown_guestHome->hide();
        //isGuestFrameVisible = false;
    }

}


void MainWindow::on_navAccount_Staff_clicked(bool checked)//staff Account DropDown menu
{
    isStaffFrameVisible = !isStaffFrameVisible;


    if(isStaffFrameVisible)
    {
        ui->Account_StaffHome->show();
        //isGuestFrameVisible = true;
    }else {
        ui->Account_StaffHome->hide();
        //isGuestFrameVisible = false;
    }
}

