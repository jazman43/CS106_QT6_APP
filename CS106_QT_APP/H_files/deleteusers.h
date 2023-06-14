#ifndef DELETEUSERS_H
#define DELETEUSERS_H

#include <QDialog>
#include "./Utils/filemanagement.h"

namespace Ui {
class deleteUsers;
}

class deleteUsers : public QDialog
{
    Q_OBJECT

public:
    explicit deleteUsers(QWidget *parent = nullptr);
    ~deleteUsers();

private slots:

    void on_pushButton_deleteUser_clicked();

    void on_pushButton_findUserID_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::deleteUsers *ui;


    fileManagement files;
};

#endif // DELETEUSERS_H
