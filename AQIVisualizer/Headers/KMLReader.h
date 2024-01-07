#pragma once
#include "State.h"
#include <vector>

class KMLReader
{
public:
	KMLReader();
	~KMLReader();
	std::vector<State> parseKMLFromFile(const std::string& filePath);

private:
	std::vector<State> parseKML(const std::string& kmlData);
};