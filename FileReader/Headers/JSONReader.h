#pragma once

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "filereader_global.h"

class FILEREADER_EXPORT JSONReader
{
public:
	JSONReader();
	~JSONReader();
	std::map<std::string, std::map<std::string, int>> readJsonFile(const QString& filePath);
};
