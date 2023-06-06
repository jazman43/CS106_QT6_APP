#include "newbook.h"
#include "ui_newbook.h"
#include "filemanagement.h"

newBook::newBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newBook)
{
    ui->setupUi(this);
}

newBook::~newBook()
{
    delete ui;
}




void newBook::on_pushButton_saveNewBook_clicked()
{
    fileManagement fileManager;


    QString bookTitle = ui->lineEdit_title->text();
    QString author = ui->lineEdit_author->text();
    QString genere = ui->lineEdit_genre->text();
    QString year = ui->lineEdit_Year->text();
    QString discripsion = ui->textEdit_discripsion->toPlainText();




    QJsonObject jsonNewUserObj;

    jsonNewUserObj["title"] = bookTitle;
    jsonNewUserObj["author"] = author;
    jsonNewUserObj["genre"] = genere;
    jsonNewUserObj["year"] = year;
    jsonNewUserObj["id"] = fileManager.checkFileID(fileManager.filePathBooks, "id");
    jsonNewUserObj["discripsion"] = discripsion;



    if(fileManager.writeToJson(fileManager.filePathBooks ,jsonNewUserObj, 1))
    {
        emit addedNewBook();

        accept();
    }

}


void newBook::on_pushButton_cancel_clicked()
{

}

