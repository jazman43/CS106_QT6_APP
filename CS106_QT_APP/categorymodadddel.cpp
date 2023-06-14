#include "./H_files/categorymodadddel.h"
#include "./UI_files/ui_categorymodadddel.h"
#include <QMessageBox>
#include <QDebug>

categoryModAddDel::categoryModAddDel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::categoryModAddDel)
{
    ui->setupUi(this);
}

categoryModAddDel::~categoryModAddDel()
{
    delete ui;
}

void categoryModAddDel::on_pushButton_findIDDel_clicked()
{
    int id = ui->pushButton_findIDDel->text().toInt();
    QJsonObject selectedID = files.selectObjectByID(files.filePathCategory, id);
    QString categoryName = selectedID["categoryName"].toString();

    ui->lineEditCategoryNameDel->setText(categoryName);
}


void categoryModAddDel::on_pushButton_DeleteCategory_clicked()
{
    int id = ui->pushButton_findIDDel->text().toInt();
    files.deleteJsonElement(files.filePathBooks, id);


    accept();
}


void categoryModAddDel::on_pushButton_AddNewCategory_clicked()
{
    // Get the category name
    QString categoryName = ui->lineEdit_categoryNameAdd->text();


    // Set the Category name/id
    QJsonObject jsonNewCategoryObj;
    jsonNewCategoryObj["categoryName"] = categoryName;
    jsonNewCategoryObj["id"] = files.checkFileID(files.filePathCategory, "id");


    // Write to json file
    if(!files.writeToJson(files.filePathCategory ,jsonNewCategoryObj, 1))
    {

        qDebug() << "File Error, Unable To Write to Category file";
    }

    accept();
}


void categoryModAddDel::on_pushButton_idFindMod_clicked()
{
    int id = ui->pushButton_idFindMod->text().toInt();
    QJsonObject selectedID = files.selectObjectByID(files.filePathCategory, id);
    QString categoryName = selectedID["categoryName"].toString();

    ui->lineEdit_categoryNameMod->setText(categoryName);
}



void categoryModAddDel::on_pushButton_Savemod_clicked()
{
    int id = ui->pushButton_idFindMod->text().toInt();
    QString categoryName = ui->lineEdit_categoryNameMod->text();

    // Modify the category name
    if(!files.modifyJson(files.filePathCategory,"categoryName",categoryName,id))
    {

       qDebug() << "File Error, Unable To Write to Category file";
    }

    accept();
}

