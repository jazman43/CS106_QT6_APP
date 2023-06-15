#include "./H_files/Utils/mainwindow.h"
#include "./UI_files/ui_mainwindow.h"

#include "./H_files/openbook.h"
#include <QDebug>
#include "./H_files/userdata.h"
#include "./H_files/bookdata.h"
#include <QScrollArea>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , signinWindow(new Signin)
{
    ui->setupUi(this); // set up ui

    // Sign out any signed in users
    QJsonObject usersLoggedIn = files.readFromJson(files.filePathCurrentUser);

    if(usersLoggedIn.empty())
    {
        logout(); //call the logout function
        qDebug() << "logged old users out";
    }


    //hide Account Drop Down Menus
    ui->account_dropDown_guestHome->hide();
    ui->Account_StaffHome->hide();

    stackedWidget = findChild<QStackedWidget*>("stackedWidget"); // find stacked widgets
    stackedWidget->setCurrentIndex(guestIndex); // set first Home screen as guest

    booksDisplay();

    connect(signinWindow, &Signin::userChecked, this, &MainWindow::loginCheck); //connect signinWindow to loginCheck

    QJsonObject adminUser = files.selectObjectByID(files.filePathMemberData,1);
    if(adminUser.empty())
    {
        defaultAdminUser();
    }

    connect(this, &MainWindow::homeWindowHidden, this, &MainWindow::onHomeWindowHidden);
    connect(ui->navSearch, &QLineEdit::textChanged, this, &MainWindow::searchBooks);

    clearCurrentBook();

    loadCurrentUser();

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Guest Sign In
void MainWindow::loginCheck()
{
    QJsonObject jsonCurrentUser = files.readFromJson(files.filePathCurrentUser); // read current user file

    QJsonObject jsonUserData = files.readFromJson(files.filePathMemberData); // read user data file
    QJsonArray jsonUserDataArray = jsonUserData.contains("data") ? jsonUserData["data"].toArray() : QJsonArray(); // get array of data from user json file
    int rowCount = jsonUserDataArray.size(); // get number of rows in user data file

    if(signinWindow->staffLoggedOn && !jsonCurrentUser.empty()) // if staff is logged on
    {
        stackedWidget->setCurrentIndex(staffIndex); // Admin Home page

        QString loggedInUsername; // set logged in username to empty string

        for (int i = 0; i < rowCount; i++) // loop through user data file
        {
            QJsonObject object = jsonUserDataArray[i].toObject(); // get object at index i
            QString username = object["username"].toString(); // get username from object
            QString password = object["password"].toString(); // get password from object

            if (username == "Admin" && password == "Admin") // if username and password are admin
            {
                QMessageBox::warning(this, "Change Password", "Please set a new password for the Admin user.");
            }

            if (username == signinWindow->getUsername() && password == signinWindow->getPassword()) // if username and password are valid
            {
                loggedInUsername = username; // set logged in username to username
                break;
            }
        }

        currentUser = loggedInUsername;
    } else if (!signinWindow->staffLoggedOn && !jsonCurrentUser.empty())
    {
        stackedWidget->setCurrentIndex(memberIndex); // Member home page
    } else
    {
        stackedWidget->setCurrentIndex(guestIndex); // Guest home page
    }
}

void MainWindow::defaultAdminUser() // create default admin user
{
    QJsonObject jsonNewUserObj;
    jsonNewUserObj["username"] = "Admin";
    jsonNewUserObj["password"] = "Admin";
    jsonNewUserObj["isAdmin"] = true;
    jsonNewUserObj["address"] = "";
    jsonNewUserObj["id"] = 1;
    jsonNewUserObj["phoneNumber"] = "";
    jsonNewUserObj["currentBooks"] = "no books"; // will be changed with a custom arrary of books
    jsonNewUserObj["over-dueBooks"] = "no over-due";

    if(files.writeToJson(files.filePathMemberData ,jsonNewUserObj, 1))
    {
        qDebug() << "Admin User Created";
    }

}

void MainWindow::onHomeWindowHidden()
{
    loginCheck(); // check if user is logged in
    show(); // show main window

    QJsonObject jsonUserData = files.readFromJson(files.filePathMemberData); // read user data file
    QJsonArray jsonUserDataArray = jsonUserData.contains("data") ? jsonUserData["data"].toArray() : QJsonArray(); // get array of data from user json file
    int rowCount = jsonUserDataArray.size(); // get number of rows in user data file

    for (int i = 0; i < rowCount; i++) // loop through user data file
    {
        QJsonObject object = jsonUserDataArray[i].toObject(); // get object at index i


    }

    if (currentUser == "Admin" || currentuserid == 1) // check if current user is member, guest or staff
    {
        //output is admin
        qDebug() << "Current User: " << currentUser;
        QMessageBox::information(this, "Logged In", "You are logged in as Admin");
    } else {
        //output is guest
        qDebug() << "Current User: " << currentUser;
        QMessageBox::information(this, "Logged In", "You are logged in as Guest");
        stackedWidget->setCurrentIndex(guestIndex);
    }

}

void MainWindow::on_pushButton_guestSignIn_clicked()
{
    signinWindow->show();
}

void MainWindow::on_pushButton_StaffUsersEdit_clicked()
{
    this->hide();
    userData *userdata = new userData();
    userdata->show();
}

void MainWindow::on_pushButton_StaffBooksEdit_clicked()
{
    this->hide();
    bookData *bookdata = new bookData();
    bookdata->show();
}

// Log Out - Staff - Button
void MainWindow::on_pushButton_StaffSignOut_clicked()
{
    logout();
    stackedWidget->setCurrentIndex(guestIndex);
    qDebug() << "Current Stacked Widget: " << stackedWidget->currentIndex(); // output which stacked widget is currently active
}

// Log Out - Staff
void MainWindow::logout()
{
    QJsonObject currentJsonUserObj = files.readFromJson(files.filePathCurrentUser);
    QJsonArray jsonUserDataArray = currentJsonUserObj.contains("data") ? currentJsonUserObj["data"].toArray() : QJsonArray();
    int rowCount = jsonUserDataArray.size();

    if (!currentJsonUserObj.isEmpty())
    {
        for (int i = 0; i < rowCount; ++i)
        {
            QJsonObject object = jsonUserDataArray[i].toObject();

            int id = object["id"].toInt();
            QString currentUsername = object["userName"].toString();

            files.deleteJsonElement(files.filePathCurrentUser,id);

            QMessageBox::information(this, "Log Out", "User " + currentUsername + " has Logged out");
        }
    }
}

void MainWindow::on_navAccount_guest_clicked(bool checked)//guest Account DropDown menu
{
    isGuestFrameVisible = !isGuestFrameVisible;

    if(isGuestFrameVisible)
    {
        ui->account_dropDown_guestHome->show();
    }else {
        ui->account_dropDown_guestHome->hide();
    }

}


void MainWindow::on_navAccount_Staff_clicked(bool checked)//staff Account DropDown menu
{
    isStaffFrameVisible = !isStaffFrameVisible;


    if(isStaffFrameVisible)
    {
        ui->Account_StaffHome->show();
    }else {
        ui->Account_StaffHome->hide();
    }
}

void MainWindow::booksDisplay()
{
    QStringList headerLabels;

    QJsonObject jsonCategory = files.readFromJson(files.filePathCategory);
    QJsonObject jsonBooks = files.readFromJson(files.filePathBooks);
    QJsonArray jsonCategoryDataArray = jsonCategory.contains("data") ? jsonCategory["data"].toArray() : QJsonArray();
    QJsonArray jsonBooksDataArray = jsonBooks.contains("data") ? jsonBooks["data"].toArray() : QJsonArray();
    int rowCount = jsonCategoryDataArray.size();
    int bookCount = jsonBooksDataArray.size();

    int row = -1;
    int column = -1;

    ui->tableWidget_BookDisplay->setRowCount(rowCount);
    ui->tableWidget_BookDisplay_2->setRowCount(rowCount);
    ui->tableWidget_BookDisplay_3->setRowCount(rowCount);

    for(int i = 0; i < rowCount; ++i)
    {
        QJsonObject object = jsonCategoryDataArray[i].toObject();
        QString categoryName = object["categoryName"].toString();
        int categoryID = object["id"].toInt();

        headerLabels << categoryName;

        column = -1; // Reset the column index for each category

        for (int b = 0; b < bookCount; ++b)
        {
            QJsonObject bookObject = jsonBooksDataArray[b].toObject();
            int bookCategory = bookObject["genre"].toInt();
            QString booktitle = bookObject["title"].toString();
            QString bookAuthor = bookObject["author"].toString();
            QString bookPublishDate = bookObject["year"].toString();

            if(categoryID == bookCategory)
            {
                ++column; // Increment the column index for each book

                if (ui->tableWidget_BookDisplay->columnCount() <= column ||
                        ui->tableWidget_BookDisplay_2->columnCount() <= column ||
                        ui->tableWidget_BookDisplay_3->columnCount() <= column)
                {
                    ui->tableWidget_BookDisplay->insertColumn(column);
                    ui->tableWidget_BookDisplay_2->insertColumn(column);
                    ui->tableWidget_BookDisplay_3->insertColumn(column);
                }

                QWidget* widget = new QWidget();
                QVBoxLayout* layout = new QVBoxLayout(widget);
                QLabel* titleLabel = new QLabel(booktitle );
                QLabel* bookAuthorLabel = new QLabel("Author: " + bookAuthor );
                QLabel* bookPublishDateLabel = new QLabel("Publish Year: " + bookPublishDate);

                layout->addWidget(titleLabel);
                layout->addWidget(bookAuthorLabel);
                layout->addWidget(bookPublishDateLabel);

                layout->setContentsMargins(3, 3, 3, 3);
                widget->setLayout(layout);

                // Set the custom widget as the table item
                ui->tableWidget_BookDisplay->setCellWidget(i, column, widget);
                ui->tableWidget_BookDisplay_2->setCellWidget(i,column,widget);
                ui->tableWidget_BookDisplay_3->setCellWidget(i,column,widget);
            }
        }
    }

    ui->tableWidget_BookDisplay->setVerticalHeaderLabels(headerLabels);
    ui->tableWidget_BookDisplay_2->setVerticalHeaderLabels(headerLabels);
    ui->tableWidget_BookDisplay_3->setVerticalHeaderLabels(headerLabels);
}

void MainWindow::on_tableWidget_BookDisplay_cellClicked(int row, int column)
{
    // Handel cell click event
    qDebug() << "Row: " << row << " Column: " << column;


    QJsonObject jsonBooks = files.readFromJson(files.filePathBooks);
    QJsonArray jsonBooksDataArray = jsonBooks.contains("data") ? jsonBooks["data"].toArray() : QJsonArray();
    int bookCount = jsonBooksDataArray.size();


    QString categoryName;
    QString bookTitle;

    // Access the custom widget inside the cell
    QWidget* widget = ui->tableWidget_BookDisplay->cellWidget(row, column);
    QWidget* widget_2 = ui->tableWidget_BookDisplay_2->cellWidget(row, column);
    QWidget* widget_3 = ui->tableWidget_BookDisplay_3->cellWidget(row, column);
    if (widget || widget_2 || widget_3)
    {

        QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(widget->layout());
        QVBoxLayout* layout_2 = qobject_cast<QVBoxLayout*>(widget_2->layout());
        QVBoxLayout* layout_3 = qobject_cast<QVBoxLayout*>(widget_3->layout());
        if (layout && layout->count() >= 2 ||
                layout_2 && layout_2->count() >= 2||
                layout_3 && layout_3->count() >= 2)
        {
            QLabel* titleLabel = qobject_cast<QLabel*>(layout->itemAt(0)->widget());
            QLabel* titleLabel_2 = qobject_cast<QLabel*>(layout_2->itemAt(0)->widget());
            QLabel* titleLabel_3 = qobject_cast<QLabel*>(layout_3->itemAt(0)->widget());
            QLabel* categoryLabel = qobject_cast<QLabel*>(layout->itemAt(1)->widget());
            QLabel* categoryLabel_2 = qobject_cast<QLabel*>(layout_2->itemAt(1)->widget());
            QLabel* categoryLabel_3 = qobject_cast<QLabel*>(layout_3->itemAt(1)->widget());
            if (titleLabel && categoryLabel)
            {
                bookTitle = titleLabel->text();
                bookTitle = titleLabel_2->text();
                bookTitle = titleLabel_3->text();
                categoryName = categoryLabel->text();
                categoryName = categoryLabel_2->text();
                categoryName = categoryLabel_3->text();


                QString bookAuthor;
                QString bookDiscrpsion;
                QString bookYear;
                bool isCheckOut;
                bool isReserved;
                int memberID;
                int bookID;



                for (int i = 0; i < bookCount; ++i) {
                    QJsonObject object = jsonBooksDataArray[i].toObject();
                    QString title = object["title"].toString();

                    if(title == bookTitle)
                    {
                        bookAuthor = object["author"].toString();
                        bookDiscrpsion = object["discripsion"].toString();
                        bookYear = object["year"].toString();
                        isCheckOut = object["isCheckOut"].toBool();
                        isReserved = object["isReserved"].toBool();
                        bookID = object["id"].toInt();
                        memberID = object["memberID"].toInt();



                        if(!files.writeToJson(files.filePathCurrentBook ,object, 1))
                        {
                            qDebug() << "failed to write to current book json file";
                        }

                    }

                }


                // Use the book information as needed
                qDebug() << "Book Title: " << bookTitle;
                qDebug() << "Category Name: " << categoryName;

                openBook* openbook = new openBook();
                openbook->show();

                //checkBookOut(bookTitle, memberID, bookID);

            }
        }
    }


}


void MainWindow::returnBook(int userId, int bookId)
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
        bookObj["memberID"] = -1; // Reset member ID to indicate its not checked out
        bookObj["isCheckOut"] = isCheckedOut = false;
        bookObj["checkoutDate"] = "";

        // Remove the book from the users currentBooks array
        QJsonArray currentBooksArray = userObj["currentBooks"].toArray();
        QJsonArray updatedCurrentBooksArray;

        // Find the checked out book in the currentBooks array and remove it
        for (int i = 0; i < currentBooksArray.size(); i++)
        {
            QJsonObject bookDataObj = currentBooksArray[i].toObject();
            if (bookDataObj["bookId"].toInt() != bookId)
            {
                updatedCurrentBooksArray.append(bookDataObj);
            }
        }

        userObj["currentBooks"] = updatedCurrentBooksArray;

        jsonUserDataArray.replace(userIndex, userObj);
        jsonUserData["data"] = jsonUserDataArray;

        if (!files.writeToJson(files.filePathMemberData, jsonUserData, 1) && !files.writeToJson(files.filePathBooks, bookObj, 1))
        {
            qDebug() << "Failed to write to json file (Return Book)";
        }
        else
        {

            QMessageBox::information(this, "Book Returned", bookName + " has been returned.");

        }
    }
    else
    {
        QMessageBox::warning(this, "Invalid Book/User", "Book or user does not exist.");
    }
}

void MainWindow::loadCurrentUser()
{
    QJsonObject jsonUserData = files.readFromJson(files.filePathCurrentUser);
    QJsonArray jsonUserDataArray = jsonUserData.contains("data") ? jsonUserData["data"].toArray() : QJsonArray();


    for (int i = 0; i < jsonUserDataArray.size(); ++i) {
        QJsonObject object = jsonUserDataArray[i].toObject();

        currentUser = object["userName"].toString();
        currentuserid = object["id"].toInt();
    }


}

void MainWindow::searchBooks(const QString& searchText)
{
    searchResults.clear();

    QJsonObject jsonBookData = files.readFromJson(files.filePathBooks);

    QJsonArray jsonBooksDataArray = jsonBookData.contains("data") ? jsonBookData["data"].toArray() : QJsonArray();

    ui->tableWidget_BookDisplay->clearContents();

    if (searchText.isEmpty())
    {
        booksDisplay();

        return;
    }

    for (int i = 0; i < jsonBooksDataArray.size(); ++i)
    {
        QJsonObject bookObject = jsonBooksDataArray[i].toObject();
        QString bookTitle = bookObject["title"].toString();

        // Perform case-insensitive search
        if (bookTitle.contains(searchText, Qt::CaseInsensitive))
        {
            QString categoryName = getCategoryName(bookObject["genre"].toInt());
            searchResults.append(qMakePair(bookTitle, categoryName));
        }
    }

    ui->tableWidget_BookDisplay->clearContents();
    ui->tableWidget_BookDisplay->setRowCount(searchResults.size());

    for (int i = 0; i < searchResults.size(); ++i)
    {
        const QPair<QString, QString>& result = searchResults[i];
        QString bookTitle = result.first;
        QString categoryName = result.second;

        // Create a custom widget for the table item
        QWidget* widget = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout(widget);
        QLabel* titleLabel = new QLabel(bookTitle);
        QLabel* categoryLabel = new QLabel(categoryName);
        layout->addWidget(titleLabel);
        layout->addWidget(categoryLabel);
        layout->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(layout);

        // Set the custom widget as the table item
        ui->tableWidget_BookDisplay->setCellWidget(i, 0, widget);
    }
}

QString MainWindow::getCategoryName(int categoryID)
{
    QJsonObject jsonUserData = files.readFromJson(files.filePathMemberData);
    QJsonArray jsonCategoryDataArray = jsonUserData.contains("data") ? jsonUserData["data"].toArray() : QJsonArray();

    for (int i = 0; i < jsonCategoryDataArray.size(); ++i)
    {
        QJsonObject object = jsonCategoryDataArray[i].toObject();
        int category = object["id"].toInt();
        if (category == categoryID)
        {
            return object["categoryName"].toString();
        }
    }
    return QString();
}

void MainWindow::on_navSearch_textChanged(const QString &arg1)
{
    searchBooks(arg1);

    //searchBooks(ui->navSearch_2->text());
    //searchBooks(ui->navSearch_3->text());
}

void MainWindow::clearCurrentBook()
{
    QJsonObject currentJsonBookObj = files.readFromJson(files.filePathCurrentBook);
    QJsonArray jsonBookDataArray = currentJsonBookObj.contains("data") ? currentJsonBookObj["data"].toArray() : QJsonArray();
    int rowCount = jsonBookDataArray.size();

    if (!currentJsonBookObj.isEmpty())
    {
        for (int i = 0; i < rowCount; ++i)
        {
            QJsonObject object = jsonBookDataArray[i].toObject();

            int id = object["id"].toInt();
            QString currentUsername = object["userName"].toString();

            files.deleteJsonElement(files.filePathCurrentBook,id);


        }
    }
}
