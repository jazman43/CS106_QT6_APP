#include "./H_files/deleteusers.h"
#include "./UI_files/ui_deleteusers.h"

// Create QWidget deleteUsers
deleteUsers::deleteUsers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deleteUsers)
{
    ui->setupUi(this);
}

// Delete QWidget deleteUsers
deleteUsers::~deleteUsers()
{
    delete ui;
}

// Delete the selected user
void deleteUsers::on_pushButton_deleteUser_clicked()
{

    int id = ui->lineEdit_findUserID->text().toInt();
    files.deleteJsonElement(files.filePathMemberData, id);
}

// Find the selected user
void deleteUsers::on_pushButton_findUserID_clicked()
{
    QString id = ui->lineEdit_findUserID->text();
    QJsonObject selectedID = files.selectObjectByID(files.filePathMemberData, id.toInt());
    QString userName = selectedID["username"].toString();

    ui->lineEdit_name->setText(userName);
}


void deleteUsers::on_pushButton_cancel_clicked()
{
    reject();
}

