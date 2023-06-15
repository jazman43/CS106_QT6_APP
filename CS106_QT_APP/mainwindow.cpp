#include "./H_files/Utils/mainwindow.h"
#include "./UI_files/ui_mainwindow.h"

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

    logout(); //call the logout function

    //hide Account Drop Down Menus
    ui->account_dropDown_guestHome->hide();
    ui->Account_StaffHome->hide();

    stackedWidget = findChild<QStackedWidget*>("stackedWidget"); // find stacked widgets
    stackedWidget->setCurrentIndex(1); // set first Home screen as guest

    booksDisplay();

    connect(signinWindow, &Signin::userChecked, this, &MainWindow::loginCheck); //connect signinWindow to loginCheck

    QJsonObject adminUser = files.selectObjectByID(files.filePathMemberData,1);
    if(adminUser.empty())
    {
        defaultAdminUser();
    }

    connect(this, &MainWindow::homeWindowHidden, this, &MainWindow::onHomeWindowHidden);

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
        stackedWidget->setCurrentIndex(2); // Admin Home page

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
        stackedWidget->setCurrentIndex(0); // Member home page
    } else
    {
        stackedWidget->setCurrentIndex(1); // Guest home page
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
        QString username = object["username"].toString(); // get username from object
    }

    if (currentUser == "Admin") // check if current user is member, guest or staff
    {
        //output is admin
        qDebug() << "Current User: " << currentUser;
        QMessageBox::information(this, "Logged In", "You are logged in as Admin");
    } else {
        //output is guest
        qDebug() << "Current User: " << currentUser;
        QMessageBox::information(this, "Logged In", "You are logged in as Guest");
        stackedWidget->setCurrentIndex(1);
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
    stackedWidget->setCurrentIndex(1);
    qDebug() << "Current Stacked Widget: " << stackedWidget->currentIndex(); // output which stacked widget is currently active
}

// Log Out - Member - Button
void MainWindow::on_pushButton_MemberSignOut_clicked()
{
    logout();
    stackedWidget->setCurrentIndex(1);
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
    QJsonArray jsonCategoryDataArray = jsonCategory.contains("data") ? jsonCategory["data"].toArray() : QJsonArray();
    int rowCount = jsonCategoryDataArray.size();

    ui->tableWidget_BookDisplay->setRowCount(rowCount);

    for(int i = 0; i < rowCount; ++i)
    {
        QJsonObject object = jsonCategoryDataArray[i].toObject();
        QString categoryName = object["categoryName"].toString();
        headerLabels << categoryName;
    }

    ui->tableWidget_BookDisplay->setVerticalHeaderLabels(headerLabels);
}

void MainWindow::on_tableWidget_BookDisplay_cellClicked(int row, int column)
{
    // Handel cell click event
    qDebug() << "Row: " << row << " Column: " << column;
}

void MainWindow::checkBookOut()
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
