#include "./H_files/bookdata.h"
#include "./UI_files/ui_bookdata.h"
#include "./H_files/editbooks.h"
#include "./H_files/deletebooks.h"
#include "./H_files/Utils/mainwindow.h"


bookData::bookData(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bookData)
{
    ui->setupUi(this);




    onNewBookAdded();

}

bookData::~bookData()
{
    delete ui;
}

void bookData::on_pushButton_newBook_clicked()
{
    newBook *newbook = new newBook();
    newbook->show();


    connect(newbook, &newBook::addedNewBook, this, &bookData::onNewBookAdded);
}

void bookData::onNewBookAdded()
{
    QJsonObject jsonBookData = files.readFromJson(files.filePathBooks);
    QJsonArray jsonBookDataArray = jsonBookData.contains("data") ? jsonBookData["data"].toArray() : QJsonArray();

    int rowCount = jsonBookDataArray.size();



    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels({"Title","Author" ,"genere","year","ID","Discripsion"});
    ui->tableWidget->setRowCount(rowCount);



    for(int i = 0; i < rowCount; i++)
    {
        QJsonObject object = jsonBookDataArray[i].toObject();


        QString bookTitle = object["title"].toString();
        QString author = object["author"].toString();
        QString genere = object["genre"].toString();
        QString year = object["year"].toString();
        QString id = QString::number(object["id"].toInt());
        QString discripsion = object["discripsion"].toString();

        QTableWidgetItem *item1 = new QTableWidgetItem(bookTitle);
        QTableWidgetItem *item2 = new QTableWidgetItem(author);
        QTableWidgetItem *item3 = new QTableWidgetItem(genere);
        QTableWidgetItem *item4 = new QTableWidgetItem(year);
        QTableWidgetItem *item5 = new QTableWidgetItem(id);
        QTableWidgetItem *item6 = new QTableWidgetItem(discripsion);

        ui->tableWidget->setItem(i, 0, item1);
        ui->tableWidget->setItem(i, 1, item2);
        ui->tableWidget->setItem(i, 2, item3);
        ui->tableWidget->setItem(i, 3, item4);
        ui->tableWidget->setItem(i, 4, item5);
        ui->tableWidget->setItem(i, 5, item6);

    }

}

void bookData::on_pushButton_modfiyBook_clicked()
{
    editBooks *editbook = new editBooks();
    editbook->show();


    connect(editbook, &editBooks::modifyedBook, this, &bookData::onNewBookAdded);
}


void bookData::on_pushButton_deleteBook_clicked()
{
    deleteBooks *deletebook = new deleteBooks();
    deletebook->show();


    connect(deletebook, &deleteBooks::deleteSelectedBook, this, &bookData::onNewBookAdded);
}


void bookData::on_pushButton_backToMenu_clicked()
{
    MainWindow* adminMenu = new MainWindow();
    emit adminMenu->homeWindowHidden();
    this->hide();
}

