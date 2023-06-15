#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStackedWidget>
#include "filemanagement.h"
#include "signin.h"




namespace Ui { class MainWindow; }


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loginCheck();

    void onHomeWindowHidden();

private slots:



    void on_pushButton_guestSignIn_clicked();

    void on_pushButton_StaffUsersEdit_clicked();

    void on_pushButton_StaffBooksEdit_clicked();

    void on_pushButton_StaffSignOut_clicked();
    void on_pushButton_MemberSignOut_clicked();

    void booksDisplay();

    void on_navAccount_guest_clicked(bool checked);

    void on_navAccount_Staff_clicked(bool checked);

    void on_tableWidget_BookDisplay_cellClicked(int row, int column);

private:
    Ui::MainWindow *ui;

    QString currentUser;
    int currentuserid;

    QStackedWidget* stackedWidget;

    fileManagement files;

    Signin* signinWindow = new Signin();

    void defaultAdminUser();

    void logout();

    bool isGuestFrameVisible = false;
    bool isStaffFrameVisible = false;


    void checkBookOut();

    void returnBook(int userId, int bookId);

    //home page Indexs
    int memberIndex = 0;
    int guestIndex =1;
    int staffIndex = 2;


    void loadCurrentUser();
signals:
    void homeWindowHidden();
};
#endif // MAINWINDOW_H
