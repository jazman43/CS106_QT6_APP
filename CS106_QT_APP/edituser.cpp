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
        // Get the user information
        QString userName = selectedID["userName"].toString();
        QString password = selectedID["password"].toString();
        QString address = selectedID["address"].toString();
        QString phoneNumber = selectedID["phoneNumber"].toString();

        // Set the user information in the UI
        ui->lineEdit_name->setText(userName);
        ui->lineEdit_password->setText(password);
        ui->lineEdit_address->setText(address);
        ui->lineEdit_number->setText(phoneNumber);
    } else {
        QMessageBox::warning(this, "Error", "User not found.");
    }
}

// Save the new user information
void editUser::on_pushButton_save_changes_clicked()
{

    int id = ui->lineEdit_find_ID->text().toInt();

    QString userName = ui->lineEdit_name->text();
    QString password = ui->lineEdit_password->text();
    QString address = ui->lineEdit_address->text();
    QString phoneNumber = ui->lineEdit_number->text();



    // Modify the user information
    if(files.modifyJson(files.filePathMemberData,"userName",userName,id))
    {
        // What is this ?
    }
    // Modify the user information
    files.modifyJson(files.filePathMemberData,"password",password,id);
    files.modifyJson(files.filePathMemberData,"address",address,id);
    files.modifyJson(files.filePathMemberData,"phoneNumber",phoneNumber,id);

    emit userModify();

    accept();
}

