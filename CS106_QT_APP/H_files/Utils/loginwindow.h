#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "filemanagement.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class loginWindow;
}
QT_END_NAMESPACE

class loginWindow : public QMainWindow
{
    Q_OBJECT

public:
    loginWindow(QWidget *parent = nullptr);
    ~loginWindow();

private slots:
    void on_pushButton_login_clicked();

private:
    Ui::loginWindow *ui;

    fileManagement files;
};

#endif // LOGINWINDOW_H
