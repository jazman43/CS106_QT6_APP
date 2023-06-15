#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H



#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>




class fileManagement
{

public:
    fileManagement();
    ~fileManagement();

    bool writeToJson(const QString& filePath, const QJsonObject& jsonObject, bool append);



    QJsonObject readFromJson(const QString& filePath);

    bool modifyJson(const QString& filePath, const QString& elementKey, const QJsonValue& newValue, const int& objectId);

    bool deleteJsonElement(const QString& filePath, const int& objectId);
    //finds last id of an json object and returns it with one added
    int checkFileID(const QString& filePath, const QString& key);

    QJsonObject selectObjectByID(const QString& filePath, int id);

    bool checkFileExists(const QString& filePath);

    //file paths
    QString filePathMemberData = "./MemberData.json";

    QString filePathBooks= "./BooksData.json";

    QString filePathCurrentUser = "./CurrentUser.json";

    QString filePathCategory = "./BookCategorys.json";

    QString filePathCurrentBook = "./CurrentBook.json";

private:



};

#endif // FILEMANAGEMENT_H
