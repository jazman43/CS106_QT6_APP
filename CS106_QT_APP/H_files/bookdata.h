#ifndef BOOKDATA_H
#define BOOKDATA_H

#include <QMainWindow>
#include "newbook.h"
#include "./Utils/filemanagement.h"

namespace Ui {
class bookData;
}

class bookData : public QMainWindow
{
    Q_OBJECT

public:
    explicit bookData(QWidget *parent = nullptr);
    ~bookData();

private slots:
    void on_pushButton_newBook_clicked();

    void on_pushButton_modfiyBook_clicked();

    void on_pushButton_deleteBook_clicked();

    void on_pushButton_backToMenu_clicked();

private:
    Ui::bookData *ui;

    void onNewBookAdded();


    newBook book;

    fileManagement files;
};

#endif // BOOKDATA_H
