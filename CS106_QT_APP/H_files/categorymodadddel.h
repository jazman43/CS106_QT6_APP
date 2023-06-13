#ifndef CATEGORYMODADDDEL_H
#define CATEGORYMODADDDEL_H

#include <QDialog>
#include "filemanagement.h"

namespace Ui {
class categoryModAddDel;
}

class categoryModAddDel : public QDialog
{
    Q_OBJECT

public:
    explicit categoryModAddDel(QWidget *parent = nullptr);
    ~categoryModAddDel();

private slots:
    void on_pushButton_findIDDel_clicked();

    void on_pushButton_DeleteCategory_clicked();

    void on_pushButton_AddNewCategory_clicked();

    void on_pushButton_idFindMod_clicked();

    void on_pushButton_Savemod_clicked();

private:
    Ui::categoryModAddDel *ui;

    fileManagement files;
};

#endif // CATEGORYMODADDDEL_H
