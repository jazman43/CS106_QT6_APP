#ifndef OPENBOOK_H
#define OPENBOOK_H

#include <QDialog>
#include "./Utils/filemanagement.h"

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



    void checkBookOut();
    void loadbook();
};

#endif // OPENBOOK_H
