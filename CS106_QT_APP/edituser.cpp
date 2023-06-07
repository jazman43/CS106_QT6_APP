#include "./H_files/edituser.h"
#include "./UI_files/ui_edituser.h"



editUser::editUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editUser)
{
    ui->setupUi(this);
}

editUser::~editUser()
{
    delete ui;
}

void editUser::on_pushButton_find_ID_clicked()
{


    QString id = ui->lineEdit_find_ID->text();


    QJsonObject selectedID = files.selectObjectByID(files.filePathMemberData, id.toInt());

    QString userName = selectedID["userName"].toString();
    QString password = selectedID["password"].toString();
    QString address = selectedID["address"].toString();
    QString phoneNumber = selectedID["phoneNumber"].toString();



    ui->lineEdit_name->setText(userName);
    ui->lineEdit_password->setText(password);
    ui->lineEdit_address->setText(address);
    ui->lineEdit_number->setText(phoneNumber);

}


void editUser::on_pushButton_save_changes_clicked()
{
    QString id = ui->lineEdit_find_ID->text();

    QString userName = ui->lineEdit_name->text();
    QString password = ui->lineEdit_password->text();
    QString address = ui->lineEdit_address->text();
    QString phoneNumber = ui->lineEdit_number->text();



    if(files.modifyJson(files.filePathMemberData,"userName",userName,id))
    {

    }
    files.modifyJson(files.filePathMemberData,"password",password,id);
    files.modifyJson(files.filePathMemberData,"address",address,id);
    files.modifyJson(files.filePathMemberData,"phoneNumber",phoneNumber,id);
}

