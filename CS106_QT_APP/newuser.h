#ifndef NEWUSER_H
#define NEWUSER_H

#include <QDialog>

namespace Ui {
class newUser;
}

class newUser : public QDialog
{
    Q_OBJECT

public:
    explicit newUser(QWidget *parent = nullptr);
    ~newUser();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();



private:
    Ui::newUser *ui;

signals:

    void userAdded();

};

#endif // NEWUSER_H
