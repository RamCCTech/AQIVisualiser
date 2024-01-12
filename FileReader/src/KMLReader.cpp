#include "filereader_global.h"
#include "KMLReader.h"
#include <iostream>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QBuffer>
#include <QFile>

KMLReader::KMLReader() {
}

KMLReader::~KMLReader() {
}

std::vector<State> KMLReader::parseKMLFromFile(const QString& filePath) {
    // Open and read KML file using Qt
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error: Unable to open file "<< std::endl;
        return {};
    }

    // Use QBuffer for parsing KML content
    QBuffer buffer;
    buffer.setData(file.readAll());
    buffer.open(QIODevice::ReadOnly);

    // Parse KML content
    QDomDocument kmlDocument;
    if (!kmlDocument.setContent(&buffer)) {
        std::cerr << "Error: Unable to parse KML content from file " << std::endl;
        return {};
    }

    // Extract Placemark data using QDomDocument
    return parseKML(kmlDocument);
}

std::vector<State> KMLReader::parseKML(const QDomDocument& kmlDocument) {
    std::vector<State> states;

    // Get the root element of the KML document
    QDomElement kmlRoot = kmlDocument.documentElement();
    QDomNodeList placemarks = kmlRoot.elementsByTagName("Placemark");

    for (int i = 0; i < placemarks.size(); ++i) {
        QDomElement placemark = placemarks.at(i).toElement();

        // Extract Name and Type data
        QDomElement nameElement = placemark.firstChildElement("ExtendedData").firstChildElement("Data").firstChildElement("value");
        QString name = nameElement.text();

        // Extract coordinate data
        QDomElement coordinatesElement = placemark.firstChildElement("Polygon").firstChildElement("outerBoundaryIs").firstChildElement("LinearRing").firstChildElement("coordinates");
        QString coordinatesStr = coordinatesElement.text();

        // Process coordinate data
        std::vector<Point3D> statePoints = processCoordinates(coordinatesStr);

        // Create a State object for each Placemark
        State state;
        state.setName(name.toStdString());
        state.setPoints(statePoints);
        states.push_back(state);
    }

    return states;
}

std::vector<Point3D> KMLReader::processCoordinates(const QString& coordinatesStr) {
    std::vector<Point3D> statePoints;

    QStringList coordinatePairs = coordinatesStr.split('\n', Qt::SkipEmptyParts);
    for (int i = 0; i < coordinatePairs.size();i+=10) {
        QStringList coordinates = coordinatePairs[i].split(' ', Qt::SkipEmptyParts);
        if (coordinates.size() == 2) {
            bool conversionSuccessX, conversionSuccessY;
            float x = coordinates.at(0).toFloat(&conversionSuccessX);
            float y = coordinates.at(1).toFloat(&conversionSuccessY);

            if (conversionSuccessX && conversionSuccessY) {
                Point3D point(x - 80, y - 20);
                statePoints.push_back(point);
            }
        }
    }
    return statePoints;
}
