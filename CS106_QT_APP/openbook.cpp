#include "./H_files/openbook.h"
#include "./UI_files/ui_openbook.h"
#include <QMessageBox>


openBook::openBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::openBook)
{
    ui->setupUi(this);



    loadbook();
}

openBook::~openBook()
{
    delete ui;
}



void openBook::on_pushButton_BookCheckOutReserve_clicked()
{
    QJsonObject jsonBooks = files.readFromJson(files.filePathCurrentBook);
    QJsonArray jsonBooksDataArray = jsonBooks.contains("data") ? jsonBooks["data"].toArray() : QJsonArray();
    QJsonObject jsonCurrentUser = files.readFromJson(files.filePathCurrentUser);
    QJsonArray jsonCurrentUsersDataArray = jsonCurrentUser.contains("data") ? jsonCurrentUser["data"].toArray() : QJsonArray();


    int memberID = -1;
    int bookID = -1;

    for (int x = 0; x < jsonCurrentUsersDataArray.size(); ++x) {
        QJsonObject objectUser = jsonCurrentUsersDataArray[x].toObject();

        memberID = objectUser["id"].toInt();

    }

    for(int i =0;i < jsonBooksDataArray.size(); i++)
    {
        QJsonObject object = jsonBooksDataArray[i].toObject();

        isCheckedOut = object["isCheckOut"].toBool();
        isReserved = object["isReserved"].toBool();
        bookID = object["id"].toInt();

    }


    if(memberID > 0 && !isReserved)
    {
        ui->pushButton_BookCheckOutReserve->setText("Reserve");
        isCheckedOut = true;


    }else if(memberID > 0 && !isCheckedOut){

        isReserved = true;
        ui->pushButton_BookCheckOutReserve->setText("Check out");
    }else
    {
        ui->pushButton_BookCheckOutReserve->setText("Locked");
        QMessageBox::warning(this, "guest user", "this is not a member account");
    }



    files.modifyJson(files.filePathBooks,"memberID",memberID,bookID);
    files.modifyJson(files.filePathBooks,"isCheckOut",isCheckedOut,bookID);
    files.modifyJson(files.filePathBooks,"isReserved",isReserved,bookID);
    checkBookOut();
    accept();

}


void openBook::on_pushButton_BookFav_clicked()
{

}

void openBook::loadbook()
{
    QJsonObject jsonBooks = files.readFromJson(files.filePathCurrentBook);
    QJsonArray jsonBooksDataArray = jsonBooks.contains("data") ? jsonBooks["data"].toArray() : QJsonArray();


    for(int i =0;i < jsonBooksDataArray.size(); i++)
    {
        QJsonObject object = jsonBooksDataArray[i].toObject();

        //int ID = object["id"].toInt();
        isCheckedOut = object["isCheckOut"].toBool();
        isReserved = object["isReserved"].toBool();


        ui->label_BookAuthor->setText("Author: " + object["author"].toString());
        ui->label_BookTitle->setText(object["title"].toString());
        ui->label_BookYear->setText("Publish Year: " + object["year"].toString());
        ui->textBrowser_discrpsion->setText(object["discripsion"].toString());

        if(isReserved)
        {
            ui->pushButton_BookCheckOutReserve->setText("Check out");
            qDebug() <<"currently free to check out";
        }else if(isCheckedOut)
        {
            ui->pushButton_BookCheckOutReserve->setText("Reserve");
            qDebug() <<"currently checked out";

        }
    }
}

void openBook::checkBookOut()
{

    QJsonObject jsonBookData = files.readFromJson(files.filePathBooks);
    QJsonObject jsonUserData = files.readFromJson(files.filePathMemberData);
    QJsonArray jsonUserDataArray = jsonUserData.contains("data") ? jsonUserData["data"].toArray() : QJsonArray();
    QJsonArray jsonBookDataArray = jsonBookData.contains("data") ? jsonBookData["data"].toArray() : QJsonArray();


    int rowCount = jsonUserDataArray.size();
    int bookCount = jsonBookDataArray.size();

    int selectedUserIndex = 0;
    int selectedBookIndex = 0;


    if (selectedUserIndex >= 0 && selectedUserIndex < rowCount && selectedBookIndex >= 0 && selectedBookIndex < bookCount)
    {

        QJsonObject selectedUser = jsonUserDataArray[selectedUserIndex].toObject();
        QJsonObject selectedBook = jsonBookDataArray[selectedBookIndex].toObject();


        int memberID = selectedUser["id"].toInt();
        bool isCheckedOut = selectedBook["isCheckOut"].toBool();
        int bookId = selectedBook["id"].toInt();
        QString bookName = selectedBook["title"].toString();
        QString checkoutDate = QDate::currentDate().toString("dd-MM-yyyy");

        QDate dueDate = QDate::currentDate().addDays(10);

        selectedBook["memberID"] = memberID;
        selectedBook["isCheckOut"] = isCheckedOut = true;


        QJsonObject checkedOutBook;
        checkedOutBook["bookId"] = bookId;
        checkedOutBook["checkoutDate"] = checkoutDate;


        QJsonArray currentBooksArray = selectedUser["currentBooks"].toArray();
        currentBooksArray.append(checkedOutBook);
        selectedUser["currentBooks"] = currentBooksArray;


        jsonUserDataArray.replace(selectedUserIndex, selectedUser);


        jsonUserData["data"] = jsonUserDataArray;


        if (!files.writeToJson(files.filePathMemberData, jsonUserData,1) && !files.writeToJson(files.filePathBooks,selectedBook,1))
        {
            qDebug() << "Failed to write to json file (Check Out)";

        }else
        {


            QDate today = QDate::currentDate();
            int daysUntilDue = today.daysTo(dueDate);

            if (daysUntilDue == 1)
            {
               QMessageBox::information(this, "Book Due", "" + bookName + " is Due IN ONE day");
            }
            else if (daysUntilDue <= 0)
            {
                QMessageBox::warning(this, "Book Due", "" + bookName + " is Over-Due Please Return");
            }
        }



    }
    else
    {
        QMessageBox::warning(this, "Check out", "user or Book dosent Exist");
    }


}
