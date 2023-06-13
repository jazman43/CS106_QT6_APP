#include "./H_files/editbooks.h"
#include "./UI_files/ui_editbooks.h"

// Create QWidget editBooks
editBooks::editBooks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editBooks)
{
    ui->setupUi(this);
}

// Delete QWidget editBooks
editBooks::~editBooks()
{
    delete ui;
}

// Find the selected book
void editBooks::on_pushButton_findID_clicked()
{
    QString id = ui->lineEdit_findId->text();
    QJsonObject selectedID = files.selectObjectByID(files.filePathBooks, id.toInt());

    // Get the book information
    QString bookTitle = selectedID["title"].toString();
    QString author = selectedID["author"].toString();
    QString genere = selectedID["genre"].toString();
    QString year = selectedID["year"].toString();
    QString discripsion = selectedID["discripsion"].toString();

    // Set the book information
    ui->lineEdit_title->setText(bookTitle);
    ui->lineEdit_author->setText(author);
    ui->lineEdit_genre->setText(genere);
    ui->lineEdit_Year->setText(year);
    ui->textEdit_discripsion->setPlainText(discripsion);
}

// Save the new book information
void editBooks::on_pushButton_saveNewBook_clicked()
{

    int id = ui->lineEdit_findId->text().toInt();
    QString bookTitle = ui->lineEdit_title->text();
    QString author = ui->lineEdit_author->text();
    QString genere = ui->lineEdit_genre->text();
    QString year  = ui->lineEdit_Year->text();
    QString discripsion = ui->textEdit_discripsion->toPlainText();

    // Modify the book information
    if(files.modifyJson(files.filePathBooks,"title",bookTitle,id))
    {
        // What is this ?
    }
    // Modify the book information
    files.modifyJson(files.filePathBooks,"author",author,id);
    files.modifyJson(files.filePathBooks,"genre",genere,id);
    files.modifyJson(files.filePathBooks,"year",year,id);
    files.modifyJson(files.filePathBooks,"discripsion",discripsion,id);

    emit modifyedBook();

    accept();
}

