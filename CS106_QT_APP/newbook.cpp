#include "./H_files/newbook.h"
#include "./UI_files/ui_newbook.h"


// Create QWidget newBook
newBook::newBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newBook)
{
    ui->setupUi(this);

    addCategorys();
}

// Delete QWidget newBook
newBook::~newBook()
{
    delete ui;
}

// Save the new book information
void newBook::on_pushButton_saveNewBook_clicked()
{


    // Get the book information
    QString bookTitle = ui->lineEdit_title->text();
    QString author = ui->lineEdit_author->text();
    int genre = ui->comboBox_Genere->currentIndex();
    QString year = ui->lineEdit_Year->text();
    QString description = ui->textEdit_description->toPlainText();

    bool isCheckedOut = false;
    bool isReaeved = false;
    int memberID = -1;
    int checkoutDate = -1;
    // Set the book information

    QJsonObject jsonNewBookObj;
    jsonNewBookObj["title"] = bookTitle;
    jsonNewBookObj["author"] = author;
    jsonNewBookObj["genre"] = genre;
    jsonNewBookObj["year"] = year;
    jsonNewBookObj["id"] = files.checkFileID(files.filePathBooks, "id");
    jsonNewBookObj["description"] = description;
    jsonNewBookObj["isCheckOut"] = isCheckedOut;
    jsonNewBookObj["isReserved"] = isReaeved;
    jsonNewBookObj["memberID"] = memberID;
    jsonNewBookObj["checkoutDate"] = checkoutDate;

    // Write to json file
    if(files.writeToJson(files.filePathBooks ,jsonNewBookObj, 1))
    {
        emit addedNewBook();

        accept();
    }

}

// Cancel the new book information
void newBook::on_pushButton_cancel_clicked()
{
    reject();
}

void newBook::addCategorys()
{
    QJsonObject categoryObject = files.readFromJson(files.filePathCategory);
    QJsonArray jsonCategoryObjectArray = categoryObject.contains("data") ? categoryObject["data"].toArray() : QJsonArray();

    int rowCount = jsonCategoryObjectArray.size();

    for(int i = 0; i < rowCount;i++)
    {
        QJsonObject object = jsonCategoryObjectArray[i].toObject();

        QString categoryName = object["categoryName"].toString();

        ui->comboBox_Genere->addItem(categoryName);
    }
}
