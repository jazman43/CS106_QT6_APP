#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>





namespace Ui { class MainWindow; }


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void onAdminWindowHidden();

private slots:


    void on_add_modify_delete_users_clicked();

    void on_pushButton_books_clicked();

private:
    Ui::MainWindow *ui;



signals:
    void adminWindowHidden();
};
#endif // MAINWINDOW_H
