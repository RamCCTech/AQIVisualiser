#pragma once
#include "State.h"
#include <vector>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include "filereader_global.h"

class FILEREADER_EXPORT KMLReader
{
public:
	KMLReader();
	~KMLReader();
	std::vector<State> parseKMLFromFile(const QString& filePath);

private:
	std::vector<State> parseKML(const QDomDocument& kmlDocument);
	std::vector<Point3D> processCoordinates(const QString& coordinatesStr);
};