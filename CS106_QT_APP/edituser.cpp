#include "./H_files/edituser.h"
#include "./UI_files/ui_edituser.h"
#include <QMessageBox>

// Create QWidget editUser
editUser::editUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editUser)
{
    ui->setupUi(this);
}

// Delete QWidget editUser
editUser::~editUser()
{
    delete ui;
}

// Find the selected user
void editUser::on_pushButton_find_ID_clicked()
{
    // Get the book information
    int id = ui->lineEdit_find_ID->text().toInt();
    QJsonObject selectedID = files.selectObjectByID(files.filePathMemberData, id);


    if (!selectedID.isEmpty()) {
       // Get the book information
      QString username = selectedID["username"].toString();
      QString password = selectedID["password"].toString();
      QString address = selectedID["address"].toString();
      QString phonenumber = selectedID["phonenumber"].toString();
  
      // Set the book information
      ui->lineEdit_name->setText(username);
      ui->lineEdit_password->setText(password);
      ui->lineEdit_address->setText(address);
      ui->lineEdit_number->setText(phonenumber);
    } else {
        QMessageBox::warning(this, "Error", "User not found.");
    }

}

// Save the new user information
void editUser::on_pushButton_save_changes_clicked()
{

    int id = ui->lineEdit_find_ID->text().toInt();
    QString username = ui->lineEdit_name->text();
    QString password = ui->lineEdit_password->text();
    QString address = ui->lineEdit_address->text();
    QString phonenumber = ui->lineEdit_number->text();

    QJsonObject object;




    // Modify the user information
    if(files.modifyJson(files.filePathMemberData,"username",id,QJsonObject(),username))
    {
        qDebug() << "saved to MemberData.json (edit user) ";
    }

    if(files.modifyJson(files.filePathMemberData,"address",id,QJsonObject(),address))
    {
        qDebug() << "saved to MemberData.json (edit user) ";
    }
    if(files.modifyJson(files.filePathMemberData,"phonenumber",id,QJsonObject(),phonenumber))
    {
        qDebug() << "saved to MemberData.json (edit user) ";
    }

    if(files.modifyJson(files.filePathMemberData,"password",id,QJsonObject(),password))
    {
        qDebug() << "saved to MemberData.json (edit user) ";
    }

    emit userModify();

    accept();

}

