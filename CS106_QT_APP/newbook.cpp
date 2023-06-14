#include "./H_files/newbook.h"
#include "./UI_files/ui_newbook.h"
#include "./H_files/Utils/filemanagement.h"

// Create QWidget newBook
newBook::newBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newBook)
{
    ui->setupUi(this);
}

// Delete QWidget newBook
newBook::~newBook()
{
    delete ui;
}

// Save the new book information
void newBook::on_pushButton_saveNewBook_clicked()
{
    fileManagement fileManager;

    // Get the book information
    QString bookTitle = ui->lineEdit_title->text();
    QString author = ui->lineEdit_author->text();
    int genere = ui->comboBox_Genere->currentIndex();
    QString year = ui->lineEdit_Year->text();
    QString discripsion = ui->textEdit_discripsion->toPlainText();


    // Set the book information
    QJsonObject jsonNewUserObj;
    jsonNewUserObj["title"] = bookTitle;
    jsonNewUserObj["author"] = author;
    jsonNewUserObj["genre"] = genere;
    jsonNewUserObj["year"] = year;
    jsonNewUserObj["id"] = fileManager.checkFileID(fileManager.filePathBooks, "id");
    jsonNewUserObj["discripsion"] = discripsion;

    // Write to json file
    if(fileManager.writeToJson(fileManager.filePathBooks ,jsonNewUserObj, 1))
    {
        emit addedNewBook();

        accept();
    }

}

// Cancel the new book information
void newBook::on_pushButton_cancel_clicked()
{

}

