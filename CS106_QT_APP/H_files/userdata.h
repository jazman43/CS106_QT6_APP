#ifndef USERDATA_H
#define USERDATA_H

#include <QMainWindow>
#include "filemanagement.h"

namespace Ui {
class userData;
}

class userData : public QMainWindow
{
    Q_OBJECT

public:
    explicit userData(QWidget *parent = nullptr);
    ~userData();

private slots:
    void on_pushButton_newUser_clicked();

    void on_pushButton_modfiyUser_clicked();

    void on_pushButton_deleteUser_clicked();

    void on_pushButton_backToMenu_clicked();



private:
    Ui::userData *ui;


    void loadUserDataTable();

    void onNewUserAdded();

    void defaultAdminUser();



    fileManagement filemanager;
};

#endif // USERDATA_H
