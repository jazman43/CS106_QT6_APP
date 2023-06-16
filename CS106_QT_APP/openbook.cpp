#include "./H_files/openbook.h"
#include "./UI_files/ui_openbook.h"
#include <QMessageBox>


openBook::openBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::openBook)
{
    ui->setupUi(this);
    QJsonObject jsonCurrentUser = files.readFromJson(files.filePathCurrentUser);
    QJsonArray jsonCurrentUsersDataArray = jsonCurrentUser.contains("data") ? jsonCurrentUser["data"].toArray() : QJsonArray();
    if(jsonCurrentUsersDataArray.empty())
    {
        ui->pushButton_BookCheckOutReserve->setText("Locked");
        QMessageBox::warning(this, "guest user", "This is not a member account");
        reject();
    }

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
    int bookMemberID = -1;

    for (int x = 0; x < jsonCurrentUsersDataArray.size(); ++x) {
        QJsonObject objectUser = jsonCurrentUsersDataArray[x].toObject();
        memberID = objectUser["id"].toInt();
    }

    for (int i = 0; i < jsonBooksDataArray.size(); i++) {
        QJsonObject object = jsonBooksDataArray[i].toObject();
        isCheckedOut = object["isCheckOut"].toBool();
        isReserved = object["isReserved"].toBool();
        bookID = object["id"].toInt();
        bookMemberID = object["memberID"].toInt();
    }

    if (!jsonCurrentUsersDataArray.empty() && !isReserved) {
        ui->pushButton_BookCheckOutReserve->setText("Reserve");
        isCheckedOut = true;
        checkBookOut();
    }

    if (!jsonCurrentUsersDataArray.empty() && !isCheckedOut) {
        isReserved = true;
        ui->pushButton_BookCheckOutReserve->setText("Check out");
    }

    if(jsonCurrentUsersDataArray.empty())
    {
        ui->pushButton_BookCheckOutReserve->setText("Locked");
        QMessageBox::warning(this, "guest user", "This is not a member account");
        this->hide();
    }


    if(!jsonCurrentUsersDataArray.empty() && memberID == bookMemberID)
    {
        ui->pushButton_BookCheckOutReserve->setText("Return");
        returnBook(memberID,bookID);
        isCheckedOut = false;
        qDebug() << "currently Returning book";
    }


    files.modifyJson(files.filePathBooks, "memberID", memberID, bookID);
    files.modifyJson(files.filePathBooks, "isCheckOut", isCheckedOut, bookID);
    files.modifyJson(files.filePathBooks, "isReserved", isReserved, bookID);

    // delete current book

    int rowCount = jsonBooksDataArray.size();

    if (!jsonBooks.isEmpty()) {
        for (int i = 0; i < rowCount; ++i) {
            QJsonObject object = jsonBooksDataArray[i].toObject();
            int id = object["id"].toInt();
            QString currentUsername = object["userName"].toString();
            files.deleteJsonElement(files.filePathCurrentBook, id);
        }
    }

    accept();
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


    int currentmemberID= -1;

    for (int i = 0; i < jsonBooksDataArray.size(); i++) {
        QJsonObject object = jsonBooksDataArray[i].toObject();
        for (int x = 0; x < jsonCurrentUsersDataArray.size(); ++x) {
            QJsonObject currentUserObj = jsonCurrentUsersDataArray[x].toObject();

            currentmemberID = currentUserObj["id"].toInt();
        }


        isCheckedOut = object["isCheckOut"].toBool();
        isReserved = object["isReserved"].toBool();

        int bookMemberID = object["memberID"].toInt();
        int bookID = object["id"].toInt();

        ui->label_BookAuthor->setText("Author: " + object["author"].toString());
        ui->label_BookTitle->setText(object["title"].toString());
        ui->label_BookYear->setText("Publish Year: " + object["year"].toString());
        ui->textBrowser_discrpsion->setText(object["discripsion"].toString());

        if (isReserved) {
            ui->pushButton_BookCheckOutReserve->setText("Check out");
            qDebug() << "currently free to check out";
        } else if (isCheckedOut) {
            ui->pushButton_BookCheckOutReserve->setText("Reserve");
            qDebug() << "currently checked out";
            if(currentmemberID == bookMemberID)
            {
                ui->pushButton_BookCheckOutReserve->setText("Return");

                qDebug() << "currently Returning book";
            }
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
    QJsonArray jsonCurrentUserDataArray = jsonCurrentUserData.contains("data") ? jsonCurrentUserData["data"].toArray() : QJsonArray();
    QJsonArray jsonCurrentBookDataArray = jsonCurrentBookData.contains("data") ? jsonCurrentBookData["data"].toArray() : QJsonArray();

    int rowCount = jsonUserDataArray.size();
    int bookCount = jsonBookDataArray.size();
    int currentUser = jsonCurrentUserData.size();
    int currentbook = jsonCurrentBookDataArray.size();

    int selectedUserIndex = 0;
    int selectedBookIndex = 0;



    for (int i = 0; i < currentUser; ++i) {

        QJsonObject currentUserObject = jsonCurrentUserDataArray[i].toObject();
        selectedUserIndex = currentUserObject["id"].toInt();
    }

    for (int i = 0; i < currentbook; ++i) {
        QJsonObject currentBookObject = jsonCurrentBookDataArray[i].toObject();
        selectedBookIndex = currentBookObject["id"].toInt();
    }

    if (selectedUserIndex >= 0 && selectedUserIndex < rowCount && selectedBookIndex >= 0 && selectedBookIndex < bookCount) {
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
        checkedOutBook["id"] = bookId;
        checkedOutBook["checkoutDate"] = checkoutDate;

        QJsonArray currentBooksArray = selectedUser["currentBooks"].toArray();
        //currentBooksArray.append(checkedOutBook);
        selectedUser["currentBooks"] = currentBooksArray;

        jsonUserDataArray.replace(selectedUserIndex, selectedUser);
        jsonUserData["data"] = jsonUserDataArray;

        if (!files.writeToJson(files.filePathMemberData, jsonUserData, 1) && !files.writeToJson(files.filePathBooks, selectedBook, 1)) {
            qDebug() << "Failed to write to json file (Check Out)";
        } else {
            qDebug() << "check out saved";

            QDate today = QDate::currentDate();
            int daysUntilDue = today.daysTo(dueDate);

            if (daysUntilDue == 1) {
                QMessageBox::information(this, "Book Due", "" + bookName + " is Due IN ONE day");
            } else if (daysUntilDue <= 0) {
                QMessageBox::warning(this, "Book Due", "" + bookName + " is Over-Due Please Return");
            }
        }
    } else {
        QMessageBox::warning(this, "Check out", "User or Book doesn't exist");
    }
}


void openBook::returnBook(int userId, int bookId)
{
    QJsonObject jsonBookData = files.readFromJson(files.filePathBooks);
        QJsonObject jsonUserData = files.readFromJson(files.filePathMemberData);
        QJsonArray jsonUserDataArray = jsonUserData.contains("data") ? jsonUserData["data"].toArray() : QJsonArray();
        QJsonArray jsonBookDataArray = jsonBookData.contains("data") ? jsonBookData["data"].toArray() : QJsonArray();

        int rowCount = jsonUserDataArray.size();
        int bookCount = jsonBookDataArray.size();

        int userIndex = -1;
        int bookIndex = -1;

        // Find the user index
        for (int i = 0; i < rowCount; i++)
        {
            QJsonObject userObj = jsonUserDataArray[i].toObject();
            if (userObj["id"].toInt() == userId)
            {
                userIndex = i;
                break;
            }
        }

        // Find the book index
        for (int i = 0; i < bookCount; i++)
        {
            QJsonObject bookObj = jsonBookDataArray[i].toObject();
            if (bookObj["id"].toInt() == bookId)
            {
                bookIndex = i;
                break;
            }
        }

        if (userIndex != -1 && bookIndex != -1)
        {
            QJsonObject userObj = jsonUserDataArray[userIndex].toObject();
            QJsonObject bookObj = jsonBookDataArray[bookIndex].toObject();

            bool isCheckedOut = bookObj["isCheckOut"].toBool();
            QString bookName = bookObj["title"].toString();
            QString checkoutDate = bookObj["checkoutDate"].toString();

            // Update book information
            bookObj["memberID"] = -1; // Reset member ID to indicate it's not checked out
            bookObj["isCheckOut"] = isCheckedOut = false;
            bookObj["checkoutDate"] = "";

            // Remove the book from the user's currentBooks array
            QJsonArray currentBooksArray = userObj["currentBooks"].toArray();
            QJsonArray updatedCurrentBooksArray;

            for (int i = 0; i < currentBooksArray.size(); i++)
            {
                QJsonObject bookDataObj = currentBooksArray[i].toObject();
                if (bookDataObj["bookID"].toInt() != bookId)
                {
                    updatedCurrentBooksArray.append(bookDataObj);
                }
            }

            userObj["currentBooks"] = updatedCurrentBooksArray;

            jsonUserDataArray[userIndex] = userObj;

            jsonUserData["data"] = jsonUserDataArray;

            // Use the modifyJson function to update the JSON files
            bool jsonModified = files.modifyJson(files.filePathMemberData, "data", jsonUserData["data"], 1);
            if (!jsonModified)
            {
                qDebug() << "Failed to write to json file (Return Book)";
            }
            else
            {
                QMessageBox::information(this, "Book Returned", "The book with ID " + QString::number(bookId) + " has been returned.");
            }
        }
        else
        {
            QMessageBox::warning(this, "Invalid Book/User", "Book or user does not exist.");
        }
}
