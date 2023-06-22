#include "./H_files/openbook.h"
#include "./UI_files/ui_openbook.h"
#include <QMessageBox>


openBook::openBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::openBook)
{
    ui->setupUi(this);

//load selected book
    loadbook();
}

openBook::~openBook()
{
    delete ui;
}



void openBook::on_pushButton_BookCheckOutReserve_clicked()
{

//check books current stats and set flags
    checkIfBookIsCheckedOut();
//if checkout flag true and if member id saved on the book is not the same as the logged in member
    if(canCheckOut && currentBookMemberID != currentUserID){
        //checkes out book to current member then reset flag and delete selected book from the currentBook.json file
        checkBookOut();
        canCheckOut =false;
        deleteCurrentBook();
        accept();
        return;
    }
//if reserve flag true and if member id saved on the book is not the same as the logged in member
    if(canReserve && currentBookMemberID != currentUserID)
    {
        //reserves a book to current member then reset flag and delete selected book from the currentBook.json file
        reserveBook();
        qDebug() << "can reserve";
        canReserve = false;
        deleteCurrentBook();
        accept();
        return;
    }
//if return flag true and if member id saved on the book is the same as the logged in member
    if(canReturn && currentBookMemberID == currentUserID)
    {
        //returns a book for current member then resets all flags and delete selected book from the currentBook.json file
        returnBook();
        canReturn = false;
        deleteCurrentBook();
        accept();
        return;
    }

    qDebug() << "failed all checks";

}

void openBook::on_pushButton_BookFav_clicked()
{

}
//load selected book
void openBook::loadbook()
{
    //gets the json objects and arrays
    QJsonObject jsonBooks = files.readFromJson(files.filePathCurrentBook);
    QJsonArray jsonBooksDataArray = jsonBooks.contains("data") ? jsonBooks["data"].toArray() : QJsonArray();
    QJsonObject jsonCurrentUser = files.readFromJson(files.filePathCurrentUser);
    QJsonArray jsonCurrentUsersDataArray = jsonCurrentUser.contains("data") ? jsonCurrentUser["data"].toArray() : QJsonArray();


    //checks if there is anyone logged in else its logged in as guest and cant check out any books
    if(jsonCurrentUsersDataArray.empty())
    {
        ui->pushButton_BookCheckOutReserve->setText("Locked");
        QMessageBox::warning(this, "guest user", "This is not a member account");
        reject();
        return;
    }

    // loop over each object in the book array
    for (int i = 0; i < jsonBooksDataArray.size(); i++)
    {
        //get each Json object from json array in the book array and store it in object
        QJsonObject object = jsonBooksDataArray[i].toObject();

        //loop over each object in the members array get each object and store in currentUserObj then use it the set current user id
        for (int x = 0; x < jsonCurrentUsersDataArray.size(); ++x)
        {
            QJsonObject currentUserObj = jsonCurrentUsersDataArray[x].toObject();

            currentUserID = currentUserObj["id"].toInt();            
        }

//set flags for checked out and reserved
        isCheckedOut = object["isCheckOut"].toBool();
        isReserved = object["isReserved"].toBool();

        //get ids for the book the member who has checked out the book and the member how has reserved the book
        currentReserveMemberID = object["reserveMemberID"].toInt();
        currentBookMemberID = object["memberID"].toInt();
        currentBookID = object["id"].toInt();
        //get current books check out date
        currentBookReturnDate = object["checkoutDate"].toString();

        //get current book title
        currentBookTitle = object["title"].toString();

        //set UI text for selected book
        ui->label_BookAuthor->setText("Author: " + object["author"].toString());
        ui->label_BookTitle->setText(currentBookTitle);
        ui->label_BookYear->setText("Publish Year: " + object["year"].toString());
        ui->textBrowser_discrpsion->setText(object["discripsion"].toString());

    }


    if (!isReserved)
    {
        //set ui button to "check out"
        ui->pushButton_BookCheckOutReserve->setText("Check out");
        qDebug() << "currently free to check out";
        return;
    } else
    {

        if(currentUserID == currentBookMemberID)
        {
            //set ui button to "return"
            ui->pushButton_BookCheckOutReserve->setText("Return");
            //let user know they have the book and can return it
            QMessageBox::information(this, "currently checked out", "currently checked out by you, you can return");
            qDebug() << "can Return book";
            return;
        }
        //set ui button to "reserve"
        ui->pushButton_BookCheckOutReserve->setText("Reserve");
        qDebug() << "currently checked out";

        //let user know that book is currently check out
        QMessageBox::information(this, "currently checked out", "currently checked out");
        return;
    }
}

//check selected book out to current member
void openBook::checkBookOut()
{
    //get Json objects from each json file
    QJsonObject jsonCurrentBookData = files.readFromJson(files.filePathCurrentBook);
    QJsonObject jsonCurrentUserData = files.readFromJson(files.filePathCurrentUser);
    //get Json Arrays of json objects
    QJsonArray jsonUserDataArray = jsonCurrentUserData.contains("data") ? jsonCurrentUserData["data"].toArray() : QJsonArray();
    QJsonArray jsonBookDataArray = jsonCurrentBookData.contains("data") ? jsonCurrentBookData["data"].toArray() : QJsonArray();

    //get arrays size
    int rowCount = jsonUserDataArray.size();
    int bookCount = jsonBookDataArray.size();


    //check to see if member or seleted book is empty
    if (!jsonCurrentBookData.empty() && !jsonCurrentUserData.empty()) {
        //get member and book from main bookData and UserData json files
        QJsonObject selectedUser;

        QJsonObject selectedBook;

        for(int i = 0;i <rowCount;i++)
        {
            selectedUser = jsonUserDataArray[i].toObject();
        }
        for (int x = 0; x < bookCount; ++x) {
            selectedBook = jsonBookDataArray[x].toObject();
        }

        //get and store seleted book title
        QString bookName = selectedBook["title"].toString();
        //get current date and store for use later
        QString checkoutDate = QDate::currentDate().toString("dd-MM-yyyy");

        //add ten days to current date for due date (may move the hard coded 10 into a public var for admin/staff to change)
        QDate dueDate = QDate::currentDate().addDays(10);
        //set check out flag
        isCheckedOut = true;
        //set current member id
        currentBookMemberID = selectedUser["id"].toInt();

        //save updated info in bookdata json file
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
        if(!files.modifyJson(files.filePathBooks,"reserveMemberID",  currentBookID, QJsonObject(), currentReserveMemberID))
        {
            qDebug() << "unable to save reserveMemberID to book file BooksData.json";
            return;
        }

        qDebug() << "check out saved";

        QString date = dueDate.toString();
        //let user know date to return book
        QMessageBox::information(this, "Book checked Out", "The book with name of " + bookName + " has been checked out return by date of " + date);    


    } else {
        //warning book or user dosnt exist
        QMessageBox::warning(this, "Check out", "User or Book doesn't exist");
    }
}

//return selected book
void openBook::returnBook()
{
    //get Json objects from each json file
    QJsonObject jsonCurrentBookData = files.readFromJson(files.filePathCurrentBook);
    QJsonObject jsonCurrentUserData = files.readFromJson(files.filePathCurrentUser);
    //get Json Arrays of json objects
    QJsonArray jsonBookDataArray = jsonCurrentBookData.contains("data") ? jsonCurrentBookData["data"].toArray() : QJsonArray();

    //get arrays size
    int bookCount = jsonBookDataArray.size();


    //check to see if member or seleted book is empty
    if (!jsonCurrentBookData.empty() && !jsonCurrentUserData.empty()) {
        //get book from main bookData json files
        QJsonObject selectedBook;

        for (int x = 0; x < bookCount; ++x) {
            selectedBook = jsonBookDataArray[x].toObject();
        }

        //get book title
        QString bookName = selectedBook["title"].toString();

        //reset flags
        isCheckedOut = false;
        isReserved = false;

        // reset book data
        currentBookMemberID = -1;
        QString checkoutDate = "";


        // Use the modifyJson function to update the JSON files
        //save updated info in bookdata json file
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
        if(!files.modifyJson(files.filePathBooks,"reserveMemberID",  currentBookID, QJsonObject(), currentReserveMemberID))
        {
            qDebug() << "unable to save reserveMemberID to book file BooksData.json";
            return;
        }
        //let user know that there book has been returned
        QMessageBox::information(this, "Book Returned", "The book with name " + bookName + " has been returned.");

    }
    else
    {
        QMessageBox::warning(this, "Invalid Book/User Return", "Book or user does not exist.");
    }
}

void openBook::checkIfBookIsCheckedOut()
{
     //get Json objects from each json file
    QJsonObject jsonBooks = files.readFromJson(files.filePathCurrentBook);
    QJsonArray jsonBooksDataArray = jsonBooks.contains("data") ? jsonBooks["data"].toArray() : QJsonArray();
    QJsonObject jsonCurrentUser = files.readFromJson(files.filePathCurrentUser);
    QJsonArray jsonCurrentUsersDataArray = jsonCurrentUser.contains("data") ? jsonCurrentUser["data"].toArray() : QJsonArray();

    //set flags
    canCheckOut = false;
    canReturn = false;
    canReserve = false;


    for (int i = 0; i < jsonBooksDataArray.size(); i++) {
        QJsonObject object = jsonBooksDataArray[i].toObject();
        isCheckedOut = object["isCheckOut"].toBool();
        isReserved = object["isReserved"].toBool();        
    }

    if (!jsonCurrentUsersDataArray.empty() && !isCheckedOut) {        
        isCheckedOut = true;
        canCheckOut = true;        
    }

    if (!jsonCurrentUsersDataArray.empty() && currentUserID != currentBookMemberID) {
        isReserved = true;
        canReserve =true;
    }


    if(jsonCurrentUsersDataArray.empty())
    {
        QMessageBox::warning(this, "guest user", "This is not a member account");
        reject();
        return;
    }


    if(!jsonCurrentUsersDataArray.empty() && currentUserID == currentBookMemberID)
    {        
        canReturn =true;
        isCheckedOut = false;
        qDebug() << "currently Returning book";
    }

}

void openBook::reserveBook()
{


    QDate dueDate = QDate::fromString(currentBookReturnDate).addDays(10);

    QMessageBox::information(this, "Reserve book", "reserveing book " + currentBookTitle + " can collect , " + dueDate.toString());

    if(!files.modifyJson(files.filePathBooks,"reserveMemberID",  currentBookID, QJsonObject(), currentReserveMemberID))
    {
        qDebug() << "unable to save reserveMemberID to book file BooksData.json";
        return;
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

