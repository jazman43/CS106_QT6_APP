#include "./H_files/signin.h"
#include "./UI_files/ui_signin.h"
#include <QMessageBox>


Signin::Signin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Signin)
{
    ui->setupUi(this);
}

Signin::~Signin()
{
    delete ui;
}



QString Signin::getUsername()
{
    return currentUserName;
}

QString Signin::getPassword()
{
    return currentUserPass;
}

void Signin::on_pushButton_StaffLogin_clicked()
{
    int staffID = ui->lineEdit_StaffIDLogin->text().toInt();
    QString staffPassword = ui->lineEdit_StaffPasswordLogin->text();

    QJsonObject userJsonObject;

    userJsonObject = files.readFromJson(files.filePathMemberData);

    QJsonArray jsonUserDataArray = userJsonObject.contains("data") ? userJsonObject["data"].toArray() : QJsonArray();

    int rowCount = jsonUserDataArray.size();

    userFound = false;
    for(int i = 0; i < rowCount; i++)
    {
        QJsonObject object = jsonUserDataArray[i].toObject();
        staffLoggedOn = false;

        int IDSaved = object["id"].toInt();
        QString userName =object["userName"].toString();
        QString passwordSaved = object["password"].toString();
        bool isAdmin = object["isAdmin"].toBool();

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

    if(!userFound)
    {
        QMessageBox::warning(this, "Staff signin", "Staff does not esest or Incorrect ID/password inputted");
    }
}


void Signin::on_pushButton_memberLogin_clicked()
{
    int memberID = ui->lineEdit_memberID_login->text().toInt();


    QJsonObject userJsonObject;

    userJsonObject = files.readFromJson(files.filePathMemberData);

    QJsonArray jsonUserDataArray = userJsonObject.contains("data") ? userJsonObject["data"].toArray() : QJsonArray();

    int rowCount = jsonUserDataArray.size();

    userFound = false;
    for(int i = 0; i < rowCount; i++)
    {
        QJsonObject object = jsonUserDataArray[i].toObject();
        staffLoggedOn = false;

        int IDSaved = object["id"].toInt();
        bool isAdmin = object["isAdmin"].toBool();


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




    }
    if(!userFound)
    {
        QMessageBox::warning(this, "Member signin", "Member does not esest or Incorrect ID inputted");
    }
}

