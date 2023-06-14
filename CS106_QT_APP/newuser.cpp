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
    bool isAdminFalse = ui->radioButton_false->isChecked();
    bool isAdminTrue = ui->radioButton_true->isChecked();
    QString address = ui->lineEdit_address->text();
    QString phonenumber = ui->lineEdit_phonenumber->text();

    bool isAdmin= false;

    if(isAdminTrue)
        isAdmin = true;
    else if(isAdminFalse)
        isAdmin = false;


    // Set the user information
    QJsonObject jsonNewUserObj;
    jsonNewUserObj["username"] = username;
    jsonNewUserObj["password"] = password;
    jsonNewUserObj["isAdmin"] = isAdmin;
    jsonNewUserObj["address"] = address;
    jsonNewUserObj["id"] = fileManager.checkFileID(fileManager.filePathMemberData, "id");
    jsonNewUserObj["phonenumber"] = phonenumber;

    QJsonArray bookArray;
    QJsonObject book;
    book["bookID"];
    book["checkoutDate"];
    bookArray.append(book);
    jsonNewUserObj["currentBooks"] = bookArray;//will be changed with a custom arrary of books :: ??

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

