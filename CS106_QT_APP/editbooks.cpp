#include "./H_files/editbooks.h"
#include "./UI_files/ui_editbooks.h"

// Create QWidget editBooks
editBooks::editBooks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editBooks)
{
    ui->setupUi(this);

    addCategorys();
}

// Delete QWidget editBooks
editBooks::~editBooks()
{
    delete ui;
}

// Find the selected book
void editBooks::on_pushButton_findID_clicked()
{
    QString id = ui->lineEdit_findId->text();
    QJsonObject selectedID = files.selectObjectByID(files.filePathBooks, id.toInt());

    // Get the book information
    QString bookTitle = selectedID["title"].toString();
    QString author = selectedID["author"].toString();
    int genre = selectedID["genre"].toInt();
    QString year = selectedID["year"].toString();
    QString description = selectedID["description"].toString();

    // Set the book information
    ui->lineEdit_title->setText(bookTitle);
    ui->lineEdit_author->setText(author);
    ui->comboBox_Genre->setCurrentIndex(genre);
    ui->lineEdit_Year->setText(year);
    ui->textEdit_description->setPlainText(description);
}

// Save the new book information
void editBooks::on_pushButton_saveNewBook_clicked()
{

    int id = ui->lineEdit_findId->text().toInt();
    QString bookTitle = ui->lineEdit_title->text();
    QString author = ui->lineEdit_author->text();
    int genre = ui->comboBox_Genre->currentIndex();
    QString year  = ui->lineEdit_Year->text();
    QString description = ui->textEdit_description->toPlainText();

    QJsonObject object;
    object["tilte"] =bookTitle;
    object["author"] = author;
    object["genre"] = genre;
    object["year"] = year;
    object["description"] = description;

    // Modify the book information
    if(files.modifyJson(files.filePathBooks,"tilte",id, QJsonObject(),bookTitle))
    {
        qDebug() << "saveed to BooksData.json (Edit Books)";
    }

    if(files.modifyJson(files.filePathBooks,"author",id, QJsonObject(),author))
    {
        qDebug() << "saveed to BooksData.json (Edit Books)";
    }
    if(files.modifyJson(files.filePathBooks,"genre",id, QJsonObject(),genre))
    {
        qDebug() << "saveed to BooksData.json (Edit Books)";
    }
    if(files.modifyJson(files.filePathBooks,"year",id, QJsonObject(),year))
    {
        qDebug() << "saveed to BooksData.json (Edit Books)";
    }
    if(files.modifyJson(files.filePathBooks,"description",id, QJsonObject(),description))
    {
        qDebug() << "saveed to BooksData.json (Edit Books)";
    }

    emit modifyedBook();

    accept();
}

void editBooks::addCategorys()
{
    QJsonObject categoryObject = files.readFromJson(files.filePathCategory);
    QJsonArray jsonCategoryObjectArray = categoryObject.contains("data") ? categoryObject["data"].toArray() : QJsonArray();

    int rowCount = jsonCategoryObjectArray.size();

    for(int i = 0; i < rowCount;i++)
    {
        QJsonObject object = jsonCategoryObjectArray[i].toObject();

        QString categoryName = object["categoryName"].toString();

        ui->comboBox_Genre->addItem(categoryName);
    }
}
