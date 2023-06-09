#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QStackedWidget>
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

    void login();

private slots:

    void on_stackedWidget_currentChanged(int arg1);



private:
    Ui::loginWindow *ui;

    QComboBox* comboBox;
    QStackedWidget* stackedWidget;

    fileManagement files;
};

#endif // LOGINWINDOW_H
