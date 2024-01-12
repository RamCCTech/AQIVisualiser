#include "JSONReader.h"

JSONReader::JSONReader() {
}

JSONReader::~JSONReader() {
}

std::map<std::string, std::map<std::string, int>> JSONReader::readJsonFile(const QString& filePath) {
    std::map<std::string, std::map<std::string, int>> result;

    // Open the JSON file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Unable to open file" << filePath;
        return result;  // Return an empty map if there's an error
    }

    // Read the JSON data from the file
    QByteArray jsonData = file.readAll();
    file.close();

    // Parse the JSON document
    QJsonDocument document = QJsonDocument::fromJson(jsonData);

    if (!document.isNull()) {
        if (document.isObject()) {
            // Extract data from the JSON object
            QJsonObject jsonObject = document.object();

            // Iterate over each state in the JSON object
            for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
                QString state = it.key();
                QJsonObject dateAqiObject = it.value().toObject();
                std::map<std::string, int> dateAqiMap;

                // Iterate over date-AQI pairs in the state
                for (auto dateIt = dateAqiObject.begin(); dateIt != dateAqiObject.end(); ++dateIt) {
                    QString date = dateIt.key();
                    int aqi = dateIt.value().toInt();
                    dateAqiMap[date.toStdString()] = aqi;
                }

                // Store the state and its corresponding date-AQI map
                result[state.toStdString()] = dateAqiMap;
            }
        }
        else {
            qDebug() << "Error: The JSON document is not an object.";
        }
    }
    else {
        qDebug() << "Error: Failed to parse JSON document.";
    }

    return result;
}
