#include "./H_files/newuser.h"
#include "./UI_files/ui_newuser.h"
#include "./H_files/Utils/filemanagement.h"

newUser::newUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newUser)
{
    ui->setupUi(this);
}

newUser::~newUser()
{
    delete ui;
}

void newUser::on_buttonBox_accepted()
{
    fileManagement fileManager;

    //save data to json and exit diloge
    QString userName = ui->lineEdit_name->text();
    QString password = ui->lineEdit_pasword->text();
    QString isAdmin = ui->lineEdit_isAdmin->text();
    QString address = ui->lineEdit_address->text();

    QString phoneNumber = ui->lineEdit_phoneNumber->text();


    QJsonObject jsonNewUserObj;

    jsonNewUserObj["userName"] = userName;
    jsonNewUserObj["password"] = password;
    jsonNewUserObj["isAdmin"] = isAdmin;
    jsonNewUserObj["address"] = address;
    jsonNewUserObj["id"] = fileManager.checkFileID(fileManager.filePathMemberData, "id");
    jsonNewUserObj["phoneNumber"] = phoneNumber;
    //defult velues
    jsonNewUserObj["currentBooks"] = "no books";//will be changed with a custom arrary of books
    jsonNewUserObj["over-dueBooks"] = "no over-due";


    if(fileManager.writeToJson(fileManager.filePathMemberData ,jsonNewUserObj, 1))
    {
        emit userAdded();

        accept();
    }


}



void newUser::on_buttonBox_rejected()
{
    //exit without saving
}

