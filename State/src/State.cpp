#include "pch.h"
#include "State.h"

// Default constructor
State::State() {
}

// Destructor
State::~State() {
}

// Set the name of the state
void State::setName(const std::string& name) {
    mName = name;
}

// Set the coordinates of the state
void State::setPoints(const std::vector<Point3D>& coordinates) {
    mCoordinates = coordinates;
}

// Set the AQI (Air Quality Index) data for the state
void State::setAQIData(const std::map<std::string, int>& aqiData) {
    mAqiData = aqiData;
}

// Get the name of the state
std::string State::name() const {
    return mName;
}

// Get the coordinates of the state
std::vector<Point3D> State::coordinate() const {
    return mCoordinates;
}

// Get the AQI (Air Quality Index) data for the state
std::map<std::string, int> State::getAQIData() const {
    return mAqiData;
}
