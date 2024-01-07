#include "stdafx.h"
#include "KMLReader.h"
#include <iostream>
#include <vector>
#include <regex>
#include <sstream>
#include <fstream>

KMLReader::KMLReader() {
}

KMLReader::~KMLReader() {
}

std::vector<State> KMLReader::parseKMLFromFile(const std::string& filePath) {
    // Open and read KML file
    std::ifstream kmlFile(filePath);
    if (!kmlFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
        return {};
    }

    // Read KML content from the file
    std::string kmlContent((std::istreambuf_iterator<char>(kmlFile)), std::istreambuf_iterator<char>());
    kmlFile.close();

    // Parse KML content
    return parseKML(kmlContent);
}

std::vector<State> KMLReader::parseKML(const std::string& kmlData) {
    std::vector<State> states;

    // Define regex pattern for extracting Placemark data
    std::regex placemarkRegex(R"(<Placemark>[^]*?<Data name="Name"><value>([^<]*)<\/value>[^]*?<outerBoundaryIs>[^]*?<coordinates>([^<]*)<\/coordinates>)");

    std::smatch match;
    auto start = kmlData.cbegin();

    while (std::regex_search(start, kmlData.cend(), match, placemarkRegex)) {
        // Create a State object for each Placemark
        State state;
        state.setName(match[1].str());

        // Extract and process coordinate data
        std::string coordinatesStr = match[2].str();
        std::istringstream coordinatesStream(coordinatesStr);
        std::string coordinatePair;
        std::vector<Point3D> statePoints;

        while (std::getline(coordinatesStream, coordinatePair, '\n')) {
            std::istringstream coordinatePairStream(coordinatePair);

            float x, y;
            coordinatePairStream >> x >> y;
            Point3D point(x - 80, y - 20);
            statePoints.push_back(point);
        }

        state.setPoints(statePoints);
        states.push_back(state);

        // Move to the next Placemark in the input
        start = match.suffix().first;
    }

    return states;
}