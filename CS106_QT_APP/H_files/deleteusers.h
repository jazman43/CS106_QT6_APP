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

    void on_pushButton_close_clicked();

private:
    Ui::deleteUsers *ui;


    fileManagement files;


signals:
    void deleteSelectedUser();
};

#endif // DELETEUSERS_H
