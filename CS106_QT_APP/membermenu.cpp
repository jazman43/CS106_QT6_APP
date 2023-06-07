#include "./H_files/Utils/membermenu.h"
#include "./UI_files/ui_membermenu.h"

memberMenu::memberMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::memberMenu)
{
    ui->setupUi(this);
}

memberMenu::~memberMenu()
{
    delete ui;
}
