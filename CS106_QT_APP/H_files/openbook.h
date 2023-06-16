#ifndef OPENBOOK_H
#define OPENBOOK_H

#include <QDialog>
#include "./Utils/filemanagement.h"
#include <QStackedWidget>

namespace Ui {
class openBook;
}

class openBook : public QDialog
{
    Q_OBJECT

public:
    explicit openBook(QWidget *parent = nullptr);
    ~openBook();



private slots:
    void on_pushButton_BookCheckOutReserve_clicked();

    void on_pushButton_BookFav_clicked();

private:
    Ui::openBook *ui;
    fileManagement files;

    QStackedWidget* stackedWidget;

    void checkBookOut();
    void loadbook();

    void returnBook(int userId, int bookId);
    bool isCheckedOut = false;
    bool isReserved = false;
};

#endif // OPENBOOK_H
