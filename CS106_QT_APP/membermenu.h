#ifndef MEMBERMENU_H
#define MEMBERMENU_H

#include <QMainWindow>

namespace Ui {
class memberMenu;
}

class memberMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit memberMenu(QWidget *parent = nullptr);
    ~memberMenu();

private:
    Ui::memberMenu *ui;
};

#endif // MEMBERMENU_H
