#include "./H_files/signin.h"
#include "./UI_files/ui_signin.h"
#include <QMessageBox>

// Create QWidget Signin
Signin::Signin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Signin)
{
    ui->setupUi(this);
}

// Delete QWidget Signin
Signin::~Signin()
{
    delete ui;
}

// Get username
QString Signin::getUsername()
{
    return currentUserName;
}

// Get password
QString Signin::getPassword()
{
    return currentUserPass;
}

// Get staff logged on
void Signin::on_pushButton_StaffLogin_clicked()
{
    // Get staff ID and password
    int staffID = ui->lineEdit_StaffIDLogin->text().toInt();
    QString staffPassword = ui->lineEdit_StaffPasswordLogin->text();

    // Read from json file
    QJsonObject userJsonObject;
    userJsonObject = files.readFromJson(files.filePathMemberData);
    QJsonArray jsonUserDataArray = userJsonObject.contains("data") ? userJsonObject["data"].toArray() : QJsonArray();
    int rowCount = jsonUserDataArray.size();

    // Check if user is found
    userFound = false;
    for(int i = 0; i < rowCount; i++)
    {
        QJsonObject object = jsonUserDataArray[i].toObject();
        staffLoggedOn = false;

        int IDSaved = object["id"].toInt();
        QString userName =object["userName"].toString();
        QString passwordSaved = object["password"].toString();
        bool isAdmin = object["isAdmin"].toBool();

        // If staff ID, password and is admin is true then set staff logged on to true
        if(staffID == IDSaved && staffPassword == passwordSaved && isAdmin )
        {
            staffLoggedOn = true;
            userFound =true;
            currentUserName = userName;
            currentUserPass = passwordSaved;

            if(files.writeToJson(files.filePathCurrentUser ,object, 1))
            {
            }

            emit userChecked();

            accept();
            break;
        }
    }

    // If user not found then display message box
    if(!userFound)
    {
        QMessageBox::warning(this, "Staff signin", "Staff does not esest or Incorrect ID/password inputted");
    }
}

// Get member logged on
void Signin::on_pushButton_memberLogin_clicked()
{
    // Get member ID
    int memberID = ui->lineEdit_memberID_login->text().toInt();

    QJsonObject userJsonObject;

    userJsonObject = files.readFromJson(files.filePathMemberData);
    QJsonArray jsonUserDataArray = userJsonObject.contains("data") ? userJsonObject["data"].toArray() : QJsonArray();
    int rowCount = jsonUserDataArray.size();

    // Check if user is found
    userFound = false;
    for(int i = 0; i < rowCount; i++)
    {
        QJsonObject object = jsonUserDataArray[i].toObject();
        staffLoggedOn = false;

        int IDSaved = object["id"].toInt();
        bool isAdmin = object["isAdmin"].toBool();

        // If member ID and is admin is false then set staff logged on to false
        if(memberID == IDSaved && !isAdmin)
        {
            staffLoggedOn = false;
            userFound =true;

            if(files.writeToJson(files.filePathCurrentUser ,object, 1))
            {
            }

            emit userChecked();

            accept();
            break;

        }

    // If user not found then display message box
    }
    if(!userFound)
    {
        QMessageBox::warning(this, "Member signin", "Member does not esest or Incorrect ID inputted");
    }
}

