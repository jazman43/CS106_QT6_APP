#include "./H_files/Utils/filemanagement.h"

fileManagement::fileManagement()
{
    QFile fileUser(filePathMemberData);

    if(!fileUser.exists())
    {
        if(fileUser.open(QIODevice::WriteOnly))
        {
            fileUser.close();
        }
    }

}

fileManagement::~fileManagement(){}

// Read from json file
QJsonObject fileManagement::readFromJson(const QString& filePath)
{
    QFile jsonFile(filePath);
    if(!jsonFile.open(QIODevice::ReadOnly))
    {
        return QJsonObject();
    }

    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();


    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    if(jsonDoc.isNull())
    {
        return QJsonObject();
    }

    return jsonDoc.object();

}


// Write to json file
bool fileManagement::writeToJson(const QString& filePath, const QJsonObject& jsonObject, bool append)
{
    QFile jsonFile(filePath);

    if (!jsonFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return false;
    }

    QJsonDocument jsonDoc;
    QJsonObject existingJsonObject;

    if (append)
    {
        QByteArray existingData = jsonFile.readAll();
        jsonDoc = QJsonDocument::fromJson(existingData);

        if (!jsonDoc.isNull() && jsonDoc.isObject())
        {
            existingJsonObject = jsonDoc.object();
        }
        else
        {
            jsonDoc = QJsonDocument();
        }
    }

    if (existingJsonObject.contains("data") && existingJsonObject["data"].isArray())
    {
        QJsonArray dataArray = existingJsonObject["data"].toArray();
        dataArray.append(jsonObject);
        existingJsonObject["data"] = dataArray;
    }
    else
    {
        QJsonArray dataArray;
        dataArray.append(jsonObject);
        existingJsonObject["data"] = dataArray;
    }

    jsonDoc.setObject(existingJsonObject);

    jsonFile.resize(0);

    QByteArray jsonData = jsonDoc.toJson();

    qint64 bytesWritten = jsonFile.write(jsonData);

    jsonFile.close();

    return (bytesWritten != -1);

}

// Select object by id and modify it
bool fileManagement::modifyJson(const QString& filePath, const QString& elementKey, const QJsonValue& newValue, const int& objectId)
{
    QFile jsonFile(filePath);
    if (!jsonFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return false;
    }

    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    if (jsonDoc.isNull() || !jsonDoc.isObject())
    {
        return false;
    }

    QJsonObject jsonObject = jsonDoc.object();

    if (!jsonObject.contains("data") || !jsonObject["data"].isArray())
    {
        return false;
    }

    QJsonArray jsonArray = jsonObject["data"].toArray();

    bool found = false;
    for (int i = 0; i < jsonArray.size(); i++)
    {
        QJsonObject innerObject = jsonArray[i].toObject();
        if (innerObject.contains("id") && innerObject["id"].toInt() == objectId)
        {
            found = true;
            if (innerObject.contains(elementKey))
            {
                innerObject[elementKey] = newValue;
                jsonArray.replace(i, innerObject);
                break;
            }
        }
    }

    if (!found)
    {
        return false;
    }

    jsonObject["data"] = jsonArray;
    jsonDoc.setObject(jsonObject);

    if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        return false;
    }

    jsonData = jsonDoc.toJson();

    qint64 bytesWritten = jsonFile.write(jsonData);

    jsonFile.close();

    return (bytesWritten != -1 && bytesWritten == jsonData.size());
}

// Delete object by id
bool fileManagement::deleteJsonElement(const QString& filePath, const int& objectId)
{

    QFile jsonFile(filePath);
    if (!jsonFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return false;
    }

    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    if (jsonDoc.isNull() || !jsonDoc.isObject())
    {
        return false;
    }

    QJsonObject jsonObject = jsonDoc.object();

    if (!jsonObject.contains("data") || !jsonObject["data"].isArray())
    {
        return false;
    }

    QJsonArray jsonArray = jsonObject["data"].toArray();

    bool found = false;
    for (int i = 0; i < jsonArray.size(); i++)
    {
        QJsonObject innerObject = jsonArray[i].toObject();
        if (innerObject.contains("id") && innerObject["id"].toInt())
        {
            found = true;
            jsonArray.removeAt(i);
            break;
        }
    }

    if (!found)
    {
        return false;
    }

    jsonObject["data"] = jsonArray;
    jsonDoc.setObject(jsonObject);

    if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        return false;
    }

    jsonData = jsonDoc.toJson();

    qint64 bytesWritten = jsonFile.write(jsonData);

    jsonFile.close();

    return (bytesWritten != -1 && bytesWritten == jsonData.size());

}

//finds last id of an json object and returns it with one added
int fileManagement::checkFileID(const QString& filePath, const QString& key)
{
    int  maxId = 1;


    QJsonObject jsonObject = readFromJson(filePath);


    if (jsonObject.isEmpty() || !jsonObject.contains("data")) {
        return 1;
    }
    else {
        QJsonArray dataArray = jsonObject["data"].toArray();

        for (const QJsonValue& value : dataArray) {
            QJsonObject nestedObject = value.toObject();

            if (nestedObject.contains(key))
            {
                int id = nestedObject[key].toInt();

                if (id > maxId)
                    maxId = id;
            }


        }
    }

    return maxId + 1;
}

// Select object by ID
QJsonObject fileManagement::selectObjectByID(const QString& filePath, int id)
{
    QJsonObject jsonObject = readFromJson(filePath);
    const QString& key = "id";

    if (jsonObject.contains("data") && jsonObject["data"].isArray()) {
        QJsonArray dataArray = jsonObject["data"].toArray();

        for (const QJsonValue& value : dataArray) {
            QJsonObject nestedObject = value.toObject();

            if (nestedObject.contains(key) && nestedObject[key].toInt() == id) {
                return nestedObject;
            }
        }
    }

    // If the ID is not found or the JSON structure is invalid, return an empty object
    return QJsonObject();
}

// checks if fine exists
bool fileManagement::checkFileExists(const QString& filePath)
{
    QFile checkFile(filePath);

    return checkFile.exists();
}
