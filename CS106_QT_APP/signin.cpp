#include "./H_files/signin.h"
#include "./UI_files/ui_signin.h"
#include <QMessageBox>


SignIn::SignIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignIn)
{
    ui->setupUi(this);
}

SignIn::~SignIn()
{
    delete ui;
}

void SignIn::on_pushButton_memberSignin_clicked()
{
    int memberID = ui->lineEdit_membersID_signin->text().toInt();


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


void SignIn::on_pushButton_staffSignin_clicked()
{

    int staffID = ui->lineEdit_StaffID_signin->text().toInt();
    QString staffPassword = ui->lineEdit_staffPassword_singin->text();

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

QString SignIn::getUsername()
{
    return currentUserName;
}

QString SignIn::getPassword()
{
    return currentUserPass;
}
