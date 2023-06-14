#include "./H_files/Utils/mainwindow.h"
#include "./UI_files/ui_mainwindow.h"

#include <QDebug>
#include "./H_files/userdata.h"
#include "./H_files/bookdata.h"
#include <QScrollArea>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , signinWindow(new Signin)
{
    ui->setupUi(this); // set up ui

    // Sign out any signed in users
    logout(); //call the logout function

    //hide Account Drop Down Menus
    ui->account_dropDown_guestHome->hide();
    ui->Account_StaffHome->hide();

    stackedWidget = findChild<QStackedWidget*>("stackedWidget"); // find stacked widgets
    stackedWidget->setCurrentIndex(1); // set first Home screen as guest

    booksDisplay();

    connect(signinWindow, &Signin::userChecked, this, &MainWindow::loginCheck); //connect signinWindow to loginCheck

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

// Guest Sign In
void MainWindow::loginCheck()
{

    QJsonObject jsonCurrentUser = files.readFromJson(files.filePathCurrentUser); // read current user file

    QJsonObject jsonUserData = files.readFromJson(files.filePathMemberData); // read user data file
    QJsonArray jsonUserDataArray = jsonUserData.contains("data") ? jsonUserData["data"].toArray() : QJsonArray(); // get array of data from user json file
    int rowCount = jsonUserDataArray.size(); // get number of rows in user data file

    if(signinWindow->staffLoggedOn && !jsonCurrentUser.empty()) // if staff is logged on
    {
        stackedWidget->setCurrentIndex(2); // admin is logged in

        QString loggedInUsername; // set logged in username to empty string

        for (int i = 0; i < rowCount; i++) // loop through user data file
        {
            QJsonObject object = jsonUserDataArray[i].toObject(); // get object at index i
            QString username = object["username"].toString(); // get username from object
            QString password = object["password"].toString(); // get password from object

            if (username == "Admin" && password == "Admin") // if username and password are admin
            {
                QMessageBox::warning(this, "Change Password", "Please set a new password for the Admin user.");
            }

            if (username == signinWindow->getUsername() && password == signinWindow->getPassword()) // if username and password are valid
            {
                loggedInUsername = username; // set logged in username to username
                break;
            }
        }

        currentUser = loggedInUsername;
    } else
    {
        stackedWidget->setCurrentIndex(0);
    }
}

void MainWindow::defaultAdminUser() // create default admin user
{
    QJsonObject jsonNewUserObj;
    jsonNewUserObj["username"] = "Admin";
    jsonNewUserObj["password"] = "Admin";
    jsonNewUserObj["isAdmin"] = true;
    jsonNewUserObj["address"] = "";
    jsonNewUserObj["id"] = 1;
    jsonNewUserObj["phoneNumber"] = "";
    jsonNewUserObj["currentBooks"] = "no books";//will be changed with a custom arrary of books
    jsonNewUserObj["over-dueBooks"] = "no over-due";

    if(files.writeToJson(files.filePathMemberData ,jsonNewUserObj, 1))
    {
        //qDebug() << "Admin User Created";
    }

}

void MainWindow::onHomeWindowHidden()
{
    loginCheck();
    show();
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

// Log Out - Staff - Button
void MainWindow::on_pushButton_StaffSignOut_clicked()
{
    logout();
    stackedWidget->setCurrentIndex(0);
    // output which stacked widget is currently active
    qDebug() << "Current Stacked Widget: " << stackedWidget->currentIndex();
}

// Log Out - Staff
void MainWindow::logout()
{
    QJsonObject currentJsonUserObj = files.readFromJson(files.filePathCurrentUser);
    QJsonArray jsonUserDataArray = currentJsonUserObj.contains("data") ? currentJsonUserObj["data"].toArray() : QJsonArray();
    int rowCount = jsonUserDataArray.size();

    if (!currentJsonUserObj.isEmpty())
    {
        for (int i = 0; i < rowCount; ++i)
        {
            QJsonObject object = jsonUserDataArray[i].toObject();

            int id = object["id"].toInt();
            QString currentUsername = object["username"].toString();

            files.deleteJsonElement(files.filePathCurrentUser,id);

            QMessageBox::information(this, "Log Out", "User " + currentUsername + " has Logged out");
        }
    }
}

void MainWindow::on_navAccount_guest_clicked(bool checked)//guest Account DropDown menu
{
    isGuestFrameVisible = !isGuestFrameVisible;

    if(isGuestFrameVisible)
    {
        ui->account_dropDown_guestHome->show();
    }else {
        ui->account_dropDown_guestHome->hide();
    }

}


void MainWindow::on_navAccount_Staff_clicked(bool checked)//staff Account DropDown menu
{
    isStaffFrameVisible = !isStaffFrameVisible;


    if(isStaffFrameVisible)
    {
        ui->Account_StaffHome->show();
    }else {
        ui->Account_StaffHome->hide();
    }
}

void MainWindow::booksDisplay()
{
    QStringList headerLabels;

    QJsonObject jsonCategory = files.readFromJson(files.filePathCategory);
    QJsonArray jsonCategoryDataArray = jsonCategory.contains("data") ? jsonCategory["data"].toArray() : QJsonArray();
    int rowCount = jsonCategoryDataArray.size();

    ui->tableWidget_BookDisplay->setRowCount(rowCount);

    for(int i = 0; i < rowCount; ++i)
    {
        QJsonObject object = jsonCategoryDataArray[i].toObject();
        QString categoryName = object["categoryName"].toString();
        headerLabels << categoryName;
    }

    ui->tableWidget_BookDisplay->setVerticalHeaderLabels(headerLabels);
}

void MainWindow::on_tableWidget_BookDisplay_cellClicked(int row, int column)
{
    // Handel cell click event
    qDebug() << "Row: " << row << " Column: " << column;
}

