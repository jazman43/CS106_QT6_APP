#ifndef DELETEBOOKS_H
#define DELETEBOOKS_H

#include <QDialog>
#include "filemanagement.h"

namespace Ui {
class deleteBooks;
}

class deleteBooks : public QDialog
{
    Q_OBJECT

public:
    explicit deleteBooks(QWidget *parent = nullptr);
    ~deleteBooks();

private slots:
    void on_pushButton_deleteBook_clicked();

    void on_pushButton_findID_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::deleteBooks *ui;

    fileManagement files;

signals:

    void deleteSelectedBook();
};

#endif // DELETEBOOKS_H
