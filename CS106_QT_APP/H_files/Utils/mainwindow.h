#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStackedWidget>
#include "filemanagement.h"
#include "signin.h"
#include <QTableWidget>
#include <QWidget>
#include <QString>
#include <QTableWidget>




namespace Ui { class MainWindow; }


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loginCheck();
    void logout();
    void onHomeWindowHidden();

private slots:



    void on_pushButton_guestSignIn_clicked();

    void on_pushButton_StaffUsersEdit_clicked();

    void on_pushButton_StaffBooksEdit_clicked();

    void on_pushButton_StaffSignOut_clicked();
    void on_pushButton_MemberSignOut_clicked();


    // Account button clicked
    void on_navAccount_guest_clicked(bool checked);
    void on_navAccount_Staff_clicked(bool checked);
    void on_navAccount_Member_clicked(bool checked);

    // Notifications button clicked
    void on_navNotification_Member_clicked(bool checked);
    void on_navNotification_Staff_clicked(bool checked);

    // Wishlist button clicked
    void on_navWishlist_Member_clicked(bool checked);



    void on_navSearch_textChanged(const QString &arg1);

    void on_navSearch_3_textChanged(const QString &arg1);

    void on_navSearch_2_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    QString currentUser;
    int currentuserid;

    QStackedWidget* stackedWidget;

    fileManagement files;

    Signin* signinWindow = new Signin();



    bool isGuestAccountFrameVisible = false;
    bool isStaffAccountFrameVisible = false;
    bool isMemberAccountFrameVisible = false;

    bool isStaffNotificationFrameVisible = false;
    bool isMemberNotificationFrameVisible = false;

    bool isMemberWishlistFrameVisible = false;
    void on_tableWidget_BookDisplay_cellClicked(QTableWidget* tableWidget, int row, int column);
    QWidget* createWidget(const QString& bookTitle, const QString& bookAuthor, const QString& bookPublishDate);

    QList<QPair<QString, QString>> searchResults;

    //home page Indexs
    int memberIndex = 0;
    int guestIndex =1;
    int staffIndex = 2;


    void loadCurrentUser();
    void defaultAdminUser();

    void booksDisplay(QTableWidget* &bookTable);
    void clearCurrentBook();
    void returnBook(int userId, int bookId);
    void searchBooks(const QString& searchText);
    QString getCategoryName(int categoryID);




signals:
    void homeWindowHidden();
};
#endif // MAINWINDOW_H
