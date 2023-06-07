#include "./H_files/editbooks.h"
#include "./UI_files/ui_editbooks.h"


editBooks::editBooks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editBooks)
{
    ui->setupUi(this);
}

editBooks::~editBooks()
{
    delete ui;
}

void editBooks::on_pushButton_findID_clicked()
{
    QString id = ui->lineEdit_findId->text();


    QJsonObject selectedID = files.selectObjectByID(files.filePathBooks, id.toInt());


    QString bookTitle = selectedID["title"].toString();
    QString author = selectedID["author"].toString();
    QString genere = selectedID["genre"].toString();
    QString year = selectedID["year"].toString();

    QString discripsion = selectedID["discripsion"].toString();


    ui->lineEdit_title->setText(bookTitle);
    ui->lineEdit_author->setText(author);
    ui->lineEdit_genre->setText(genere);
    ui->lineEdit_Year->setText(year);
    ui->textEdit_discripsion->setPlainText(discripsion);

}


void editBooks::on_pushButton_saveNewBook_clicked()
{
    QString id = ui->lineEdit_findId->text();

    QString bookTitle = ui->lineEdit_title->text();
    QString author = ui->lineEdit_author->text();
    QString genere = ui->lineEdit_genre->text();
    QString year  = ui->lineEdit_Year->text();
    QString discripsion = ui->textEdit_discripsion->toPlainText();



    if(files.modifyJson(files.filePathBooks,"title",bookTitle,id))
    {

    }
    files.modifyJson(files.filePathBooks,"author",author,id);
    files.modifyJson(files.filePathBooks,"genre",genere,id);
    files.modifyJson(files.filePathBooks,"year",year,id);
    files.modifyJson(files.filePathBooks,"discripsion",discripsion,id);


    emit modifyedBook();

    accept();
}

