#include "userdata.h"
#include "ui_userdata.h"
#include "newuser.h"
#include <QMessageBox>
#include "edituser.h"
#include "deleteusers.h"
#include "mainwindow.h"


userData::userData(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::userData)
{
    ui->setupUi(this);



    QJsonObject adminUser = filemanager.selectObjectByID(filemanager.filePathMemberData,1);

    if(adminUser.empty())
    {
        defaultAdminUser();
    }

    loadUserDataTable();


}

userData::~userData()
{
    delete ui;
}

void userData::on_pushButton_newUser_clicked()
{
    newUser *newuser = new newUser();
    newuser->show();


    connect(newuser, &newUser::userAdded, this, &userData::onNewUserAdded);
}

void userData::onNewUserAdded()
{
    loadUserDataTable();
}


void userData::on_pushButton_modfiyUser_clicked()
{
    editUser *edituser = new editUser();
    edituser->show();
}


void userData::on_pushButton_deleteUser_clicked()
{
    deleteUsers * deleteuser = new deleteUsers();
    deleteuser->show();

}


void userData::on_pushButton_backToMenu_clicked()
{
    MainWindow* adminMenu = new MainWindow();
    emit adminMenu->adminWindowHidden();
    this->hide();
}



void userData::loadUserDataTable()
{
    QJsonObject jsonUserData = filemanager.readFromJson(filemanager.filePathMemberData);
    QJsonArray jsonUserDataArray = jsonUserData.contains("data") ? jsonUserData["data"].toArray() : QJsonArray();

    int rowCount = jsonUserDataArray.size();

    ui->tableWidget->setColumnCount(9);
    ui->tableWidget->setHorizontalHeaderLabels({"user name","parssword" ,"is Admin","address","ID","phone number" ,"current Books","Over-Due books", "start date"});
    ui->tableWidget->setRowCount(rowCount);



    for(int i = 0; i < rowCount; i++)
    {
        QJsonObject object = jsonUserDataArray[i].toObject();


        QString userName = object["userName"].toString();
        QString password = object["password"].toString();
        QString isAdmin = object["isAdmin"].toBool() ? "true" : "false";
        QString address = object["address"].toString();
        QString id = QString::number(object["id"].toInt());
        QString phoneNumber = object["phoneNumber"].toString();
        QString currentBooks = object["currentBooks"].toString();
        QString overDueBooks = object["over-dueBooks"].toString();
        QString startDate = object["startDate"].toString();





        QTableWidgetItem *item1 = new QTableWidgetItem(userName);
        QTableWidgetItem *item2 = new QTableWidgetItem(password);
        if (userName == "Admin" && password == "Admin") {

            item2->setBackground(Qt::yellow);

            QMessageBox::warning(this, "Change Password", "Please change the default password for the admin user.");
        }

        QTableWidgetItem *item3 = new QTableWidgetItem(isAdmin);
        QTableWidgetItem *item4 = new QTableWidgetItem(address);
        QTableWidgetItem *item5 = new QTableWidgetItem(id);
        QTableWidgetItem *item6 = new QTableWidgetItem(phoneNumber);
        QTableWidgetItem *item7 = new QTableWidgetItem(currentBooks);
        QTableWidgetItem *item8 = new QTableWidgetItem(overDueBooks);
        QTableWidgetItem *item9 = new QTableWidgetItem(startDate);

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
    //defult velues
    jsonNewUserObj["currentBooks"] = "no books";//will be changed with a custom arrary of books
    jsonNewUserObj["over-dueBooks"] = "no over-due";


    if(filemanager.writeToJson(filemanager.filePathMemberData ,jsonNewUserObj, 1))
    {
    }

}
