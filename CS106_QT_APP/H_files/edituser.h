#ifndef EDITUSER_H
#define EDITUSER_H

#include <QDialog>
#include "./Utils/filemanagement.h"

namespace Ui {
class editUser;
}

class editUser : public QDialog
{
    Q_OBJECT

public:
    explicit editUser(QWidget *parent = nullptr);
    ~editUser();

private slots:
    void on_pushButton_find_ID_clicked();

    void on_pushButton_save_changes_clicked();

private:
    Ui::editUser *ui;

    fileManagement files;

signals:
    void modifySelectedUser();
};

#endif // EDITUSER_H
