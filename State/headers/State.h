#pragma once

#include "Point3D.h"
#include "pch.h"
#include <map>
#include <string>
#include <vector>

class STATE_API State {
public:
    State();
    ~State();

    // Set the name of the state
    void setName(const std::string& name);

    // Set the coordinates of the state
    void setPoints(const std::vector<Point3D>& coordinates);

    // Set the AQI (Air Quality Index) data for the state
    void setAQIData(const std::map<std::string, int>& aqiData);

    // Get the name of the state
    std::string name() const;

    // Get the coordinates of the state
    std::vector<Point3D> coordinate() const;

    // Get the AQI (Air Quality Index) data for the state
    std::map<std::string, int> getAQIData() const;

private:
    std::string mName;
    std::vector<Point3D> mCoordinates;
    std::map<std::string, int> mAqiData;
};
