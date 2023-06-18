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
    void checkIfBookIsCheckedOut();
    void returnBook();

    void deleteCurrentBook();
    void saveModifyedData(QJsonObject object, QString checkoutDate);

    bool isCheckedOut = false;
    bool isReserved = false;

    bool canCheckOut =false;
    bool canRserve = false;
    bool canReturn = false;

    int currentBookID = -1;
    int currentUserID = -1;
    int currentBookMemberID = -1;
};

#endif // OPENBOOK_H
