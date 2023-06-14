#include "./H_files/newuser.h"
#include "./UI_files/ui_newuser.h"
#include "./H_files/Utils/filemanagement.h"

// Create QWidget newUser
newUser::newUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newUser)
{
    ui->setupUi(this);
}

// Delete QWidget newUser
newUser::~newUser()
{
    delete ui;
}

// Save the new user information
void newUser::on_buttonBox_accepted()
{
    fileManagement fileManager;

    // Get the user information
    QString username = ui->lineEdit_name->text();
    QString password = ui->lineEdit_password->text();
    QString isAdmin = ui->lineEdit_isAdmin->text();
    QString address = ui->lineEdit_address->text();
    QString phonenumber = ui->lineEdit_phonenumber->text();

    // Set the user information
    QJsonObject jsonNewUserObj;
    jsonNewUserObj["username"] = username;
    jsonNewUserObj["password"] = password;
    jsonNewUserObj["isAdmin"] = isAdmin;
    jsonNewUserObj["address"] = address;
    jsonNewUserObj["id"] = fileManager.checkFileID(fileManager.filePathMemberData, "id");
    jsonNewUserObj["phonenumber"] = phonenumber;
    //default values
    jsonNewUserObj["currentBooks"] = "no books";//will be changed with a custom arrary of books :: ??
    jsonNewUserObj["over-dueBooks"] = "no over-due";

    // Write to json file
    if(fileManager.writeToJson(fileManager.filePathMemberData ,jsonNewUserObj, 1))
    {
        emit userAdded();

        accept();
    }


}

// Cancel the new user information
void newUser::on_buttonBox_rejected()
{
    //exit without saving
}

