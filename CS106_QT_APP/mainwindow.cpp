#include "./H_files/Utils/mainwindow.h"
#include "./UI_files/ui_mainwindow.h"

#include "./H_files/openbook.h"
#include <QDebug>
#include "./H_files/userdata.h"
#include "./H_files/bookdata.h"
#include <QScrollArea>
#include <QMessageBox>
#include <QTableWidget>
#include <QWidget>
#include <QString>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , signinWindow(new Signin)
{
    ui->setupUi(this); // set up ui

    // Sign out any signed in users
    QJsonObject usersLoggedIn = files.readFromJson(files.filePathCurrentUser);




    //hide Account Drop Down Menus
    ui->account_dropDown_guestHome->hide();
    ui->Account_StaffHome->hide();
    ui->Account_MemberHome->hide();

    // Hide Notification Dropdown Menues
    ui->Notification_MemberHome->hide();

    // Hide Wishlist Dropdown Menu
    ui->Wishlist_MemberHome->hide();

    stackedWidget = findChild<QStackedWidget*>("stackedWidget"); // find stacked widgets
    stackedWidget->setCurrentIndex(guestIndex); // set first Home screen as guest

    booksDisplay(ui->tableWidget_BookDisplay);
    booksDisplay(ui->tableWidget_BookDisplay_Member);
    booksDisplay(ui->tableWidget_BookDisplay_3);

    connect(signinWindow, &Signin::userChecked, this, &MainWindow::loginCheck); //connect signinWindow to loginCheck

    // Cell pressed book highlights
    connect(ui->tableWidget_BookDisplay, &QTableWidget::cellClicked, this, [=](int row, int column){
        on_tableWidget_BookDisplay_cellClicked(ui->tableWidget_BookDisplay, row, column);
    });
    connect(ui->tableWidget_BookDisplay_Member, &QTableWidget::cellClicked, this, [=](int row, int column){
        on_tableWidget_BookDisplay_cellClicked(ui->tableWidget_BookDisplay_Member, row, column);
    });
    connect(ui->tableWidget_BookDisplay_3, &QTableWidget::cellClicked, this, [=](int row, int column){
        on_tableWidget_BookDisplay_cellClicked(ui->tableWidget_BookDisplay_3, row, column);
    });




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

    QString username;
    QString loggedInUsername;

    if(signinWindow->staffLoggedOn && !jsonCurrentUser.empty()) // if staff is logged on
    {
        stackedWidget->setCurrentIndex(staffIndex); // Admin Home page


        for (int i = 0; i < rowCount; i++) // loop through user data file
        {
            QJsonObject object = jsonUserDataArray[i].toObject(); // get object at index i
            username = object["username"].toString(); // get username from object
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

        qDebug() << "staff logged in " + loggedInUsername;

    } else if (!signinWindow->staffLoggedOn && !jsonCurrentUser.empty()) // if member is logged on
    {
        stackedWidget->setCurrentIndex(memberIndex); // Member home page


        for (int i = 0; i < rowCount; i++) // loop through user data file
        {
            QJsonObject memberObject = jsonUserDataArray[i].toObject(); // get object at index i
            username = memberObject["username"].toString(); // get username from object
            int userID = memberObject["id"].toInt();


            if (userID == signinWindow->getId()) // if username and password are valid
            {
                loggedInUsername = username; // set logged in username to username
                break;
            }
        }
        currentUser = loggedInUsername;

        qDebug() << "member logged in " + loggedInUsername;
    } else // if no user is logged on
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

    loadCurrentUser();
    loginCheck(); // check if user is logged in
    show();// show main window


    /*
     * setting page index based on whos logged in inbween pages/windows
     *
     *
     * /*/

    QJsonObject jsonUserData = files.readFromJson(files.filePathMemberData); // read user data file
    QJsonArray jsonUserDataArray = jsonUserData.contains("data") ? jsonUserData["data"].toArray() : QJsonArray(); // get array of data from user json file
    int rowCount = jsonUserDataArray.size(); // get number of rows in user data file

    for (int i = 0; i < rowCount; i++) // loop through user data file
    {
        QJsonObject object = jsonUserDataArray[i].toObject(); // get object at index i


        if (currentUser == "Admin" || currentuserid == 1) // check if current user is member, guest or staff
        {
            //output is admin
            qDebug() << "Current User: " << currentuserid;
            //QMessageBox::information(this, "Logged In", "You are logged in as Admin");
            stackedWidget->setCurrentIndex(staffIndex);
        } else if (currentuserid >= 2)
        {
            //output is member
            qDebug() << "Current User: " << currentuserid;
            //QMessageBox::information(this, "Logged In", "You are logged in as Member");
            stackedWidget->setCurrentIndex(memberIndex);
        } else
        {
            //output is guest
            qDebug() << "Current User: " << currentuserid;
            //QMessageBox::information(this, "Logged Out", "Hello Guest");
            stackedWidget->setCurrentIndex(guestIndex);
        }
    }
}
//shows the sign in window when user clicks sign in button
void MainWindow::on_pushButton_guestSignIn_clicked()
{
    signinWindow->show();
}
//shows the user Data window when Staff/Admin clicks the user button and hides main window
void MainWindow::on_pushButton_StaffUsersEdit_clicked()
{
    this->hide();
    userData *userdata = new userData();
    userdata->show();
}
//shows the book Data window when Staff/Admin clicks the book button and hides main window
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

// Log Out - Member - Button
void MainWindow::on_pushButton_MemberSignOut_clicked()
{
    logout();
    stackedWidget->setCurrentIndex(guestIndex);
    qDebug() << "Current Stacked Widget: " << stackedWidget->currentIndex(); // output which stacked widget is currently active
}

// Log Out
void MainWindow::logout()
{
    QJsonObject currentJsonUserObj = files.readFromJson(files.filePathCurrentUser);
    QJsonArray jsonUserDataArray = currentJsonUserObj.contains("data") ? currentJsonUserObj["data"].toArray() : QJsonArray();
    int rowCount = jsonUserDataArray.size();


    //goes though and deletes the contents of current uses json file
    if (!currentJsonUserObj.isEmpty())
    {
        for (int i = 0; i < rowCount; ++i)
        {
            QJsonObject object = jsonUserDataArray[i].toObject();

            int id = object["id"].toInt();
            QString currentUsername = object["username"].toString();

            files.deleteJsonElement(files.filePathCurrentUser,id);

            QMessageBox::information(this, "Log Out", "User " + currentUsername + " has Logged out");
        }
    }

}

/* -----
Navigation Buttons
----- */

/* Account Buttons */

// Guest Account
void MainWindow::on_navAccount_guest_clicked(bool checked)//guest Account DropDown menu
{
    isGuestAccountFrameVisible = !isGuestAccountFrameVisible;

    if(isGuestAccountFrameVisible)
    {
        ui->account_dropDown_guestHome->show();
    }else {
        ui->account_dropDown_guestHome->hide();
    }

}

// Staff Account
void MainWindow::on_navAccount_Staff_clicked(bool checked)//staff Account DropDown menu
{
    isStaffAccountFrameVisible = !isStaffAccountFrameVisible;


    if(isStaffAccountFrameVisible)
    {
        ui->Account_StaffHome->show();
    }else {
        ui->Account_StaffHome->hide();
    }
}

// Member Account
void MainWindow::on_navAccount_Member_clicked(bool checked)
{
    isMemberAccountFrameVisible = !isMemberAccountFrameVisible;

    if(isMemberAccountFrameVisible)
    {
        ui->Account_MemberHome->show();
    }else {
        ui->Account_MemberHome->hide();
    }
}

/* Notification Buttons */

// Member Notification
void MainWindow::on_navNotification_Member_clicked(bool checked)
{
    isMemberNotificationFrameVisible = !isMemberNotificationFrameVisible;

    if(isMemberNotificationFrameVisible)
    {
        ui->Notification_MemberHome->show();
    }else {
        ui->Notification_MemberHome->hide();
    }
}

// Staff Notification
void MainWindow::on_navNotification_Staff_clicked(bool checked)
{

}

/* Wishlist Button */

// Member Wishlist
void MainWindow::on_navWishlist_Member_clicked(bool checked)
{
    isMemberWishlistFrameVisible = !isMemberWishlistFrameVisible;

    if(isMemberWishlistFrameVisible)
    {
        ui->Wishlist_MemberHome->show();
    }else {
        ui->Wishlist_MemberHome->hide();
    }
}
//displays all books in a table
void MainWindow::booksDisplay(QTableWidget* &bookTable)
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

    bookTable->setRowCount(rowCount);

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

                if (bookTable->columnCount() <= column )
                {
                    bookTable->insertColumn(column);

                }

                // Create a new widget for each table
                QWidget* widget1 = createWidget(booktitle, bookAuthor, bookPublishDate);


                // Set the custom widget as the table item
                bookTable->setCellWidget(i, column, widget1);

            }
        }
    }

    bookTable->setVerticalHeaderLabels(headerLabels);

}

QWidget* MainWindow::createWidget(const QString& booktitle, const QString& bookAuthor, const QString& bookPublishDate)
{
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

    return widget;
}


void MainWindow::on_tableWidget_BookDisplay_cellClicked(QTableWidget* tableWidget, int row, int column)
{
    // Handel cell click event
    qDebug() << "Row: " << row << " Column: " << column;

    QJsonObject jsonBooks = files.readFromJson(files.filePathBooks);
    QJsonArray jsonBooksDataArray = jsonBooks.contains("data") ? jsonBooks["data"].toArray() : QJsonArray();
    int bookCount = jsonBooksDataArray.size();

    QString categoryName;
    QString bookTitle;

    // Access the custom widget inside the cell
    QWidget* widget = tableWidget->cellWidget(row, column);
    if (widget)
    {
        QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(widget->layout());
        if (layout && layout->count() >= 3)
        {
            QLabel* titleLabel = qobject_cast<QLabel*>(layout->itemAt(0)->widget());
            QLabel* authorLabel = qobject_cast<QLabel*>(layout->itemAt(1)->widget());
            QLabel* publishDateLabel = qobject_cast<QLabel*>(layout->itemAt(2)->widget());
            if (titleLabel && authorLabel && publishDateLabel)
            {
                bookTitle = titleLabel->text();
                QString bookAuthor = authorLabel->text().remove(0, 8); // Remove "Author: " prefix
                QString bookPublishDate = publishDateLabel->text().remove(0, 13); // Remove "Publish Year: " prefix

                QString bookDescription;
                bool isCheckOut;
                bool isReserved;
                int memberID;
                int bookID;

                for (int i = 0; i < bookCount; ++i) {
                    QJsonObject object = jsonBooksDataArray[i].toObject();
                    QString title = object["title"].toString();

                    if(title == bookTitle)
                    {
                        bookDescription = object["description"].toString();
                        isCheckOut = object["isCheckOut"].toBool();
                        isReserved = object["isReserved"].toBool();
                        bookID = object["id"].toInt();
                        memberID = object["memberID"].toInt();

                        if(!files.writeToJson(files.filePathCurrentBook, object, 1))
                        {
                            qDebug() << "failed to write to current book json file";
                        }
                    }
                }

                // Use the book information as needed
                qDebug() << "Book Title: " << bookTitle;
                qDebug() << "Book Author: " << bookAuthor;
                qDebug() << "Publish Year: " << bookPublishDate;

                openBook* openbook = new openBook();
                openbook->show();

                //checkBookOut(bookTitle, memberID, bookID);
            }
        }
    }
}

void MainWindow::loadCurrentUser()
{
    QJsonObject jsonUserData = files.readFromJson(files.filePathCurrentUser);
    QJsonArray jsonUserDataArray = jsonUserData.contains("data") ? jsonUserData["data"].toArray() : QJsonArray();


    for (int i = 0; i < jsonUserDataArray.size(); ++i) {
        QJsonObject object = jsonUserDataArray[i].toObject();

        currentUser = object["username"].toString();
        currentuserid = object["id"].toInt();
    }


}

void MainWindow::searchBooks(const QString& searchText)
{
    searchResults.clear();

    QJsonObject jsonBookData = files.readFromJson(files.filePathBooks);

    QJsonArray jsonBooksDataArray = jsonBookData.contains("data") ? jsonBookData["data"].toArray() : QJsonArray();

    ui->tableWidget_BookDisplay->clearContents();
    ui->tableWidget_BookDisplay_3->clearContents();
    ui->tableWidget_BookDisplay_Member->clearContents();

    if (searchText.isEmpty())
    {
        booksDisplay(ui->tableWidget_BookDisplay);
        booksDisplay(ui->tableWidget_BookDisplay_Member);
        booksDisplay(ui->tableWidget_BookDisplay_3);

        return;
    }

    for (int i = 0; i < jsonBooksDataArray.size(); ++i)
    {
        QJsonObject bookObject = jsonBooksDataArray[i].toObject();
        QString bookTitle = bookObject["title"].toString();
        QString bookAuthor = bookObject["author"].toString();
        QString publishDate = bookObject["year"].toString();

        // Perform case-insensitive search
        if (bookTitle.contains(searchText, Qt::CaseInsensitive))
        {
            //QString categoryName = getCategoryName(bookObject["genre"].toInt());
            searchResults.append(qMakePair(bookTitle, QString("%1\n%2").arg(bookAuthor, publishDate)));
        }
    }

    ui->tableWidget_BookDisplay->clearContents();
    ui->tableWidget_BookDisplay_3->clearContents();
    ui->tableWidget_BookDisplay_Member->clearContents();

    ui->tableWidget_BookDisplay->setRowCount(searchResults.size());
    ui->tableWidget_BookDisplay_3->setRowCount(searchResults.size());
    ui->tableWidget_BookDisplay_Member->setRowCount(searchResults.size());

    for (int i = 0; i < searchResults.size(); ++i)
    {
        const QPair<QString, QString>& result = searchResults[i];
        QString bookTitle = result.first;
        QString bookDetails = result.second;

        //createWidget(bookTitle,bookDetails,"");

        // Create a custom widget for the table item
        QWidget* widget = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout(widget);
        QLabel* titleLabel = new QLabel(bookTitle);
        QLabel* categoryLabel = new QLabel(bookDetails);
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

            files.deleteJsonElement(files.filePathCurrentBook,id);

        }
    }
}

void MainWindow::on_navSearch_3_textChanged(const QString &arg1)
{
    searchBooks(arg1);

}


void MainWindow::on_navSearch_2_textChanged(const QString &arg1)
{
    searchBooks(arg1);
}

