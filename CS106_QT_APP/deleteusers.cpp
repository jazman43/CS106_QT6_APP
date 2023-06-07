#include "./H_files/deleteusers.h"
#include "./UI_files/ui_deleteusers.h"

deleteUsers::deleteUsers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deleteUsers)
{
    ui->setupUi(this);
}

deleteUsers::~deleteUsers()
{
    delete ui;
}

void deleteUsers::on_pushButton_clicked()
{

}


void deleteUsers::on_pushButton_deleteUser_clicked()
{
    QString id = ui->lineEdit_findUserID->text();

    files.deleteJsonElement(files.filePathMemberData, id);
}


void deleteUsers::on_pushButton_findUserID_clicked()
{
    QString id = ui->lineEdit_findUserID->text();


    QJsonObject selectedID = files.selectObjectByID(files.filePathMemberData, id.toInt());

    QString userName = selectedID["userName"].toString();


    ui->lineEdit_name->setText(userName);

}

