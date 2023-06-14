#include "./H_files/bookdata.h"
#include "./UI_files/ui_bookdata.h"
#include "./H_files/editbooks.h"
#include "./H_files/deletebooks.h"
#include "./H_files/Utils/mainwindow.h"
#include "./H_files/categorymodadddel.h"

// Create QWidget bookData
bookData::bookData(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bookData)
{
    ui->setupUi(this);

    onNewBookAdded();
}

// Delete QWidget bookData
bookData::~bookData()
{
    delete ui;
}

void bookData::on_pushButton_newBook_clicked() // on_pushButton_newBook_clicked():
{
    newBook *newbook = new newBook(); // create a new newBook object
    newbook->show(); // show the newBook object
    connect(newbook, &newBook::addedNewBook, this, &bookData::onNewBookAdded); // connect the addedNewBook signal to the onNewBookAdded slot
}

// Adds the new book to the table
void bookData::onNewBookAdded() // onNewBookAdded():
{
    // get the book data from the json file
    QJsonObject jsonBookData = files.readFromJson(files.filePathBooks);
    QJsonArray jsonBookDataArray = jsonBookData.contains("data") ? jsonBookData["data"].toArray() : QJsonArray();
    int rowCount = jsonBookDataArray.size();

    // set up the table
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels({"Title","Author" ,"Genre","Year","ID","Description"});
    ui->tableWidget->setRowCount(rowCount);

    // Set table data
    for(int i = 0; i < rowCount; i++) // loop through the book data
    {
        QJsonObject object = jsonBookDataArray[i].toObject(); // get the book object

        // get the book data
        QString bookTitle = object["title"].toString();
        QString author = object["author"].toString();
        QString genere = object["genre"].toString();
        QString year = object["year"].toString();
        QString id = QString::number(object["id"].toInt());
        QString description = object["description"].toString();

        // set the book data to the table
        QTableWidgetItem *item1 = new QTableWidgetItem(bookTitle);
        QTableWidgetItem *item2 = new QTableWidgetItem(author);
        QTableWidgetItem *item3 = new QTableWidgetItem(genere);
        QTableWidgetItem *item4 = new QTableWidgetItem(year);
        QTableWidgetItem *item5 = new QTableWidgetItem(id);
        QTableWidgetItem *item6 = new QTableWidgetItem(description);

        // add the book data to the table
        ui->tableWidget->setItem(i, 0, item1);
        ui->tableWidget->setItem(i, 1, item2);
        ui->tableWidget->setItem(i, 2, item3);
        ui->tableWidget->setItem(i, 3, item4);
        ui->tableWidget->setItem(i, 4, item5);
        ui->tableWidget->setItem(i, 5, item6);

    }
}

// Open editBooks window
void bookData::on_pushButton_modfiyBook_clicked()
{
    editBooks *editbook = new editBooks();
    editbook->show();

    connect(editbook, &editBooks::modifyedBook, this, &bookData::onNewBookAdded);
}

// Open deleteBooks window
void bookData::on_pushButton_deleteBook_clicked()
{
    deleteBooks *deletebook = new deleteBooks();
    deletebook->show();

    connect(deletebook, &deleteBooks::deleteSelectedBook, this, &bookData::onNewBookAdded);
}

// Open the admin menu window
void bookData::on_pushButton_backToMenu_clicked()
{
    MainWindow* adminMenu = new MainWindow();
    emit adminMenu->homeWindowHidden();
    this->hide();
}

void bookData::on_pushButton_Catagoryes_clicked()
{
    categoryModAddDel* categoryWin = new categoryModAddDel();

    categoryWin->show();
}

