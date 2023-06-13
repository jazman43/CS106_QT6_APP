#include "./H_files/deletebooks.h"
#include "./UI_files/ui_deletebooks.h"

// Create QWidget deleteBooks
deleteBooks::deleteBooks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deleteBooks)
{
    ui->setupUi(this);
}

// Delete QWidget deleteBooks
deleteBooks::~deleteBooks()
{
    delete ui;
}

// Delete the selected book
void deleteBooks::on_pushButton_deleteBook_clicked()
{
    QString id = ui->lineEdit_findId->text();
    files.deleteJsonElement(files.filePathBooks, id);
    emit deleteSelectedBook();

    accept();
}

// Find the selected book
void deleteBooks::on_pushButton_findID_clicked()
{
    QString id = ui->lineEdit_findId->text();
    QJsonObject selectedID = files.selectObjectByID(files.filePathBooks, id.toInt());
    QString title = selectedID["title"].toString();

    ui->lineEdit_bookTitle->setText(title);
}

// Cancel the delete book
void deleteBooks::on_pushButton_cancel_clicked()
{
    reject();
}

