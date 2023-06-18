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


    checkIfBookIsCheckedOut();

    if(canCheckOut && currentBookMemberID != currentUserID){
        checkBookOut();
        canCheckOut =false;
        deleteCurrentBook();
        accept();
        return;
    }
    if(canRserve && currentBookMemberID != currentUserID)
    {
        canRserve = false;
        deleteCurrentBook();
        accept();
        return;
    }
    if(canReturn && currentBookMemberID == currentUserID)
    {
        returnBook();
        canReturn = false;
        deleteCurrentBook();
        accept();
        return;
    }


}

void openBook::on_pushButton_BookFav_clicked()
{
    // Implement your code for handling the favorite button click
}

void openBook::loadbook()
{
    QJsonObject jsonBooks = files.readFromJson(files.filePathCurrentBook);
    QJsonArray jsonBooksDataArray = jsonBooks.contains("data") ? jsonBooks["data"].toArray() : QJsonArray();
    QJsonObject jsonCurrentUser = files.readFromJson(files.filePathCurrentUser);
    QJsonArray jsonCurrentUsersDataArray = jsonCurrentUser.contains("data") ? jsonCurrentUser["data"].toArray() : QJsonArray();



    if(jsonCurrentUsersDataArray.empty())
    {
        ui->pushButton_BookCheckOutReserve->setText("Locked");
        QMessageBox::warning(this, "guest user", "This is not a member account");
        reject();
        return;
    }


    for (int i = 0; i < jsonBooksDataArray.size(); i++) {
        QJsonObject object = jsonBooksDataArray[i].toObject();
        for (int x = 0; x < jsonCurrentUsersDataArray.size(); ++x) {
            QJsonObject currentUserObj = jsonCurrentUsersDataArray[x].toObject();

            currentUserID = currentUserObj["id"].toInt();
        }


        isCheckedOut = object["isCheckOut"].toBool();
        isReserved = object["isReserved"].toBool();

        currentBookMemberID = object["memberID"].toInt();
        currentBookID = object["id"].toInt();

        ui->label_BookAuthor->setText("Author: " + object["author"].toString());
        ui->label_BookTitle->setText(object["title"].toString());
        ui->label_BookYear->setText("Publish Year: " + object["year"].toString());
        ui->textBrowser_discrpsion->setText(object["discripsion"].toString());

        if (!isReserved) {
            ui->pushButton_BookCheckOutReserve->setText("Check out");
            qDebug() << "currently free to check out";
            return;
        } else if (!isCheckedOut) {
            ui->pushButton_BookCheckOutReserve->setText("Reserve");
            qDebug() << "currently checked out";
            QMessageBox::information(this, "currently checked out", "currently checked out");
            if(currentUserID == currentBookMemberID)
            {
                ui->pushButton_BookCheckOutReserve->setText("Return");
                QMessageBox::information(this, "currently checked out", "currently checked out by you you can return");
                qDebug() << "can Return book";
                return;
            }
            return;
        }
    }
}

void openBook::checkBookOut()
{
    QJsonObject jsonBookData = files.readFromJson(files.filePathBooks);
    QJsonObject jsonCurrentBookData = files.readFromJson(files.filePathCurrentBook);
    QJsonObject jsonUserData = files.readFromJson(files.filePathMemberData);
    QJsonObject jsonCurrentUserData = files.readFromJson(files.filePathCurrentUser);

    QJsonArray jsonUserDataArray = jsonUserData.contains("data") ? jsonUserData["data"].toArray() : QJsonArray();
    QJsonArray jsonBookDataArray = jsonBookData.contains("data") ? jsonBookData["data"].toArray() : QJsonArray();


    int rowCount = jsonUserDataArray.size();
    int bookCount = jsonBookDataArray.size();




    if (!jsonCurrentBookData.empty() && !jsonCurrentUserData.empty()) {

        QJsonObject selectedUser;

        QJsonObject selectedBook;

        for(int i = 0;i <rowCount;i++)
        {
            selectedUser = jsonUserDataArray[i].toObject();
        }
        for (int x = 0; x < bookCount; ++x) {
            selectedBook = jsonBookDataArray[x].toObject();
        }




        QString bookName = selectedBook["title"].toString();
        QString checkoutDate = QDate::currentDate().toString("dd-MM-yyyy");

        QDate dueDate = QDate::currentDate().addDays(10);
        isCheckedOut = true;
        currentBookMemberID = selectedUser["id"].toInt();
        selectedBook["isCheckOut"] = isCheckedOut;

        //QJsonObject checkedOutBook;
        //checkedOutBook["id"] = currentBookID;
        //checkedOutBook["checkoutDate"] = checkoutDate;


        selectedUser["bookID"] = currentBookID;
        selectedUser["checkoutDate"] = checkoutDate;



        saveModifyedData(selectedUser,"");
        saveModifyedData(QJsonObject(),checkoutDate);

        qDebug() << "check out saved";


        QDate today = QDate::currentDate();
        int daysUntilDue = today.daysTo(dueDate);

        QString date = dueDate.toString();
        QMessageBox::information(this, "Book checked Out", "The book with name of " + bookName + " has been checked out return by date of " + date);

        if (daysUntilDue == 1) {
            QMessageBox::information(this, "Book Due", "" + bookName + " is Due IN ONE day");
        } else if (daysUntilDue <= 0) {
            QMessageBox::warning(this, "Book Due", "" + bookName + " is Over-Due Please Return");
        }

    } else {
        QMessageBox::warning(this, "Check out", "User or Book doesn't exist");
    }
}


void openBook::returnBook()
{
    QJsonObject jsonBookData = files.readFromJson(files.filePathBooks);
    QJsonObject jsonCurrentBookData = files.readFromJson(files.filePathCurrentBook);
    QJsonObject jsonUserData = files.readFromJson(files.filePathMemberData);
    QJsonObject jsonCurrentUserData = files.readFromJson(files.filePathCurrentUser);

    QJsonArray jsonUserDataArray = jsonUserData.contains("data") ? jsonUserData["data"].toArray() : QJsonArray();
    QJsonArray jsonBookDataArray = jsonBookData.contains("data") ? jsonBookData["data"].toArray() : QJsonArray();
    QJsonArray jsonCurrentUserDataArray = jsonCurrentUserData.contains("data") ? jsonCurrentUserData["data"].toArray() : QJsonArray();
    QJsonArray jsonCurrentBookDataArray = jsonCurrentBookData.contains("data") ? jsonCurrentBookData["data"].toArray() : QJsonArray();

    int bookCount = jsonBookDataArray.size();
    int currentUserCounter = jsonCurrentUserDataArray.size();





    if (!jsonCurrentBookData.empty() && !jsonCurrentUserData.empty()) {

        QJsonObject bookObj;

        QJsonObject userObj;

        for(int i = 0;i <currentUserCounter;i++)
        {
            userObj = jsonUserDataArray[i].toObject();
        }
        for (int x = 0; x < bookCount; ++x) {
            bookObj = jsonBookDataArray[x].toObject();
        }


        bool isCheckedOut = bookObj["isCheckOut"].toBool();
        QString bookName = bookObj["title"].toString();
        QString checkoutDate = bookObj["checkoutDate"].toString();

        isCheckedOut = false;
        isReserved = false;

        // Update book information
        currentBookMemberID = -1;
        checkoutDate = "";
/*
        for (int i = 0; i < currentBooksArray.size(); i++)
        {
            QJsonObject bookDataObj = currentBooksArray[i].toObject();
            if (bookDataObj["bookID"].toInt() != currentUserID)
            {
                updatedCurrentBooksArray.append(bookDataObj);
            }
        }
*/
        userObj["bookID"] = -1;
        userObj["checkoutDate"] = checkoutDate;

        // Use the modifyJson function to update the JSON files

        saveModifyedData(QJsonObject(),checkoutDate);
        saveModifyedData(userObj,"");

        QMessageBox::information(this, "Book Returned", "The book with ID " + bookName + " has been returned.");

    }
    else
    {
        QMessageBox::warning(this, "Invalid Book/User Return", "Book or user does not exist.");
    }
}

void openBook::checkIfBookIsCheckedOut()
{
    QJsonObject jsonBooks = files.readFromJson(files.filePathCurrentBook);
    QJsonArray jsonBooksDataArray = jsonBooks.contains("data") ? jsonBooks["data"].toArray() : QJsonArray();
    QJsonObject jsonCurrentUser = files.readFromJson(files.filePathCurrentUser);
    QJsonArray jsonCurrentUsersDataArray = jsonCurrentUser.contains("data") ? jsonCurrentUser["data"].toArray() : QJsonArray();


    canCheckOut = false;
    canReturn = false;
    canRserve = false;

    bool canSave = false;


    for (int i = 0; i < jsonBooksDataArray.size(); i++) {
        QJsonObject object = jsonBooksDataArray[i].toObject();
        isCheckedOut = object["isCheckOut"].toBool();
        isReserved = object["isReserved"].toBool();        
    }

    if (!jsonCurrentUsersDataArray.empty() && !isCheckedOut) {        
        isCheckedOut = true;
        canCheckOut = true;
        canSave = true;
    }

    if (!jsonCurrentUsersDataArray.empty() && !isReserved) {        
        isReserved = true;
        canRserve =true;
        canSave = true;
    }


    if(jsonCurrentUsersDataArray.empty())
    {
        QMessageBox::warning(this, "guest user", "This is not a member account");
        reject();
        return;
    }


    if(!jsonCurrentUsersDataArray.empty() && currentUserID == currentBookMemberID)
    {

        canSave = true;
        canReturn =true;
        isCheckedOut = false;
        qDebug() << "currently Returning book";
    }

}


void openBook::deleteCurrentBook()
{
    QJsonObject jsonBooks = files.readFromJson(files.filePathCurrentBook);
    // delete current book

    if (!jsonBooks.isEmpty()) {
        files.deleteJsonElement(files.filePathCurrentBook, currentBookID);
    }
}

void openBook::saveModifyedData(QJsonObject object , QString checkoutDate)
{



    if(!files.modifyJson(files.filePathMemberData,"currentBooks",  currentUserID, object,QJsonValue()))
    {
        qDebug() << "unable to save to user file MemberData.json";
        return;
    }
    if(!files.modifyJson(files.filePathBooks,"checkoutDate", currentBookID, QJsonObject(),checkoutDate))
    {
        qDebug() << "unable to save check out date to book file BooksData.json";
        return;
    }
    if(!files.modifyJson(files.filePathBooks,"isCheckOut", currentBookID, QJsonObject(), isCheckedOut))
    {
        qDebug() << "unable to save isCheckOut to book file BooksData.json";
        return;
    }
    if(!files.modifyJson(files.filePathBooks,"isReserved", currentBookID, QJsonObject(), isReserved))
    {
        qDebug() << "unable to save isReserved to book file BooksData.json";
        return;
    }
    if(!files.modifyJson(files.filePathBooks,"memberID", currentBookID, QJsonObject(), currentBookMemberID))
    {
        qDebug() << "unable to save memberID to book file BooksData.json";
        return;
    }
    qDebug() << "write to Json was secceful";
}
