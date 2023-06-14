#ifndef EDITBOOKS_H
#define EDITBOOKS_H

#include <QDialog>
#include "./Utils/filemanagement.h"

namespace Ui {
class editBooks;
}

class editBooks : public QDialog
{
    Q_OBJECT

public:
    explicit editBooks(QWidget *parent = nullptr);
    ~editBooks();

private slots:
    void on_pushButton_findID_clicked();

    void on_pushButton_saveNewBook_clicked();

private:
    Ui::editBooks *ui;

    fileManagement files;

    void addCategorys();

signals:

    void modifyedBook();
};

#endif // EDITBOOKS_H
