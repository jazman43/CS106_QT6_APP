#include "./H_files/userdata.h"
#include "./UI_files/ui_userdata.h"
#include "./H_files/newuser.h"
#include <QMessageBox>
#include "./H_files/edituser.h"
#include "./H_files/deleteusers.h"
#include "./H_files/Utils/mainwindow.h"

// Create QWidget userData
userData::userData(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::userData)
{
    ui->setupUi(this);

    QJsonObject adminUser = filemanager.selectObjectByID(filemanager.filePathMemberData,1); // Select admin user



    // If admin user is empty then create default admin user
    if(adminUser.empty())
    {
        defaultAdminUser();
    }
    loadUserDataTable();
}

// Delete QWidget userData
userData::~userData()
{
    delete ui;
}

// Create new user
void userData::on_pushButton_newUser_clicked()
{
    newUser *newuser = new newUser();
    newuser->show();

    connect(newuser, &newUser::userAdded, this, &userData::loadUserDataTable);
}


// Open edit user window
void userData::on_pushButton_modfiyUser_clicked()
{
    editUser *edituser = new editUser();
    edituser->show();
    connect(edituser, &editUser::userModify, this, &userData::loadUserDataTable);
}

// Open delete user window
void userData::on_pushButton_deleteUser_clicked()
{
    deleteUsers * deleteuser = new deleteUsers();
    deleteuser->show();
    connect(deleteuser, &deleteUsers::userDeleted, this, &userData::loadUserDataTable);

}

// Back to main window
void userData::on_pushButton_backToMenu_clicked()
{
    MainWindow* adminMenu = new MainWindow();
    emit adminMenu->homeWindowHidden();
    this->hide();
}

// Load user table
void userData::loadUserDataTable()
{
    // Read user data from json file
    QJsonObject jsonUserData = filemanager.readFromJson(filemanager.filePathMemberData);
    QJsonArray jsonUserDataArray = jsonUserData.contains("data") ? jsonUserData["data"].toArray() : QJsonArray();
    int rowCount = jsonUserDataArray.size();

    // Set table column and row count
    ui->tableWidget->setColumnCount(9);
    ui->tableWidget->setHorizontalHeaderLabels({"user name","password" ,"is Admin","address","ID","phone number" ,"current Books","Over-Due books", "start date"});
    ui->tableWidget->setRowCount(rowCount);

    // Set table data
    for(int i = 0; i < rowCount; i++)
    {
        QJsonObject object = jsonUserDataArray[i].toObject(); // Get user data

        // Set user data
        QString userName = object["userName"].toString();
        QString password = object["password"].toString();
        QString isAdmin = object["isAdmin"].toBool() ? "true" : "false";
        QString address = object["address"].toString();
        QString id = QString::number(object["id"].toInt());
        QString phoneNumber = object["phoneNumber"].toString();
        QString currentBooks = object["currentBooks"].toString();
        QString overDueBooks = object["over-dueBooks"].toString();
        QString startDate = object["startDate"].toString();

        // Set table item
        QTableWidgetItem *item1 = new QTableWidgetItem(userName);
        QTableWidgetItem *item2 = new QTableWidgetItem(password);

        // If user is admin then set background color to yellow
        if (userName == "Admin" && password == "Admin")
        {
            item2->setBackground(Qt::yellow);
            QMessageBox::warning(this, "Change Password", "Please change the default password for the admin user.");
        }

        // Set table item
        QTableWidgetItem *item3 = new QTableWidgetItem(isAdmin);
        QTableWidgetItem *item4 = new QTableWidgetItem(address);
        QTableWidgetItem *item5 = new QTableWidgetItem(id);
        QTableWidgetItem *item6 = new QTableWidgetItem(phoneNumber);
        QTableWidgetItem *item7 = new QTableWidgetItem(currentBooks);
        QTableWidgetItem *item8 = new QTableWidgetItem(overDueBooks);
        QTableWidgetItem *item9 = new QTableWidgetItem(startDate);

        // Add data to table
        ui->tableWidget->setItem(i, 0, item1);
        ui->tableWidget->setItem(i, 1, item2);
        ui->tableWidget->setItem(i, 2, item3);
        ui->tableWidget->setItem(i, 3, item4);
        ui->tableWidget->setItem(i, 4, item5);
        ui->tableWidget->setItem(i, 5, item6);
        ui->tableWidget->setItem(i, 6, item7);
        ui->tableWidget->setItem(i, 7, item8);
        ui->tableWidget->setItem(i, 8, item9);
    }

}


void userData::defaultAdminUser()
{
    QJsonObject jsonNewUserObj;

    jsonNewUserObj["userName"] = "Admin";
    jsonNewUserObj["password"] = "Admin";
    jsonNewUserObj["isAdmin"] = true;
    jsonNewUserObj["address"] = "";
    jsonNewUserObj["id"] = 1;
    jsonNewUserObj["phoneNumber"] = "";
    //default values
    jsonNewUserObj["currentBooks"] = "no books";//will be changed with a custom arrary of books
    jsonNewUserObj["over-dueBooks"] = "no over-due";


    if(filemanager.writeToJson(filemanager.filePathMemberData ,jsonNewUserObj, 1))
    {
    }

}
