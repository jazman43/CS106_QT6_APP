#ifndef NEWBOOK_H
#define NEWBOOK_H

#include <QDialog>
#include "./Utils/filemanagement.h"

namespace Ui {
class newBook;
}

class newBook : public QDialog
{
    Q_OBJECT

public:
    explicit newBook(QWidget *parent = nullptr);
    ~newBook();

private:
    Ui::newBook *ui;

    fileManagement files;

    void addCategorys();
signals:

    void addedNewBook();
private slots:
    void on_pushButton_saveNewBook_clicked();
    void on_pushButton_cancel_clicked();
};

#endif // NEWBOOK_H
