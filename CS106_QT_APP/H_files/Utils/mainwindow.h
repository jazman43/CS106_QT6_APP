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

    void booksDisplay();

    void on_navAccount_guest_clicked(bool checked);

    void on_navAccount_Staff_clicked(bool checked);

    void on_tableWidget_BookDisplay_cellClicked(int row, int column);

    void on_navSearch_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    QString currentUser;
    int currentuserid;

    QStackedWidget* stackedWidget;

    fileManagement files;

    Signin* signinWindow = new Signin();



    bool isGuestFrameVisible = false;
    bool isStaffFrameVisible = false;

    QList<QPair<QString, QString>> searchResults;


    //home page Indexs
    int memberIndex = 0;
    int guestIndex =1;
    int staffIndex = 2;


    void loadCurrentUser();
    void defaultAdminUser();
    void logout();

    void clearCurrentBook();
    void returnBook(int userId, int bookId);
    void searchBooks(const QString& searchText);
    QString getCategoryName(int categoryID);

signals:
    void homeWindowHidden();
};
#endif // MAINWINDOW_H
