#include "./H_files/Utils/mainwindow.h"
#include "./UI_files/ui_mainwindow.h"

#include <QDebug>
#include "./H_files/userdata.h"
#include "./H_files/bookdata.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


/*
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setHorizontalHeaderLabels({"ID","User Name" ,"Password", "Is Admin", "Current Book Count", "Over-Due Count", "Address", "Phone Number"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

*/
    connect(this, &MainWindow::adminWindowHidden, this, &MainWindow::onAdminWindowHidden);

}

MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::on_add_modify_delete_users_clicked()
{
    this->hide();
    userData *user_data = new userData();
    user_data->show();
}


void MainWindow::on_pushButton_books_clicked()
{
    this->hide();

    bookData *book_data = new bookData();
    book_data->show();

}

void MainWindow::onAdminWindowHidden()
{
    show();
}
